// File: yagol_curses_view.cpp
#include "yagol_curses_view.hpp"

#include <tuple>
#include <list>

//////////////////////////////////////////////////////////////////////

YAGoLCursesView::YAGoLCursesView(char alive_char, char dead_char)
    : alive_char_( alive_char )
    , dead_char_( dead_char )
{
    ::initscr();
    ::cbreak();
    ::noecho();
    ::start_color();
    ::keypad(stdscr, 1);
    ::timeout(-1);
    ::curs_set(0);
    ::refresh();

    map_events();
}

//////////////////////////////////////////////////////////////////////

YAGoLCursesView::~YAGoLCursesView()
{
    close();
}

//////////////////////////////////////////////////////////////////////

void YAGoLCursesView::set_state(size_t x, size_t y, bool state)
{
    if (state) {
        mvaddch(y,x, alive_char_);
    } else {
        mvaddch(y,x, dead_char_);
    }
}

//////////////////////////////////////////////////////////////////////

YAGoLEvent YAGoLCursesView::get_event()
{
    YAGoLEvent event;

    short key = getch();

    if (event_map_.count(key) != 0) {
        event = event_map_[key];
    } else {
        event = YAGoLEvent::unknown;
    }

    return event;
}

//////////////////////////////////////////////////////////////////////

void YAGoLCursesView::stop()
{
    ::timeout(-1);
}
void YAGoLCursesView::start()
{
    ::timeout(50);
}

//////////////////////////////////////////////////////////////////////

void YAGoLCursesView::clear()
{
    ::clear();
}

//////////////////////////////////////////////////////////////////////

void YAGoLCursesView::show()
{
    ::refresh();
}

//////////////////////////////////////////////////////////////////////

void YAGoLCursesView::close()
{
    ::curs_set(1);
    ::endwin();
}

//////////////////////////////////////////////////////////////////////

static inline int center_window(const int screen_size, const int win_size)
{
    return ( ( screen_size - (win_size+2) ) / 2 );
}

void YAGoLCursesView::notify(std::string message, const int width = 50)
{
    int w,h;

    std::tie(w,h) = get_term_size();

    std::list<std::string> lines;
    while (message.length() > width) {
        std::string line = message.substr(0, width);
        message.erase(0, width);
        lines.push_back(line);
    }
    if (!message.empty()) {
        lines.push_back(message);
    }

    const int height = lines.size();

    WINDOW* window = newwin(height+2,
                            width+2,
                            center_window(h, height),
                            center_window(w, width));
    box(window, 0,0);

    int i = 0;
    for (auto line : lines) {
        mvwaddstr(window, ++i, 1, line.c_str());
    }

    wrefresh(window);

    wtimeout(window, -1);
    wgetch(window);

    delwin(window);
}

//////////////////////////////////////////////////////////////////////

std::string YAGoLCursesView::prompt_for_string(std::string prompt)
{
    return "TODO";
}

//////////////////////////////////////////////////////////////////////

std::pair<int, int> YAGoLCursesView::get_term_size() const
{
    return std::make_pair(COLS, LINES);
}

//////////////////////////////////////////////////////////////////////

std::map<int, YAGoLEvent> YAGoLCursesView::event_map_;

std::map<int, YAGoLEvent>& YAGoLCursesView::map_events()
{
    event_map_[' '] = YAGoLEvent::start_or_stop;
    event_map_['q'] = YAGoLEvent::quit;
    event_map_['R'] = YAGoLEvent::randomize;
    event_map_['r'] = YAGoLEvent::redraw;
    event_map_['s'] = YAGoLEvent::step;
    event_map_['T'] = YAGoLEvent::TEST; // TODO
    event_map_[-1 ] = YAGoLEvent::null;

    return event_map_;
}
