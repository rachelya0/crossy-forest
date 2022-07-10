//
// Created by Rachel Yao on 5/28/22.
//

#include "forest_object.hxx"
#include "game_config.hxx"

Forest_object::Forest_object(Material material, Position position)
        : top_left_(position),
          material_(material)
{}

Forest_object::~Forest_object() {
   // std::cout << "memory freed";
}

bool
Forest_object::is_junk() const
{
    return junk_;
}

Forest_object::Material
Forest_object::material() const
{
    return material_;
}

Forest_object::Position
Forest_object::position() const
{
    return top_left_;
}

void
Forest_object::update_position(Position pos)
{
    top_left_ = pos;
}

Avatar::Avatar(
        Material mat,
        ge211::Posn<int> pos,
        Game_config const& config)
        : Forest_object(mat, pos),
          collectible_count_(0),
          is_boosted_(false)
{}

Avatar
Avatar::next(ge211::Dims<int> direction, Game_config const& config) const
{
    Avatar result(*this);
    if (direction == ge211::Dims<int>(0,100)) {
        result.top_left_.y = result.top_left_.y + config.lane_thickness;
    }
    else if (direction == ge211::Dims<int>(0,-100)) {
        result.top_left_.y = result.top_left_.y - config.lane_thickness;
    }
    else if (direction == ge211::Dims<int>(100, 0)) {
        result.top_left_.x = result.top_left_.x + config.lane_thickness;
    }
    else if (direction == ge211::Dims<int>(-100,0)) {
        result.top_left_.x = result.top_left_.x - config.lane_thickness;
    }
    return result;
}

bool
Avatar::hits_bottom (Game_config const& config) const
{
    return top_left_.y + 90 > config.scene_dims.height;
}

bool
Avatar::hits_top (Game_config const& config) const
{
    Avatar a = next({0, -100}, config);
    return a.top_left_.y<0;
}

bool
Avatar::hits_side (Game_config const& config) const
{
    return top_left_.x+ 100 >config.scene_dims.width || top_left_
    .x<0;
}

ge211::geometry::Dims<int> Avatar::dimensions()
{
    return {100,90};
}

ge211::geometry::Dims<int> Obstacle::dimensions()
{
    return {116,68};
}

ge211::geometry::Dims<int> Collectible::dimensions()
{
    return {82,74};
}

Obstacle::Obstacle(
        Material mat,
        ge211::Posn<int> pos,
        Game_config const& config,
        bool moving)
        : Forest_object(mat, pos)
{
    if (moving){
        moving_ = true;
        velocity_.width = config.moving_right_obstacle_velocity.width;
        velocity_.height = config.moving_right_obstacle_velocity.height;
    }
    else{
        moving_ = false;
        velocity_ = {0,0};
    }
}

Collectible::Collectible(
        Material mat,
        ge211::Posn<int> pos,
        Game_config const& config,
        bool booster)
        : Forest_object(mat, pos)
{
    if (booster){
        is_booster_ = true;
    }
    else{
        is_booster_ = false;
    }
}



// COLLISIONS


bool
Avatar::collision_with_object(Forest_object ob, Game_config const& config)
{
    int frog_top = top_left_.y;
    int frog_left = top_left_.x;
    int frog_bottom = top_left_.y + dimensions().height;
    int frog_right = top_left_.x + dimensions().width;

    int ob_top;
    int ob_left;
    int ob_bottom;
    int ob_right;

    if (ob.material() == Forest_object::Material::bush || ob.material() ==
    Forest_object::Material::owl)
    {
        Obstacle obst = Obstacle(ob.material(), ob.position(), config,
                                 ob.is_moving());
        ob_top = obst.position().y;
        ob_left = obst.position().x;
        ob_bottom = obst.position().y + obst.dimensions().height;
        ob_right = obst.position().x + obst.dimensions().width;
    }

    if (ob.material() == Forest_object::Material::lilypad || ob.material()
    == Forest_object::Material::mushroom) {
        Collectible coll = Collectible(ob.material(), ob.position(),
                                       config, ob.has_booster());
        ob_top = coll.position().y;
        ob_left = coll.position().x;
        ob_bottom = coll.position().y + coll.dimensions().height;
        ob_right = coll.position().x + coll.dimensions().width;
    }

    //cases where they don't collide
    bool case1 = frog_right < ob_left;
    bool case2 = ob_right < frog_left;
    bool case3 = frog_bottom < ob_top;
    bool case4 = ob_bottom < frog_top;

    return !(case1 || case2 || case3 || case4);

}



