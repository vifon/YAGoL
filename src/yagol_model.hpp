// File: yagol_model.hpp
#ifndef _YAGOL_MODEL_H_
#define _YAGOL_MODEL_H_

#include <utility>
#include <tuple>
#include <string>
#include <cstdint>
#include "matrix.hpp"
#include "sparsematrix.hpp"

//////////////////////////////////////////////////////////////////////

class YAGoLModel
{
  public:
    typedef SparseMatrix<bool>  diff_type;
    typedef WrappedMatrix<bool> board_type;
    typedef int16_t             rules_type;

    YAGoLModel( const std::string& rules_survival = "23",
                const std::string& rules_birth    = "3",
                const unsigned int width  = 1,
                const unsigned int height = 1 );

    YAGoLModel( const rules_type rules_survival,
                const rules_type rules_birth,
                const unsigned int width  = 1,
                const unsigned int height = 1 );

    static std::tuple<size_t, size_t, bool> unpack_diff_iterator(const std::pair< std::pair<size_t, size_t>, bool >& iterator);
    const diff_type& next_generation();
    board_type::iterator begin();
    board_type::iterator end();
    board_type::reference operator()(const int x, const int y);
    board_type::const_reference operator()(const int x, const int y) const;

    void resize(const size_t w, const size_t h);

    void set_rules(const std::string& rules_survival,
                   const std::string& rules_birth);
    void set_rules(const rules_type rules_survival,
                   const rules_type rules_birth);

    size_t width() const;
    size_t height() const;

  private:
    rules_type        rules_survival_;
    rules_type        rules_birth_;
    board_type        board_;
    mutable diff_type diff_;

    bool will_live(int x, int y) const;

    // returns how many living neighbours a given field has
    int16_t neighbours( const size_t x, const size_t y ) const;

    // generates diff between this generation and the next one
    const diff_type& next_generation_stage() const;
    // moves on to the next generation
    void next_generation_commit();

    static rules_type read_rules(const std::string& rules_string);
};

//////////////////////////////////////////////////////////////////////

#endif
