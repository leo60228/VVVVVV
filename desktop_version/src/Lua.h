extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

lua_State* make_lua();
int exec_lua(lua_State* L, const char* code);

#define V6LUA_GAMEPROPS \
    X(gamestate, int)
