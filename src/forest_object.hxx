//
// Created by Rachel Yao on 5/28/22.
//

#ifndef GAME_FOREST_OBJECT_HXX
#define GAME_FOREST_OBJECT_HXX

#pragma once

#include <iostream>
#include <ge211.hxx>
#include "game_config.hxx"

/// An abstract class encompassing avatars, obstacles, and collectibles.
class Forest_object
{
public:
    /// Positions represented as GE211 Posn<int>'s.
    using Position = ge211::Posn<int>;

    /// Material needed for avatar, obstacles, and collisions.
    enum class Material
    {
        frog,
        mushroom,
        bush,
        owl,
        lilypad,
    };

    /// Constructor for Forest_object abstract class.
    Forest_object(Material, Position);

    /// Destructor for Forest_object abstract class.
    virtual ~Forest_object();

    // MEMBER FUNCTIONS

    /// Returns the material of a Forest_object type.
    Material material() const;

    /// Returns the position of a Forest_object type.
    Position position() const;

    /// Updates the position of a Forest_object type.
    void update_position(Position pos);

    /// Returns whether a Forest_object is junk, or is dead.
    bool is_junk() const;

    /// Changes the state of a Forest_object - whether it is alive or dead.
    void set_junk(bool j){
        junk_ = j;
    }

    /// Returns the dimensions of a Forest_object.
    virtual ge211::geometry::Dims<int> dimensions() {
        return {0,0};
    }

    /// Returns whether a Forest_object is moving.
    bool is_moving() const
    {
        return moving_;
    }

    /// Returns whether a Forest_object has a booster.
    bool has_booster()
    {
        return is_booster_;
    }


protected:
    // MEMBER VARIABLES

    /// The state (dead or alive) of the Forest_object.
    bool junk_ = false;

    /// The position of the top left corner of the Forest_object.
    Position top_left_;

    /// The moving state of the Forest_object.
    bool moving_;

    /// The booster state of the Forest_object.
    bool is_booster_;


private:
    /// The material of the Forest_object.
    Material material_;

};

/// Class that inherits from Forest_object to create obstacles for the game.
class Obstacle : public Forest_object
{
public:
    /// Velocities represented as GE211 Dims<int>'s.
    using Velocity = ge211::Dims<int>;

    /// Constructor for Obstacle class.
    Obstacle(
            Material mat,
            ge211::Posn<int> pos,
            Game_config const& config,
            bool is_moving // birds
    );

    // MEMBER FUNCTIONS

    /// Inherited function from Forest_object overridden for obstacles.
    ge211::geometry::Dims<int> dimensions() override;

private:
    // MEMBER VARIABLES

    /// The velocity of an obstacle.
    Velocity velocity_;

};

/// Class that inherits from Forest_object to create collectibles for the game.
class Collectible : public Forest_object
{
public:

    /// Constructor for Collectible class.
    Collectible(
            Material mat,
            ge211::Posn<int> pos,
            Game_config const& config,
            bool booster  // lilypad = true
    );

    // MEMBER FUNCTIONS

    /// Inherited function from Forest_object overridden for collectibles.
    ge211::geometry::Dims<int> dimensions() override;

};

/// Class that inherits from Forest_object to create an avatar for the game.
class Avatar : public Forest_object
{
public:

    /// Constructor for Avatar class.
    Avatar(
            Material mat,
            ge211::Posn<int> pos,
            Game_config const& config);

    // MEMBER FUNCTIONS

    /// Inherited function from Forest_object overridden for avatars.
    ge211::geometry::Dims<int> dimensions() override;

    /// Function to return next position of an avatar, given a direction, as
    // it moves across the screen.
    Avatar next(ge211::Dims<int> direction, Game_config const& config) const;

    /// Function to return the count of collectible items that the avatar has
    // collected during the game.
    int get_collectible_count(){
        return collectible_count_;
    }

    /// Checks collisions between Forest_objects.
    bool collision_with_object(Forest_object ob, Game_config const& config);

    // Determines whether the avatar moves past the bottom of
    // the scene.
    bool hits_bottom (Game_config const&) const;

    // Determines whether the avatar moves past the left of right edge
    // of the scene.
    bool hits_side(Game_config const&) const;

    // Determines whether the avatar moves past the top of
    // the scene.
    bool hits_top(Game_config const&) const;

    // Increments the count of collectibles collected by the avatar.
    void collect_collectible(){
        collectible_count_++;
    };

    // Returns whether the avatar is boosted by a collectible.
    bool get_boosted_state(){
        return is_boosted_;
    }

    // Changes the boosted state of the avatar.
    void set_boosted_state(bool state){
        is_boosted_ = state;
    }

private:
    ///MEMBER VARIABLES

    // Collectible count
    int collectible_count_;

    // Whether a booster is applied
    bool is_boosted_;
};


#endif //GAME_FOREST_OBJECT_HXX
