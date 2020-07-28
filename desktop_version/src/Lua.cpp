#include "Lua.h"
#include <string.h>
#include <stdio.h>

void luaopen_v6(lua_State* L) {
    
}

lua_State* make_lua() {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    luaopen_v6(L);
    return L;
}

int exec_lua(lua_State* L, const char* code) {
    int error = 0;
    error = error || luaL_loadbuffer(L, code, strlen(code), "script");
    error = error || lua_pcall(L, 0, 0, 0);
    if (error) {
        printf("Lua error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }
    return error;
}
