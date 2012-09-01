// File: yagol_controller.cpp
#include "yagol_controller.hpp"

#include <stdexcept>
#include <tuple>
#include <cstddef>
#include <cstdlib>
#include <unistd.h>

//////////////////////////////////////////////////////////////////////

YAGoLController::YAGoLController(YAGoLModel& model, YAGoLView& view)
    : model_( model )
    , view_( view )
    , current_speed_( NORMAL )
    , stopped_( true )
{
    resize(); // set the proper board size
    randomize(5, 1); // 1/5: 20% alive, 80% dead

    redraw();
}

//////////////////////////////////////////////////////////////////////

int YAGoLController::event_loop()
{
    YAGoLEvent event;

    while (( event = view_.get_event() ).type != YAGoLEventType::quit) {
        try {
            switch (event.type) {
                case YAGoLEventType::redraw:
                    redraw();
                    break;
                case YAGoLEventType::randomize:
                    randomize_with_prompt();
                    redraw();
                    break;
                case YAGoLEventType::stop:
                    stop();
                    break;
                case YAGoLEventType::start:
                    start();
                    break;
                case YAGoLEventType::toggle:
                    toggle();
                    break;
                case YAGoLEventType::speed:
                    set_speed(event.arg);
                    break;
                case YAGoLEventType::resize:
                    resize();
                    break;
                case YAGoLEventType::null:
                    if (!stopped_)
                        step();
                    break;
                case YAGoLEventType::step:
                    step();
                    break;
                case YAGoLEventType::unknown:
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

void YAGoLController::redraw()
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

void YAGoLController::stop()
{
    stopped_ = true;
}
void YAGoLController::start()
{
    stopped_ = false;
}
void YAGoLController::toggle()
{
    if (stopped_) {
        start();
    } else {
        stop();
    }
}

//////////////////////////////////////////////////////////////////////

void YAGoLController::step()
{
    for (auto it : model_.next_generation()) {
        size_t x,y;
        bool val;

        std::tie(x,y, val) = YAGoLModel::unpack_diff_iterator(it);

        view_.set_state(x,y, val);
    }

    view_.show();
}

//////////////////////////////////////////////////////////////////////

void YAGoLController::randomize(int range, int density)
{
    last_range_   = range;
    last_density_ = density;
    for (auto it : model_) {
        it = (std::rand() % range) < density;
    }
}
void YAGoLController::randomize()
{
    for (auto it : model_) {
        it = (std::rand() % last_range_) < last_density_;
    }
}

//////////////////////////////////////////////////////////////////////

void YAGoLController::randomize_with_prompt()
{
    std::string ratio = view_.prompt_for_string("Randomize: specify the alive/dead ratio");
    redraw();

    size_t pos;
    int density = std::stoul(ratio, &pos);
    if (ratio[pos] != '/') {
        throw std::invalid_argument("YAGoLController::randomize_with_prompt");
    }

    std::string second_number = ratio.substr(pos+1);
    int range = std::stoul(second_number, &pos);
    if (second_number[pos] != '\0') {
        throw std::invalid_argument("YAGoLController::randomize_with_prompt");
    }

    randomize(range, density);
}

//////////////////////////////////////////////////////////////////////

void YAGoLController::notify(const std::string& message)
{
    redraw();
    view_.notify(message);
    redraw();
}

//////////////////////////////////////////////////////////////////////

void YAGoLController::wait(const time_type t)
{
    usleep(t);
}

//////////////////////////////////////////////////////////////////////

void YAGoLController::set_speed(const int arg)
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
        default:
            throw std::invalid_argument("YAGoLController::set_speed");
            break;
    }
}

//////////////////////////////////////////////////////////////////////

void YAGoLController::resize()
{
    const size_t old_width = model_.width();
    const size_t old_height = model_.height();

    size_t w,h;
    std::tie(w,h) = view_.get_size();
    model_.resize(w,h);

    for (size_t y = 0; y < old_height; ++y) {
        for (size_t x = old_width; x < model_.width(); ++x) {
            model_(x,y) = (std::rand() % last_range_) < last_density_;
        }
    }

    for (size_t y = old_height; y < model_.height(); ++y) {
        for (size_t x = 0; x < model_.width(); ++x) {
            model_(x,y) = (std::rand() % last_range_) < last_density_;
        }
    }

    redraw();
}
