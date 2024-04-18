#define LIB_NAME "ParlousWaveFunctionCollapse"
#define MODULE_NAME "parlous_wfc"

#include <parlous_array.h>
#include <cassert>
#include <math.h>
#include <string.h>
#include "wfc.h"
#include "parlous_wfc.h"


template <typename T>
static int gen(lua_State* L)                      //// [-0, +0, m]
{
    // Check and collect parameters from stack
    lua_Integer length = lua_tointeger(L, lua_upvalueindex(1));
    T *arr = (T *)lua_touserdata(L, -2);
    luaL_checktype(L, -1, LUA_TFUNCTION);
    lua_pushvalue(L, -1);                           // [-0, +1, -]
    // lua_Integer maxDepth = luaL_checkinteger(L, -1);
    // lua_Integer prime = luaL_checkinteger(L, -1);

    // // Setup loop
    // for (lua_Integer i = 0; i < length; i++)
    // {
    //     // Quasirandommly select cells on map
    //     uint_fast16_t l = (prime*i)%(map.columns*map.rows);

    //     // Check if cell has already completely collapsed the wave
    //     if (arr[l] > 0 && __countBits(~arr[l]) == 1) continue;
        
    //     // Update domain at present point
    //     int err = __updateDomainAtPoint(map, l, true);
    //     if (err != 0) return err;


    //     lua_pushinteger(L, i);                      // [-0, +1, -]
    //     lua_pushnumber(L, (T)arr[i]);               // [-0, +1, -]
    //     lua_call(L, 2, 1);                          // [-3, +1, e]
    //     T result = (T)luaL_checknumber(L, -1);
    //     arr[i] = result;
    //     lua_pop(L, 1);                              // [-1, +0, -]
    //     lua_pushvalue(L, -1);                       // [-0, +1, -]
    // }

    // Return 0 items
    return 0;
}

template <typename T>
static int wfc__index_call(lua_State* L)          //// [-0, +1, m]
{
    lua_Integer length = lua_tointeger(L, lua_upvalueindex(1));

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
            lua_pushcclosure(L, gen<T>, 1);     // [-1, +1, -]
            return 1;
        }
        else
            return luaL_error(L, "Invalid key");
    }
    else if(LUA_TNUMBER == lua_type(L, -1))
        return get<T>(L);                           // [-0, +1, m]
    
    // Return 1 item
    return 1;
}

template <typename T>
static int wfc__call(lua_State* L)                //// [-?, +1, m]
{
    lua_Integer width = lua_tointeger(L, lua_upvalueindex(3));
    lua_Integer length = luaL_checkinteger(L, -1);
    int numArgs = lua_gettop(L);
    lua_Integer row = 0;
    if(numArgs >= 2 && length < width)
    {
        row = luaL_checkinteger(L, -2);
        length = width*row + length;
        lua_pop(L, 2);                              // [-2, +0, -]
        lua_pushinteger(L, length);                 // [-0, +1, -]
    }

    // Call __get
    return get<T>(L);                               // [-0, +1, m]
}

template <typename T>
static void wfc__defineMetatable(lua_State* L,    //// [-0, +0, m]
                                 lua_Integer length,
                                 lua_Integer type_size,
                                 lua_Integer width)
{
    // Create a metatable for the user data
    lua_createtable(L, 0, 1);                       // [-0, +1, m]

    // Define __len
    lua_pushstring(L, "__len");                     // [-0, +1, m]
    len_factory(L, length, type_size, width, len);  // [-0, +1, m]
    lua_settable(L, -3);                            // [-2, +0, -]

    // Define the __index method
    lua_pushstring(L, "__index");                   // [-0, +1, m]
    len_factory(L, length, type_size, width, wfc__index_call<T>);   // [-0, +1, m]
    lua_settable(L, -3);                            // [-2, +0, -]

    // Define the __newindex method
    lua_pushstring(L, "__newindex");                // [-0, +1, m]
    len_factory(L, length, type_size, width, put<T>);      // [-0, +1, m]
    lua_settable(L, -3);                            // [-2, +0, -]

    // Define the __call method
    lua_pushstring(L, "__call");                    // [-0, +1, m]
    len_factory(L, length, type_size, width, wfc__call<T>); // [-0, +1, m]
    lua_settable(L, -3);                            // [-2, +0, -]

    // Set the metatable
    lua_setmetatable(L, -2);                        // [-1, +0, -]
}

template <typename T>
int newWfcMap(lua_State* L)                        //// [-0, +1, m]
{
    // Check and get parameter string from stack
    lua_Integer length = luaL_checkinteger(L, -3);
    lua_Integer type_size = luaL_checkinteger(L, -2);
    lua_Integer width = luaL_checkinteger(L, -1);

    // Check parameters
    if(length%width != 0)
        length = length*width;

    // Allocate array
    lua_newuserdata(L, length*type_size);           // [-0, +1, m]

    // Define metatable
    wfc__defineMetatable<T>(L,                      // [-0, +0, m]
                            length,
                            type_size,
                            width);
    
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
