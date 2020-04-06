#pragma once
#include <SDL2/SDL.h>
#include <stdexcept>
#include <cmath>

class Chip8Screen {
private:
    unsigned short w;
    unsigned short h;
    unsigned char scale;
    bool* pixels;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* surface;

    void draw_pixel(unsigned char x, unsigned char y, bool pixel) {
        // if (pixel)
        //     printf("sreen - drawing pixel @(%d, %d)\n", x, y);
        if (pixel)
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        else
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        for (size_t i = 0; i < this->scale; i++) {
            for (size_t k = 0; k < this->scale; k++)
                SDL_RenderDrawPoint(this->renderer, (x*this->scale)+i, (y*this->scale)+k);
        }
    }
public:
    Chip8Screen(unsigned char scale) {
        this->w = 64 * scale;
        this->h = 32 * scale;
        this->scale = scale;
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

    void update(unsigned char*& memory) {
        for (size_t i = 0xF00; i < 0xFFF; i++) {
            for (size_t k = 0; k < 8; k++)
                this->draw_pixel((((i - 0xF00)*8 + k) % 64), floor(((i - 0xF00)*8 + k) / 64), ((memory[i] >> (7 - k)) & 0x01));
        }
        SDL_RenderPresent(this->renderer);
    }
};
