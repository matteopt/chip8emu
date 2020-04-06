#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include "chip8screen.hpp"
#include "chip8cpu.hpp"

class Chip8Emu {
private:
    unsigned char* memory;
    Chip8Screen* screen;
    Chip8Cpu* cpu;
public:
    Chip8Emu(Chip8Screen& screen, Chip8Cpu& cpu) {
        // allocate memory
        this->memory = new unsigned char [4096];

        // check for allocation errors
        if (this->memory == NULL)
            throw std::runtime_error("Failed to allocate memory!");

        this->screen = &screen;
        this->cpu = &cpu;
    }

    ~Chip8Emu() {
        // cleanup
        delete[] this->memory;
    }

    bool play() {
        SDL_Event event;
        while (true) {
            if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
                break;
            this->cpu->cycle(this->memory);
        }

        return true;
    }

    bool load_game(const char* filename) {
        // open the specified file
        if (FILE* game_file = fopen(filename, "rb")) {
            // calculate file size
            fseek(game_file, 0, SEEK_END);
            size_t file_size = ftell(game_file);

            // if the file wouldn't fit in memory (4096 - 512 - 256 - 96)
            if (file_size > 3232) {
                printf("* File too large! (%d)\n", file_size);
                return false;
            }

            // read the file and copy it to memory
            rewind(game_file);
            size_t result = fread(this->memory + 512, 1, file_size, game_file);
            if (result != file_size) {
                printf("* File reading failed! (%d/%d)\n", result, file_size);
                return false;
            }

            return true;

        } else {
            printf("* Unable to open file! (%s)\n", filename);
            return false;
        }
    }

    void print_memory() {
        for (size_t i = 0; i < 4096; i += 2) {
            if (i % 16 == 0)
                printf("\n%08x: ", i);
            printf("%02x%02x ", this->memory[i], this->memory[i+1]);
        }
    }
};

int main() {
    try {
        Chip8Screen screen = Chip8Screen(4);
        Chip8Cpu cpu = Chip8Cpu();

        Chip8Emu emu = Chip8Emu(screen, cpu);

        printf("%s", emu.load_game("spaceinvaders.ch8") ? "Game loaded!\n" : "");
        emu.play();
        emu.print_memory();

        return 0;
    } catch (const std::exception& e) {
        printf("* Runtime error! %s\n", e.what());
        return 1;
    }
}
