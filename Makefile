MAKE_BUILD_DIRECTORY=mkdir -p ./build

build/ld44:	code/*.c
	$(MAKE_BUILD_DIRECTORY)
	clang -Wall -g -O0 -fsanitize=undefined -o build/ld44 code/main.c `sdl2-config --cflags` `sdl2-config --libs` -lm

prod:	code/*.c
	$(MAKE_BUILD_DIRECTORY)
	clang -O3 -o build/ld44 code/main.c `sdl2-config --cflags` `sdl2-config --libs` -lm


COPY_WEB_FILES=cp code/web/* build/

web:	code/*.c code/web/*
	$(MAKE_BUILD_DIRECTORY)
	$(COPY_WEB_FILES)
	emcc -Wall -g -O0 -fsanitize=undefined -s ASSERTIONS=1 -s SAFE_HEAP=1 -s ENVIRONMENT=web -s USE_SDL=2 --preload-file assets -o build/ld44.js code/main.c

web-prod:	code/*.c code/web/*
	$(MAKE_BUILD_DIRECTORY)
	$(COPY_WEB_FILES)
	emcc -O3 -s ENVIRONMENT=web -s USE_SDL=2 --preload-file assets -o build/ld44.js code/main.c
