#include <math.h> 
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <exception>

#include "Chaos.h"
#include "Screen.h"
#include "Entity.h"
#include "Maths.h"
#include "Game.h"
#include "Graphics.h"
#include "UtilityClass.h"
#include "Vlogging.h"
#include "Map.h"

namespace Chaos
{
    int waitTime;
    std::vector<ActiveEffect> activeEffects;
}

void Chaos::Initialize()
{
    Chaos::waitTime = -1;
    gameScreen.badSignalEffect = false;
    gameScreen.isFiltered = false;
    graphics.flipmode = false;
}

void Chaos::AddEffect(Effects effect)
{
    ActiveEffect newEffect;
    newEffect.timeRemaining = (int) round(fRandom() * (MAX_EFFECT_TIME - MIN_EFFECT_TIME) + MIN_EFFECT_TIME);
    newEffect.effect = effect;
    activeEffects.push_back(newEffect);
    ApplyEffect(newEffect);
    vlog_info("Effect %i applied", (int) effect);
}

void Chaos::ProcessEffects()
{
    if (game.gamestate != GAMEMODE && game.gamestate != MAPMODE && game.gamestate != TELEPORTERMODE) return;

    if (waitTime == -1)
    {
        Chaos::AddEffect(PUSH_AWAY);
        Chaos::waitTime = INITIAL_WAIT_TIME;
    }


    waitTime--;
    if (waitTime <= 0)
    {
        waitTime = (int) round(fRandom() * (MAX_WAIT_TIME - MIN_WAIT_TIME) + MIN_WAIT_TIME);
        AddEffect((Effects) (int) round(fRandom() * (EFFECT_AMOUNT - 1)));
    }
    for (int i = 0; i < activeEffects.size(); i++)
    {
        UpdateEffect(activeEffects[i]);
        activeEffects[i].timeRemaining--;
        if (activeEffects[i].timeRemaining <= 0)
        {
            RemoveEffect(activeEffects[i]);
            activeEffects.erase(activeEffects.begin() + i);
            i--;
        }
    }
}


void Chaos::ApplyEffect(ActiveEffect effect)
{
    switch (effect.effect)
    {
    case RANDOM_COLOR:
    {
        // 0-23, 30-40, 100, 101, 102
        int rand = (int)round(fRandom() * (23 + 10 + 3));
        int color = rand;
        if (rand >= 23)
        {
            color = 30 + (color - 23);
        }
        if (rand >= 40)
        {
            color = 100 + (color - 40);
        }

        game.savecolour = color;

        int i = obj.getplayer();

        if (INBOUNDS_VEC(i, obj.entities))
        {
            obj.entities[i].colour = color;
        }
    }
    break;
    case BUS:
    {
        obj.createentity(0, -200, 1, 16, 12, -64, -500, 320 + 64, 340);
        for (int i = 0; i < obj.entities.size(); i++)
        {
            if (obj.entities[i].behave == 16)
            {
                obj.entities[i].colour = 6;
                obj.entities[i].size = 9;
                obj.entities[i].w = 64;
                obj.entities[i].h = 44;
                obj.entities[i].animate = 4;
            }
        }
        break;
    }
    case FLIP_MODE:
        graphics.flipmode = true;
        break;
    case GRAVITATION_POTION:
        graphics.flipmode = false;
        break;
    case ANALOGUE:
        gameScreen.badSignalEffect = true;
        break;
    case BILINEAR:
        gameScreen.isFiltered = true;
        break;
    case PLAYER_SPRITE:
    {
        int i = obj.getplayer();
        if (INBOUNDS_VEC(i, obj.entities))
        {
            obj.entities[i].tile = (int)round(fRandom() * (graphics.sprites.size() - 1));
        }
        break;
    }
    case GAMESTATE:
    {
        std::vector<int> validStates;
        for (int i = 1; i < 18; i++)
        {
            if (i == 9 || i == 10) continue;
            validStates.push_back(i);
        }

        validStates.push_back(22);
        validStates.push_back(30);
        validStates.push_back(31);
        validStates.push_back(32);

        for (int i = 47; i < 57; i++)
        {
            validStates.push_back(i);
        }

        for (int i = 115; i <= 118; i++)
        {
            validStates.push_back(i);
        }

        validStates.push_back(132);

        for (int i = 2500; i <= 2514; i++)
        {
            validStates.push_back(i);
        }

        // Pick a state
        game.state = validStates[(int)round(fRandom() * (validStates.size() - 1))];
        vlog_info("Entering gamestate %i", (int)game.state);
        break;
    }
    case FAKE_TRINKET:
    {
        game.state = 1000;
        vlog_info("Entering gamestate %i", (int)game.state);
        break;
    }
    case VVVVVVMAN:
    {
        int i = obj.getplayer();
        if (INBOUNDS_VEC(i, obj.entities))
        {
            obj.entities[i].xp = 30;
            obj.entities[i].yp = 46;
            obj.entities[i].size = 13;
            obj.entities[i].colour = 23;
            obj.entities[i].cx = 36;// 6;
            obj.entities[i].cy = 12 + 80;// 2;
            obj.entities[i].h = 126 - 80;// 21;
        }
    }
    }
}

void Chaos::UpdateEffect(ActiveEffect effect)
{
    switch (effect.effect)
    {
    case GRAVITATION_POTION:
        graphics.flipmode = (game.gravitycontrol == 1);
        break;
    case PUSH_AWAY:
    {
        int i = obj.getplayer();
        if (INBOUNDS_VEC(i, obj.entities))
        {
            // Loop through all entities, and if they're close to the player, push the entity away
            for (int j = 0; j < obj.entities.size(); j++)
            {
                if (j == i) continue;
                int distance = sqrt(pow(obj.entities[i].xp - obj.entities[j].xp, 2) + pow(obj.entities[i].yp - obj.entities[j].yp, 2));
                if (distance < 32)
                {
                    // Use sin and cos to get the angle between the player and the entity
                    float angle = atan2(obj.entities[i].yp - obj.entities[j].yp, obj.entities[i].xp - obj.entities[j].xp);
                    obj.entities[j].xp -= cos(angle) * (32 - distance);
                    obj.entities[j].yp -= sin(angle) * (32 - distance);
                }
            }
        }
        break;
    }
    case RANDOM_BLOCK:
    {
        if (fRandom() < 0.175)
        {
            int x = (int)round(fRandom() * 39);
            int y = (int)round(fRandom() * 29);
            map.settile(x, y, (int)round(fRandom() * 1199));
            graphics.foregrounddrawn = false;
        }
    }
    case WARP_DIR:
    {
        if (game.framecounter % 60 == 0)
        {
            graphics.backgrounddrawn = false;
            map.warpx = false;
            map.warpy = false;
            map.background = 1 + (int)round(fRandom() * 6);
            if (map.background == 7) map.background = 6;

            if (map.background == 3) {
                map.warpx = true;
            }
            if (map.background == 4) {
                map.warpy = true;
            }
            if (map.background == 5) {
                map.warpx = true;
                map.warpy = true;
            }
        }
    }
    }
}

void Chaos::RemoveEffect(ActiveEffect effect)
{
    switch (effect.effect)
    {
    case RANDOM_COLOR:
        game.savecolour = 0;
        break;
    case FLIP_MODE:
    case GRAVITATION_POTION:
        graphics.flipmode = false;
        break;
    case ANALOGUE:
    {
        gameScreen.badSignalEffect = false;
        break;
    }
    case BILINEAR:
    {
        gameScreen.isFiltered = false;
        break;
    }
    case PLAYER_SPRITE:
    {
        int i = obj.getplayer();
        if (INBOUNDS_VEC(i, obj.entities))
        {
            obj.entities[i].tile = 0;
        }
        break;
    }
    case WARP_DIR:
    {
        graphics.backgrounddrawn = false;
        map.warpx = false;
        map.warpy = false;
        map.background = 1 + (int)round(fRandom() * 6);
    }
    case VVVVVVMAN:
    {
        int i = obj.getplayer();
        if (INBOUNDS_VEC(i, obj.entities))
        {
            obj.entities[i].xp = 100;
            obj.entities[i].size = 0;
            obj.entities[i].colour = 0;
            obj.entities[i].cx = 6;
            obj.entities[i].cy = 2;
            obj.entities[i].h = 21;
        }
    }
    }
}

bool Chaos::IsActive(Effects effect)
{
    for (int i = 0; i < activeEffects.size(); i++)
    {
        if (activeEffects[i].effect == effect)
        {
            return true;
        }
    }
    return false;
}
