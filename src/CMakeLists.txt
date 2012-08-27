set(yagol_SOURCES
  main.cpp
  yagol_model.cpp
  )

set(yagol_HEADERS
  boolmatrix.hpp
  sparseboolmatrix.hpp
  yagol_model.hpp
  )

add_executable(yagol ${yagol_SOURCES})
