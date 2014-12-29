// File: yagol_curses_view.cpp
#include "yagol_curses_view.hpp"

#include <tuple>
#include <list>
#include <stdexcept>
#include <cctype>

namespace yagol {

//////////////////////////////////////////////////////////////////////

CursesView::CursesView(char alive_char, char dead_char)
    : alive_char_( alive_char )
    , dead_char_( dead_char )
{
    ::initscr();
    ::cbreak();
    ::noecho();
    ::start_color();
    ::keypad(stdscr, 1);
    ::timeout(0);
    ::curs_set(0);
    ::refresh();
}

//////////////////////////////////////////////////////////////////////

CursesView::~CursesView()
{
    close();
}

//////////////////////////////////////////////////////////////////////

void CursesView::set_state(size_t x, size_t y, bool state)
{
    if (state) {
        mvaddch(y,x, alive_char_);
    } else {
        mvaddch(y,x, dead_char_);
    }
}

//////////////////////////////////////////////////////////////////////

Event CursesView::get_event()
{
    Event event;

    short key = getch();

    try {
        event = event_map_.at(key);
    } catch (const std::out_of_range& e) {
        event = Event(EventType::unknown);
    }

    return event;
}

const std::map<int, Event> CursesView::event_map_ =
{ { '1'        , Event( EventType::speed     , 1  ) }  ,
  { '2'        , Event( EventType::speed     , 2  ) }  ,
  { '3'        , Event( EventType::speed     , 3  ) }  ,
  { '4'        , Event( EventType::speed     , 4  ) }  ,
  { ' '        , Event( EventType::toggle         ) }  ,
  { 'q'        , Event( EventType::quit           ) }  ,
  { 'R'        , Event( EventType::randomize      ) }  ,
  { 'r'        , Event( EventType::redraw         ) }  ,
  { 's'        , Event( EventType::step           ) }  ,
  { 'u'        , Event( EventType::change_rules   ) }  ,
  { ERR        , Event( EventType::null           ) }  ,
  { KEY_RESIZE , Event( EventType::resize         ) } };

//////////////////////////////////////////////////////////////////////

void CursesView::clear()
{
    ::clear();
}

//////////////////////////////////////////////////////////////////////

void CursesView::show()
{
    ::refresh();
}

//////////////////////////////////////////////////////////////////////

void CursesView::close()
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

void CursesView::notify(std::string message, const int width)
{
    int w,h;

    std::tie(w,h) = get_size();

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

std::string CursesView::prompt_for_string(std::string prompt, const unsigned int width)
{
    int w,h;

    std::tie(w,h) = get_size();

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
    echo();
    keypad(subwindow, 1);
    curs_set(1);
    wrefresh(window);
    touchwin(window);
    wrefresh(subwindow);

    char* input = new char[width];

    mvwgetnstr(subwindow, height-1, 0, input, width);

    prompt.assign(input);
    delete[] input;

    wattroff(subwindow, A_UNDERLINE);
    noecho();
    curs_set(0);

    delwin(subwindow);
    delwin(window);

    return prompt;
}

//////////////////////////////////////////////////////////////////////

std::pair<int, int> CursesView::get_size() const
{
    return std::make_pair(COLS, LINES);
}

} // namespace yagol
