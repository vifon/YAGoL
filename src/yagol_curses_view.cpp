// File: yagol_curses_view.cpp
#include "yagol_curses_view.hpp"

#include <tuple>
#include <list>
#include <cctype>

//////////////////////////////////////////////////////////////////////

YAGoLCursesView::YAGoLCursesView(char alive_char, char dead_char)
    : alive_char_( alive_char )
    , dead_char_( dead_char )
    , auto_ ( false )
{
    ::initscr();
    ::cbreak();
    ::noecho();
    ::start_color();
    ::keypad(stdscr, 1);
    ::timeout(-1);
    ::curs_set(0);
    ::refresh();
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

bool YAGoLCursesView::started() const
{
    return auto_;
}

//////////////////////////////////////////////////////////////////////

void YAGoLCursesView::stop()
{
    ::timeout(-1);
    auto_ = false;
}
void YAGoLCursesView::start()
{
    // one frame per 50 ms or 20 frames per second
    ::timeout(0);
    auto_ = true;
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

static std::list<std::string> split_at_N_char(std::string& str, const unsigned int width)
{
    std::list<std::string> lines;

    while (str.length() > width) {
        std::string line = str.substr(0, width);
        str.erase(0, width);
        lines.push_back(line);
    }
    if (!str.empty()) {
        lines.push_back(str);
    }

    return lines;
}

void YAGoLCursesView::notify(std::string message, const int width)
{
    int w,h;

    std::tie(w,h) = get_term_size();

    std::list<std::string> lines = split_at_N_char(message, width);

    const int height = lines.size();

    WINDOW* window = newwin(height+2,
                            width+2,
                            center_window(h, height),
                            center_window(w, width));
    WINDOW* subwindow = derwin(window,
                               height,
                               width,
                               1,1);
    box(window, 0,0);

    int i = 0;
    for (auto line : lines) {
        mvwaddstr(subwindow, i++, 0, line.c_str());
    }

    wrefresh(window);
    touchwin(window);
    wrefresh(subwindow);

    wtimeout(subwindow, -1);
    wgetch(subwindow);

    delwin(subwindow);
    delwin(window);
}

//////////////////////////////////////////////////////////////////////

static inline void move_rel(WINDOW* window, const int y_move, const int x_move)
{
    int y,x;
    getyx(window, y,x);

    wmove(window, y + y_move,
                  x + x_move);
}

// TODO
std::string YAGoLCursesView::prompt_for_string(std::string prompt, const unsigned int width)
{
    int w,h;

    std::tie(w,h) = get_term_size();

    std::list<std::string> lines = split_at_N_char(prompt, width);

    const unsigned int height = lines.size() + 2;

    WINDOW* window = newwin(height+2,
                            width+2,
                            center_window(h, height),
                            center_window(w, width));
    WINDOW* subwindow = derwin(window,
                               height,
                               width,
                               1,1);
    box(window, 0,0);

    int i = 0;
    for (auto line : lines) {
        mvwaddstr(subwindow, i++, 0, line.c_str());
    }


    // underline the text area
    prompt.assign(width, ' ');  // lets reuse the argument
    wattron(subwindow, A_UNDERLINE);
    mvwaddstr(subwindow, height-1, 0, prompt.c_str());
    prompt.clear();

    // prepare for input
    wtimeout(subwindow, -1);
    curs_set(1);
    wrefresh(window);
    touchwin(window);
    wrefresh(subwindow);
    wmove(subwindow, height-1, 0);

    const int KEY_BACKSPACE2 = '';

    int k;
    while (( k = wgetch(subwindow) ) != '\n') {
        if (std::isprint(k) && prompt.size() < width-2) {
            waddch(subwindow, k);
            prompt += k;
        } else if (!prompt.empty() &&
                   (k == KEY_BACKSPACE || k == KEY_BACKSPACE2)) {
            move_rel(subwindow, 0,-1);
            waddch(subwindow, ' ');
            move_rel(subwindow, 0,-1);
            prompt.pop_back();
        }
    }
    wattroff(subwindow, A_UNDERLINE);
    curs_set(0);

    delwin(subwindow);
    delwin(window);

    return prompt;
}

//////////////////////////////////////////////////////////////////////

std::pair<int, int> YAGoLCursesView::get_term_size() const
{
    return std::make_pair(COLS, LINES);
}

//////////////////////////////////////////////////////////////////////

std::map<int, YAGoLEvent> YAGoLCursesView::event_map_ =
    { { ' ' , YAGoLEvent::toggle    }  ,
      { 'q' , YAGoLEvent::quit      }  ,
      { 'R' , YAGoLEvent::randomize }  ,
      { 'r' , YAGoLEvent::redraw    }  ,
      { 's' , YAGoLEvent::step      }  ,
      { -1  , YAGoLEvent::null      } };
