// File: yagol_curses_view.cpp
#include "yagol_curses_view.hpp"

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
}

//////////////////////////////////////////////////////////////////////

YAGoLCursesView::~YAGoLCursesView()
{
    ::curs_set(1);
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

    int key = getch();

    switch (key) {
        case 'q':
            event = YAGoLEvent::quit;
            break;
        case 's':
            event = YAGoLEvent::step;
            break;
        case ' ':
            event = YAGoLEvent::start_or_stop;
            break;
        case 'r':
            event = YAGoLEvent::redraw;
            break;
        case 'R':
            event = YAGoLEvent::randomize;
            break;
        case -1: // TIMEOUT
            event = YAGoLEvent::null;
            break;
        default:
            event = YAGoLEvent::unknown;
            break;
    }

    return event;
}

//////////////////////////////////////////////////////////////////////

void YAGoLCursesView::stop()
{
    ::timeout(50);
}
void YAGoLCursesView::start()
{
    ::timeout(-1);
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
    ::endwin();
}

//////////////////////////////////////////////////////////////////////

std::pair<int, int> YAGoLCursesView::get_term_size()
{
    return std::make_pair(COLS, LINES);
}
