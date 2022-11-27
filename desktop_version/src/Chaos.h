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
    NO_FLIPPING = -1, // [DISABLED] Can't flip!
    NO_LEFT = -1,     // [DISABLED] Can't walk left!
    NO_RIGHT = -1,    // [DISABLED] Can't move right!
    NO_MAP = 0,       // Can't view map (if you were able to)
    RANDOM_COLOR,     // Change to a random color, reset respawn color afterwards
    JUMPING,          // Jumping instead of flipping
    BUS,              // Spawn a bus that tries to fucking run you over
    FLIP_MODE,        // Enter flip mode for a little bit
    RANDOM_BLOCK,     // Add a random tile to the room
    WARP_DIR,         // Change the room's warp direction
    GAMESTATE,        // Change to a random gamestate
    PLAYER_SPRITE,    // Change the player sprites
    REVERSE_CONTROLS, // Reverse the controls
    INFINIFLIP,       // Can flip in air
    GRAVITATION_POTION, // Flip mode depending on the players gravity control
    RANDOM_FLIPPING,  // Flip randomly
    ANALOGUE,         // Analogue mode on for a bit
    BILINEAR,         // Make the game blurry because fuck you
    HOLDING_RIGHT,    // Force the player to hold right constantly (holding left overrides)
    FAKE_TRINKET,     // "You got a shiny trinket!" dialogue without getting a trinket
    PUSH_AWAY,        // Entities get pushed away by Viridian
    VVVVVVMAN,        // vvvvvvman() lol

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

    bool IsActive(Effects effect);

    extern int waitTime;
}

#endif /* CHAOS_H */
