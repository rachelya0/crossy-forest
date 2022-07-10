#include "model.hxx"

Model::Model(Game_config const& config)
        : lock_up_(false),
          lock_down_(false),
          lock_right_(false),
          lock_left_(false),
          config_(config),
          avatar_(Forest_object::Material::frog,
                 ge211::Posn<int>(config.scene_dims.width/2 - 50,500),
                 config),
          round_score_(0),
          count_dt_(0),
          three_secs_(3),
          random_positions_(0,config_.scene_dims.width-30),
          random_type_(0,3)

{
    forest_objects_ = std::vector<Forest_object> ();
    dead_state();

}

void
Model::dead_state() {
    if (avatar_.is_junk()) {
        Avatar a(Forest_object::Material::frog,
                 ge211::Posn<int>(config_.scene_dims.width/2 - 50,500),
                 config_);
        avatar_ = a;
        forest_objects_ = std::vector<Forest_object> ();
        count_dt_ = 0;
        three_secs_ = 3;
        round_score_ = 0;
        lock_up_ = false;
        lock_down_ = false;
        lock_right_ = false;
        lock_left_ = false;
    }
    new_lane(0);
    new_lane(100);
    new_lane(200);
    new_lane(300);
    new_lane(400);

}


void
Model::on_frame(double dt)
{

    count_dt_ = count_dt_ + dt;
    if (count_dt_ >= three_secs_)
    {
        shift_new_lane();
        new_lane(0);
        lock_up_ = false;
        count_dt_ = 0;
        three_secs_ = 3;
    }
    check_events();

    if (avatar_.get_boosted_state())
    {
        // if boosted, screen moves down 5 sec later
        three_secs_ = 5;
        // set boosted state back to false
        avatar_.set_boosted_state(false);
    }

    // moving obstacle moves ever 0.5s
    // count_dt is always a fraction, can't use ==
    if ((count_dt_ > 0.48 && count_dt_ < 0.52)||
            (count_dt_ > 0.98 && count_dt_ < 1.02) ||
            (count_dt_ > 1.48 && count_dt_ < 1.52) ||
            (count_dt_ > 1.98 && count_dt_ < 2.02) ||
            (count_dt_ > 2.48 && count_dt_ < 2.52) ||
            (count_dt_ > 2.98 && count_dt_ < 3.02))
    {
        for (size_t i = 0; i < forest_objects_.size(); i++)
        {
            if (forest_objects_[i].material() == Forest_object::Material::owl)
            {
                forest_objects_[i].update_position({forest_objects_[i]
                .position().x + config_.moving_right_obstacle_velocity.width,
                forest_objects_[i].position().y});

            }
        }
    }
}

bool
Model::move_up()
{
    // Check if the NEXT avatar will collide with a bush
    Avatar next_av = avatar_.next({0, -100}, config_);
    for (size_t i = 0; i < forest_objects_.size(); i++) {
        if (forest_objects_[i].material() ==
           Forest_object::Material::bush) {

            lock_up_ = next_av.collision_with_object(forest_objects_[i],
                                                     config_);//if
            // collision, then lock_up_ is true

            if (lock_up_){
                //if lock_up_ ever becomes true, exit the loop
                break;
            }

        }
    }

    if (!lock_up_)
    {
        avatar_.update_position({avatar_.position().x, avatar_.position()
        .y-100});
        round_score_ = round_score_ + 1;
        return true;
    }
    return false;
}

bool
Model::move_down()
{
    Avatar next_av = avatar_.next({0, 100}, config_);
    for (size_t i = 0; i < forest_objects_.size(); i++) {
        if (forest_objects_[i].material() ==
            Forest_object::Material::bush) {

            lock_down_ = next_av.collision_with_object(forest_objects_[i],
                                                       config_);//if
            // collision, then lock_up_ is true

            if (lock_down_){
                //if lock_up_ ever becomes true, exit the loop
                break;
            }

        }
    }

    if (!lock_down_)
    {
        avatar_.update_position({avatar_.position().x, avatar_.position()
        .y+100});
        round_score_ = round_score_ - 1;
        return true;
    }
    return false;
}

bool
Model::move_right()
{
    Avatar next_av = avatar_.next({100, 0}, config_);
    for (size_t i = 0; i < forest_objects_.size(); i++) {
        if (forest_objects_[i].material() ==
            Forest_object::Material::bush) {

            lock_right_ = next_av.collision_with_object(forest_objects_[i],
                                                        config_);//if
            // collision, then lock_up_ is true

            if (lock_right_){
                //if lock_up_ ever becomes true, exit the loop
                break;
            }

        }
    }

    if (!lock_right_)
    {
        avatar_.update_position({avatar_.position().x+100, avatar_.position()
                                                              .y});
        return true;
    }
    return false;
}

bool
Model::move_left()
{
    Avatar next_av = avatar_.next({-100, 0}, config_);
    for (size_t i = 0; i < forest_objects_.size(); i++) {
        if (forest_objects_[i].material() ==
            Forest_object::Material::bush) {

            lock_left_ = next_av.collision_with_object(forest_objects_[i],
                                                       config_);//if
            // collision, then lock_up_ is true

            if (lock_left_){
                //if lock_up_ ever becomes true, exit the loop
                break;
            }

        }
    }

    if (!lock_left_)
    {
        avatar_.update_position({avatar_.position().x-100, avatar_.position()
                                                              .y});
        return true;
    }
    return false;
}


// HELPER FUNCTIONS

void
Model::check_events()
{
    if (avatar_.hits_bottom(config_) || avatar_.hits_side(config_))
    {
        avatar_.set_junk(true);
    }
    else if(avatar_.hits_top(config_))
    {
        lock_up_ = true;
    }
    else {
        for (size_t i = 0; i < forest_objects_.size(); i++)
        {
            // Check Moving Obstacle
            if (forest_objects_[i].material() == Forest_object::Material::owl &&
                avatar_.collision_with_object(forest_objects_[i], config_))
            {
                avatar_.set_junk(true);

            }

            // Check Collectible
            if ((forest_objects_[i].material() ==
                 Forest_object::Material::mushroom ||
                    forest_objects_[i].material() ==
                    Forest_object::Material::lilypad)
                                                      && avatar_
                        .collision_with_object(forest_objects_[i], config_)) {

                    // mushroom
                    if (forest_objects_[i].has_booster()) {
                        avatar_.set_boosted_state(true);
                        forest_objects_[i] = forest_objects_.back();
                        forest_objects_.pop_back();
                    }
                    // lilypad
                    else {
                        avatar_.collect_collectible();
                        forest_objects_[i] = forest_objects_.back();
                        forest_objects_.pop_back();
                    }
                }
            }

        }
    }

void
Model::new_lane(int y)
{
    int type_to_add = random_type_.next();
    float position_to_add = random_positions_.next();
    if (type_to_add == 0) { // unmoving obstacle
        Obstacle ob = Obstacle(Forest_object::Material::bush,
                               ge211::Posn<int>
                                       (position_to_add, y),
                               config_, false);
        forest_objects_.push_back(ob);
    }
    else if (type_to_add == 1) { // moving obstacle
        Obstacle ob = Obstacle(Forest_object::Material::owl,
                               ge211::Posn<int>(0, y),
                               config_, true);
        forest_objects_.push_back(ob);
    }
    else if (type_to_add == 2){ // collectible booster
        Collectible ob = Collectible(Forest_object::Material::mushroom,
                                     ge211::Posn<int>(position_to_add,
                                                        y),
                                                        config_,
                                     true);
        forest_objects_.push_back(ob);
    }
    else {  //collectible object
        Collectible ob = Collectible(Forest_object::Material::lilypad,
                                     ge211::Posn<int>(position_to_add,
                                                      y),
                                                      config_,
                                     false);
        forest_objects_.push_back(ob);
    }
}

void
Model::shift_new_lane()
{
    avatar_.update_position({avatar_.position().x, avatar_.position()
    .y+100});
    for (size_t i = 0; i < forest_objects_.size(); i++) {

        // if in bottom lane, delete objects in lane
        if (forest_objects_[i].position().y >= 500 ) {
            forest_objects_[i] = forest_objects_.back();
            forest_objects_.pop_back();
            i--; //because if you pop off, everything shifts
        }

        // in other lanes, update positions to add 100 to y value
        else {
            ge211::Posn<int> new_posn = {forest_objects_[i].position().x,
            forest_objects_[i].position().y+100};
            if (forest_objects_[i].material() ==
                Forest_object::Material::mushroom) {
                forest_objects_[i] = Collectible
                        (Forest_object::Material::mushroom,
                                    new_posn,
                                config_, true);
            }

            else if (forest_objects_[i].material() ==
                     Forest_object::Material::bush){
                forest_objects_[i] = Obstacle(Forest_object::Material::bush,
                                            new_posn,
                                            config_, false);

            }

            else if (forest_objects_[i].material() ==
                     Forest_object::Material::owl){
                forest_objects_[i] = Obstacle(Forest_object::Material::owl,
                                           new_posn,
                                           config_, true);

            }

            else if (forest_objects_[i].material() ==
                     Forest_object::Material::lilypad){
                forest_objects_[i] = Collectible(
                        Forest_object::Material::lilypad,
                                            new_posn,
                                            config_, false);

            }
        }

    }
}