set(CMAKE_INCLUDE_CURRENT_DIR ON)

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

set(DISPLAY_MODE "Curses")
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/yagol_main.cpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/yagol_curses_main.cpp
  @ONLY
  )
set(DISPLAY_MODE "Allegro")
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/yagol_main.cpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/yagol_allegro_main.cpp
  @ONLY
  )
unset(DISPLAY_MODE)

find_package(Curses)
if (CURSES_FOUND)
  include_directories(${CURSES_INCLUDE_DIRS})
  add_executable(yagol_curses ${yagol_curses_SOURCES})
  target_link_libraries(yagol_curses
    yagol ${CURSES_LIBRARIES})
endif(CURSES_FOUND)

find_library(allegro allegro)
find_library(allegro_primitives allegro_primitives)
if(allegro AND allegro_primitives)
  add_executable(yagol_allegro ${yagol_allegro_SOURCES})
  target_link_libraries(yagol_allegro
    yagol
    ${allegro} ${allegro_primitives})
endif(allegro AND allegro_primitives)
