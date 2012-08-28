// File: yagol_view.hpp
#ifndef _YAGOL_VIEW_H_
#define _YAGOL_VIEW_H_

#include <cstddef>

//////////////////////////////////////////////////////////////////////

enum class YAGoLEvent
{
    redraw,
    randomize,
    stop,
    start,
    start_or_stop,
    step,
    quit,
    null,
    unknown
};

//////////////////////////////////////////////////////////////////////

class YAGoLView
{
  public:
    virtual ~YAGoLView()
    {}

    virtual void set_state(size_t x, size_t y, bool state) = 0;
    void make_alive(size_t x, size_t y)
    {
        set_state(x,y, true);
    }
    void make_dead(size_t x, size_t y)
    {
        set_state(x,y, false);
    }

    virtual YAGoLEvent get_event() = 0;

    virtual void stop() = 0;
    virtual void start() = 0;

    virtual void clear() = 0;
    virtual void show() = 0;
    virtual void close() = 0;
};

//////////////////////////////////////////////////////////////////////

#endif
