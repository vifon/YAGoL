// File: yagol_allegro_view.cpp
#include "yagol_allegro_view.hpp"

#include <tuple>
#include <list>
#include <stdexcept>
#include <cctype>

namespace yagol {

//////////////////////////////////////////////////////////////////////

AllegroView::AllegroView()
    : screen_w_(1280)
    , screen_h_(720)
{
    al_init();
    al_install_keyboard();
    al_init_primitives_addon();
    al_set_new_display_flags(ALLEGRO_GENERATE_EXPOSE_EVENTS);
    display_ = al_create_display(screen_w_, screen_h_);
    event_queue_ = al_create_event_queue();
    al_register_event_source(event_queue_, al_get_display_event_source(display_));
    al_register_event_source(event_queue_, al_get_keyboard_event_source());

    background_color_ = al_map_rgb(0x11, 0x11, 0x11);

    cell_bitmap_alive_ = al_create_bitmap(cell_size, cell_size);
    al_set_target_bitmap(cell_bitmap_alive_);
    al_clear_to_color(al_map_rgb(0x00, 0xF3, 0x0A));

    cell_bitmap_dead_ = al_create_bitmap(cell_size, cell_size);
    al_set_target_bitmap(cell_bitmap_dead_);
    al_clear_to_color(background_color_);

    al_set_target_bitmap(al_get_backbuffer(display_));

    al_clear_to_color(background_color_);
}

//////////////////////////////////////////////////////////////////////

AllegroView::~AllegroView()
{
    close();
}

//////////////////////////////////////////////////////////////////////

void AllegroView::set_state(size_t x, size_t y, bool state)
{
    if (state) {
        al_draw_bitmap(cell_bitmap_alive_, x*cell_size, y*cell_size, 0);
    } else {
        al_draw_bitmap(cell_bitmap_dead_, x*cell_size, y*cell_size, 0);
    }
}

//////////////////////////////////////////////////////////////////////

Event AllegroView::get_event()
{
    Event event;

    ALLEGRO_EVENT ev;
    bool event_received = al_get_next_event(event_queue_, &ev);

    if (!event_received) {
        return EventType::null;
    }

    if (ev.type == ALLEGRO_EVENT_KEY_CHAR) {
        try {
            event = event_map_.at(ev.keyboard.keycode);
        } catch (const std::out_of_range& e) {
            event = Event(EventType::unknown);
        }
    } else if (ev.type == ALLEGRO_EVENT_DISPLAY_EXPOSE) {
        event = EventType::redraw;
    } else if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
        event = EventType::resize;
    } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        event = EventType::quit;
    }

    return event;
}

const std::map<int, Event> AllegroView::event_map_ =
{ { ALLEGRO_KEY_1        , Event( EventType::speed     , 1  ) }  ,
  { ALLEGRO_KEY_2        , Event( EventType::speed     , 2  ) }  ,
  { ALLEGRO_KEY_3        , Event( EventType::speed     , 3  ) }  ,
  { ALLEGRO_KEY_4        , Event( EventType::speed     , 4  ) }  ,
  { ALLEGRO_KEY_SPACE    , Event( EventType::toggle         ) }  ,
  { ALLEGRO_KEY_Q        , Event( EventType::quit           ) }  ,
  { ALLEGRO_KEY_R        , Event( EventType::randomize      ) }  ,
  { ALLEGRO_KEY_S        , Event( EventType::step           ) } };

//////////////////////////////////////////////////////////////////////

void AllegroView::clear()
{
    al_clear_to_color(background_color_);
}

//////////////////////////////////////////////////////////////////////

void AllegroView::show()
{
    al_flip_display();
}

//////////////////////////////////////////////////////////////////////

void AllegroView::close()
{
    al_destroy_display(display_);
    al_destroy_event_queue(event_queue_);
    al_destroy_bitmap(cell_bitmap_alive_);
    al_destroy_bitmap(cell_bitmap_dead_);
    al_shutdown_primitives_addon();
}

//////////////////////////////////////////////////////////////////////

void AllegroView::notify(std::string message, const int width)
{
    // TODO
}

//////////////////////////////////////////////////////////////////////

std::string AllegroView::prompt_for_string(std::string prompt, const unsigned int width)
{
    // TODO
    return "1/5";
}

//////////////////////////////////////////////////////////////////////

std::pair<int, int> AllegroView::get_size() const
{
    return std::make_pair(screen_w_ / 10,
                          screen_h_ / 10);
}

} // namespace yagol
