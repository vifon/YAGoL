// File: yagol_model.cpp
#include "yagol_model.hpp"
#ifdef DEBUG
#include <iostream>
#endif

//////////////////////////////////////////////////////////////////////

YAGoLModel::YAGoLModel( const unsigned int width,
                        const unsigned int height,
                        const char* rules_survival,
                        const char* rules_birth )
    : width_( width )
    , height_( height )
    , rules_survival_(0)
    , rules_birth_(0)
    , board_( width_, height_ )
    , diff_()
{
    for (auto rule : std::string(rules_survival)) {
        if (rule >= '0' && rule <= '9') {
            rules_survival_ |= 1 << (rule-'0');
        }
    }

    for (auto rule : std::string(rules_birth)) {
        if (rule >= '0' && rule <= '9') {
            rules_birth_ |= 1 << (rule-'0');
        }
    }
}

//////////////////////////////////////////////////////////////////////

YAGoLModel::YAGoLModel( const unsigned int width,
                        const unsigned int height,
                        const rules_type rules_survival,
                        const rules_type rules_birth )
    : width_( width )
    , height_( height )
    , rules_survival_( rules_survival )
    , rules_birth_( rules_birth )
    , board_( width_, height_ )
    , diff_()
{}

//////////////////////////////////////////////////////////////////////

int16_t YAGoLModel::neighbours( const size_t x, const size_t y ) const
{
    int16_t count = 0;

    // iterate through 3x3 square...
    for (int i = x-1; i <= static_cast<signed int>(x+1); ++i) {
        for (int j = y-1; j <= static_cast<signed int>(y+1); ++j) {
            count += board_(i,j) ? 1 : 0;
        }
    }
    // ...and discard the central element's value
    count -= board_(x,y) ? 1 : 0;

    return count;
}

//////////////////////////////////////////////////////////////////////

YAGoLModel::diff_type::const_iterator YAGoLModel::next_generation_stage() const
{
    diff_.clear();

    for (size_t y = 0; y < height_; ++y) {
        for (size_t x = 0; x < width_; ++x) {
            bool new_state = will_live(x,y);
            if ( new_state != board_(x,y) ) {
                diff_.set( x,y, new_state );
            }
        }
    }
    return diff_.cbegin();
}

//////////////////////////////////////////////////////////////////////

void YAGoLModel::next_generation_commit()
{
    for (auto it : diff_) {
        size_t x = it.first.first;
        size_t y = it.first.second;
        bool val = it.second;

        board_(x,y) = val;
    }

#ifdef DEBUG
    for (size_t y = 0; y < height_; ++y) {
        for (size_t x = 0; x < width_; ++x) {
            if (board_(x,y)) {
                std::cout << '@';
            } else {
                std::cout << ' ';
            }
        }
        std::cout << std::endl;
    }
#endif
}

//////////////////////////////////////////////////////////////////////

YAGoLModel::diff_type::const_iterator YAGoLModel::next_generation()
{
    auto diff = next_generation_stage();
    next_generation_commit();

    return diff;
}

//////////////////////////////////////////////////////////////////////

void YAGoLModel::randomize(int range, int density)
{
    for (auto it : board_) {
        it = (std::rand() % range) < density;
    }
}

//////////////////////////////////////////////////////////////////////

bool YAGoLModel::will_live(int x, int y) const
{
    rules_type used_rules;
    if (board_(x,y)) {
        used_rules = rules_survival_;
    } else {
        used_rules = rules_birth_;
    }

    return used_rules & (1 << neighbours(x,y));
}
