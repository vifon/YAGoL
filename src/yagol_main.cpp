// File: yagol_main.cpp
#include "yagol_controller.hpp"
#include "yagol_model.hpp"
#include "yagol_curses_view.hpp"

#include <tuple>
#include <string>
#include <cstdlib>
#include <ctime>

int main(int argc, char *argv[])
{
    srand(time(0));

    std::string rules_survival = "23";
    std::string rules_birth = "3";

    if (argc >= 2) {
        rules_survival = argv[1];
    }

    if (argc >= 3) {
        rules_birth = argv[1];
    }

    YAGoLCursesView view;

    YAGoLModel model(rules_survival, rules_birth);

    YAGoLController controller(model, view);

    return controller.event_loop();
}
