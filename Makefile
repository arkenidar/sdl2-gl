all: produce_executable execute_executable

produce_executable:
ifeq (Linux,$(shell uname))
	gcc -o app -g -Wall sdl2-gl.c $(shell sdl2-config --cflags --libs) -lGL -lGLU
else
	gcc -o app -g -Wall sdl2-gl.c $(shell sdl2-config --cflags --libs) -lopengl32 -lglu32 -mwindows
endif

execute_executable:
	./app

clean:
	rm app*
