#ifndef HEXMAP_HELPER_H_INCLUDED
#define HEXMAP_HELPER_H_INCLUDED

#include "indefold.h"
// Replace below lua includes with "#include <dmsdk/sdk.h>" when using in Defold
#ifndef IN_DEFOLD
extern "C"{
    #include <lua.h>
    #include <luaconf.h>
    #include <lauxlib.h>
    #include <lualib.h>
}
#else
    #include <dmsdk/sdk.h>
#endif // IN_DEFOLD

#define Y(l,c) (l/c)
#define row(l,c) (l/c)
#define X(l,c) (l%c)
#define col(l,c) (l%c)
#define even_axial_to_l(w, q, r) (q + (r + (r&1))/2 + w*r)


template <typename T>
static int hh__call(lua_State* L)
{
    T *hexMap = (T *)lua_touserdata(L, lua_upvalueindex(1));
    lua_Integer length = lua_tointeger(L, lua_upvalueindex(2));
    lua_Integer width = lua_tointeger(L, lua_upvalueindex(3));
    lua_Integer idx = lua_tointeger(L, lua_upvalueindex(4));
    lua_Integer q = luaL_checkinteger(L, -2);
    lua_Integer r = luaL_checkinteger(L, -1);

    // Convert axial coordinates to array index
    lua_Integer l = even_axial_to_l(width, q, r);
    // TODO Introduce flag for edge overflows
    if(length <= idx + l || 0 > idx + l)
        return luaL_error(L, "Index out of bounds: (q=%d, r=%d)", q, r);
    lua_pushinteger(L, hexMap[idx + l]);

    return 1;
}

static int init_state(lua_State* L,               //// [-0, +1, m]
                      lua_Integer idx,
                      lua_Integer length,
                      lua_Integer type_size,
                      lua_Integer width,
                      lua_Integer maxDepth,
                      lua_Integer prime,
                      lua_CFunction fn)
{
    // Verify userdata of map is on stack
    luaL_checktype(L, -3, LUA_TUSERDATA);
    lua_pushvalue(L, -3);                           // [-0, +1, -]

    // Collect len and put on stack
    lua_pushinteger(L, idx);                        // [-0, +1, -]
    lua_pushinteger(L, length);                     // [-0, +1, -]
    lua_pushinteger(L, type_size);                  // [-0, +1, -]
    lua_pushinteger(L, width);                      // [-0, +1, -]
    lua_pushinteger(L, maxDepth);                   // [-0, +1, -]
    lua_pushinteger(L, prime);                      // [-0, +1, -]
    lua_pushcclosure(L, fn, 7);                     // [-7, +1, -]
    
    // Return 1 item
    return 1;
}

// Top of stack must be userdata metatable will be applied to
template <typename T>
static void hh__defineMetatable(lua_State* L,     //// [-0, +0, m]
                                lua_Integer idx,
                                lua_Integer length,
                                lua_Integer type_size,
                                lua_Integer width,
                                lua_Integer maxDepth,
                                lua_Integer prime)
{
    // Create a metatable for the user data
    lua_createtable(L, 0, 1);                       // [-0, +1, m]

    // Define the __call method
    lua_pushstring(L, "__call");                    // [-0, +1, m]
    init_state(L,                                   // [-0, +1, m]
               idx,
               length,
               type_size,
               width,
               maxDepth,
               prime,
               hh__call<T>);
    lua_settable(L, -3);                            // [-2, +0, -]

    // Set the metatable
    lua_setmetatable(L, -2);                        // [-1, +0, -]
}

/**
 * @brief Hex map userdata must be on top of stack
 * 
 * @tparam T 
 * @param L 
 * @param idx 
 * @param length 
 * @param type_size 
 * @param width 
 * @param maxDepth 
 * @param prime 
 * @return int 
 */
template <typename T>
int newHexMapHelper(lua_State* L,                 //// [-0, +1, m]
                    lua_Integer idx,
                    lua_Integer length,
                    lua_Integer type_size,
                    lua_Integer width,
                    lua_Integer maxDepth,
                    lua_Integer prime)
{
    // Allocate array
    lua_newuserdata(L, 2*type_size);                // [-0, +1, m]

    // Define metatable
    hh__defineMetatable<T>(L,                       // [-0, +0, m]
                           idx,
                           length,
                           type_size,
                           width,
                           maxDepth,
                           prime);
    
    // Return 1 item
    return 1;
}


#endif  // HEXMAP_HELPER_H_INCLUDED