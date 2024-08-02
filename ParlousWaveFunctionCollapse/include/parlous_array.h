#ifndef PARLOUS_ARRAY_H_INCLUDED
#define PARLOUS_ARRAY_H_INCLUDED

#define PA_MODULE_NAME "parlous_array"

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


////////////////////////////////////////
////////// Static Functions ////////////
////////////////////////////////////////
template <typename T>
static int pprintMap(lua_State* L)
{
    lua_Integer length = lua_tointeger(L, lua_upvalueindex(1));
    lua_Integer width = lua_tointeger(L, lua_upvalueindex(2));
    T *hexMap = (T *)lua_touserdata(L, -1);
    const lua_Integer rows = length/width;
    for (lua_Integer i = 0; i < rows; i++) {
        #ifndef IN_DEFOLD
            if (i < 10 && i%2 == 0) printf("map[%lu][:]:  ", i);
            else if (i < 10 && i%2 == 1) printf("map[%lu][:]: ", i);
            else if (i%2 == 0) printf("map[%lu][:]: ", i);
            else printf("map[%lu][:]:", i);
            for (lua_Integer j = 0; j < width; j++)
            {
                lua_Integer v = hexMap[j + width*i];
                printf(" %lu", v != 0 ? ~v : 0);
            }
        #else
            if (i < 10 && i%2 == 0) printf("map[%llu][:]:  ", i);
            else if (i < 10 && i%2 == 1) printf("map[%llu][:]: ", i);
            else if (i%2 == 0) printf("map[%llu][:]: ", i);
            else printf("map[%llu][:]:", i);
            for (lua_Integer j = 0; j < width; j++)
            {
                lua_Integer v = hexMap[j + width*i];
                printf(" %llu", v != 0 ? ~v : 0);
            }
        #endif // IN_DEFOLD
        printf("\n");
    }
    printf("-----------");
    for (lua_Integer j = 0; j < width; j++) {
        printf("--");
    }
    printf("\n");

    // Return 0 items
    return 0;
}

static int len(lua_State* L)                      //// [-0, +1, m]
{
    // Collect len and put on stack
    lua_Integer length = lua_tointeger(L, lua_upvalueindex(1));
    lua_pushinteger(L, length);                     // [-0, +1, -]
    
    // Return 1 item
    return 1;
}

template <typename T>
static int get(lua_State* L)                      //// [-0, +1, m]
{
    printf("### v-----<!!!>-----v\n"); // ### DEBUG   vvv ?MEMORY ERROR BELOW? vvv
    // Check and collect parameters from stack
    lua_Integer length = lua_tointeger(L, lua_upvalueindex(1));
    T *arr = (T *)lua_touserdata(L, -2);
    lua_Integer i = luaL_checkinteger(L, -1);
    if(length <= i || 0 > i)
        return luaL_error(L, "Index out of bounds");
    printf("### -----<i=%lld>-----\n", i); // ### DEBUG   vvv ?MEMORY ERROR BELOW? vvv

    // Get array at index
    lua_pushnumber(L, (T)arr[i]);                   // [-0, +1, -]
    printf("### ^-----<!!!>-----^\n"); // ### DEBUG   ^^^ ?MEMORY ERROR ABOVE? ^^^
    
    // Return 1 item
    return 1;
}

template <typename T>
static int put(lua_State* L)                      //// [-0, +0, m]
{
    // Check and collect parameters from stack
    lua_Integer length = lua_tointeger(L, lua_upvalueindex(1));
    T *arr = (T *)lua_touserdata(L, -3);
    lua_Integer i = luaL_checkinteger(L, -2);
    T v = (T)luaL_checknumber(L, -1);
    if(length <= i || 0 > i)
    {
        lua_pushstring(L, "Index out of bounds");
        lua_error(L);
    }

    // Put value in array at index
    arr[i] = v;
    
    // Return 1 item
    return 1;
}

template <typename T>
static int map(lua_State* L)                      //// [-0, +0, m]
{
    // Check and collect parameters from stack
    lua_Integer length = lua_tointeger(L, lua_upvalueindex(1));
    T *arr = (T *)lua_touserdata(L, -2);
    luaL_checktype(L, -1, LUA_TFUNCTION);
    lua_pushvalue(L, -1);                           // [-0, +1, -]

    // Setup loop
    for (lua_Integer i = 0; i < length; i++)
    {
        lua_pushnumber(L, (T)arr[i]);               // [-0, +1, -]
        lua_call(L, 1, 1);                          // [-2, +1, e]
        T result = (T)luaL_checknumber(L, -1);
        arr[i] = result;
        lua_pop(L, 1);                              // [-1, +0, -]
        lua_pushvalue(L, -1);                       // [-0, +1, -]
    }
    lua_pop(L, 1);                                  // [-1, +0, -]

    // Return 0 items
    return 0;
}

template <typename T>
static int foreach(lua_State* L)                  //// [-0, +0, m]
{
    // Check and collect parameters from stack
    lua_Integer length = lua_tointeger(L, lua_upvalueindex(1));
    T *arr = (T *)lua_touserdata(L, -2);
    luaL_checktype(L, -1, LUA_TFUNCTION);
    lua_pushvalue(L, -1);                           // [-0, +1, -]

    // Setup loop
    for (lua_Integer i = 0; i < length; i++)
    {
        lua_pushinteger(L, i);                      // [-0, +1, -]
        lua_pushnumber(L, (T)arr[i]);               // [-0, +1, -]
        lua_call(L, 2, 0);                          // [-3, +0, e]
        printf("### v-----<B>-----v\n"); // ### DEBUG   vvv ?MEMORY ERROR BELOW? vvv
        lua_pushvalue(L, -1);                       // [-0, +1, -]
        printf("### ^-----<B>-----^\n"); // ### DEBUG   ^^^ ?MEMORY ERROR ABOVE? ^^^
    }
    lua_pop(L, 1);                                  // [-1, +0, -]

    // Return 0 items
    return 0;
}

static int len_factory(lua_State* L,              //// [-0, +1, m]
                       lua_Integer length,
                       lua_Integer type_size,
                       lua_Integer width,
                       lua_Integer maxDepth,
                       lua_Integer prime,
                       lua_CFunction fn)
{

    // Collect len and put on stack
    lua_pushinteger(L, length);                     // [-0, +1, -]
    lua_pushinteger(L, type_size);                  // [-0, +1, -]
    lua_pushinteger(L, width);                      // [-0, +1, -]
    lua_pushinteger(L, maxDepth);                   // [-0, +1, -]
    lua_pushinteger(L, prime);                      // [-0, +1, -]
    lua_pushcclosure(L, fn, 5);                     // [-5, +1, -]
    
    // Return 1 item
    return 1;
}
////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
// int get_int(lua_State* L);
// int put_int(lua_State* L);
// void defineMetatable(lua_State* L);
template <typename T> int newArray(lua_State* L);

void LuaInitPArray(lua_State* L);

#endif  // PARLOUS_ARRAY_H_INCLUDED