// File: yagol_view.hpp
#ifndef _YAGOL_VIEW_H_
#define _YAGOL_VIEW_H_

#include "lexical_cast.hpp"

#include <string>
#include <cstddef>

//////////////////////////////////////////////////////////////////////

enum class YAGoLEvent : short
{
    quit = 0,
    redraw,
    randomize,
    stop,
    start,
    start_or_stop,
    step,
    null,
    unknown,
    TEST
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

    virtual void notify(std::string message) = 0;

    virtual std::string prompt_for_string(std::string prompt) = 0;
    virtual int prompt_for_number(std::string prompt) throw(std::bad_cast)
    {
        std::string answer = prompt_for_string(prompt);
        int number;

        try {
            number = vfn::lexical_cast<int>(prompt);
        } catch (const std::bad_cast& e) {
            throw e;
        }

        return number;
    }
};

//////////////////////////////////////////////////////////////////////

#endif
