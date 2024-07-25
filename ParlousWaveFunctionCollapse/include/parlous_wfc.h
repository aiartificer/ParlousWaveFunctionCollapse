#ifndef PARLOUS_WFC_H_INCLUDED
#define PARLOUS_WFC_H_INCLUDED

#define REGION_SIZE 5

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

#define MODULE_NAME "parlous_wfc"

#define Y(l,c) (l/c)
#define row(l,c) (l/c)
#define X(l,c) (l%c)
#define col(l,c) (l%c)
#define even_axial_to_l(w, q, r) (q + (r + ((r)&1))/2 + (w)*(r))


void LuaInitHexMap(lua_State* L);

#endif  // PARLOUS_WFC_H_INCLUDED