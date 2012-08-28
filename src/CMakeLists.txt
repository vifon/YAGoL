set(yagol_SOURCES
  yagol_controller.cpp
  yagol_curses_view.cpp
  yagol_main.cpp
  yagol_model.cpp
  )

set(yagol_HEADERS
  boolmatrix.hpp
  lexical_cast.hpp
  sparseboolmatrix.hpp
  yagol_controller.hpp
  yagol_curses_view.hpp
  yagol_model.hpp
  yagol_view.hpp
  )

add_executable(yagol ${yagol_SOURCES})
target_link_libraries(yagol ncurses)
