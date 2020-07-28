extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

lua_State* make_lua();
int exec_lua(lua_State* L, const char* code);

#define V6LUA_GAMEPROPS \
    X(saveFilePath, stdstring) \
    X(door_left, int) \
    X(door_right, int) \
    X(door_up, int) \
    X(door_down, int) \
    X(roomx, int) \
    X(roomy, int) \
    X(roomchangedir, int) \
    X(savex, int) \
    X(savey, int) \
    X(saverx, int) \
    X(savery, int) \
    X(savegc, int) \
    X(savedir, int) \
    X(edsavex, int) \
    X(edsavey, int) \
    X(edsaverx, int) \
    X(edsavery, int) \
    X(edsavegc, int) \
    X(edsavedir, int) \
    X(state, int) \
    X(statedelay, int) \
    X(glitchrunkludge, bool) \
    X(usingmmmmmm, int) \
    X(gamestate, int) \
    X(hascontrol, bool) \
    X(jumpheld, bool) \
    X(jumppressed, int) \
    X(gravitycontrol, int) \
    X(muted, bool) \
    X(mutebutton, int) \
    X(musicmuted, bool) \
    X(musicmutebutton, int) \
    X(tapleft, int) \
    X(tapright, int) \
    X(mapheld, bool) \
    X(menupage, int) \
    X(lastsaved, int) \
    X(deathcounts, int) \
    X(frames, int) \
    X(seconds, int) \
    X(minutes, int) \
    X(hours, int) \
    X(gamesaved, bool) \
    X(savetime, stdstring) \
    X(savearea, stdstring) \
    X(savetrinkets, int) \
    X(startscript, bool) \
    X(newscript, stdstring) \
    X(mainmenu, int) \
    X(menustart, bool) \
    X(teleport_to_new_area, bool) \
    X(teleport_to_x, int) \
    X(teleport_to_y, int) \
    X(teleportscript, stdstring) \
    X(useteleporter, bool) \
    X(teleport_to_teleporter, int) \
/*  X(menuoptions, menuoption_vector) */ \
    X(currentmenuoption, int) \
    X(currentmenuname, MenuName) \
    X(kludge_ingametemp, MenuName) \
    X(current_credits_list_index, int) \
    X(menuxoff, int) \
    X(menuyoff, int) \
    X(menuspacing, int) \
/*  X(menustack, menustackframe_vector) */ \
    X(menucountdown, int) \
    X(menudest, MenuName) \
    X(creditposx, int) \
    X(creditposy, int) \
    X(creditposdelay, int) \
    X(oldcreditposx, int) \
    X(swnmode, bool) \
    X(swngame, int) \
    X(swnstate, int) \
    X(swnstate2, int) \
    X(swnstate3, int) \
    X(swnstate4, int) \
    X(swndelay, int) \
    X(swndeaths, int) \
    X(swntimer, int) \
    X(swncolstate, int) \
    X(swncoldelay, int) \
    X(swnrecord, int) \
    X(swnbestrank, int) \
    X(swnrank, int) \
    X(swnmessage, int) \
    X(supercrewmate, bool) \
    X(scmhurt, bool) \
    X(scmmoveme, bool) \
    X(scmprogress, int) \
    X(colourblindmode, bool) \
    X(noflashingmode, bool) \
    X(slowdown, int) \
    X(gameframerate, uint32) \
    X(nodeathmode, bool) \
    X(gameoverdelay, int) \
    X(nocutscenes, bool) \
    X(intimetrial, bool) \
    X(timetrialparlost, bool) \
    X(timetrialcountdown, int) \
    X(timetrialshinytarget, int) \
    X(timetriallevel, int) \
    X(timetrialpar, int) \
    X(timetrialresulttime, int) \
    X(timetrialresultframes, int) \
    X(timetrialrank, int) \
    X(creditposition, int) \
    X(oldcreditposition, int) \
    X(insecretlab, bool) \
    X(inintermission, bool) \
/*  X(crewstats, crewstats) */ \
    X(alarmon, bool) \
    X(alarmdelay, int) \
    X(blackout, bool) \
/*  X(tele_crewstats, crewstats) */ \
/*  X(quick_crewstats, crewstats) */ \
/*  X(unlock, unlocks) */ \
/*  X(unlocknotify, unlocks) */ \
    X(stat_trinkets, int) \
    X(fullscreen, bool) \
    X(bestgamedeaths, int) \
/*  X(besttimes, trialbests) */ \
/*  X(bestframes, trialbests) */ \
/*  X(besttrinkets, trialbests) */ \
/*  X(bestlives, trialbests) */ \
/*  X(bestrank, trialbests) */ \
    X(tele_gametime, stdstring) \
    X(tele_trinkets, int) \
    X(tele_currentarea, stdstring) \
    X(quick_gametime, stdstring) \
    X(quick_trinkets, int) \
    X(quick_currentarea, stdstring) \
    X(mx, int) \
    X(my, int) \
    X(screenshake, int) \
    X(flashlight, int) \
    X(advancetext, bool) \
    X(pausescript, bool) \
    X(deathseq, int) \
    X(lifeseq, int) \
    X(savepoint, int) \
    X(teleportxpos, int) \
    X(teleport, bool) \
    X(edteleportent, int) \
    X(completestop, bool) \
    X(inertia, float) \
    X(companion, int) \
    X(roomchange, bool) \
/*  X(teleblock, rect) */ \
    X(activetele, bool) \
    X(readytotele, int) \
    X(oldreadytotele, int) \
    X(activity_r, int) \
    X(activity_g, int) \
    X(activity_b, int) \
    X(activity_lastprompt, stdstring) \
    X(telesummary, stdstring) \
    X(quicksummary, stdstring) \
    X(customquicksummary, stdstring) \
    X(backgroundtext, bool) \
    X(activeactivity, int) \
    X(act_fade, int) \
    X(prev_act_fade, int) \
    X(press_left, bool) \
    X(press_right, bool) \
    X(press_action, bool) \
    X(press_map, bool) \
    X(totalflips, int) \
    X(hardestroom, stdstring) \
    X(hardestroomdeaths, int) \
    X(currentroomdeaths, int) \
    X(savemystats, bool) \
    X(fullScreenEffect_badSignal, bool) \
    X(useLinearFilter, bool) \
    X(stretchMode, int) \
    X(controllerSensitivity, int) \
/*  X(customscript, customscript) */ \
    X(customcol, int) \
    X(levelpage, int) \
    X(playcustomlevel, int) \
    X(customleveltitle, stdstring) \
    X(customlevelfilename, stdstring) \
/*  X(customlevelstats, customlevelstats) */ \
    X(customlevelstatsloaded, bool) \
/*  X(controllerButton_map, buttons) */ \
/*  X(controllerButton_flip, buttons) */ \
/*  X(controllerButton_esc, buttons) */ \
    X(skipfakeload, bool) \
    X(ghostsenabled, bool) \
    X(cliplaytest, bool) \
    X(playx, int) \
    X(playy, int) \
    X(playrx, int) \
    X(playry, int) \
    X(playgc, int) \
    X(playmusic, int) \
    X(playassets, stdstring) \
    X(fadetomenu, bool) \
    X(fadetomenudelay, int) \
    X(fadetolab, bool) \
    X(fadetolabdelay, int) \
    X(shouldreturntoeditor, bool) \
    X(gametimer, int) \
    X(over30mode, bool) \
    X(glitchrunnermode, bool) \
    X(ingame_titlemode, bool) \
    X(shouldreturntopausemenu, bool) \
    X(disablepause, bool)
