#ifndef WFC_H_INCLUDED
#define WFC_H_INCLUDED

#define REGION_SIZE 5

#include <stdint.h>
#include <cstddef>
#include "indefold.h"
// Replace below lua includes with "#include <dmsdk/sdk.h>" when using in Defold
#ifndef IN_DEFOLD
extern "C"{
    #include <lua.h>
    #include <luaconf.h>
    #include <lauxlib.h>
}
#else
    #include <dmsdk/sdk.h>
#endif // IN_DEFOLD


#define Y(l,c) (l/c)
#define row(l,c) (l/c)
#define X(l,c) (l%c)
#define col(l,c) (l%c)

// typedef uint_fast64_t (*bound_rule_fp)(uint_fast64_t (*)[REGION_SIZE],
//                                        uint_fast64_t,
//                                        size_t, size_t);
typedef uint_fast64_t (*rule_fp)(uint_fast64_t **,
                                 uint_fast64_t,
                                 size_t entropy,
                                 size_t &r_idx,
                                 size_t r_prime);

struct BoundedMap {
    uint_fast64_t *map;
    size_t columns;
    size_t rows;
};

/**
 * @brief 
 * 
 * [0, +1, m]
 * 
 * @param L 
 * @param columns 
 * @param rows 
 * @return BoundedMap 
 */
BoundedMap init_BoundedMap(lua_State *L, size_t columns, size_t rows);

struct AdjRuleList {
    size_t len;
    // uint_fast64_t (**rule)(uint_fast64_t *adjPoint[REGION_SIZE],
    //                        uint_fast64_t point,
    //                        size_t r_idx,
    //                        size_t r_prime);
    rule_fp *rule;
    size_t index;
    size_t prime;
};

/**
 * @brief 
 * 
 * [0, +1, m]
 * 
 * @param L 
 * @param numRules 
 * @param prime 
 * @return AdjRuleList 
 */
AdjRuleList init_AdjRuleList(lua_State *L, size_t numRules, size_t prime);

/**
 * @brief 
 * 
 * @param rules 
 * @param ruleIdx 
 * @param rule 
 * @return int 
 */
int addRule_AdjRuleList(AdjRuleList rules, size_t ruleIdx, rule_fp rule);

/**
 * @brief Calculates the coordinates of the perimeter of a haxagonal circle of
 * the given radious centered at the given location on the given map
 * 
 * @param[in] L the lua state
 * @param[in] map hexagonal map to get adjacent locations from
 * @param[in] l location on map
 * @param[in] r radious of circle to get adjacent locations from
 * @param[out] circle Lua user data buffer to hold circle of adjacent locations
 * @param[in] hexCircleLen Length of circle buffer
 * @return int int error code (0 if no error, 1 if lenth of circle buffer is too
 * small)
 */
int hexCircle(lua_State *L,
              BoundedMap map,
              uint_fast16_t l,
              size_t r,
              uint_fast64_t *circle,
              size_t hexCircleLen);

/**
 * @brief Updates the domain at a point in the map using the given list of rules
 * 
 * @param[in] map map to update
 * @param[in] l 1D array position of point in map to update
 * @param[in] rules list of rules defining how to update map
 * @return int error code (0 if no error)
 */
int __updateDomainAtPoint(BoundedMap map, size_t l, AdjRuleList &rules,
                          bool newWave = false);

/**
 * @brief Updates the domain at a point in the map using the given list of rules
 * 
 * @param[in] map map to update
 * @param[in] x column of point in map to update
 * @param[in] y row of point in map to update
 * @param[in] rules list of rules defining how to update map
 * @return int error code (0 if no error)
 */
int __updateDomainAtPoint(BoundedMap map,
                          size_t x,
                          size_t y,
                          AdjRuleList &rules,
                          bool newWave = false);

// /**
//  * @brief Updates the domain at points adjacent to the given point in the map
//  * using the given list of rules
//  * 
//  * @param[in] map map to update
//  * @param[in] rules list of rules defining how to update map
//  * @param[in] l 1D array position of point in map to update
//  * @param[in] depth current distance of WFC wave propagation
//  * @param[in] maxDepth maximum distance of WFC wave propagation
//  * @return int 
//  */
// int updateAdjDomain(BoundedMap map,
//                     uint_fast16_t l,
//                     AdjRuleList& rules,
//                     size_t depth,
//                     size_t maxDepth);

/**
 * @brief Updates the domain of the map using the given list of rules
 * 
 * @param[in] map map to update
 * @param[in] rules list of rules defining how to update map
 * @param[in] prime prime number that acts as a random seed
 * @param[in] maxDepth maximum distance of WFC wave propagation
 * @return int error code (0 if no error)
 */
int updateDomain(BoundedMap map,
                 AdjRuleList &rules,
                 size_t prime,
                 size_t maxDepth);

#endif // WFC_H_INCLUDED