set(yagol_SOURCES
  yagol_controller.cpp
  yagol_model.cpp
  yagol_view.cpp
  )

set(yagol_curses_SOURCES
  yagol_curses_view.cpp
  yagol_curses_main.cpp
  )

set(yagol_allegro_SOURCES
  yagol_allegro_view.cpp
  yagol_allegro_main.cpp
  )

add_library(yagol STATIC
  ${yagol_SOURCES})


find_package(Curses)
if (CURSES_FOUND)
  include_directories(${CURSES_INCLUDE_DIRS})
  add_executable(yagol_curses ${yagol_curses_SOURCES})
  target_link_libraries(yagol_curses
    yagol ${CURSES_LIBRARIES})
endif(CURSES_FOUND)

add_executable(yagol_allegro ${yagol_allegro_SOURCES})
target_link_libraries(yagol_allegro
  yagol allegro allegro_primitives)
