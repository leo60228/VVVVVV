#include <math.h> 
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <exception>

#include "Chaos.h"
#include "Entity.h"
#include "Game.h"
#include "Graphics.h"
#include "Map.h"
#include "Maths.h"
#include "Music.h"
#include "Screen.h"
#include "UtilityClass.h"
#include "Vlogging.h"


static void ChangePlayerSize()
{
    int i = obj.getplayer();
    if (INBOUNDS_VEC(i, obj.entities))
    {
        obj.entities[i].colour = 0;
        obj.entities[i].size = 14;
        obj.entities[i].w = 21 * (fRandom() * 5);
        obj.entities[i].h = 12 * (fRandom() * 7);
        obj.entities[i].cx = 6 * ((float)obj.entities[i].w / 12);
        obj.entities[i].cy = 2 * ((float)obj.entities[i].h / 21);
        obj.entities[i].yp += 21 - obj.entities[i].h;
        obj.entities[i].xp += 12 - obj.entities[i].w;
    }
}

namespace Chaos
{
    int waitTime;
    std::vector<ActiveEffect> active_effects;
    std::vector<CloneInfo> clone_info;
    std::vector<CloneInfo> dash_trail;
    int clone_timer;
    int clone_count;
    int random_sprite;
    bool reloading;
    bool random_effects;
    int last_dir;
    int chaos_option;
    int chaos_scroll;
    bool in_chaos_menu;

    const char* effect_ids[EFFECT_AMOUNT];
    const char* effect_names[EFFECT_AMOUNT];
    const char* effect_descriptions[EFFECT_AMOUNT];
}

static void createLookupTable(const char* ids[EFFECT_AMOUNT], const char* names[EFFECT_AMOUNT], const char* descriptions[EFFECT_AMOUNT])
{
#define INFO(ENUM, ID, NAME, DESC) ids[ENUM] = ID; names[ENUM] = NAME; descriptions[ENUM] = DESC;
    INFO(NO_MAP, "no_map", "No map", "Disables the map");
    INFO(RANDOM_COLOR, "random_color", "Random color", "Changes Viridian's color");
    INFO(JUMPING, "jumping", "Jumping", "Enables jumping");
    INFO(BUS, "bus", "Bus", "A bus appears to run you over");
    INFO(FLIP_MODE, "flip_mode", "Flip mode", "Enables flip mode");
    INFO(RANDOM_BLOCK, "random_block", "Random block", "Adds random blocks");
    INFO(WARP_DIR, "warp_dir", "Warp direction", "Randomizes the warp background");
    INFO(GAMESTATE, "gamestate", "Gamestate", "Runs a random (safe) gamestate");
    INFO(PLAYER_SPRITE, "player_sprite", "Random player sprite", "Changes the player's sprites to something random");
    INFO(REVERSE_CONTROLS, "reverse_controls", "Reverse controls", "Reverses controls");
    INFO(INFINIFLIP, "infiniflip", "Infiniflip", "Allows flipping in the air");
    INFO(GRAVITATION_POTION, "gravitation_potion", "Gravitation potion", "Flip mode depending on the player's gravity");
    INFO(RANDOM_FLIPPING, "random_flipping", "Random flipping", "Randomly presses the flip button");
    INFO(ANALOGUE, "analogue", "Analogue", "Enables analogue mode");
    INFO(BILINEAR, "bilinear", "Bilinear", "Enables bilinear scaling");
    INFO(HOLDING_RIGHT, "holding_right", "Holding right", "Holds down right");
    INFO(FAKE_TRINKET, "fake_trinket", "Fake trinket", "Shows the collected trinket dialogue, without collecting a trinket");
    INFO(PUSH_AWAY, "push_away", "Push away", "Pushes entities away from Viridian");
    INFO(VVVVVVMAN, "vvvvvvman", "VVVVVVMAN", "Makes Viridian huge");
    INFO(ICE, "ice", "Ice", "Adds ice physics");
    INFO(SOLITAIRE, "solitaire", "Solitaire", "Stops drawing the background, for the solitaire effect");
    INFO(RANDOM_ACCEL, "random_accel", "Random accel", "Changes acceleration to be random");
    INFO(UNCAPPED_SPEED, "uncapped_speed", "Uncapped speed", "Uncaps all acceleration in the game");
    INFO(CYCLING_SPRITES, "cycling_sprites", "Cycling sprites", "Cycles through entity sprites");
    INFO(SHUFFLE_ENTITIES, "shuffle_entities", "Shuffle entities", "Shuffles entity sprites");
    INFO(MODIFY_ENTITIES, "modify_entities", "Modify entities", "Do random things to an entity every frame");
    INFO(BURST_ENTITIES, "burst_entities", "Burst entities", "Does random things to 10 entities every second");
    INFO(MOVE_LEFT, "move_left", "Move left", "Slowly moves the player left");
    INFO(MOVE_RIGHT, "move_right", "Move right", "Slowly moves the player right");
    INFO(SIDEWAYS_FLIPPING, "sideways_flipping", "Sideways flipping", "Flips Viridian on their side");
    INFO(TORNADO, "tornado", "Tornado", "Draws every entity towards Viridian");
    INFO(SHAKING, "shaking", "Shaking", "Makes Viridian shake");
    INFO(FLAG, "flag", "Flag", "Toggles a random flag");
    INFO(RANDOM_SIZE, "random_size", "Random size", "Changes Viridian's size randomly");
    INFO(COSMIC_CLONES, "cosmic_clones", "Cosmic clones", "Spawns clones of Viridian which follow you and hurt you");
    INFO(RANDOM_SPRITE, "random_sprite", "Random sprite", "Makes Viridian look like a different object");
    INFO(FLIP_TELEPORT, "flip_teleport", "Flip teleport", "Teleports slightly when you flip");
    INFO(BLINKING_VIRIDIAN, "blinking_viridian", "Blinking Viridian", "Makes Viridian blink");
    INFO(BLINKING_ENEMIES, "blinking_enemies", "Blinking enemies", "Makes enemies blink");
    INFO(INVINCIBILITY, "invincibility", "Invincibility", "Makes Viridian invincible (without making spikes solid)");
    INFO(TRINKETS_KILL, "trinkets_kill", "Trinkets kill", "Trinkets hurt the player");
    INFO(ROOM_EXPLODE, "room_explode", "Room explode", "Random explosions in the room every now and then");
    INFO(RANDOM_MUSIC, "random_music", "Random music", "Randomizes room music");
    INFO(GOOFY_AAH, "goofy_aah", "Goofy aah", "goofy ahh vvvvvv");
    INFO(NOCLIP, "noclip", "Noclip", "Allows Viridian to fly around and go through walls");
    INFO(CANT_STOP, "cant_stop", "Can't stop", "You can't stop moving");
    INFO(HORIZONTAL_FLIP, "horizontal_flip", "Horizontal flip", "Horizontally flips the screen");
    INFO(DOWNSCALED, "downscaled", "Downscaled", "Downscales the screen");
    INFO(ZOOMED, "zoomed", "Zoomed", "Zooms in on Viridian");
    INFO(VIGNETTE, "vignette", "Vignette", "Draws a vignette around Viridian");
    INFO(CONVEYORS_OFF, "conveyors_off", "Conveyors off", "Turns off conveyors");
    INFO(ASKEW, "askew", "Askew", "Makes the screen askew");
#undef INFO
}

const char* Chaos::getEffectID(Effects effect)
{
    return effect_ids[effect];
}

Effects Chaos::getEffectFromID(const char* id)
{
    for (int i = 0; i < EFFECT_AMOUNT; i++)
    {
        if (strcmp(id, effect_ids[i]) == 0)
        {
            return (Effects)i;
        }
    }
    return INVALID;
}

const char* Chaos::getEffectName(Effects effect)
{
    return effect_names[effect];
}

const char* Chaos::getEffectDescription(Effects effect)
{
    return effect_descriptions[effect];
}

void Chaos::Initialize()
{
    waitTime = -1;
    gameScreen.badSignalEffect = false;
    gameScreen.isFiltered = true;
    gameScreen.toggleLinearFilter();
    graphics.flipmode = false;
    reloading = false;
    random_effects = true;
    dash_trail.clear();
    clone_info.clear();
    last_dir = 0;
    chaos_option = 0;
    chaos_scroll = 0;
    in_chaos_menu = false;

    createLookupTable(effect_ids, effect_names, effect_descriptions);

    //random_effects = false;
    //Chaos::AddEffect(RANDOM_SIZE, true);
    //Chaos::AddEffect(ZOOMED, true);
    //Chaos::AddEffect(ASKEW, true);
}

void Chaos::OnPlayerReset()
{
    for (int i = 0; i < active_effects.size(); i++)
    {
        ActiveEffect effect = active_effects[i];
        switch (effect.effect)
        {
        case RANDOM_SIZE:
            ChangePlayerSize();
            break;
        }
    }
}

void Chaos::AddEffect(Effects effect, bool infinite)
{
    // check if the effect already exists

    for (int i = 0; i < active_effects.size(); i++)
    {
        if (active_effects[i].effect == effect)
        {
            active_effects[i].timeRemaining = SDL_max(active_effects[i].timeRemaining, (int)round(fRandom() * (MAX_EFFECT_TIME - MIN_EFFECT_TIME) + MIN_EFFECT_TIME));
            active_effects[i].timer = 0;
            //active_effects[i].infinite = infinite;
            return;
        }
    }

    ActiveEffect newEffect;
    newEffect.timeRemaining = (int)round(fRandom() * (MAX_EFFECT_TIME - MIN_EFFECT_TIME) + MIN_EFFECT_TIME);

    newEffect.infinite = infinite;
    newEffect.effect = effect;
    newEffect.timer = 0;
    active_effects.push_back(newEffect);
    ApplyEffect(newEffect);
}

void Chaos::AddEffect(ActiveEffect effect)
{
    active_effects.push_back(effect);
    ApplyEffect(effect);
}

void Chaos::ProcessEffects()
{
    if (game.gamestate != GAMEMODE && game.gamestate != MAPMODE && game.gamestate != TELEPORTERMODE) return;

    if (waitTime == -1)
    {
        waitTime = INITIAL_WAIT_TIME;
    }

    waitTime--;
    if (waitTime <= 0)
    {
        waitTime = (int) round(fRandom() * (MAX_WAIT_TIME - MIN_WAIT_TIME) + MIN_WAIT_TIME);
        if (random_effects)
        {
            AddEffect((Effects)(int)round(fRandom() * (EFFECT_AMOUNT - 1)), false);
        }
    }
    for (int i = 0; i < active_effects.size(); i++)
    {
        UpdateEffect(active_effects[i]);
        active_effects[i].timer--;
        if (!active_effects[i].infinite)
        {
            active_effects[i].timeRemaining--;
        }
        if (active_effects[i].timeRemaining <= 0)
        {
            RemoveEffect(active_effects[i]);
            active_effects.erase(active_effects.begin() + i);
            i--;
        }
    }
}


void Chaos::ApplyEffect(ActiveEffect& effect)
{
    switch (effect.effect)
    {
    case INFINIFLIP:
        graphics.createtextbox(" Infiniflip is ON ", -1, 3, 192, 100, 255);
        graphics.textboxtimer(45);
        break;
    case TRANSLUCENT_WINDOW:
    {
        SDL_SetWindowOpacity(gameScreen.m_window, 0.5f);
        break;
    }
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
            obj.entities[i].tile = (int)round(fRandom() * (graphics.sprites_surf.size() - 1));
        }
        break;
    }
    case GAMESTATE:
    {
        std::vector<int> validStates;
        for (int i = 1; i < 18; i++)
        {
            if (i == 5 || i == 9 || i == 10) continue;
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

        for (int i = 4000; i <= 4069; i++)
        {
            validStates.push_back(i);
        }

        // Pick a state
        game.state = validStates[(int)round(fRandom() * (validStates.size() - 1))];

        graphics.createtextbox(" Running gamestate " + std::to_string(game.state) + " ", -1, 3, 192, 100, 255);
        graphics.textboxtimer(45);
        break;
    }
    case FAKE_TRINKET:
    {
        waitTime = SDL_max(waitTime - 15, MIN_WAIT_TIME);
        effect.timeRemaining = 0;
        game.state = 1000;
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
    case RANDOM_SIZE:
    {
        ChangePlayerSize();
        break;
    }
    case SIDEWAYS_FLIPPING:
    {
        int i = obj.getplayer();
        if (INBOUNDS_VEC(i, obj.entities))
        {
            obj.entities[i].w = 21;
            obj.entities[i].h = 12;
            if (obj.entities[i].tile == 0)
            {
                obj.entities[i].tile = 192;
            }
            else if (obj.entities[i].tile == 144)
            {
                obj.entities[i].tile = 228;
            }
        }
        break;
    }
    case FLAG:
    {
        waitTime = SDL_max(waitTime - 15, MIN_WAIT_TIME);
        effect.timeRemaining = 0;
        // Pick a random flag to turn on or off
        int flag = (int)round(fRandom() * 99);
        bool on = (int)round(fRandom()) == 1;

        obj.flags[flag] = on;

        graphics.createtextbox(" Flag " + std::to_string(flag) + (on ? " ON " : " OFF "), -1, 3, 192, 100, 255);
        graphics.textboxtimer(45);

        break;
    }
    case COSMIC_CLONES:
    {
        clone_info.clear();
        clone_timer = 30;
        clone_count = 0;
        break;
    }
    case RANDOM_SPRITE:
    {
        random_sprite = (int)round(fRandom() * 3);
        break;
    }
    case BUS:
    {
        waitTime = SDL_max(waitTime - 15, MIN_WAIT_TIME);
        effect.timeRemaining = 0;
        // Check if there's already one in the room
        for (int i = 0; i < obj.entities.size(); i++)
        {
            if (obj.entities[i].behave == 16)
            {
                return;
            }
        }

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
    }
}

void Chaos::UpdateEffect(ActiveEffect& effect)
{
    switch (effect.effect)
    {
    case CONVEYORS_OFF:
    {
        for (size_t i = 0; i < obj.entities.size(); i++)
        {
            if (obj.entities[i].behave >= 8 && obj.entities[i].behave < 10)
            {
                if (obj.entities[i].para > 0)
                {
                    obj.entities[i].para--;
                }
                else if (obj.entities[i].para < 0)
                {
                    obj.entities[i].para++;
                }
            }
        }
        break;
    }
    case ROOM_EXPLODE:
    {
        if ((effect.timer < 0) && (fRandom() < 0.5))
        {
            effect.timer = (int)round(fRandom() * 60) + 30;
            int x = (int)round(fRandom() * 39);
            int y = (int)round(fRandom() * 29);

            int r = (int)round(fRandom() * 6) + 6;

            music.playef(23);
            game.screenshake = r;

            // Around X and Y, use settile to place a filled circle of tile 0.

            std::vector<SDL_Point> positions;

            for (int i = 0; i < 360; i++)
            {
                for (int j = 0; j < r; j++)
                {
                    int x2 = (int)round(x + j * cos(i * M_PI / 180));
                    int y2 = (int)round(y + j * sin(i * M_PI / 180));

                    bool include = true;
                    for (int k = 0; k < positions.size(); k++)
                    {
                        if (positions[k].x == x2 && positions[k].y == y2) include = false;
                    }

                    if (!include) continue;

                    if ((j == r - 5) && (fRandom() < 0.25)) include = false;
                    if ((j == r - 4) && (fRandom() < 0.50)) include = false;
                    if ((j == r - 3) && (fRandom() < 0.50)) include = false;
                    if ((j == r - 2) && (fRandom() < 0.75)) include = false;
                    if ((j == r - 1) && (fRandom() < 0.75)) include = false;

                    SDL_Point p;
                    p.x = x2;
                    p.y = y2;
                    positions.push_back(p);

                    if (include)
                    {
                        map.settile(x2, y2, 0);
                    }
                }
            }
            graphics.foregrounddrawn = false;
        }
        break;
    }

    case TRINKETS_KILL:
    {
        for (size_t i = 0; i < obj.entities.size(); i++)
        {
            if (obj.entities[i].type == 7)
            {
                obj.entities[i].harmful = true;
            }
        }
        break;
    }
    case BLINKING_VIRIDIAN:
    {
        int i = obj.getplayer();
        if (INBOUNDS_VEC(i, obj.entities))
        {
            obj.entities[i].invis = ((game.framecounter % 30) < 15);
        }
        break;
    }
    case BLINKING_ENEMIES:
    {
        for (size_t i = 0; i < obj.entities.size(); i++)
        {
            if (obj.entities[i].harmful)
            {
                obj.entities[i].invis = ((game.framecounter % 30) < 15);
            }
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
            if (obj.entities[i].tile == 0)
            {
                obj.entities[i].tile = 192;
            }
            else if (obj.entities[i].tile == 144)
            {
                obj.entities[i].tile = 216;
            }
        }
        break;
    }
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
            if (obj.entities[i].tile >= graphics.sprites_surf.size())
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
                obj.entities[i].tile = (int)round(fRandom() * (graphics.sprites_surf.size() - 1));
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
            (Chaos::IsActive(SIDEWAYS_FLIPPING) ? obj.entities[i].yp : obj.entities[i].xp)--;
        }
        break;
    }
    case MOVE_RIGHT:
    {
        int i = obj.getplayer();
        if (INBOUNDS_VEC(i, obj.entities))
        {
            (Chaos::IsActive(SIDEWAYS_FLIPPING) ? obj.entities[i].yp : obj.entities[i].xp)++;
        }
        break;
    }
    case SHAKING:
    {
        int i = obj.getplayer();
        if (INBOUNDS_VEC(i, obj.entities))
        {
            (Chaos::IsActive(SIDEWAYS_FLIPPING) ? obj.entities[i].yp : obj.entities[i].xp) += (game.framecounter % 2 == 0) ? -1 : 1;
        }
        break;
    }
    case COSMIC_CLONES:
    {
        int i = obj.getplayer();
        if (INBOUNDS_VEC(i, obj.entities))
        {
            CloneInfo clone;
            clone.rx = game.roomx - 100;
            clone.ry = game.roomy - 100;
            clone.x = obj.entities[i].xp;
            clone.y = obj.entities[i].yp;
            clone.frame = obj.entities[i].drawframe;
            clone.visible = !obj.entities[i].invis;
            clone.time = 0;
            clone_info.insert(clone_info.begin(), clone);
        }

        clone_timer--;
        if (clone_timer <= 0)
        {
            clone_timer += 30;
            clone_count++;
            if (clone_count <= CLONE_COUNT) {
                obj.createentity(-100, -100, 200, clone_count);
            }
        }

        while (clone_info.size() > (CLONE_COUNT * CLONE_OFFSET) + 2) // 2 for leniency
        {
            clone_info.pop_back();
        }

        for (int i = 0; i < obj.entities.size(); i++)
        {
            if (obj.entities[i].behave > 0 && obj.entities[i].rule == 10)
            {
                obj.entities[i].invis = false;
                if (obj.entities[i].behave == 2) obj.entities[i].invis = true;
                if (obj.entities[i].behave == 6) obj.entities[i].invis = true;
                if (obj.entities[i].behave >= 8) obj.entities[i].invis = true;

                obj.entities[i].behave--;
                if (obj.entities[i].behave <= 0)
                {
                    obj.entities[i].invis = false;
                }
            }
        }
        break;
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
    case 21: entity.realcol = { (unsigned char)round(fRandom() * 255), (unsigned char)round(fRandom() * 255), (unsigned char)round(fRandom() * 255) }; break;
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

void Chaos::RemoveEffect(Effects effect)
{
    for (int i = 0; i < active_effects.size(); i++)
    {
        if (active_effects[i].effect == effect)
        {
            RemoveEffect(active_effects[i]);
            active_effects.erase(active_effects.begin() + i);
            i--;
        }
    }
}

void Chaos::RemoveEffect(ActiveEffect& effect)
{
    switch (effect.effect)
    {
    case TRANSLUCENT_WINDOW:
    {
        SDL_SetWindowOpacity(gameScreen.m_window, 1.0f);
        break;
    }
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
    case SHUFFLE_ENTITIES:
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
    case RANDOM_SIZE:
    {
        int i = obj.getplayer();
        if (INBOUNDS_VEC(i, obj.entities))
        {
            obj.entities[i].size = 0;
            obj.entities[i].colour = 0;
            obj.entities[i].cx = 6;
            obj.entities[i].cy = 2;
            obj.entities[i].h = 21;
            obj.entities[i].w = 12;
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
    for (int i = 0; i < active_effects.size(); i++)
    {
        if (active_effects[i].effect == effect)
        {
            return true;
        }
    }
    return false;
}
