#include "Lua.h"
#include "Game.h"
#include <string>
#include <string.h>
#include <stdio.h>

#define push_lua_int lua_pushinteger
#define pop_lua_int luaL_checkinteger

#define push_lua_bool lua_pushboolean
#define pop_lua_bool lua_toboolean

#define push_lua_float lua_pushnumber
#define pop_lua_float lua_tonumber

#define push_lua_uint32 lua_pushinteger
#define pop_lua_uint32 luaL_checkinteger

#define push_lua_MenuName(L, val) lua_pushinteger(L, (Menu::MenuName) val)
#define pop_lua_MenuName(L, pos) ((Menu::MenuName) luaL_checkinteger(L, pos))

static void push_lua_stdstring(lua_State* L, std::string s) {
    lua_pushstring(L, s.c_str());
}

static std::string pop_lua_stdstring(lua_State* L, int pos) {
    const char* cstr = luaL_checkstring(L, pos);
    std::string str = cstr;
    return str;
}

#define V6LUA_PUSHTYPE(L, type, val) (push_lua_ ## type(L, val))
#define V6LUA_POPTYPE(L, type, pos) (pop_lua_ ## type(L, pos))

#define __V6LUA_STRINGIFY_INTERNAL(s) str(s)
#define V6LUA_STRINGIFY(s) #s

static int make_game(lua_State* L) {
    lua_newuserdatauv(L, 0, 0);
    luaL_getmetatable(L, "v6.game");
    lua_setmetatable(L, -2);

    return 1;
}

static int set_game_prop(lua_State* L) {
    luaL_checkudata(L, 1, "v6.game");
    const char* name = luaL_checkstring(L, 2);

#define X(NAME, TYPE) if (strcmp(name, V6LUA_STRINGIFY(NAME)) == 0) { \
    game.NAME = V6LUA_POPTYPE(L, TYPE, 3); \
} else

    V6LUA_GAMEPROPS
#undef X
    {
        luaL_argerror(L, 2, "invalid property");
    }

    return 0;
}

static int get_game_prop(lua_State* L) {
    luaL_checkudata(L, 1, "v6.game");
    const char* name = luaL_checkstring(L, 2);

#define X(NAME, TYPE) if (strcmp(name, V6LUA_STRINGIFY(NAME)) == 0) { \
    V6LUA_PUSHTYPE(L, TYPE, game.NAME); \
    return 1; \
} else

    V6LUA_GAMEPROPS
#undef X
    {
        luaL_argerror(L, 2, "invalid property");
        return 0;
    }
}

static const struct luaL_Reg game_funcs[] = {
    {"__index", get_game_prop},
    {"__newindex", set_game_prop},
    {NULL, NULL}
};

static void luaopen_v6(lua_State* L) {
    luaL_newmetatable(L, "v6.game");
    luaL_setfuncs(L, game_funcs, 0);

    lua_createtable(L, 0, 1);
    lua_pushstring(L, "game");
    make_game(L);
    lua_settable(L, -3);
    lua_setglobal(L, "v6");
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
