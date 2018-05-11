cmake_minimum_required(VERSION 3.1)
project(framework_OpenGL LANGUAGES CXX)

file( GLOB SOURCES src/*.cpp include/*.hpp include/*.h)
add_executable(framework_OpenGL
${PROJECT_SOURCE_DIR}/src ${SOURCES})

set_target_properties(framework_OpenGL
    PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    RUNTIME_OUTPUT_DIRECTORY "bin"
    LIBRARY_OUTPUT_DIRECTORY "lib")

find_package(OpenGL REQUIRED)

set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "turn off build examples" FORCE)
set( GLFW_BUILD_TESTS OFF CACHE BOOL "turn off tests" FORCE)

target_include_directories(framework_OpenGL
  PRIVATE ${PROJECT_SOURCE_DIR}/include
  PRIVATE ${PROJECT_SOURCE_DIR}/glew-2.1.0/include
  PRIVATE ${PROJECT_SOURCE_DIR}/freetype-2.9/include
  PRIVATE ${PROJECT_SOURCE_DIR}/glfw-3.2.1/include
  PRIVATE ${PROJECT_SOURCE_DIR}/glm-0.9.9-a2/glm
  PRIVATE ${PROJECT_SOURCE_DIR}/assimp/include
  PRIVATE ${PROJECT_SOURCE_DIR}/build/assimp/include
  PRIVATE ${PROJECT_SOURCE_DIR}/SOIL/src)

add_subdirectory(${PROJECT_SOURCE_DIR}/glfw-3.2.1)
add_subdirectory(${PROJECT_SOURCE_DIR}/assimp)
add_subdirectory(${PROJECT_SOURCE_DIR}/freetype-2.9)

set(CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/glew-2.1.0/)
set(CMAKE_LIBRARY_PATH ${PROJECT_SOURCE_DIR}/glew-2.1.0/lib/Release/Win32/)
find_package(GLEW REQUIRED)
configure_file(${PROJECT_SOURCE_DIR}/glew-2.1.0/bin/Release/Win32/glew32.dll ${PROJECT_SOURCE_DIR}/build/glew32.dll COPYONLY)
file(COPY ${PROJECT_SOURCE_DIR}/shaders/ DESTINATION ${PROJECT_SOURCE_DIR}/build/shaders/)
file(COPY ${PROJECT_SOURCE_DIR}/assets/ DESTINATION ${PROJECT_SOURCE_DIR}/build/assets/)
find_library(GLEW_LIBRARY NAMES GLEW glew32 glew glew32s PATH_SUFFIXES lib32)

target_link_libraries(framework_OpenGL
  PRIVATE ${OPENGL_LIBRARIES}
  PRIVATE ${GLEW_LIBRARIES}
  PRIVATE glfw ${GLFW_LIBRARIES}
  PRIVATE ${SOIL_LIBRARY}
  PRIVATE ${FREETYPE_LIBRARIES}
  PRIVATE assimp)
