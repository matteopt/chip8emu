#pragma once
#include <SDL2/SDL.h>
#include <stdexcept>

class Chip8Screen {
private:
    unsigned short w;
    unsigned short h;
    bool* pixels;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* surface;
public:
    Chip8Screen(unsigned char scale) {
        this->w = 64 * scale;
        this->h = 32 * scale;
        this->pixels = new bool [64 * 32];

        // check for allocation errors
        if (this->pixels == NULL)
            throw std::runtime_error("Failed to allocate memory!");

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            throw std::runtime_error("SDL Error!");
        } else {
            SDL_CreateWindowAndRenderer(this->w, this->h, 0, &this->window, &this->renderer);
            this->surface = SDL_GetWindowSurface(this->window);
        }
    }

    ~Chip8Screen() {
        delete[] this->pixels;
        SDL_DestroyWindow(this->window);
        SDL_DestroyRenderer(this->renderer);
        SDL_Quit();
    }
};
