#  cmake -S . -B build && cmake --build build && ./app

cmake_minimum_required(VERSION 3.26)
project( app VERSION 0.1 )

file(GLOB SOURCE_FILES
    "sdl2-gl.c"
)
add_executable(app ${SOURCE_FILES})

find_package( SDL2 REQUIRED )
find_package( OpenGL REQUIRED )
target_link_libraries(app ${SDL2_LIBRARIES} 
${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} )
