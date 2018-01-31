cmake_minimum_required(VERSION 2.8.12)

project(GluiExamples)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glui REQUIRED)

set(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations")
set(CMAKE_VERBOSE_MAKEFILE on)

add_executable(hello-spinner hello-spinner.cpp)
target_link_libraries(hello-spinner glui::glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(hello-checkbox hello-checkbox.cpp)
target_link_libraries(hello-checkbox glui::glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(hello-motion hello-motion.cpp)
target_link_libraries(hello-motion glui::glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
