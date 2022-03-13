build/ld44:	code/*.c
	gcc -Wall -g -O0 -o build/ld44 code/main.c `sdl2-config --cflags` `sdl2-config --libs` -lm

web:	code/*.c
	emcc -Wall -g -O0 -fsanitize=undefined -s ASSERTIONS=1 -s SAFE_HEAP=1 -s ENVIRONMENT=web -s USE_SDL=2  -o build/index.html --preload-file assets code/main.c
