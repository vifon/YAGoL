// File: yagol_controller.hpp
#ifndef _YAGOL_CONTROLLER_H_
#define _YAGOL_CONTROLLER_H_

#include "yagol_model.hpp"
#include "yagol_view.hpp"

#include <cstddef>
#include <cstdlib>
#include <exception>

//////////////////////////////////////////////////////////////////////

class YAGoLController
{
  public:
    class unhandled_event : public std::exception
    {
      public:
        const char* what() const throw()
        {
            return "YAGoLController::unhandled_event";
        }
    };

    YAGoLController(YAGoLModel& model, YAGoLView& view)
        : model_( model )
        , view_( view )
        , stopped_ ( true )
    {
        randomize(5);

        redraw();
    }

    int event_loop()
    {
        YAGoLEvent event;

        while (( event = view_.get_event() ) != YAGoLEvent::quit) {
            try {
                switch (event) {
                    case YAGoLEvent::redraw:
                        redraw();
                        break;
                    case YAGoLEvent::randomize:
                        // stop();
                        // int limit = view_.get_number(); // TODO
                        // int density = view_.get_number();
                        // redraw();
                        // randomize();
                        // redraw();
                        break;
                    case YAGoLEvent::stop:
                        stop();
                        break;
                    case YAGoLEvent::start:
                        start();
                        break;
                    case YAGoLEvent::start_or_stop:
                        start_or_stop();
                        break;
                    case YAGoLEvent::step:
                    case YAGoLEvent::null:
                        step();
                        break;
                    case YAGoLEvent::unknown:
                        break;
                    default:
                        throw unhandled_event();
                        break;
                }
            } catch (unhandled_event& e) {
                view_.close();
                throw;
            }
        }

        return 0;
    }

  private:
    void redraw()
    {
        view_.clear();
        size_t x = 0;
        size_t y = 0;
        const size_t width = model_.width();
        for (auto it : model_) {
            view_.set_state(x,y, it);

            if (x >= width) {
                x = 0;
                ++y;
            }
        }

        view_.show();
    }

    void stop()
    {
        view_.stop();
        stopped_ = true;
    }
    void start()
    {
        view_.start();
        stopped_ = false;
    }
    void start_or_stop()
    {
        if (stopped_) {
            view_.start();
        } else {
            view_.stop();
        }
        stopped_ = !stopped_;
    }

    void step()
    {
        for (auto it : model_.next_generation()) {
            size_t x,y;
            bool val;

            std::tie(x,y, val) = YAGoLModel::unpack_diff_iterator(it);

            view_.set_state(x,y, val);
        }

        view_.show();
    }

    void randomize(int range, int density = 1)
    {
        for (auto it : model_) {
            it = (std::rand() % range) < density;
        }
    }

    YAGoLModel& model_;
    YAGoLView& view_;

    bool stopped_;
};

//////////////////////////////////////////////////////////////////////

#endif
