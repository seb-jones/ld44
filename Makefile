a.out:	main.c render.c game.c
	gcc -g main.c `sdl2-config --cflags` `sdl2-config --libs`
