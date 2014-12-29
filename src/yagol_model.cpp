// File: yagol_model.cpp
#include "yagol_model.hpp"

namespace yagol {

//////////////////////////////////////////////////////////////////////

Model::Model( const std::string& rules_survival,
                        const std::string& rules_birth,
                        const unsigned int width,
                        const unsigned int height )
    : rules_survival_( read_rules(rules_survival) )
    , rules_birth_( read_rules(rules_birth) )
    , board_( width, height )
    , diff_()
{}

//////////////////////////////////////////////////////////////////////

Model::Model( const rules_type rules_survival,
                        const rules_type rules_birth,
                        const unsigned int width,
                        const unsigned int height )
    : rules_survival_( rules_survival )
    , rules_birth_( rules_birth )
    , board_( width, height )
    , diff_()
{}

//////////////////////////////////////////////////////////////////////

Model::rules_type Model::neighbours( const size_t x, const size_t y ) const
{
    rules_type count = 0;

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

const Model::diff_type& Model::next_generation_stage() const
{
    diff_.clear();

    for (size_t y = 0; y < height(); ++y) {
        for (size_t x = 0; x < width(); ++x) {
            bool new_state = will_live(x,y);
            if ( new_state != board_(x,y) ) {
                diff_.set( x,y, new_state );
            }
        }
    }
    return diff_;
}

//////////////////////////////////////////////////////////////////////

std::tuple<size_t, size_t, bool> Model::unpack_diff_iterator(const std::pair< std::pair<size_t, size_t>, bool >& iterator)
{
    return std::make_tuple(iterator.first.first,
                           iterator.first.second,
                           iterator.second);
}

void Model::next_generation_commit()
{
    for (auto it : diff_) {
        size_t x,y;
        bool val;

        std::tie(x,y, val) = unpack_diff_iterator(it);

        board_(x,y) = val;
    }
}

//////////////////////////////////////////////////////////////////////

const Model::diff_type& Model::next_generation()
{
    auto& diff = next_generation_stage();
    next_generation_commit();

    return diff;
}

//////////////////////////////////////////////////////////////////////

Model::board_type::iterator Model::begin()
{
    return board_.begin();
}
Model::board_type::iterator Model::end()
{
    return board_.end();
}

//////////////////////////////////////////////////////////////////////

Model::board_type::reference Model::operator()(const int x, const int y)
{
    return board_(x,y);
}

Model::board_type::const_reference Model::operator()(const int x, const int y) const
{
    return board_(x,y);
}

//////////////////////////////////////////////////////////////////////

size_t Model::width() const
{
    return board_.width();
}
size_t Model::height() const
{
    return board_.height();
}

//////////////////////////////////////////////////////////////////////

void Model::resize(const size_t w, const size_t h)
{
    board_.resize(w,h);
}

//////////////////////////////////////////////////////////////////////

void Model::set_rules(const std::string& rules_survival,
                           const std::string& rules_birth)
{
    rules_survival_ = read_rules(rules_survival);
    rules_birth_    = read_rules(rules_birth);
}


void Model::set_rules(const rules_type rules_survival,
                           const rules_type rules_birth)
{
    rules_survival_ = rules_survival;
    rules_birth_    = rules_birth;
}

//////////////////////////////////////////////////////////////////////

bool Model::will_live(int x, int y) const
{
    rules_type used_rules;
    if (board_(x,y)) {
        used_rules = rules_survival_;
    } else {
        used_rules = rules_birth_;
    }

    return used_rules & (1 << neighbours(x,y));
}

//////////////////////////////////////////////////////////////////////

Model::rules_type Model::read_rules(const std::string& rules_string)
{
    rules_type rules = 0;

    for (auto rule : rules_string) {
        if (rule >= '0' && rule <= '9') {
            rules |= 1 << (rule-'0');
        }
    }

    return rules;
}

} // namespace yagol
