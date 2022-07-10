#pragma once

#include "model.hxx"
#include "view.hxx"

#include <ge211.hxx>

class Controller : public ge211::Abstract_game
{
public:

    // CONSTRUCTOR

    /// Constructs a `Controller` given a reference to the model that
    /// stores the actual state of the game.
    explicit Controller(Model&);

    // MEMBER FUNCTIONS

    /// Defines how the game responds to mouse clicks.
    void on_mouse_up(ge211::Mouse_button, ge211::Posn<int>) override;

    void on_frame(double dt) override;

    /// Defines how the game responds to key events. There are four keys
    /// recognized:
    ///
    ///  - On up arrow, avatar moves up the screen.
    ///  - On down arrow, avatar moves down the screen.
    ///  - On down arrow, avatar moves to the left.
    ///  - On down arrow, avatar moves to the right.
    void on_key(ge211::Key) override;

    /// The GE211 framework calls this to find out how big to make the
    /// window.
    ge211::Dims<int> initial_window_dimensions() const override;

protected:
    /// The GE211 framework calls this at regular intervals to ask the
    /// game what should appear on the screen.
    void draw(ge211::Sprite_set& set) override;

private:
    // MEMBER VARIABLES

    /// Reference to the model for controller to have access.
    Model& model_;

    /// Contains game's data and code for displaying model's state
    /// to screen.
    View view_;
};
