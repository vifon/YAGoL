// File: yagol_controller.hpp
#ifndef _YAGOL_CONTROLLER_H_
#define _YAGOL_CONTROLLER_H_

#include "yagol_model.hpp"
#include "yagol_view.hpp"

#include <exception>

namespace yagol {

//////////////////////////////////////////////////////////////////////

class Controller
{
  public:
    class unhandled_event : public std::exception
    {
      public:
        unhandled_event(const EventType& event)
            : event_( event )
        {}

        const char* what() const throw()
        {
            return "Controller::unhandled_event";
        }

        int event_id() const throw()
        {
            return static_cast<short>(event_);
        }

      private:
        EventType event_;
    };

    ////////////////////////////////////////

    Controller(Model& model, View& view);

    int event_loop();

  private:
    void redraw();

    void stop();
    void start();
    void toggle();

    void step();

    void randomize(int range, int density = 1);
    void randomize();
    void randomize_with_prompt();

    void notify(const std::string& message);

    void resize();

    typedef useconds_t time_type;
    void wait(const time_type t);

    void set_speed(const int arg);

    void change_rules();

    ////////////////////////////////////////

    Model& model_;
    View&  view_;

    ////////////////////////////////////////

    static const time_type SLOW    = 150 * 1000;
    static const time_type NORMAL  =  75 * 1000;
    static const time_type FAST    =  25 * 1000;
    static const time_type INSTANT =   0;

    time_type current_speed_;
    bool      stopped_;

    // used for resize
    int last_random_range_;
    int last_random_density_;
};

//////////////////////////////////////////////////////////////////////

} // namespace yagol

#endif
