#ifndef PARLOUS_WFC_H_INCLUDED
#define PARLOUS_WFC_H_INCLUDED

#define DLIB_LOG_DOMAIN "ParlousArray"

#define REGION_SIZE 0

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

// Using even-r offset (first row shifted right of second row)
#define Y(l,c) (l/c)
#define row(l,c) (l/c)
#define axialRow(q,r) (r)
#define X(l,c) (l%c)
#define col(l,c) (l%c)
#define evenAxialCol(q,r) (q + ((r+1)>>1))
// #define oddAxialCol(q,r) (q - (r + (r&1))/2)
#define even_axial_to_l(w, q, r) (q + ((r+1)>>1) + (w)*(r))
// #define evenAxialCol(q,r) (q + (r + (r&1))/2)
// #define oddAxialCol(q,r) (q - (r + (r&1))/2)
// #define even_axial_to_l(w, q, r) (q + (r + ((r)&1))/2 + (w)*(r))
// #define even_axial_to_l(w, q, r) (q + (w)*(r))


void LuaInitHexMap(lua_State* L);

#endif  // PARLOUS_WFC_H_INCLUDED