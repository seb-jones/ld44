a.out:	main.c globals.c input.c render.c font.c game.c sprite.c
	gcc -g main.c `sdl2-config --cflags` `sdl2-config --libs`
