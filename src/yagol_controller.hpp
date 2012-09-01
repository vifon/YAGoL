// File: yagol_controller.hpp
#ifndef _YAGOL_CONTROLLER_H_
#define _YAGOL_CONTROLLER_H_

#include "yagol_model.hpp"
#include "yagol_view.hpp"

#include <exception>

//////////////////////////////////////////////////////////////////////

class YAGoLController
{
  public:
    class unhandled_event : public std::exception
    {
      public:
        unhandled_event(const YAGoLEventType& event)
            : event_( event )
        {}

        const char* what() const throw()
        {
            return "YAGoLController::unhandled_event";
        }

        int event_id() const throw()
        {
            return static_cast<short>(event_);
        }

      private:
        YAGoLEventType event_;
    };

    ////////////////////////////////////////

    YAGoLController(YAGoLModel& model, YAGoLView& view);

    int event_loop();

  private:
    void redraw();

    void stop();
    void start();
    void toggle();

    void step();

    void randomize(int range, int density = 1);
    void randomize();

    void notify(const std::string& message);

    typedef useconds_t time_type;
    void wait(const time_type t);

    void set_speed(const int arg);

    ////////////////////////////////////////

    YAGoLModel& model_;
    YAGoLView&  view_;

    ////////////////////////////////////////

    static const time_type SLOW   = 150 * 1000;
    static const time_type NORMAL =  75 * 1000;
    static const time_type FAST   =  25 * 1000;

    time_type current_speed_;
    bool      stopped_;
};

//////////////////////////////////////////////////////////////////////

#endif
