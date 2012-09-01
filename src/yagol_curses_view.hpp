// File: yagol_curses_view.hpp
#ifndef _YAGOL_CURSES_VIEW_H_
#define _YAGOL_CURSES_VIEW_H_

#include "yagol_view.hpp"

#include <utility>
#include <string>
#include <map>
#include <cstddef>
#include <ncurses.h>

//////////////////////////////////////////////////////////////////////

class YAGoLCursesView : public YAGoLView
{
  public:
    YAGoLCursesView(char alive_char = '@', char dead_char = ' ');
    ~YAGoLCursesView();

    void set_state(size_t x, size_t y, bool state);

    YAGoLEvent get_event();

    bool started() const;

    void stop();
    void start();

    void clear();
    void show();
    void close();

    void notify(std::string message, const int width = 50);
    std::string prompt_for_string(std::string prompt, const unsigned int width = 50);

    std::pair<int, int> get_term_size() const;

  private:
    static const std::map<int, YAGoLEvent> event_map_;

    char alive_char_;
    char dead_char_;
};

//////////////////////////////////////////////////////////////////////

#endif
