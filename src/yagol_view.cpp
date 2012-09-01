// File: yagol_view.cpp
#include "yagol_view.hpp"

//////////////////////////////////////////////////////////////////////

YAGoLView::~YAGoLView()
{}

//////////////////////////////////////////////////////////////////////

void YAGoLView::make_alive(size_t x, size_t y)
{
    set_state(x,y, true);
}
void YAGoLView::make_dead(size_t x, size_t y)
{
    set_state(x,y, false);
}

//////////////////////////////////////////////////////////////////////

int YAGoLView::prompt_for_number(std::string prompt, const unsigned int width) throw(std::invalid_argument)
{
    std::string answer = prompt_for_string(prompt, width);
    int number;

    size_t pos;
    number = std::stoi(answer, &pos);
    if (prompt[pos] != '\0') { // more strict "convertability" test
        throw std::invalid_argument("YAGoLView::prompt_for_number");
    }

    return number;
}
