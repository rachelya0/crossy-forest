#pragma once

#include "model.hxx"

// view constants

/// Colors needed for the sprites
static ge211::Color const lane_color {129, 84, 31};
static ge211::Color const green_box_color {63, 101, 24};
static ge211::Color const game_over_color {255, 255, 255};
static ge211::Color const collectible_count_color {164, 195, 132};
static ge211::Color const lanes_count_color {162, 203, 160};
static ge211::Color const score_count_color {0, 195, 132};


class View
{
public:

    /// Constructs a `View` given a const reference to the model that
    /// stores the actual state of the game. This lets the view observe
    /// the model’s state but not modify it.
    explicit View(Model const&);

    /// Renders all the game entities to the screen
    void draw(ge211::Sprite_set& set);

    /// Returns the size of the window as given by `config.scene_dims`.
    ge211::Dims<int> initial_window_dimensions() const;


private:
    /// Referenced to the model so view has access to it.
    Model const& model_;

    /// Sprites needed to display contents of the screen
    ge211::Image_sprite frog_ {"frog_avatar-removebg-preview.png"};
    ge211::Image_sprite mushroom_ {"mushroom-removebg-preview.png"};
    ge211::Image_sprite bush_ {"bush-removebg-preview.png"};
    ge211::Image_sprite owl_ {"owl-removebg-preview.png"};
    ge211::Image_sprite lilypad_ {"lilypad-removebg-preview.png"};

    ge211::Rectangle_sprite lane_;
    ge211::Rectangle_sprite green_box_;

    /// Sprites needed to display the text of the screen
    ge211::Font sans80{"sans.ttf", 80};
    ge211::Text_sprite game_over_;
    ge211::Font sans30{"sans.ttf", 30};
    ge211::Text_sprite collectible_count_;
    ge211::Text_sprite lanes_count_;
    ge211::Text_sprite score_count_;
};
