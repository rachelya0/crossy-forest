#pragma once

#include <ge211.hxx>
#include "game_config.hxx"
#include "forest_object.hxx"

class Model
{
public:

    /// CONSTRUCTOR: constructs a model from a game configuration
    explicit Model(Game_config const& config = Game_config());

    /// Sets the state of the game to the initial dead state before the game
    /// has started. Frog is set to the center of the bottom lane while the
    /// other lanes have randomly generated obstacles
    void dead_state();

    /// Updates the state of the game for one frame. Calls the three helper
    /// functions.
    void on_frame(double dt);

    /// Accesses the Game_config config data member variable
    Game_config get_config() const{
        return config_;
    }

    /// Updates the avatar position, returns true if able to move (key is not
    /// locked)
    /// Controller will call these functions after key presses
    bool move_up();
    bool move_down();
    bool move_right();
    bool move_left();

    /// Keeps track of which directions are 'locked'. Returns true if the
    /// direction is locked, so the player is not allowed to move in that
    /// direction.
    bool lock_up_;
    bool lock_down_;
    bool lock_right_;
    bool lock_left_;

    /// Vector of all live Forest_objects (Collectibles and Obstacles) on the
    /// game screen
    std::vector<Forest_object> forest_objects_;

    /// Accesses the avatar data member variable
    Avatar get_avatar() const{
        return avatar_;
    }

    /// Accesses the current score data member variable
    int get_score() const{
        return round_score_;
    }

#ifdef CS211_TESTING
    // When this class is compiled for testing, members of a struct named
    // Test_access will be allowed to access private members of this class.

    friend struct Test_access;
#endif

private:

    /// Game_config in the game
    Game_config const config_;

    /// Avatar in the game
    Avatar avatar_;

    /// Tracks the current score of the game
    int round_score_;

    /// Tracks time passage of the game for line shifts
    double count_dt_;
        // add dt, reset to 0 to start counting
        // reset to 0 when we call new_lane()
        // call new_lane when dt = 180

    /// Time in seconds between lane shifts (usually 3 seconds, becomes 5
    /// seconds when booster is on)
    double three_secs_;

    /// A source of random position values within the first (top) lane for
    /// obstacle/collectible generation. Only applies to unmoving
    /// Forest_objects. Moving Forest_objects ) always start at 0,0.
    ge211::Random_source<int> random_positions_;

    /// A source of random numbers to randomly choose a unmoving obstacle (0),
    /// moving obstacle (1), a collectible booster (2), or a collectible object
    /// (3) to generate.
    ge211::Random_source<int> random_type_;


    /// HELPER FUNCTIONS

    /// Checks for any events in the game (collisions with the screen walls
    /// or with other objects) and implements the correct action as necessary.
    void check_events();

    /// Adds new random Obstacle or Collectible object to the lane at height y
    void new_lane(int y);

    /// Shifts the lanes down by one. Pops the last bottom lane and its
    /// corresponding Forest_objects from the forest_objects_ vector.
    void shift_new_lane();

};
