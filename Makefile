a.out:	code/*.c
	gcc -g -O0 code/main.c `sdl2-config --cflags` `sdl2-config --libs` -lm
