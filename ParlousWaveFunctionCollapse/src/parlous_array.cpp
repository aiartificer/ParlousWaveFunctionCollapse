// parlous_array.cpp
// Extension lib defines
#define LIB_NAME "ParlousArray"
#define PA_MODULE_NAME "parlous_array"

#include <cassert>
#include <math.h>
#include <string.h>
#include "parlous_array.h"
using namespace std;


template <typename T>
static int newArray(lua_State* L);


template <typename T>
static int add(lua_State* L)                      //// [-0, +1, m]
{
    // Check and collect parameters from stack
    lua_Integer length = lua_tointeger(L, lua_upvalueindex(1));
    lua_Integer type_size = lua_tointeger(L, lua_upvalueindex(2));
    lua_Integer width = lua_tointeger(L, lua_upvalueindex(3));
    T *arrA = (T *)lua_touserdata(L, -2);
    T *arrB = (T *)lua_touserdata(L, -1);

    // Allocate array to hold results
    lua_pushinteger(L, length);                     // [-0, +1, -]
    lua_pushinteger(L, type_size);                  // [-0, +1, -]
    lua_pushinteger(L, width);                      // [-0, +1, -]
    newArray<T>(L);                                 // [-0, +1, m]
    T *arrC = (T *)lua_touserdata(L, -1);
    lua_insert(L, lua_gettop(L) - 3);
    lua_pop(L, 3);                                  // [-3, +0, -]

    // Add value to all elements
    for (lua_Integer i = 0; i < length; i++)
        arrC[i] = arrA[i] + arrB[i];

    // Return 0 items
    return 1;
}

template <typename T>
static int sub(lua_State* L)                      //// [-0, +1, m]
{
    // Check and collect parameters from stack
    lua_Integer length = lua_tointeger(L, lua_upvalueindex(1));
    lua_Integer type_size = lua_tointeger(L, lua_upvalueindex(2));
    lua_Integer width = lua_tointeger(L, lua_upvalueindex(3));
    T *arrA = (T *)lua_touserdata(L, -2);
    T *arrB = (T *)lua_touserdata(L, -1);

    // Allocate array to hold results
    lua_pushinteger(L, length);                     // [-0, +1, -]
    lua_pushinteger(L, type_size);                  // [-0, +1, -]
    lua_pushinteger(L, width);                      // [-0, +1, -]
    newArray<T>(L);                                 // [-0, +1, m]
    T *arrC = (T *)lua_touserdata(L, -1);
    lua_insert(L, lua_gettop(L) - 3);
    lua_pop(L, 3);                                  // [-3, +0, -]

    // Add value to all elements
    for (lua_Integer i = 0; i < length; i++)
        arrC[i] = arrA[i] - arrB[i];

    // Return 0 items
    return 1;
}

template <typename T>
static int mul(lua_State* L)                      //// [-0, +1, m]
{
    // Check and collect parameters from stack
    lua_Integer length = lua_tointeger(L, lua_upvalueindex(1));
    lua_Integer type_size = lua_tointeger(L, lua_upvalueindex(2));
    lua_Integer width = lua_tointeger(L, lua_upvalueindex(3));
    T *arrA = (T *)lua_touserdata(L, -2);
    T *arrB = (T *)lua_touserdata(L, -1);

    // Allocate array to hold results
    lua_pushinteger(L, length);                     // [-0, +1, -]
    lua_pushinteger(L, type_size);                  // [-0, +1, -]
    lua_pushinteger(L, width);                      // [-0, +1, -]
    newArray<T>(L);                                 // [-0, +1, m]
    T *arrC = (T *)lua_touserdata(L, -1);
    lua_insert(L, lua_gettop(L) - 3);
    lua_pop(L, 3);                                  // [-3, +0, -]

    // Add value to all elements
    for (lua_Integer i = 0; i < length; i++)
        arrC[i] = arrA[i]*arrB[i];

    // Return 0 items
    return 1;
}

template <typename T>
static int div(lua_State* L)                      //// [-0, +1, m]
{
    // Check and collect parameters from stack
    lua_Integer length = lua_tointeger(L, lua_upvalueindex(1));
    lua_Integer type_size = lua_tointeger(L, lua_upvalueindex(2));
    lua_Integer width = lua_tointeger(L, lua_upvalueindex(3));
    T *arrA = (T *)lua_touserdata(L, -2);
    T *arrB = (T *)lua_touserdata(L, -1);

    // Allocate array to hold results
    lua_pushinteger(L, length);                     // [-0, +1, -]
    lua_pushinteger(L, type_size);                  // [-0, +1, -]
    lua_pushinteger(L, width);                      // [-0, +1, -]
    newArray<T>(L);                                 // [-0, +1, m]
    T *arrC = (T *)lua_touserdata(L, -1);
    lua_insert(L, lua_gettop(L) - 3);
    lua_pop(L, 3);                                  // [-3, +0, -]

    // Add value to all elements
    for (lua_Integer i = 0; i < length; i++)
        arrC[i] = arrA[i]/arrB[i];

    // Return 0 items
    return 1;
}

template <typename T>
static int mod(lua_State* L)                      //// [-0, +1, m]
{
    // Check and collect parameters from stack
    lua_Integer length = lua_tointeger(L, lua_upvalueindex(1));
    lua_Integer type_size = lua_tointeger(L, lua_upvalueindex(2));
    lua_Integer width = lua_tointeger(L, lua_upvalueindex(3));
    T *arrA = (T *)lua_touserdata(L, -2);
    T *arrB = (T *)lua_touserdata(L, -1);

    // Allocate array to hold results
    lua_pushinteger(L, length);                     // [-0, +1, -]
    lua_pushinteger(L, type_size);                  // [-0, +1, -]
    lua_pushinteger(L, width);                      // [-0, +1, -]
    newArray<T>(L);                                 // [-0, +1, m]
    T *arrC = (T *)lua_touserdata(L, -1);
    lua_insert(L, lua_gettop(L) - 3);
    lua_pop(L, 3);                                  // [-3, +0, -]

    // Add value to all elements
    for (lua_Integer i = 0; i < length; i++)
        arrC[i] = fmod(arrA[i], arrB[i]);

    // Return 0 items
    return 1;
}

template <typename T>
static int pow(lua_State* L)                      //// [-0, +1, m]
{
    // Check and collect parameters from stack
    lua_Integer length = lua_tointeger(L, lua_upvalueindex(1));
    lua_Integer type_size = lua_tointeger(L, lua_upvalueindex(2));
    lua_Integer width = lua_tointeger(L, lua_upvalueindex(3));
    T *arrA = (T *)lua_touserdata(L, -2);
    T *arrB = (T *)lua_touserdata(L, -1);

    // Allocate array to hold results
    lua_pushinteger(L, length);                     // [-0, +1, -]
    lua_pushinteger(L, type_size);                  // [-0, +1, -]
    lua_pushinteger(L, width);                      // [-0, +1, -]
    newArray<T>(L);                                 // [-0, +1, m]
    T *arrC = (T *)lua_touserdata(L, -1);
    lua_insert(L, lua_gettop(L) - 3);
    lua_pop(L, 3);                                  // [-3, +0, -]

    // Add value to all elements
    for (lua_Integer i = 0; i < length; i++)
        arrC[i] = pow(arrA[i], arrB[i]);

    // Return 0 items
    return 1;
}

template <typename T>
static int index_call(lua_State* L)               //// [-0, +1, m]
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
        else
            return luaL_error(L, "Invalid key");
    }
    else if(LUA_TNUMBER == lua_type(L, -1))
        return get<T>(L);                           // [-0, +1, m]
    
    // Return 1 item
    return 1;
}

template <typename T>
static int call(lua_State* L)                     //// [-?, +1, m]
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
static void defineMetatable(lua_State* L,         //// [-0, +0, m]
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
    len_factory(L, length, type_size, width, index_call<T>);   // [-0, +1, m]
    lua_settable(L, -3);                            // [-2, +0, -]

    // Define the __newindex method
    lua_pushstring(L, "__newindex");                // [-0, +1, m]
    len_factory(L, length, type_size, width, put<T>);      // [-0, +1, m]
    lua_settable(L, -3);                            // [-2, +0, -]

    // Define the __add method
    lua_pushstring(L, "__add");                     // [-0, +1, m]
    len_factory(L, length, type_size, width, add<T>);      // [-0, +1, m]
    lua_settable(L, -3);                            // [-2, +0, -]

    // Define the __sub method
    lua_pushstring(L, "__sub");                     // [-0, +1, m]
    len_factory(L, length, type_size, width, sub<T>);      // [-0, +1, m]
    lua_settable(L, -3);                            // [-2, +0, -]

    // Define the __mul method
    lua_pushstring(L, "__mul");                     // [-0, +1, m]
    len_factory(L, length, type_size, width, mul<T>);      // [-0, +1, m]
    lua_settable(L, -3);                            // [-2, +0, -]

    // Define the __div method
    lua_pushstring(L, "__div");                     // [-0, +1, m]
    len_factory(L, length, type_size, width, div<T>);      // [-0, +1, m]
    lua_settable(L, -3);                            // [-2, +0, -]

    // Define the __mod method
    lua_pushstring(L, "__mod");                     // [-0, +1, m]
    len_factory(L, length, type_size, width, mod<T>);      // [-0, +1, m]
    lua_settable(L, -3);                            // [-2, +0, -]

    // Define the __pow method
    lua_pushstring(L, "__pow");                     // [-0, +1, m]
    len_factory(L, length, type_size, width, pow<T>);      // [-0, +1, m]
    lua_settable(L, -3);                            // [-2, +0, -]

    // Define the __call method
    lua_pushstring(L, "__call");                    // [-0, +1, m]
    len_factory(L, length, type_size, width, call<T>); // [-0, +1, m]
    lua_settable(L, -3);                            // [-2, +0, -]

    // Set the metatable
    lua_setmetatable(L, -2);                        // [-1, +0, -]
}

template <typename T>
int newArray(lua_State* L)                        //// [-0, +1, m]
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
    defineMetatable<T>(L,                           // [-0, +0, m]
                       length,
                       type_size,
                       width);
    
    // Return 1 item
    return 1;
}


// ========================
// ====== Lua Stuff =======
// ========================
static const luaL_reg PA_Module_methods[] =
{
    {"new_int_array", newArray<lua_Integer>},
    {"new_num_array", newArray<lua_Number>},
    {0, 0}
};

void LuaInitPArray(lua_State* L)
{
    int top = lua_gettop(L);

    // Register lua names
    luaL_register(L, PA_MODULE_NAME, PA_Module_methods);

    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

