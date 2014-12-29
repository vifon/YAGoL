// File: yagol_controller.cpp
#include "yagol_controller.hpp"

#include <stdexcept>
#include <tuple>
#include <cstddef>
#include <cstdlib>
#include <unistd.h>

namespace yagol {

//////////////////////////////////////////////////////////////////////

Controller::Controller(Model& model, View& view)
    : model_( model )
    , view_( view )
    , current_speed_( NORMAL )
    , stopped_( true )
    , last_random_range_(1)
    , last_random_density_(1)
{
    resize(); // set the proper board size
    randomize(5, 1); // 1/5: 20% alive, 80% dead

    redraw();
}

//////////////////////////////////////////////////////////////////////

int Controller::event_loop()
{
    Event event;

    while (( event = view_.get_event() ).type != EventType::quit) {
        try {
            switch (event.type) {
                case EventType::redraw:
                    redraw();
                    break;
                case EventType::randomize:
                    randomize_with_prompt();
                    redraw();
                    break;
                case EventType::stop:
                    stop();
                    break;
                case EventType::start:
                    start();
                    break;
                case EventType::toggle:
                    toggle();
                    break;
                case EventType::speed:
                    set_speed(event.arg);
                    break;
                case EventType::change_rules:
                    change_rules();
                    break;
                case EventType::resize:
                    resize();
                    break;
                case EventType::null:
                    if (!stopped_)
                        step();
                    break;
                case EventType::step:
                    step();
                    break;
                case EventType::unknown:
                    break;
                default:
                    throw unhandled_event(event.type);
                    break;
            }
        } catch (const unhandled_event& e) {
            view_.close();
            return e.event_id();
        } catch (const std::invalid_argument& e) {
            notify("error");
        }

        wait(current_speed_);
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////

void Controller::redraw()
{
    view_.clear();
    size_t x = 0;
    size_t y = 0;
    const size_t width = model_.width();
    for (auto it : model_) {
        view_.set_state(x++,y, it);

        if (x >= width) {
            x = 0;
            ++y;
        }
    }

    view_.show();
}

//////////////////////////////////////////////////////////////////////

void Controller::stop()
{
    stopped_ = true;
}
void Controller::start()
{
    stopped_ = false;
}
void Controller::toggle()
{
    if (stopped_) {
        start();
    } else {
        stop();
    }
}

//////////////////////////////////////////////////////////////////////

void Controller::step()
{
    for (auto it : model_.next_generation()) {
        size_t x,y;
        bool val;

        std::tie(x,y, val) = Model::unpack_diff_iterator(it);

        view_.set_state(x,y, val);
    }

    view_.show();
}

//////////////////////////////////////////////////////////////////////

void Controller::randomize(int range, int density)
{
    last_random_range_   = range;
    last_random_density_ = density;
    for (auto it : model_) {
        it = (std::rand() % range) < density;
    }
}
void Controller::randomize()
{
    for (auto it : model_) {
        it = (std::rand() % last_random_range_) < last_random_density_;
    }
}

//////////////////////////////////////////////////////////////////////

void Controller::randomize_with_prompt()
{
    int range;
    int density;

    std::tie(density, range) = view_.prompt_for_numbers_with_slash("Randomize: specify the alive/dead ratio");

    randomize(range, density);
}

//////////////////////////////////////////////////////////////////////

void Controller::notify(const std::string& message)
{
    redraw();
    view_.notify(message);
    redraw();
}

//////////////////////////////////////////////////////////////////////

void Controller::wait(const time_type t)
{
    if (t != INSTANT) {
        usleep(t);
    }
}

//////////////////////////////////////////////////////////////////////

void Controller::set_speed(const int arg)
{
    switch (arg) {
        case 1:
            current_speed_ = SLOW;
            break;
        case 2:
            current_speed_ = NORMAL;
            break;
        case 3:
            current_speed_ = FAST;
            break;
        case 4:
            current_speed_ = INSTANT;
            break;
        default:
            throw std::invalid_argument("Controller::set_speed");
            break;
    }
}

//////////////////////////////////////////////////////////////////////

void Controller::change_rules()
{
    int survival;
    int birth;

    std::tie(survival, birth) = view_.prompt_for_numbers_with_slash("Rules change: \"survival/birth\"");

    model_.set_rules( std::to_string(survival),
                      std::to_string(birth   ) );
}

//////////////////////////////////////////////////////////////////////

void Controller::resize()
{
    const size_t old_width = model_.width();
    const size_t old_height = model_.height();

    size_t w,h;
    std::tie(w,h) = view_.get_size();
    model_.resize(w,h);

    for (size_t y = 0; y < old_height; ++y) {
        for (size_t x = old_width; x < model_.width(); ++x) {
            model_(x,y) = (std::rand() % last_random_range_) < last_random_density_;
        }
    }

    for (size_t y = old_height; y < model_.height(); ++y) {
        for (size_t x = 0; x < model_.width(); ++x) {
            model_(x,y) = (std::rand() % last_random_range_) < last_random_density_;
        }
    }

    redraw();
}

} // namespace yagol
