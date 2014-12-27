// File: yagol_curses_main.cpp
#include "yagol_controller.hpp"
#include "yagol_model.hpp"
#include "yagol_curses_view.hpp"

#include <cstdlib>
#include <ctime>

int main()
{
    srand(time(0));

    YAGoLCursesView view;
    YAGoLModel      model;
    YAGoLController controller(model, view);

    return controller.event_loop();
}
