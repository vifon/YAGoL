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

std::pair<int, int> YAGoLView::prompt_for_numbers_with_slash(std::string prompt, const unsigned int width)
{
    std::string string_with_slash = prompt_for_string(prompt, width);

    size_t pos;
    int first = std::stoul(string_with_slash, &pos);
    if (string_with_slash[pos] != '/') {
        throw std::invalid_argument("YAGoLView::prompt_for_string_with_slash");
    }

    std::string second_number = string_with_slash.substr(pos+1);
    int second = std::stoul(second_number, &pos);
    if (second_number[pos] != '\0') {
        throw std::invalid_argument("YAGoLView::prompt_for_string_with_slash");
    }

    return std::make_pair(first, second);
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
