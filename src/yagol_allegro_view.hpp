// File: yagol_allegro_view.hpp
#ifndef _YAGOL_ALLEGRO_VIEW_H_
#define _YAGOL_ALLEGRO_VIEW_H_

#include "yagol_view.hpp"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <map>

namespace yagol {

//////////////////////////////////////////////////////////////////////

class AllegroView : public View
{
  public:
    AllegroView();
    ~AllegroView();

    void set_state(size_t x, size_t y, bool state);

    Event get_event();

    bool started() const;

    void stop();
    void start();

    void clear();
    void show();
    void close();

    void notify(std::string message, const int width = 50);
    std::string prompt_for_string(std::string prompt, const unsigned int width = 50);

    std::pair<int, int> get_size() const;

  private:
    static const std::map<int, Event> event_map_;

    const int screen_w_;
    const int screen_h_;

    ALLEGRO_DISPLAY* display_;
    ALLEGRO_EVENT_QUEUE* event_queue_;
    ALLEGRO_BITMAP* cell_bitmap_alive_;
    ALLEGRO_BITMAP* cell_bitmap_dead_;

    ALLEGRO_COLOR background_color_;

    static const int cell_size = 10;
};

//////////////////////////////////////////////////////////////////////

} // namespace yagol

#endif
