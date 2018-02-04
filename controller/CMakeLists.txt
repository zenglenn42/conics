cmake_minimum_required(VERSION 2.8.12)

project(GluiExamples)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glui REQUIRED)

if(APPLE)
  set(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations")
endif(APPLE)

set(CMAKE_VERBOSE_MAKEFILE on)

set(hello-spinner_SRCS hello-spinner.cpp)
add_executable(hello-spinner ${hello-spinner_SRCS})
target_link_libraries(hello-spinner glui::glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

set(hello-checkbox_SRCS hello-checkbox.cpp)
add_executable(hello-checkbox ${hello-checkbox_SRCS})
target_link_libraries(hello-checkbox glui::glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

set(hello-motion_SRCS hello-motion.cpp)
add_executable(hello-motion ${hello-motion_SRCS})
target_link_libraries(hello-motion glui::glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

set(hello-ui_SRCS hello-ui.cpp)
add_executable(hello-ui ${hello-ui_SRCS})
target_link_libraries(hello-ui glui::glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

set(hello-ui-1_SRCS hello-ui-1.cpp)
add_executable(hello-ui-1 ${hello-ui-1_SRCS})
target_link_libraries(hello-ui-1 glui::glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

set(hello-conics_SRCS hello-conics.cpp)
add_executable(hello-conics ${hello-conics_SRCS})
target_link_libraries(hello-conics glui::glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

set(hello-conics-1_SRCS hello-conics-1.cpp)
add_executable(hello-conics-1 ${hello-conics-1_SRCS})
target_link_libraries(hello-conics-1 glui::glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

set(hello-conics-2_SRCS hello-conics-2.cpp)
add_executable(hello-conics-2 ${hello-conics-2_SRCS})
target_link_libraries(hello-conics-2 glui::glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

set(hello-conics-3_SRCS hello-conics-3.cpp)
add_executable(hello-conics-3 ${hello-conics-3_SRCS})
target_link_libraries(hello-conics-3 glui::glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

set(hello-conics-4_SRCS hello-conics-4.cpp)
add_executable(hello-conics-4 ${hello-conics-4_SRCS})
target_link_libraries(hello-conics-4 glui::glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

set(hello-conics-5_SRCS hello-conics-5.cpp)
add_executable(hello-conics-5 ${hello-conics-5_SRCS})
target_link_libraries(hello-conics-5 glui::glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
