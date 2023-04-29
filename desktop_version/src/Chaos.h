#ifndef CHAOS_H
#define CHAOS_H

#include <vector>

#define MIN_EFFECT_TIME 30 * 15
#define MAX_EFFECT_TIME 30 * 45

#define MIN_WAIT_TIME 30 * 10
#define MAX_WAIT_TIME 30 * 25

#define INITIAL_WAIT_TIME 30 * 5

#define CLONE_COUNT 5
#define CLONE_OFFSET 16

struct CloneInfo {
    int rx; // game.roomx-100
    int ry; // game.roomy-100
    int x; // .xp
    int y; // .yp
    int frame; // .drawframe
    int visible;
};

enum Effects
{
    NO_FLIPPING = -1,   // [DISABLED] Can't flip!
    NO_LEFT = -1,       // [DISABLED] Can't walk left!
    NO_RIGHT = -1,      // [DISABLED] Can't move right!
    NO_MAP = 0,         // Can't view map (if you were able to)
    RANDOM_COLOR,       // Change to a random color, reset respawn color afterwards
    JUMPING,            // Jumping instead of flipping
    BUS,                // Spawn a bus that tries to fucking run you over
    FLIP_MODE,          // Enter flip mode for a little bit
    RANDOM_BLOCK,       // Add a random tile to the room
    WARP_DIR,           // Change the room's warp direction
    GAMESTATE,          // Change to a random gamestate
    PLAYER_SPRITE,      // Change the player sprites
    REVERSE_CONTROLS,   // Reverse the controls
    INFINIFLIP,         // Can flip in air
    GRAVITATION_POTION, // Flip mode depending on the players gravity control
    RANDOM_FLIPPING,    // Flip randomly
    ANALOGUE,           // Analogue mode on for a bit
    BILINEAR,           // Make the game blurry because fuck you
    HOLDING_RIGHT,      // Force the player to hold right constantly (holding left overrides)
    FAKE_TRINKET,       // "You got a shiny trinket!" dialogue without getting a trinket
    PUSH_AWAY,          // Entities get pushed away by Viridian
    VVVVVVMAN,          // vvvvvvman() lol
    ICE,                // Ice physics
    SOLITAIRE,          // Solitaire effect
    RANDOM_ACCEL,       // Random horizontal accel
    UNCAPPED_SPEED,     // Uncapped player speed
    CYCLING_SPRITES,    // Cycle through sprites
    SHUFFLE_ENTITIES,   // Shuffle entity sprites
    MODIFY_ENTITIES,    // Do random things to entities
    BURST_ENTITIES,     // Random bursts
    MOVE_LEFT,          // Slowly move left
    MOVE_RIGHT,         // Slowly move right
    SIDEWAYS_FLIPPING,  // Sideways gravity (??)
    TORNADO,            // Entity tornado
    SHAKING,            // Shake the player
    FLAG,               // Change a random flag
    RANDOM_SIZE,        // Randomly change player size
    COSMIC_CLONES,      // Cosmic clones!
    RANDOM_SPRITE,      // Make Viridian look like something else, but not in a glitchy way
    FLIP_TELEPORT,      // Teleport a little on flip
    BLINKING_VIRIDIAN,  // <blink></blink>
    BLINKING_ENEMIES,   // <blink>enemy</blink>
    INVINCIBILITY,      // invincible player & spikes arent solid like built-in invincibility
    TRINKETS_KILL,      // trinkets kill!
    TRANSLUCENT_WINDOW = -1, // 50% translucent window lol
    ROOM_EXPLODE,       // Explode a part of the room
    RANDOM_MUSIC,       // Random music
    GOOFY_AAH,          // goofy ahh sounds
    NOCLIP,             // noclip


    // Mario?
    // Portal gun?
    // Invincibility star?
    // Flying and noclip
    // Viridian has a gun
    // Press down or die
    // Press down to die
    // ddr but you gotta be off beat
    // minecraft mode
    // vignette around the player
    // green demon (Green coin chases you)
    // you get hurt and drop coins
    // add input lag!
    // don't stop moving (pressing left makes you go left until you go right, etc)

    // squeaks are replaced with quacks
    

    EFFECT_AMOUNT
};

namespace Chaos
{

    struct ActiveEffect
    {
        int timeRemaining;
        Effects effect;
        int timer;
    };

    extern bool reloading;

    extern int randomSprite;

    extern std::vector<ActiveEffect> activeEffects;

    extern std::vector<CloneInfo> cloneInfo;

    extern int cloneTimer;
    extern int cloneCount;

    extern bool randomEffects;

    void Initialize();

    void AddEffect(Effects effect, bool infinite);

    void ProcessEffects();

    void ApplyEffect(ActiveEffect& effect);

    void UpdateEffect(ActiveEffect& effect);

    void RemoveEffect(ActiveEffect& effect);

    void ModifyRandomEntity();

    bool IsActive(Effects effect);

    extern int waitTime;
}

#endif /* CHAOS_H */
