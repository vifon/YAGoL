// File: yagol_model.hpp
#ifndef _YAGOL_MODEL_H_
#define _YAGOL_MODEL_H_

#include <utility>
#include <tuple>
#include <string>
#include <cstdint>
#include "boolmatrix.hpp"
#include "sparseboolmatrix.hpp"

//////////////////////////////////////////////////////////////////////

class YAGoLModel
{
  public:
    typedef SparseBoolMatrix diff_type;
    typedef BoolMatrix       board_type;
    typedef int16_t          rules_type;

    YAGoLModel( const unsigned int width,
                const unsigned int height,
                const std::string& rules_survival,
                const std::string& rules_birth );

    YAGoLModel( const unsigned int width,
                const unsigned int height,
                const rules_type rules_survival,
                const rules_type rules_birth );

    static std::tuple<size_t, size_t, bool> unpack_diff_iterator(const std::pair< std::pair<size_t, size_t>, bool >& iterator);
    const diff_type& next_generation();
    board_type::iterator begin();
    board_type::iterator end();

    size_t width();
    size_t height();

  private:
    size_t            width_;
    size_t            height_;
    rules_type        rules_survival_;
    rules_type        rules_birth_;
    board_type        board_;
    mutable diff_type diff_;

    bool will_live(int x, int y) const;

    // returns how many living neighbours has a given field
    int16_t neighbours( const size_t x, const size_t y ) const;

    // generates diff between this generation and the next one
    const diff_type& next_generation_stage() const;
    // moves on to the next generation
    void next_generation_commit();
};

//////////////////////////////////////////////////////////////////////

#endif
