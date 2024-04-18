#ifndef PARLOUS_WFC_H_INCLUDED
#define PARLOUS_WFC_H_INCLUDED

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


void LuaInitHexMap(lua_State* L);

#endif  // PARLOUS_WFC_H_INCLUDED