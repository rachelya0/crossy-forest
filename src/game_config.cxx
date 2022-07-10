//
// Created by Rachel Yao on 5/28/22.
//

/*******************************************/
/*** DO NOT CHANGE ANYTHING IN THIS FILE ***/
/*******************************************/

#include "game_config.hxx"

// This is the default (and only) constructor for `Game_config`. It
// determines all the default values of all the member variables.
Game_config::Game_config()
        : scene_dims {600, 600},
          lane_thickness{100},
          lane_spacing {15},
          moving_right_obstacle_velocity{10,0}
{ }

