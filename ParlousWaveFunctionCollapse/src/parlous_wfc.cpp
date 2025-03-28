#define LIB_NAME "ParlousWaveFunctionCollapse"

#include <parlous_array.h>
#include <cassert>
#include <math.h>
#include <string.h>
//#include "wfc.h"
#include "parlous_wfc.h"


template <typename T>
static int bitXor(lua_State* L)                   //// [-0, +1, m]
{
    lua_Integer a = luaL_checkinteger(L, -2);
    lua_Integer b = luaL_checkinteger(L, -1);
    lua_pushinteger(L, a^b);                        // [-0, +1, -]
                    

    return 1;
}

template <typename T>
static int bitNot(lua_State* L)                   //// [-0, +1, m]
{
    lua_Integer a = luaL_checkinteger(L, -1);
    lua_pushinteger(L, ~a);                         // [-0, +1, -]
                    

    return 1;
}

template <typename T>
static int bitOr(lua_State* L)                    //// [-0, +1, m]
{
    lua_Integer a = luaL_checkinteger(L, -2);
    lua_Integer b = luaL_checkinteger(L, -1);
    lua_pushinteger(L, a|b);                        // [-0, +1, -]
                    

    return 1;
}

template <typename T>
static int bitMask(lua_State* L)                  //// [-0, +1, m]
{
    lua_Integer mask = luaL_checkinteger(L, -2);
    lua_Integer v = luaL_checkinteger(L, -1);
    lua_pushinteger(L, mask&v);                     // [-0, +1, -]

    return 1;
}

template <typename T>
static int v_rel_dist(lua_State* L)               //// [-0, +1, m]
{
    lua_Integer c = lua_tointeger(L, lua_upvalueindex(1));
    lua_Integer l = lua_tointeger(L, lua_upvalueindex(2));
    lua_Integer q = luaL_checkinteger(L, -2);
    lua_Integer r = luaL_checkinteger(L, -1);
    lua_pushinteger(L,                              // [-0, +1, -]
                    (abs(q - X(l,c)) + \
                     abs(q + r - X(l,c) - Y(l,c)) + \
                     abs(r - Y(l,c)))/2);

    return 1;
}

template <typename T>
static int v_abs_dist(lua_State* L)               //// [-0, +1, m]
{
    lua_Integer q = luaL_checkinteger(L, -2);
    lua_Integer r = luaL_checkinteger(L, -1);
    lua_pushinteger(L,                              // [-0, +1, -]
                    (abs(q) + abs(q + r) + abs(r))/2);

    return 1;
}

template <typename T>
static size_t __countBits(T n)
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

template <typename T>
static int bitCount(lua_State* L)                 //// [-0, +1, m]
{
    lua_Integer v = luaL_checkinteger(L, -1);
    lua_pushinteger(L,                              // [-0, +1, -]
                    __countBits(v));

    return 1;
}

template <typename T>
static int bitVal(lua_State* L)                   //// [-0, +1, m]
{
    lua_Integer v = luaL_checkinteger(L, -1);
    lua_Integer i = 0;
    while (v)
    {
        i++;
        if (v & 1)
            break;
        v >>= 1;
    }
    lua_pushinteger(L, i);                          // [-0, +1, -]

    return 1;
}

template <typename T>
static T hx(lua_Integer idx, lua_Integer width, lua_Integer length,
            T q, T r)
{
    lua_Integer _r = Y(idx, width);
    lua_Integer _q = X(idx, width) - (_r + (_r&1))/2;
    lua_Integer new_q = _q + q;
    lua_Integer new_r = _r + r;
    lua_Integer newCol = evenAxialCol(new_q, new_r);

    // Determine if off edge of map and need to loop around
    if (newCol < 0)
        new_q += width;
    if (newCol >= width)
        new_q -= width;

    // Set l to new location
    lua_Integer l = even_axial_to_l(width, new_q, new_r);
    // lua_Integer test = even_axial_to_l(width, new_q, new_r+1)-width;  // ### DEBUG
    // printf("{hx:_q=%li,_r=%li,q=%li,r=%li,c=%li,*q=%li,*r=%li,l=%li,t=%li}", _q, _r, q, r, newCol, new_q, new_r, l, test);  // ### DEBUG
    // printf("{hx:l=%li|%li|%d}", l, length + l - width, 0 > l);  // ### DEBUG

    // Loop around Y-axis
    if (0 > l)
        l = length + l;// - width;
    if (length <= l)
        l = l - length;; //l + width - length;

    // printf("!!l=%li ", l);  // ### DEBUG
    return l;
}
#define hxL(idx, width, length) hx(idx, width, length, (lua_Integer)-1, (lua_Integer)0)
#define hxUL(idx, width, length) hx(idx, width, length, (lua_Integer)-0, (lua_Integer)-1)
#define hxUR(idx, width, length) hx(idx, width, length, (lua_Integer)1, (lua_Integer)-1)
#define hxR(idx, width, length) hx(idx, width, length, (lua_Integer)1, (lua_Integer)0)
#define hxDR(idx, width, length) hx(idx, width, length, (lua_Integer)0, (lua_Integer)1)
#define hxDL(idx, width, length) hx(idx, width, length, (lua_Integer)-1, (lua_Integer)1)

template <typename T>
static int genHexMapHelperFunc(lua_State* L)
{
    T *hexMap = (T *)lua_touserdata(L, lua_upvalueindex(1));
    lua_Integer length = lua_tointeger(L, lua_upvalueindex(2));
    lua_Integer width = lua_tointeger(L, lua_upvalueindex(3));
    lua_Integer idx = lua_tointeger(L, lua_upvalueindex(4));
    lua_Integer q = luaL_checkinteger(L, -2);
    lua_Integer r = luaL_checkinteger(L, -1);

    // Convert axial coordinates to array index
    // lua_Integer _r = Y(idx, width);
    // lua_Integer _q = X(idx, width) - (_r + (_r&1))/2;
    // lua_Integer l = even_axial_to_l(width, _q+q, _r+r);
    // // TODO Introduce flag for edge overflows
    // if(length <= l || 0 > l)
    //     return luaL_error(L, "Index out of bounds: (q=%d, r=%d)", q, r);
    lua_Integer l = hx(idx, width, length, q, r);

    // Push return value onto stack
    lua_pushinteger(L, ~hexMap[l]);            // [-0, +1, -]
    // if (idx == 32) printf("### [32]: q = %li, r = %li, l = %li, value = %li\n", q, r, l, ~hexMap[l]);  // ### DEBUG

    return 1;
}

template <typename T>
static int hexCircle(lua_State* L,
                     lua_Integer width,
                     lua_Integer length,
                     lua_Integer l,
                     lua_Integer r,
                     T *circle,
                     lua_Integer hexCircleLen)
{
    if (hexCircleLen < 6*r)
        return luaL_error(L, "Size allocated for hexCircle too small: need=%d, allocated=%d)", 6*r, hexCircleLen);\
    
    // Start directly left of point
    size_t i = 0;
    circle[i++] = hx<lua_Integer>(l, width, length, -r, 0);

    // Upper left of hex
    for (lua_Integer c = 0; c < r; c++)
    {
        circle[i] = hxUR(circle[i-1], width, length);
        // printf("[]=%li ", circle[i]);  // ### DEBUG
        i++;
    }

    // Top of hex
    for (lua_Integer c = 0; c < r; c++)
    {
        circle[i] = hxR(circle[i-1], width, length);
        i++;
    }

    // Upper right of hex
    for (lua_Integer c = 0; c < r; c++)
    {
        circle[i] = hxDR(circle[i-1], width, length);
        i++;
    }

    // Lower right of hex
    for (lua_Integer c = 0; c < r; c++)
    {
        circle[i] = hxDL(circle[i-1], width, length);
        i++;
    }

    // Bottom of hex
    for (lua_Integer c = 0; c < r; c++)
    {
        circle[i] = hxL(circle[i-1], width, length);
        i++;
    }

    // Lower left of hex
    for (lua_Integer c = 0; c < r - 1; c++)
    {
        circle[i] = hxUL(circle[i-1], width, length);
        i++;
    }

    return 0;
}

static int circle_len_factory(lua_State* L,       //// [-0, +1, m]
                              lua_Integer circleLength,
                              lua_Integer type_size,
                              lua_CFunction fn)
{
    // Collect len and put on stack
    lua_pushinteger(L, circleLength);               // [-0, +1, -]
    lua_pushinteger(L, type_size);                  // [-0, +1, -]
    lua_pushcclosure(L, fn, 2);                     // [-2, +1, -]

    // Return 1 item
    return 1;
}

template <typename T>
static int circleAny(lua_State* L)                //// [-0, +1, m]
{
    // Check and collect parameters from stack
    lua_Integer circleLength = lua_tointeger(L, lua_upvalueindex(1));
    T *arr = (T *)lua_touserdata(L, -2);
    luaL_checktype(L, -1, LUA_TFUNCTION);
    lua_pushvalue(L, -1);                           // [-0, +1, -]

    // Search for value and stop if found
    int returnVal = 0;
    for (lua_Integer i = 0; i < circleLength; i++)
    {
        // Add parameter for terrain type at present point
        lua_pushnumber(L, (T)arr[i]);               // [-0, +1, -]

        // Add bit_count and bit as function parameters
        lua_pushcclosure(L, bitCount<T>, 0);        // [-0, +1, -]
        lua_pushcclosure(L, bitVal<T>, 0);          // [-0, +1, -]

        // Add bitwise functions as function parameters
        lua_pushcclosure(L, bitMask<T>, 0);         // [-0, +1, -]
        lua_pushcclosure(L, bitOr<T>, 0);           // [-0, +1, -]
        lua_pushcclosure(L, bitNot<T>, 0);          // [-0, +1, -]
        lua_pushcclosure(L, bitXor<T>, 0);          // [-0, +1, -]

        // Call function to identify type match
        lua_call(L, 7, 1);                          // [-8, +1, e]

        // Collect return value
        returnVal = lua_toboolean(L, -1);
        lua_pop(L, 1);                              // [-1, +0, -]

        // Copy function reference and put at top of stack
        lua_pushvalue(L, -1);                       // [-0, +1, -]

        // Stop short if any matches found
        if (returnVal)
            break;
    }

    // Pop duplicate function and push return value
    lua_pop(L, 1);                                  // [-1, +0, -]
    lua_pushboolean(L, returnVal);                  // [-0, +1, -]

    // Return 1 items
    return 1;
}

template <typename T>
static int circleAll(lua_State* L)                //// [-0, +1, m]
{
    // Check and collect parameters from stack
    lua_Integer circleLength = lua_tointeger(L, lua_upvalueindex(1));
    T *arr = (T *)lua_touserdata(L, -2);
    luaL_checktype(L, -1, LUA_TFUNCTION);
    lua_pushvalue(L, -1);                           // [-0, +1, -]

    // Search for value and flag if found
    int returnVal = 1;
    for (lua_Integer i = 0; i < circleLength; i++)
    {
        // Add parameter for terrain type at present point
        lua_pushnumber(L, (T)arr[i]);               // [-0, +1, -]

        // Add bit_count and bit as function parameters
        lua_pushcclosure(L, bitCount<T>, 0);        // [-0, +1, -]
        lua_pushcclosure(L, bitVal<T>, 0);          // [-0, +1, -]

        // Add bitwise functions as function parameters
        lua_pushcclosure(L, bitMask<T>, 0);         // [-0, +1, -]
        lua_pushcclosure(L, bitOr<T>, 0);           // [-0, +1, -]
        lua_pushcclosure(L, bitNot<T>, 0);          // [-0, +1, -]
        lua_pushcclosure(L, bitXor<T>, 0);          // [-0, +1, -]

        // Call function to identify type match
        lua_call(L, 7, 1);                          // [-8, +1, e]

        // Collect return value
        returnVal = lua_toboolean(L, -1);
        lua_pop(L, 1);                              // [-1, +0, -]

        // Copy function reference and put at top of stack
        lua_pushvalue(L, -1);                       // [-0, +1, -]

        // Flag if any matches found
        if (!returnVal) returnVal = 0;
    }

    // Pop duplicate function and push return value
    lua_pop(L, 1);                                  // [-1, +0, -]
    lua_pushboolean(L, returnVal);                  // [-0, +1, -]

    // Return 1 items
    return 1;
}

template <typename T>
static int wfc__circle_index_call(lua_State* L)   //// [-0, +1, m]
{
    lua_Integer circleLength = lua_tointeger(L, lua_upvalueindex(1));

    // Check if a function call or array index
    if(LUA_TSTRING == lua_type(L, -1))
    {
        const char *f = luaL_checkstring(L, -1);
        if(strcmp("any", f) == 0)
        {
            lua_pushinteger(L, circleLength);       // [-0, +1, -]
            lua_pushcclosure(L, circleAny<T>, 1);   // [-1, +1, -]
            return 1;
        }
        else if(strcmp("all", f) == 0)
        {
            lua_pushinteger(L, circleLength);       // [-0, +1, -]
            lua_pushcclosure(L, circleAll<T>, 1);   // [-1, +1, -]
            return 1;
        }
        else
            return luaL_error(L, "Invalid key");
    }
    else if(LUA_TNUMBER == lua_type(L, -1))
        return get<T>(L);                          // [-0, +1, m]
    
    // Return 1 item
    return 1;
}

// Top of stack must be userdata metatable will be applied to
template <typename T>
static void wfc__defineCircleMetatable(lua_State* L,  //// [-0, +0, m]
                                       lua_Integer circleLength,
                                       lua_Integer type_size)
{
    // Create a metatable for the user data
    lua_createtable(L, 0, 1);                       // [-0, +1, m]

    // Define __len
    lua_pushstring(L, "__len");                     // [-0, +1, m]
    circle_len_factory(L, circleLength, type_size,
                       len);                        // [-0, +1, m]
    lua_settable(L, -3);                            // [-2, +0, -]

    // Define the __index method
    lua_pushstring(L, "__index");                   // [-0, +1, m]
    circle_len_factory(L, circleLength, type_size,
                       wfc__circle_index_call<T>);  // [-0, +1, m]
    lua_settable(L, -3);                            // [-2, +0, -]

    // Set the metatable
    lua_setmetatable(L, -2);                        // [-1, +0, -]
}

template <typename T>
static int getCircle(lua_State* L)                //// [-0, +1, m]
{
    // Check and collect parameters from stack
    T *hexMap = (T *)lua_touserdata(L, lua_upvalueindex(1));
    const lua_Integer width = lua_tointeger(L, lua_upvalueindex(2));
    const lua_Integer length = lua_tointeger(L, lua_upvalueindex(3));
    lua_Integer l = lua_tointeger(L, lua_upvalueindex(4));
    lua_Integer r = luaL_checkinteger(L, -1);

    // Allocate reusable circle buffer
    lua_Integer buffer_size = 6*r;
    T *circleBuffer = (T *)lua_newuserdata(L, buffer_size*sizeof(T));  // [-0, +1, m]
    
    // Generate list of adjacent points in hex map
    hexCircle(L, width, length, l, r, circleBuffer, buffer_size);

    // Replace indexes in cicrle buffer with values
    // FIXME Line 373 causes crash sometimes
    for (lua_Integer i = 0; i < buffer_size; i++) {
        // if (~hexMap[circleBuffer[i]] < -1) printf("###=== map[%li]=%li, c=%li, %p\n", circleBuffer[i], ~hexMap[circleBuffer[i]], i, &circleBuffer[i]);  // ### DEBUG
        // else printf("###=== .\n");  // ### DEBUG
        circleBuffer[i] = ~hexMap[circleBuffer[i]];
    }

    // Define metatable
    wfc__defineCircleMetatable<T>(L,                // [-0, +0, m]
                                  buffer_size,
                                  sizeof(T));
    
    // Return 1 item
    return 1;
}

template <typename T>
static int __updateDomainCall(lua_State* L,       //// [-0, +0, m]
                              T *hexMap,
                              lua_Integer length,
                              lua_Integer width,
                              lua_Integer l,
                              bool newWave)
{
    // Check function on stack and copy it to top of stack
    luaL_checktype(L, -1, LUA_TFUNCTION);
    lua_pushvalue(L, -1);                           // [-0, +1, -]

    // Add hex map helper function to paramter list
    lua_pushlightuserdata (L, (void *)hexMap);      // [-0, +1, -]
    lua_pushinteger(L, length);                     // [-0, +1, -]
    lua_pushinteger(L, width);                      // [-0, +1, -]
    lua_pushinteger(L, l);                          // [-0, +1, -]
    lua_pushcclosure(L, genHexMapHelperFunc<T>, 4); // [-4, +1, -]

    // Add value at present point as parameter
    // printf("!!!{%lu}!!!  ", l);  // ### DEBUG
    lua_pushnumber(L, ~(T)hexMap[l]);               // [-0, +1, -]

    // Add new wave flag parameter
    lua_pushboolean(L, newWave);                    // [-0, +1, -]

    // Add function to measure distance from present point
    lua_pushinteger(L, width);                      // [-0, +1, -]
    lua_pushinteger(L, l);                          // [-0, +1, -]
    lua_pushcclosure(L, v_rel_dist<T>, 2);          // [-2, +1, -]

    // Add function to collect values in a circle around present point
    lua_pushlightuserdata (L, (void *)hexMap);      // [-0, +1, -]
    lua_pushinteger(L, width);                      // [-0, +1, -]
    lua_pushinteger(L, length);                     // [-0, +1, -]
    lua_pushinteger(L, l);                          // [-0, +1, -]
    lua_pushcclosure(L, getCircle<T>, 4);           // [-3, +1, -]

    // Call the function and apply result to hex map
    lua_call(L, 5, 1);                              // [-6, +1, e]
    T result = (T)luaL_checknumber(L, -1);
    hexMap[l] = ~result;
    lua_pop(L, 1);                                  // [-1, +0, -]

    return 0;
}

template <typename T>
static int updateDomainAtPoint(lua_State* L,
                               lua_Integer length,
                               lua_Integer width,
                               T *hexMap,
                               lua_Integer l,
                               bool newWave)
{
    // printf("[%lu]  ", l);  // ### DEBUG
    // Skip when on map edge or when point is already determined
    // TODO Introduce flag for edge overflows
    if(length <= l || 0 > l)
        return 0;
    if (hexMap[l] != 0 && __countBits(~hexMap[l]) == 1) return 0;

    // Location is good, apply rules
    luaL_checktype(L, -1, LUA_TFUNCTION);
    lua_Integer height = length/width;
    const lua_Integer x = l%width;
    const lua_Integer y = l/width;
    if (x < REGION_SIZE/2 || x >= width - REGION_SIZE/2) return 0;
    if (y < REGION_SIZE/2 || y >= height - REGION_SIZE/2) return 0;

    // Apply rules
    __updateDomainCall<T>(L, hexMap, length, width, l, newWave);

    return 0;
}

template <typename T>
static int gen(lua_State* L)                      //// [-0, +0, m]
{
    // Check and collect parameters from stack
    const lua_Integer length = lua_tointeger(L, lua_upvalueindex(1));
    const lua_Integer width = lua_tointeger(L, lua_upvalueindex(2));
    const lua_Integer maxDepth = lua_tointeger(L, lua_upvalueindex(3));
    const lua_Integer prime = lua_tointeger(L, lua_upvalueindex(4));
    T *hexMap = (T *)lua_touserdata(L, -2);
    luaL_checktype(L, -1, LUA_TFUNCTION);
    // lua_pushvalue(L, -1);                        // [-0, +1, -]

    // Allocate reusable circle buffer
    lua_Integer buffer_size = 6*(maxDepth*maxDepth + maxDepth)/2;
    T *circleBuffer = (T *)lua_newuserdata(L, buffer_size*sizeof(T));  // [-0, +1, m]
    lua_pushvalue(L, -2);                           // [-0, +1, -]

    // Setup loop
    for (lua_Integer i = 0; i < length; i++)
    {
        // Periodically (yet unsynchronized) select cell on map
        lua_Integer l = fmod((prime*i), length);
        // printf("\n----------<l = %li>----------", l);  // ### DEBUG

        // Check if cell has already completely collapsed the wave
        if (hexMap[l] != 0 && __countBits(~hexMap[l]) == 1) continue;
        
        // Update domain at present point
        updateDomainAtPoint(L, length, width, hexMap, l, true);
        // printf("\n----wave--------------------");  // ### DEBUG

        // Generate list of adjacent points in hex map
        for (lua_Integer r = 1; r <= maxDepth; r++)
            hexCircle(L, width, length,
                      l, r, &circleBuffer[6*(r-1)*r/2], 6*r);

        // Apply rules to adjacent points
        for (lua_Integer al = 0; al < buffer_size; al++) {
            // printf("\n  +-------<l = %li>----------\t[%li]", circleBuffer[al], al+2);  // ### DEBUG
            updateDomainAtPoint(L, length, width, hexMap, circleBuffer[al], false);}
    }
    lua_pop(L, 2);                                  // [-2, +0, -]

    // Return 0 items
    return 0;
}

template <typename T>
static int wfc__call(lua_State* L)                //// [-?, +1, m]
{
    lua_Integer width = lua_tointeger(L, lua_upvalueindex(3));
    lua_Integer l = luaL_checkinteger(L, -1);
    int numArgs = lua_gettop(L);

    // Check if using axial coordinates
    lua_Integer q = 0;
    if(numArgs == 2)
    {
        q = luaL_checkinteger(L, -2);
        l = even_axial_to_l(width, q, l);
        lua_pop(L, 2);                              // [-2, +0, -]
        lua_pushinteger(L, l);                      // [-0, +1, -]
    }

    // Call __get
    return get<T>(L);                               // [-0, +1, m]
}

template <typename T>
static int wfc__index_call(lua_State* L)          //// [-0, +1, m]
{
    lua_Integer length = lua_tointeger(L, lua_upvalueindex(1));
    lua_Integer width = lua_tointeger(L, lua_upvalueindex(3));
    lua_Integer maxDepth = lua_tointeger(L, lua_upvalueindex(4));
    lua_Integer prime = lua_tointeger(L, lua_upvalueindex(5));

    // Check if a function call or array index
    if(LUA_TSTRING == lua_type(L, -1))
    {
        const char *f = luaL_checkstring(L, -1);
        if(strcmp("map", f) == 0)
        {
            lua_pushinteger(L, length);             // [-0, +1, -]
            lua_pushcclosure(L, map<T>, 1);         // [-1, +1, -]
            return 1;
        }
        else if(strcmp("foreach", f) == 0)
        {
            lua_pushinteger(L, length);             // [-0, +1, -]
            lua_pushcclosure(L, foreach<T>, 1);     // [-1, +1, -]
            return 1;
        }
        else if (strcmp("gen", f) == 0)
        {
            lua_pushinteger(L, length);             // [-0, +1, -]
            lua_pushinteger(L, width);              // [-0, +1, -]
            lua_pushinteger(L, maxDepth);           // [-0, +1, -]
            lua_pushinteger(L, prime);              // [-0, +1, -]
            lua_pushcclosure(L, gen<T>, 4);         // [-4, +1, -]
            return 1;
        }
        else if (strcmp("pprint", f) == 0)
        {
            lua_pushinteger(L, length);             // [-0, +1, -]
            lua_pushinteger(L, width);              // [-0, +1, -]
            lua_pushcclosure(L, pprintMap<T>, 2);   // [-2, +1, -]
            return 1;
        }
        else if (strcmp("dist", f) == 0)
        {
            lua_pushcclosure(L, v_abs_dist<T>, 0);  // [-0, +1, -]
            return 1;
        }
        else if (strcmp("mask", f) == 0)
        {
            lua_pushcclosure(L, bitMask<T>, 0);     // [-0, +1, -]
            return 1;
        }
        else if (strcmp("And", f) == 0)
        {
            lua_pushcclosure(L, bitMask<T>, 0);     // [-0, +1, -]
            return 1;
        }
        else if (strcmp("Or", f) == 0)
        {
            lua_pushcclosure(L, bitOr<T>, 0);       // [-0, +1, -]
            return 1;
        }
        else if (strcmp("Not", f) == 0)
        {
            lua_pushcclosure(L, bitNot<T>, 0);       // [-0, +1, -]
            return 1;
        }
        else if (strcmp("Xor", f) == 0)
        {
            lua_pushcclosure(L, bitXor<T>, 0);       // [-0, +1, -]
            return 1;
        }
        else if (strcmp("bit_count", f) == 0)
        {
            lua_pushcclosure(L, bitCount<T>, 0);     // [-0, +1, -]
            return 1;
        }
        else if (strcmp("bit", f) == 0)
        {
            lua_pushcclosure(L, bitVal<T>, 0);      // [-0, +1, -]
            return 1;
        }
        else
            return luaL_error(L, "Invalid key");
    }
    else if(LUA_TNUMBER == lua_type(L, -1))
        return get<T>(L);                          // [-0, +1, m]
    
    // Return 1 item
    return 1;
}

// Top of stack must be userdata metatable will be applied to
template <typename T>
static void wfc__defineMetatable(lua_State* L,    //// [-0, +0, m]
                                 lua_Integer length,
                                 lua_Integer type_size,
                                 lua_Integer width,
                                 lua_Integer maxDepth,
                                 lua_Integer prime)
{
    // Create a metatable for the user data
    lua_createtable(L, 0, 1);                       // [-0, +1, m]

    // Define __len
    lua_pushstring(L, "__len");                     // [-0, +1, m]
    len_factory(L, length, type_size, width, maxDepth, prime, len);     // [-0, +1, m]
    lua_settable(L, -3);                            // [-2, +0, -]

    // Define the __index method
    lua_pushstring(L, "__index");                   // [-0, +1, m]
    len_factory(L, length, type_size, width, maxDepth, prime, wfc__index_call<T>);   // [-0, +1, m]
    lua_settable(L, -3);                            // [-2, +0, -]

    // Define the __newindex method
    lua_pushstring(L, "__newindex");                // [-0, +1, m]
    len_factory(L, length, type_size, width, maxDepth, prime, put<T>);      // [-0, +1, m]
    lua_settable(L, -3);                            // [-2, +0, -]

    // Define the __call method
    lua_pushstring(L, "__call");                    // [-0, +1, m]
    len_factory(L, length, type_size, width, maxDepth, prime, wfc__call<T>); // [-0, +1, m]
    lua_settable(L, -3);                            // [-2, +0, -]

    // Set the metatable
    lua_setmetatable(L, -2);                        // [-1, +0, -]
}

template <typename T>
int newWfcMap(lua_State* L)                        //// [-0, +1, m]
{
    // Check and get parameter string from stack
    lua_Integer length = luaL_checkinteger(L, -5);
    lua_Integer type_size = luaL_checkinteger(L, -4);
    lua_Integer width = luaL_checkinteger(L, -3);
    lua_Integer maxDepth = luaL_checkinteger(L, -2);
    lua_Integer prime = luaL_checkinteger(L, -1);

    // Check parameters
    if(length%width != 0)
        length = length*width;

    // Allocate array
    lua_newuserdata(L, length*type_size);           // [-0, +1, m]

    // Define metatable
    wfc__defineMetatable<T>(L,                      // [-0, +0, m]
                            length,
                            type_size,
                            width,
                            maxDepth,
                            prime);
    
    // Return 1 item
    return 1;
}

// ========================
// ====== Lua Stuff =======
// ========================
static const luaL_reg PWFC_Module_methods[] =
{
    {"new_hex_map", newWfcMap<lua_Integer>},
    {0, 0}
};

void LuaInitHexMap(lua_State* L)
{
    int top = lua_gettop(L);

    // Register lua names
    luaL_register(L, MODULE_NAME, PWFC_Module_methods);

    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}
