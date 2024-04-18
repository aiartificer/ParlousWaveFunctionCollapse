#include "wfc.h"
#include "halton.h"
#include <stdlib.h>
#include <cassert>
#include <stdio.h>  // ### DEBUG


BoundedMap init_BoundedMap(lua_State *L, size_t columns, size_t rows)
{
    BoundedMap map;
    map.columns = columns;
    map.rows = rows;
    map.map = (uint_fast64_t*)lua_newuserdata(L, columns*rows*sizeof(uint_fast64_t));
    for (size_t i = 0; i < columns*rows; i++)
        map.map[i] = 0x0000000000000000;
    return map;
}

AdjRuleList init_AdjRuleList(lua_State *L, size_t numRules, size_t prime)
{
    AdjRuleList rules;
    rules.len = numRules;
    rules.index = 0;
    rules.prime = prime;
    void *rulesBuffer = lua_newuserdata(L, numRules*sizeof(rule_fp));
    rules.rule = (rule_fp *)rulesBuffer;
    for (size_t i = 0; i < numRules; i++)
        rules.rule[i] = NULL;
    return rules;
}

int addRule_AdjRuleList(AdjRuleList rules, size_t ruleIdx, rule_fp rule)
{
    if (ruleIdx >= rules.len) return 1;
    rules.rule[ruleIdx] = rule;
    return 0;
}

size_t __countBits(uint_fast64_t n)
{
    // Brian Kernighan’s Algorithm
    size_t count = 0;
    while (n)
    {
        n &= (n - 1);
        count++;
    }
    return count;
}

int hexCircle(lua_State *L,
              BoundedMap map,
              uint_fast16_t l,
              size_t r,
              uint_fast64_t *circle,
              size_t hexCircleLen)
{
    if (hexCircleLen < 6*r) return 1;

    // Start directly left of point
    size_t i = 0;
    circle[i++] = l - r;

    // Upper left of hex
    for (size_t c = 0; c < r; c++)
    {
        circle[i] = circle[i-1] - map.columns + (row(circle[i-1], map.columns)%2 == 0 ? 1 : 0);
        i++;
    }

    // Top of hex
    for (size_t c = 0; c < r; c++)
    {
        circle[i] = circle[i-1] + 1;
        i++;
    }

    // Upper right of hex
    for (size_t c = 0; c < r; c++)
    {
        circle[i] = circle[i-1] + map.columns + (row(circle[i-1], map.columns)%2 == 0 ? 1 : 0);
        i++;
    }

    // Lower right of hex
    for (size_t c = 0; c < r; c++)
    {
        circle[i] = circle[i-1] + map.columns - (row(circle[i-1], map.columns)%2 == 0 ? 0 : 1);
        i++;
    }

    // Bottom of hex
    for (size_t c = 0; c < r; c++)
    {
        circle[i] = circle[i-1] - 1;
        i++;
    }

    // Lower left of hex
    for (size_t c = 0; c < r - 1; c++)
    {
        circle[i] = circle[i-1] - map.columns - (row(circle[i-1], map.columns)%2 == 0 ? 0 : 1);
        i++;
    }

    return 0;
}

int __updateDomainAtPoint(BoundedMap map, size_t l, AdjRuleList &rules,
                          bool newWave)
{
    // Skip when on map edge or when point is already determined
    if (l >= map.rows*map.columns) return 0;
    if (map.map[l] > 0 && __countBits(~map.map[l]) == 1) return 0;
    if (l%map.rows < REGION_SIZE/2 || l%map.rows >= map.columns - REGION_SIZE/2) return 0;
    if (l/map.rows < REGION_SIZE/2 || l/map.rows >= map.rows - REGION_SIZE/2) return 0;

    // Loop through adjanceny range around point and apply rules
    uint_fast64_t* adjPoints[REGION_SIZE];
    adjPoints[0] = &map.map[l - REGION_SIZE/2 - 2*map.columns + 1];
    adjPoints[1] = &map.map[l - REGION_SIZE/2 - map.columns + Y(l, map.columns)%2 == 0 ? 1 : 0];
    adjPoints[2] = &map.map[l - REGION_SIZE/2];
    adjPoints[3] = &map.map[l - REGION_SIZE/2 + map.columns + Y(l, map.columns)%2 == 0 ? 1 : 0];
    adjPoints[4] = &map.map[l - REGION_SIZE/2 + 2*map.columns + 1];
    for (uint_fast16_t k = 0; k < rules.len; k++)
    {
        // Verify rule exists
        if ((rules.rule)[k] == NULL) return 2;

        // Apply rule
        map.map[l] = (rules.rule)[k](adjPoints, map.map[l],
                                     newWave ? 0 : __countBits(~map.map[l]),
                                     rules.index, rules.prime);
    }

    return 0;
}

int __updateDomainAtPoint(BoundedMap map,
                          size_t x,
                          size_t y,
                          AdjRuleList &rules,
                          bool newWave)
{ return __updateDomainAtPoint(map, x + map.columns*y, rules, newWave); }

int updateDomain(BoundedMap map,
                 AdjRuleList &rules,
                 size_t prime,
                 size_t maxDepth)
{
    // Verify parameters
    if (map.map == NULL) return 1;

    // Loop through map and apply rules hitting each point once
    for (uint_fast16_t i = 0; i < map.columns*map.rows; i++)
    {
        // Apply rules to point
        uint_fast16_t l = (prime*i)%(map.columns*map.rows);
        if (map.map[l] > 0 && __countBits(~map.map[l]) == 1) continue;
        int err = __updateDomainAtPoint(map, l, rules, true);
        if (err != 0) return err;

        // Apply rules to adjacent points
        size_t buffer_size = 6*(maxDepth*maxDepth + maxDepth)/2;
        lua_State *L = luaL_newstate();
        uint_fast64_t *circleBuffer = (uint_fast64_t *)lua_newuserdata(L, buffer_size*sizeof(uint_fast64_t));
        for (size_t r = 1; r <= maxDepth; r++)
        {
            err = hexCircle(L, map, l, r, &circleBuffer[6*(r-1)], buffer_size);
            if (err != 0) return err;
        }
        for (size_t al = 0; al < buffer_size; al++)
        {
            err = __updateDomainAtPoint(map, circleBuffer[al], rules);
            if (err != 0) return err;
        }
    }

    return 0;
}