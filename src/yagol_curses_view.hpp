// File: yagol_curses_view.hpp
#ifndef _YAGOL_CURSES_VIEW_H_
#define _YAGOL_CURSES_VIEW_H_

#include "yagol_view.hpp"

#include <utility>
#include <cstddef>
#include <ncurses.h>

//////////////////////////////////////////////////////////////////////

class YAGoLCursesView : public YAGoLView
{
  public:
    YAGoLCursesView(char alive_char = '@', char dead_char = ' ');
    ~YAGoLCursesView();

    void set_state(size_t x, size_t y, bool state);

    void change_alive_char();
    void change_dead_char();

    YAGoLEvent get_event();

    void stop();
    void start();

    void clear();
    void show();
    void close();

    std::pair<int, int> get_term_size();

  private:
    char alive_char_;
    char dead_char_;
};

//////////////////////////////////////////////////////////////////////

#endif
