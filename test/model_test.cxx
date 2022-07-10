#include "model.hxx"
#include <catch.hxx>

Game_config const config;

struct Test_access {
    Model& model;

    explicit Test_access(Model&);

    int& round_score_;

    double& three_secs_;

    void check_events();

    void shift_new_lane();
};

/*
 * Tests the screen events of the model helper function check_events()
 *        - avatar hitting left of screen (L41, 43)
 *        - avatar hitting right of screen (L56, 58)
 *        - avatar hitting bottom of screen (L72, 74)
 *        - avatar hitting top of screen (L96, 98)
 */
TEST_CASE("check_events_screen")
{

    Model m(config);
    Test_access t{m};

    /// if avatar dies, goes back to dead state
    // left side - avatar dies
    m.move_left();
    m.move_left();
    m.move_left();

    CHECK(m.get_avatar().hits_side(config));
    t.check_events();
    CHECK(m.get_avatar().is_junk());
    m.on_frame(2);
    m.dead_state();
    m.forest_objects_ = std::vector<Forest_object> ();
    CHECK(m.get_avatar().position().x == config.scene_dims.width/2 -
    50);
    CHECK(m.get_avatar().position().y == 500);

    // right side - avatar dies
    m.move_right();
    m.move_right();
    m.move_right();

    CHECK(m.get_avatar().hits_side(config));
    t.check_events();
    CHECK(m.get_avatar().is_junk());
    m.on_frame(2);
    m.dead_state();
    m.forest_objects_ = std::vector<Forest_object> ();
    CHECK(m.get_avatar().position().x == config.scene_dims.width/2 -
                                         50);
    CHECK(m.get_avatar().position().y == 500);

    // bottom - avatar dies
    m.dead_state();
    m.forest_objects_ = std::vector<Forest_object> ();
    m.move_down();
    m.move_down();

    CHECK(m.get_avatar().hits_bottom(config));
    t.check_events();
    CHECK(m.get_avatar().is_junk());
    m.on_frame(2);
    m.dead_state();
    CHECK(m.get_avatar().position().x == config.scene_dims.width/2 -
                                         50);
    CHECK(m.get_avatar().position().y == 500);

    // top - shouldn't die, can't progress until screen moves
    m.dead_state();
    m.forest_objects_ = std::vector<Forest_object> ();
    CHECK(m.get_avatar().position().y == 500);
    m.move_up();
    CHECK(m.get_avatar().position().y == 400);
    m.move_up();
    CHECK(m.get_avatar().position().y == 300);
    m.move_up();
    CHECK(m.get_avatar().position().y == 200);
    m.move_up();
    CHECK(m.get_avatar().position().y == 100);
    m.move_up();
    CHECK(m.get_avatar().position().y == 0);

    CHECK(m.get_avatar().hits_top(config));
    t.check_events();
    CHECK(m.lock_up_);
    CHECK(!m.get_avatar().is_junk());
    CHECK(m.get_avatar().position().x == config.scene_dims.width/2 -
                                         50);
    CHECK(m.get_avatar().position().y == 0);
}

/*
 * Tests the model helper function shift_new_lane()
 *        - forest_objects NOT in last lane get shifted down (L128)
 *        - avatar NOT in last lane gets shifted down (L133)
 *        - avatar in last lane dies (L137)
 *        - forest_objects in last lane get deleted (L141)
 */
TEST_CASE("shift_new_lane")
{
    Model m(config);
    Test_access t{m};
    m.dead_state();
    m.forest_objects_ = std::vector<Forest_object> ();
    Collectible l1 = Collectible(Forest_object::Material::lilypad,
                                 ge211::Posn<int>{
                                         config.scene_dims.width/2 - 50, 300},
                                         config, false);
    m.forest_objects_.push_back(l1);
    m.move_up(); //avatar is at 400, should get shifted down to 500
    t.shift_new_lane();

    // lilypad shifted down
        // remember that a NEW forest_object replaces it, so can't call l1
    CHECK(m.forest_objects_[0].position() == ge211::Posn<int>{config.scene_dims
    .width/2 - 50, 400});

    // avatar shifted down
    CHECK(m.get_avatar().position().x == config.scene_dims.width/2 - 50);
    CHECK(m.get_avatar().position().y == 500);

    t.shift_new_lane();  //lilypad at 500
    t.check_events();
    CHECK(m.get_avatar().is_junk());

    // test that bottom lane objects get deleted
    t.shift_new_lane();
    CHECK(m.forest_objects_.empty());

}

/*
 * Tests the model move_ functions
 *        - avatar moving up shifts y position -100 (L162)
 *        - avatar moving left shifts x position -100 (L164)
 *        - avatar moving right shifts y position +100 (L167)
 *        - avatar moving down shifts y position +100 (L171)
 *        - round_score_ should be updated based on lane movements (L176, 178)
 */
TEST_CASE("check move_ functions")
{
    Model m(config);
    Test_access t{m};
    m.dead_state();
    m.forest_objects_ = std::vector<Forest_object> ();
    CHECK(m.get_avatar().position().x == config.scene_dims.width/2 - 50);
    CHECK(m.get_avatar().position().y == 500);
    m.move_up();
    CHECK(m.get_avatar().position().x == config.scene_dims.width/2 - 50);
    CHECK(m.get_avatar().position().y == 400);
    m.move_left();
    CHECK(m.get_avatar().position().x == config.scene_dims.width/2 - 150);
    CHECK(m.get_avatar().position().y == 400);
    m.move_right();
    CHECK(m.get_avatar().position().x == config.scene_dims.width/2 - 50);
    CHECK(m.get_avatar().position().y == 400);
    m.move_down();
    CHECK(m.get_avatar().position().x == config.scene_dims.width/2 - 50);
    CHECK(m.get_avatar().position().y == 500);

    //checks that round_score is implemented
    m.move_up();
    CHECK(t.round_score_ == 1 );
    m.move_up();
    CHECK(t.round_score_ == 2);
    m.move_left();
    CHECK(t.round_score_ == 2);
    m.move_down();
    CHECK(t.round_score_ == 1);

}

/*
 * Tests the obstacle events of the model helper function check_events()
 *        - avatar dies upon collision with owl moving obstacle (L196)
 *        - avatar gets blocked upon a bush still obstacle
 *              - down direction (L228)
 *              - up direction (L241)
 *              - left direction (L254)
 *              - right direction (L267)
 */
TEST_CASE("check_events obstacles")
{
    // owl - avatar dies upon collision
    Model m(config);
    Test_access t{m};
    m.dead_state();
    m.forest_objects_ = std::vector<Forest_object> ();
    Obstacle o = Obstacle(Forest_object::Material::owl, ge211::Posn<int>{
            config.scene_dims.width/2 - 50-20, 400}, config, true);
    m.forest_objects_.push_back(o);
    m.move_up();
    m.on_frame(2);
    t.check_events();
    CHECK(m.get_avatar().is_junk());
    m.dead_state();
    m.forest_objects_ = std::vector<Forest_object> ();
    CHECK(m.get_avatar().position().x == config.scene_dims.width/2 -
                                          50);
    CHECK(m.get_avatar().position().y == 500);

    // bush - avatar blocked upon collision
    // bottom of avatar blocked
    m.dead_state();
    CHECK(m.get_avatar().position().y == 500);
    m.forest_objects_ = std::vector<Forest_object> ();
    m.move_up();
    CHECK(m.get_avatar().position().y == 400);
    m.move_up();
    CHECK(m.get_avatar().position().y == 300);
    Obstacle b1 = Obstacle(Forest_object::Material::bush, ge211::Posn<int>{
            config.scene_dims.width/2 - 50, 400}, config, true);
    m.forest_objects_.push_back(b1);
    m.move_down();
    CHECK(m.lock_down_);
    CHECK(!m.get_avatar().is_junk());
    CHECK(m.get_avatar().position().x == config.scene_dims.width/2 -
                                         50);
    CHECK(m.get_avatar().position().y == 300);

    // top of avatar blocked
    m.dead_state();
    m.forest_objects_ = std::vector<Forest_object> ();
    Obstacle b2 = Obstacle(Forest_object::Material::bush, ge211::Posn<int>{
            config.scene_dims.width/2 - 50, 200}, config, true);
    m.forest_objects_.push_back(b2);
    m.move_up();
    CHECK(m.lock_up_);
    CHECK(!m.get_avatar().is_junk());
    CHECK(m.get_avatar().position().x == config.scene_dims.width/2 -
                                         50);
    CHECK(m.get_avatar().position().y == 300);

    // left side of avatar blocked
    m.dead_state();
    m.forest_objects_ = std::vector<Forest_object> ();
    Obstacle b3 = Obstacle(Forest_object::Material::bush, ge211::Posn<int>{
            config.scene_dims.width/2 - 50-100, 300}, config, true);
    m.forest_objects_.push_back(b3);
    m.move_left();
    CHECK(m.lock_left_);
    CHECK(!m.get_avatar().is_junk());
    CHECK(m.get_avatar().position().x == config.scene_dims.width/2 -
                                         50);
    CHECK(m.get_avatar().position().y == 300);

    // right side of avatar blocked
    m.dead_state();
    m.forest_objects_ = std::vector<Forest_object> ();
    Obstacle b4 = Obstacle(Forest_object::Material::bush, ge211::Posn<int>{
            config.scene_dims.width/2 - 50+100, 300}, config, true);
    m.forest_objects_.push_back(b4);
    m.move_right();
    CHECK(m.lock_right_);
    CHECK(!m.get_avatar().is_junk());
    CHECK(m.get_avatar().position().x == config.scene_dims.width/2 -
                                         50);
    CHECK(m.get_avatar().position().y == 300);

}

/*
 * Tests the collectible events of the model helper function check_events()
 *        - collectible count gets updated upon avatar collision with lilypad
 *        (L303)
 *        - booster gets activated upon avatar collision with mushroom (L322)
 *              - after 5 sec, booster is inactivated (lane shift rate goes
 *              back to three_sec_ = 3)  (L339)
 */
TEST_CASE("check_events collectibles")
{
    // lilypad - avatar collects and collectible count updated
    Model m(config);
    Test_access t{m};
    m.dead_state();
    m.forest_objects_ = std::vector<Forest_object> ();
    Collectible l1 = Collectible(Forest_object::Material::lilypad,
                                ge211::Posn<int>{
            config.scene_dims.width/2 - 50, 400}, config, false);
    m.forest_objects_.push_back(l1);
    m.move_up();
    t.check_events();
    CHECK(m.forest_objects_.empty());
    CHECK(m.get_avatar().get_collectible_count() == 1);
    Collectible l2 = Collectible(Forest_object::Material::lilypad,
                                 ge211::Posn<int>{
                                         config.scene_dims.width/2 - 50 -100,
                                         400}, config, false);
    m.forest_objects_.push_back(l2);
    m.move_left();
    t.check_events();
    CHECK(m.forest_objects_.empty());
    CHECK(m.get_avatar().get_collectible_count() == 2);

    // mushroom - last lane will disappear at slower rate for 5 secs
    CHECK(t.three_secs_ == 3);
    CHECK(m.get_avatar().position().x == config.scene_dims.width/2 -
                                         150);
    CHECK(m.get_avatar().position().y == 400);
    Collectible m1 = Collectible(Forest_object::Material::mushroom,
                                 ge211::Posn<int>{
                                         config.scene_dims.width/2 - 150,
                                         300}, config, true);
    m.forest_objects_.push_back(m1);
    m.move_up();
    CHECK(m.get_avatar().position().x == config.scene_dims.width/2 -
                                         150);
    CHECK(m.get_avatar().position().y == 300);
    t.check_events();
    m.on_frame(1);
    CHECK(t.three_secs_ == 5);
    CHECK(m.forest_objects_.empty());
    Collectible m2 = Collectible(Forest_object::Material::mushroom,
                                 ge211::Posn<int>{
                                         config.scene_dims.width/2 - 50,
                                         300}, config, true);
    m.forest_objects_.push_back(m2);
    m.move_right();
    t.check_events();
    m.on_frame(1);
    CHECK(t.three_secs_ == 5);
    CHECK(m.forest_objects_.empty());

    //check that three_secs_ goes back to 3 (booster is off)
    m.on_frame(5);
    CHECK(t.three_secs_ == 3);


}

Test_access::Test_access(Model& model)
: model(model),
round_score_(model.round_score_),
three_secs_(model.three_secs_)
{}

void
Test_access::check_events() {
    model.check_events();
}

void
Test_access::shift_new_lane(){
    return model.shift_new_lane();
}


