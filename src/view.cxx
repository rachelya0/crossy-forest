#include "view.hxx"
#include "model.hxx"

View::View(Model const& model)
        : model_(model),
          lane_({model.get_config().scene_dims.width,
                 model.get_config().lane_thickness -
                 model.get_config().lane_spacing},
                lane_color),
          green_box_({model.get_config().scene_dims.width, model
                .get_config().scene_dims.height}, green_box_color)
{ }

void
View::draw(ge211::Sprite_set& set)
{
    if (model_.get_avatar().is_junk())
    {
        int final_score = model_.get_score() + model_.get_avatar()
                .get_collectible_count();
        set.add_sprite(green_box_, {0,0}, 7);

        ge211::Text_sprite::Builder game_over_builder(sans80);
        game_over_builder.color(game_over_color) << "GAME OVER!";
        game_over_.reconfigure(game_over_builder);
        set.add_sprite(game_over_, {50,50}, 8);

        ge211::Text_sprite::Builder collectible_count_builder(sans30);
        collectible_count_builder.color(collectible_count_color) <<
        "Lilypads collected: " << model_.get_avatar().get_collectible_count();
        collectible_count_.reconfigure(collectible_count_builder);
        set.add_sprite(collectible_count_, {50,200}, 8);

        ge211::Text_sprite::Builder lanes_count_builder(sans30);
        lanes_count_builder.color(lanes_count_color) <<
        "Lanes crossed: " << model_.get_score();
        lanes_count_.reconfigure(lanes_count_builder);
        set.add_sprite(lanes_count_, {50,300}, 8);

        ge211::Text_sprite::Builder score_count_builder(sans30);
        score_count_builder.color(score_count_color) <<
        "Final Score: " << final_score;
        score_count_.reconfigure(score_count_builder);
        set.add_sprite(score_count_, {50,400}, 8);

        model_.get_avatar().set_junk(false);
    }

    // draw all lanes
    for (int i = 0; i < 6; i++)
    {
        set.add_sprite(lane_, {0, i*100}, 0);
    }

    set.add_sprite(frog_, model_.get_avatar().position(), 5);

    ge211::Text_sprite::Builder collectible_count_builder2(sans30);
    collectible_count_builder2.color(collectible_count_color) <<
    "Lilypads: " << model_.get_avatar().get_collectible_count();
    collectible_count_.reconfigure(collectible_count_builder2);
    set.add_sprite(collectible_count_, {0,0}, 6);

    // iterate through model forest_objects_ to draw each
    for (size_t i = 0; i < model_.forest_objects_.size(); i++) {

        if (model_.forest_objects_[i].material() ==
        Forest_object::Material::mushroom) {
            set.add_sprite(mushroom_,
                           model_.forest_objects_[i].position(), 4);
        }

        else if (model_.forest_objects_[i].material() ==
            Forest_object::Material::frog) {
            set.add_sprite(frog_,
                           model_.forest_objects_[i].position(), 5);
        }

        else if (model_.forest_objects_[i].material() ==
            Forest_object::Material::bush){
            set.add_sprite(bush_,
                           model_.forest_objects_[i].position(), 4);
        }

        else if (model_.forest_objects_[i].material() ==
            Forest_object::Material::owl){
            set.add_sprite(owl_,
                           model_.forest_objects_[i].position(), 4);
        }

        else if (model_.forest_objects_[i].material() ==
            Forest_object::Material::lilypad){
            set.add_sprite(lilypad_,
                           model_.forest_objects_[i].position(), 3);
        }
    }

}

ge211::Dims<int>
View::initial_window_dimensions() const
{
    return model_.get_config().scene_dims;
}

