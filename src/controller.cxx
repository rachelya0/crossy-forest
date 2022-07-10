#include "controller.hxx"
#include "model.hxx"

Controller::Controller(Model& model)
        : model_(model),
          view_(model)
{ }

void
Controller::on_mouse_up(ge211::Mouse_button, ge211::Posn<int>)
{
    if (model_.get_avatar().is_junk()) {
        model_.dead_state();
    }
}

void
Controller::on_key(ge211::Key key)
{
    if (key == ge211::Key::up()) {
        model_.move_up();
        model_.lock_down_ = false;
        model_.lock_left_ = false;
        model_.lock_right_ = false;

    }
    else if(key == ge211::Key::down()) {
        model_.move_down();
        model_.lock_up_ = false;
        model_.lock_left_ = false;
        model_.lock_right_ = false;

    }
    else if(key == ge211::Key::right()) {
        model_.move_right();
        model_.lock_down_ = false;
        model_.lock_left_ = false;
        model_.lock_up_ = false;

    }
    else if(key == ge211::Key::left()) {
        model_.move_left();
        model_.lock_up_ = false;
        model_.lock_down_ = false;
        model_.lock_right_ = false;

    }

}

void
Controller::on_frame(double dt)
{
    model_.on_frame(dt);
}

ge211::Dims<int>
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

void
Controller::draw(ge211::Sprite_set& set)
{
    view_.draw(set);
}
