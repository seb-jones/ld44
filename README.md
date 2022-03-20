# Faustian Sands

Entry for Ludum Dare 44. Written in C, using SDL2 (v2.0.9). Developed on Arch Linux using Vim. Compiled on Windows with Visual Studio 2017.

## About

When I first saw the theme, I spent some time thinking and then wrote the following:

> 'Life' is not just our physical body or our conscious mind - it is everything we interactive with too. Even the stars in the sky are a part of our life.
>
> Would you give one of these things for another?
>
> How much would you give up?

This was the interpretation of the theme that I had in mind going into the compo, and hopefully this idea is reflected in the game.

## Installation

### Windows

Simply download one of the [builds](https://github.com/seb-jones/ld44/releases/tag/1.0), extract the zip file, and run the executable.

### Linux

Clone this repo into a location of your choice.

Install the SDL2 dev package from your system's package manager, or compile it yourself.

`cd` into the cloned directory and run `make` for the debug build, or `make prod` for the production build.

Run `./build/ld44` to play.

### Web

The game can also be built for the web, using [Emscripten](https://emscripten.org/). Assuming you have that set up and have `emcc` in you PATH, building should be as simple as `make web` for debug builds and `make web-prod` for production builds.

This will place an `index.html` file and other web-specific assets in the `build/` directory. To test locally you will need to fire up a server to serve the build directory. For example, if you have `npm`:

```sh
npx serve build
```

## Controls

When given options, use the `Number Keys` to choose. Otherwise, use the `Enter Key` to proceed.

`F11` toggles fullscreen mode.
