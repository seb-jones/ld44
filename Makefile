a.out:	main.c globals.c input.c render.c font.c game.c sprite.c events.c
	gcc -g -O0 main.c `sdl2-config --cflags` `sdl2-config --libs`
