/*******************************************/
/*** DO NOT CHANGE ANYTHING IN THIS FILE ***/
/*******************************************/

// Defines a struct to hold configuration parameters. You should try different
// values to ensure that your model and UI adjust appropriately. You can do this
// by creating an instance and then assigning its parameters before constructing
// a `Model`:
//
//     Game_config config;
//     config.brick_cols = 15;
//     config.ball_radius *= 2;
//     Model model(config);
//

#pragma once

#include <ge211.hxx> // made new file

struct Game_config
{
    // Constructs an instance with the default parameters.
    Game_config();

    // The dimensions of the whole window:
    ge211::Dims<int> scene_dims;

    // Pixel thickness of lane
    int lane_thickness;

    // The number of pixels of space between each column and row of
    // lanes:
    int lane_spacing;

    // The moving obstacles' initial velocities {width, height}:

    ge211::Dims<int> moving_right_obstacle_velocity;

};

