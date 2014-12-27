// File: yagol_allegro_view.cpp
#include "yagol_allegro_view.hpp"

#include <tuple>
#include <list>
#include <stdexcept>
#include <cctype>

//////////////////////////////////////////////////////////////////////

YAGoLAllegroView::YAGoLAllegroView()
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

YAGoLAllegroView::~YAGoLAllegroView()
{
    close();
}

//////////////////////////////////////////////////////////////////////

void YAGoLAllegroView::set_state(size_t x, size_t y, bool state)
{
    if (state) {
        al_draw_bitmap(cell_bitmap_alive_, x*cell_size, y*cell_size, 0);
    } else {
        al_draw_bitmap(cell_bitmap_dead_, x*cell_size, y*cell_size, 0);
    }
}

//////////////////////////////////////////////////////////////////////

YAGoLEvent YAGoLAllegroView::get_event()
{
    YAGoLEvent event;

    ALLEGRO_EVENT ev;
    bool event_received = al_get_next_event(event_queue_, &ev);

    if (!event_received) {
        return YAGoLEventType::null;
    }

    if (ev.type == ALLEGRO_EVENT_KEY_CHAR) {
        try {
            event = event_map_.at(ev.keyboard.keycode);
        } catch (const std::out_of_range& e) {
            event = YAGoLEvent(YAGoLEventType::unknown);
        }
    } else if (ev.type == ALLEGRO_EVENT_DISPLAY_EXPOSE) {
        event = YAGoLEventType::redraw;
    } else if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
        event = YAGoLEventType::resize;
    } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        event = YAGoLEventType::quit;
    }

    return event;
}

const std::map<int, YAGoLEvent> YAGoLAllegroView::event_map_ =
{ { ALLEGRO_KEY_1        , YAGoLEvent( YAGoLEventType::speed     , 1  ) }  ,
  { ALLEGRO_KEY_2        , YAGoLEvent( YAGoLEventType::speed     , 2  ) }  ,
  { ALLEGRO_KEY_3        , YAGoLEvent( YAGoLEventType::speed     , 3  ) }  ,
  { ALLEGRO_KEY_4        , YAGoLEvent( YAGoLEventType::speed     , 4  ) }  ,
  { ALLEGRO_KEY_SPACE    , YAGoLEvent( YAGoLEventType::toggle         ) }  ,
  { ALLEGRO_KEY_Q        , YAGoLEvent( YAGoLEventType::quit           ) }  ,
  { ALLEGRO_KEY_R        , YAGoLEvent( YAGoLEventType::randomize      ) }  ,
  { ALLEGRO_KEY_S        , YAGoLEvent( YAGoLEventType::step           ) } };

//////////////////////////////////////////////////////////////////////

void YAGoLAllegroView::clear()
{
    al_clear_to_color(background_color_);
}

//////////////////////////////////////////////////////////////////////

void YAGoLAllegroView::show()
{
    al_flip_display();
}

//////////////////////////////////////////////////////////////////////

void YAGoLAllegroView::close()
{
    al_destroy_display(display_);
    al_destroy_event_queue(event_queue_);
    al_destroy_bitmap(cell_bitmap_alive_);
    al_destroy_bitmap(cell_bitmap_dead_);
    al_shutdown_primitives_addon();
}

//////////////////////////////////////////////////////////////////////

void YAGoLAllegroView::notify(std::string message, const int width)
{
    // TODO
}

//////////////////////////////////////////////////////////////////////

std::string YAGoLAllegroView::prompt_for_string(std::string prompt, const unsigned int width)
{
    // TODO
    return "1/5";
}

//////////////////////////////////////////////////////////////////////

std::pair<int, int> YAGoLAllegroView::get_size() const
{
    return std::make_pair(screen_w_ / 10,
                          screen_h_ / 10);
}
