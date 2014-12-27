// File: yagol_allegro_main.cpp
#include "yagol_controller.hpp"
#include "yagol_model.hpp"
#include "yagol_allegro_view.hpp"

#include <cstdlib>
#include <ctime>

int main()
{
    srand(time(0));

    YAGoLAllegroView view;
    YAGoLModel       model;
    YAGoLController  controller(model, view);

    return controller.event_loop();
}
