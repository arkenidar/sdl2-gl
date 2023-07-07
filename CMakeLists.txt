# cmake -S . -B build && cmake --build build && build/app

# cmake -S . -B build -G "Ninja" && cmake --build build && build/app
# cmake -S . -B build -G "CodeBlocks - Ninja" && cmake --build build && build/app
# cmake -S . -B build -G "Visual Studio 17 2022" && cmake --build build && build/app

cmake_minimum_required(VERSION 3.18)
project( app VERSION 0.1 )

file(GLOB SOURCE_FILES "*.c*" )
add_executable(app ${SOURCE_FILES})

find_package( OpenGL REQUIRED )
find_package( SDL2 REQUIRED )
target_link_libraries(${PROJECT_NAME} OpenGL::GL OpenGL::GLU ${SDL2_LIBRARIES})
