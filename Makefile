build/ld44:	code/*.c
	gcc -Wall -g -O0 -o build/ld44 code/main.c `sdl2-config --cflags` `sdl2-config --libs` -lm
