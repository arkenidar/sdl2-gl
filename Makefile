all: produce_executable execute_executable

produce_executable:
	gcc -o app -g -Wall sdl2-gl.c -mwindows $(shell sdl2-config --cflags --libs) -lopengl32 -lglu32

execute_executable:
	./app