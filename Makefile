all: produce_executable execute_executable

# compiler=g++
# sources=src/*.cpp

compiler=gcc
sources=sdl2-gl.c
# sources=src/sdl2-gl.cpp

static=-static -lcfgmgr32 -lsetupapi -lwinmm -limm32 -lole32 -loleaut32 -lversion

produce_executable:
ifeq (Linux,$(shell uname))
	$(compiler) -o app -g -Wall $(sources) $(shell sdl2-config --cflags --libs) -lGL -lGLU
else
	$(compiler) -o app -g -Wall $(sources) $(shell sdl2-config --cflags --libs) -lopengl32 -lglu32 -mconsole # $(static)
endif

execute_executable:
	./app

clean:
	rm -f app*
