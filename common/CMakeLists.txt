add_library(common
matrixstack.h matrixstack.cpp
drawable.h drawable.cpp
square.h square.cpp
sphere.h sphere.cpp
cylinder.h cylinder.cpp
circle.cpp circle.h
cube.cpp cube.h
)

target_link_libraries(common compile_info)
target_include_directories(common PRIVATE ${CMAKE_CURRENT_LIST_DIR})
