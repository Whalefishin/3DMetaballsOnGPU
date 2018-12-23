#Each executable target in Cmake must have a unique name.
#We will set it as a PROGNAME here, so we can use common patterns below
set(PROGNAME lights)

add_executable(${PROGNAME} main.cpp)
target_sources(${PROGNAME} PRIVATE
  mainwindow.h mainwindow.cpp mainwindow.ui
  mypanelopengl.h mypanelopengl.cpp
  camera.h camera.cpp
  vshader.glsl fshader.glsl
  vfraglight.glsl ffraglight.glsl
)

#common boilerplate
set_target_properties(${PROGNAME} PROPERTIES
  AUTOMOC ON
  AUTOMOC_MOC_OPTIONS -p${CMAKE_CURRENT_LIST_DIR}
  AUTOUIC ON
)
target_link_libraries(${PROGNAME} compile_info common)
target_include_directories(${PROGNAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(${PROGNAME} PRIVATE ${CMAKE_SOURCE_DIR})
symlink_data(${PROGNAME}link01 *.glsl)
