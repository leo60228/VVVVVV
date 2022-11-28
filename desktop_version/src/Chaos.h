#ifndef CHAOS_H
#define CHAOS_H

#include <vector>

#define MIN_EFFECT_TIME 30 * 15
#define MAX_EFFECT_TIME 30 * 45

#define MIN_WAIT_TIME 30 * 10
#define MAX_WAIT_TIME 30 * 25

#define INITIAL_WAIT_TIME 30 * 5

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


    // Mario?
    // Music speed up?
    // Portal gun?
    // Squished?
    // Invincibility star?
    // Explode a part of the room?
    // Turn into random sprites (without animation other than flipped grav)
    // Flying and noclip
    // Viridian has a gun
    // Cosmic clones
    // Viridian ignores spikes
    // Flipping teleports Viridian upwards a bit
    // Press down or die
    // Press down to die
    

    EFFECT_AMOUNT
};

namespace Chaos
{

    struct ActiveEffect
    {
        int timeRemaining;
        Effects effect;
    };

    extern std::vector<ActiveEffect> activeEffects;

    void Initialize();

    void AddEffect(Effects effect);

    void ProcessEffects();

    void ApplyEffect(ActiveEffect effect);

    void UpdateEffect(ActiveEffect effect);

    void RemoveEffect(ActiveEffect effect);

    void ModifyRandomEntity();

    bool IsActive(Effects effect);

    extern int waitTime;
}

#endif /* CHAOS_H */
