#include "Lua.h"
#include "Game.h"
#include <string.h>
#include <stdio.h>

static int make_game(lua_State* L) {
    Game** lua = (Game**) lua_newuserdata(L, sizeof(Game*));
    *lua = &game;

    luaL_getmetatable(L, "v6.game");
    lua_setmetatable(L, -2);

    return 1;
}

static int set_game_prop(lua_State* L) {
    Game** game_ptr = (Game**) lua_touserdata(L, 1);
    const char* name = luaL_checkstring(L, 2);
    int value = luaL_checkinteger(L, 3);

    luaL_argcheck(L, game_ptr != NULL, 1, "`game` expected");
    luaL_argcheck(L, name != NULL, 2, "string expected");

    if (strcmp(name, "gamestate") == 0) {
        (*game_ptr)->gamestate = value;
    } else {
        luaL_argcheck(L, false, 2, "invalid property");
    }

    return 0;
}

static int get_game_prop(lua_State* L) {
    Game** game_ptr = (Game**) lua_touserdata(L, 1);
    const char* name = luaL_checkstring(L, 2);

    luaL_argcheck(L, game_ptr != NULL, 1, "`game` expected");
    luaL_argcheck(L, name != NULL, 2, "string expected");

    if (strcmp(name, "gamestate") == 0) {
        lua_pushinteger(L, (*game_ptr)->gamestate);
        return 1;
    } else {
        luaL_argcheck(L, false, 2, "invalid property");
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
