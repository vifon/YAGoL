// File: yagol_view.hpp
#ifndef _YAGOL_VIEW_H_
#define _YAGOL_VIEW_H_

#include <string>
#include <cstddef>
#include <stdexcept>

//////////////////////////////////////////////////////////////////////

enum class YAGoLEvent : short
{
    quit = 0,
    redraw,
    randomize,
    stop,
    start,
    toggle,
    step,
    null,
    unknown
};

//////////////////////////////////////////////////////////////////////

class YAGoLView
{
  public:
    virtual ~YAGoLView();

    virtual void set_state(size_t x, size_t y, bool state) = 0;
    void make_alive(size_t x, size_t y);
    void make_dead(size_t x, size_t y);

    virtual YAGoLEvent get_event() = 0;

    virtual bool started() const = 0;

    virtual void stop() = 0;
    virtual void start() = 0;
    void toggle();

    virtual void clear() = 0;
    virtual void show() = 0;
    virtual void close() = 0;

    virtual void notify(std::string message, const int width = 50) = 0;

    virtual std::string prompt_for_string(std::string prompt, const unsigned int width = 50) = 0;
    virtual int prompt_for_number(std::string prompt, const unsigned int width = 50) throw(std::invalid_argument);
};

//////////////////////////////////////////////////////////////////////

#endif
