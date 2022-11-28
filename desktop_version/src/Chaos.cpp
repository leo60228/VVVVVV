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
    gameScreen.isFiltered = true;
    gameScreen.toggleLinearFilter();
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
        Chaos::AddEffect(SHAKING);
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
        gameScreen.isFiltered = false;
        gameScreen.toggleLinearFilter();
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
        break;
    }
    case SIDEWAYS_FLIPPING:
    {
        int i = obj.getplayer();
        if (INBOUNDS_VEC(i, obj.entities))
        {
            obj.entities[i].w = 21;
            obj.entities[i].h = 12;
            obj.entities[i].tile = 192;
        }
        break;
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
    case TORNADO:
    {
        int i = obj.getplayer();
        if (INBOUNDS_VEC(i, obj.entities))
        {
            // Loop through all entities, and if they're close to the player, push the entity away
            for (int j = 0; j < obj.entities.size(); j++)
            {
                if (j == i) continue;
                int distance = sqrt(pow(obj.entities[i].xp - obj.entities[j].xp, 2) + pow(obj.entities[i].yp - obj.entities[j].yp, 2));
                // Use sin and cos to get the angle between the player and the entity
                float angle = atan2(obj.entities[i].yp - obj.entities[j].yp, obj.entities[i].xp - obj.entities[j].xp);
                obj.entities[j].xp += cos(angle) * 2;
                obj.entities[j].yp += sin(angle) * 2;
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
        break;
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
        break;
    }
    case CYCLING_SPRITES:
    {
        int i = obj.getplayer();
        if (INBOUNDS_VEC(i, obj.entities))
        {
            obj.entities[i].tile++;
            if (obj.entities[i].tile >= graphics.sprites.size())
            {
                obj.entities[i].tile = 0;
            }
        }
        break;
    }
    case SHUFFLE_ENTITIES:
    {
        if (game.framecounter % 30 == 0)
        {
            for (int i = 0; i < obj.entities.size(); i++)
            {
                obj.entities[i].tile = (int)round(fRandom() * (graphics.sprites.size() - 1));
            }
        }
        break;
    }
    case MODIFY_ENTITIES:
    {
        ModifyRandomEntity();
        break;
    }
    case BURST_ENTITIES:
    {
        if (game.framecounter % 30 == 0)
        {
            for (int i = 0; i < 10; i++)
            {
                ModifyRandomEntity();
            }
        }
        break;
    }
    case MOVE_LEFT:
    {
        int i = obj.getplayer();
        if (INBOUNDS_VEC(i, obj.entities))
        {
            obj.entities[i].xp--;
        }
        break;
    }
    case MOVE_RIGHT:
    {
        int i = obj.getplayer();
        if (INBOUNDS_VEC(i, obj.entities))
        {
            obj.entities[i].xp++;
        }
        break;
    }
    case SHAKING:
    {
        int i = obj.getplayer();
        if (INBOUNDS_VEC(i, obj.entities))
        {
            obj.entities[i].xp += (game.framecounter % 2 == 0) ? -1 : 1;
        }
    }
    }
}

void Chaos::ModifyRandomEntity()
{
    int counter = 0;
    int i;
    do {
        i = (int)round(fRandom() * (obj.entities.size() - 1));
        counter++;
        if (counter >= 255)
        {
            return;
        }
    } while (!INBOUNDS_VEC(i, obj.entities) || (obj.entities[i].type == -1) || i == obj.getplayer());

    entclass& entity = obj.entities[i];

    int properties = 30;

    int prop = (int)round(fRandom() * properties);
    if (prop >= properties) prop = (properties - 1);
    switch (prop)
    {
    case 0: entity.actionframe = (int)round(fRandom() * 100); break;
    case 1: entity.animate = (int)round(fRandom() * 100); break;
    case 2: entity.ax = (int)round(fRandom() * 100) - 50; break;
    case 3: entity.ay = (int)round(fRandom() * 100) - 50; break;
    case 4: entity.behave = (int)round(fRandom() * 100); break;
    case 5: entity.colour = (int)round(fRandom() * 100); break;
    case 6: entity.dir = (int)round(fRandom()); break;
    case 7: entity.drawframe = (int)round(fRandom() * 100); break;
    case 8: entity.framedelay = (int)round(fRandom() * 100); break;
    case 9: entity.gravity = fRandom() > 0.5; break;
    case 10: entity.h = (int)round(fRandom() * 100); break;
    case 11: entity.harmful = fRandom() > 0.5;; break;
    case 12: entity.invis = fRandom() > 0.5; break;
    case 13: entity.isplatform = fRandom() > 0.5; break;
    case 14: entity.life = (int)round(fRandom() * 100); break;
    case 15: entity.onentity = (int)round(fRandom() * 100); break;
    case 16: entity.onroof = (int)round(fRandom() * 100); break;
    case 17: entity.onwall = (int)round(fRandom() * 100); break;
    case 18: entity.onxwall = (int)round(fRandom() * 100); break;
    case 19: entity.onywall = (int)round(fRandom() * 100); break;
    case 20: entity.para = (int)round(fRandom() * 100); break;
    case 21: entity.realcol = (int)round(fRandom() * 10000); break;
    case 22: entity.rule = (int)round(fRandom() * 7)+1; break;
    case 23: entity.size = (int)round(fRandom() * 7); break;
    case 24: entity.type = (int)round(fRandom() * 26); break;
    case 25: entity.w = (int)round(fRandom() * 100); break;
    case 26: entity.tile = (int)round(fRandom() * 100); break;
    case 27: entity.state = (int)round(fRandom() * 4); break;
    case 28: entity.statedelay = (int)round(fRandom() * 100); break;
    case 29: entity.walkingframe = (int)round(fRandom() * 100); break;
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
        gameScreen.isFiltered = true;
        gameScreen.toggleLinearFilter();
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
        break;
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
        break;
    }
    case SIDEWAYS_FLIPPING:
    {
        int i = obj.getplayer();
        if (INBOUNDS_VEC(i, obj.entities))
        {
            obj.entities[i].w = 12;
            obj.entities[i].h = 21;
            obj.entities[i].tile = 0;
        }
        break;
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
