#pragma once
#include <cstdlib>
#include <cmath>

class Chip8Cpu {
private:
    unsigned char* v;
    unsigned short int i;
    unsigned short int pc;

    double delay;

    void clear_screen(unsigned char*& memory) {
        for (size_t i = 0xF00; i < 0xFFF; i++)
            memory[i] = 0x00;
    }

    bool draw_sprite(unsigned char*& memory, unsigned char x, unsigned char y, unsigned char h) {
        bool setvf = false;

        printf("  drawing: I=%08x @(%d, %d)\n", this->i, x, y);
        for (size_t row = 0; row < h; row++) {
            unsigned short int memptr = 0xF00 + ((y + row) * 8) + floor(x / 8);
            unsigned char newmem;
            unsigned char oldmem;


            if (!(x % 8)) {
                oldmem = memory[memptr];
                newmem = oldmem ^ memory[this->i + row];
            } else {
                oldmem = (memory[memptr] << (x % 8)) | (memory[memptr+1] >> (8 - (x % 8)));
                newmem = oldmem ^ memory[this->i + row];

                // all this stuff can be simplified todo
                memory[memptr] = (memory[memptr] >> (8 - (x % 8))) << (8 - (x % 8));
                memory[memptr] |= newmem >> (x % 8);
                memory[memptr+1] = (memory[memptr+1] << (x % 8)) >> (x % 8);
                memory[memptr+1] |= newmem << (8 - (x % 8));
            }

            if ((oldmem & newmem) != 0x00)
                setvf = true;

            memory[memptr] = newmem;
            printf("  -> %04x: %02x (I=%04x)\n", memptr, (memory[this->i + row]), (this->i + row));
        }

        return setvf;
    }
public:
    Chip8Cpu(size_t frequency) {
        // allocate registers
        this->v = new unsigned char [16];
        this->i = 0x0;
        this->pc = 0x200;

        this->delay = 60 / frequency;

        // check for allocation errors
        if (this->v == NULL)
            throw std::runtime_error("Failed to allocate memory!");

        // erase registers (init)
        for (size_t i = 0; i < 16; i++)
            this->v[i] = 0x00;
    }

    void cycle(unsigned char*& memory, Chip8Screen*& screen) {
        unsigned short int instruction = (memory[this->pc] << 8) | (memory[this->pc + 1]);
        printf("--------------------------------\ninstr: %04x\n", instruction);

        if (instruction == 0xE0)
            this->clear_screen(memory);
        else if ((instruction >> 12) == 0x1)
            this->pc = (instruction & 0x0FFF) - 2;
        else if ((instruction >> 12) == 0x3) {
            if (this->v[(instruction >> 8) & 0x000F] == (instruction & 0x00FF))
                this->pc += 2;
        }
        else if ((instruction >> 12) == 0x4) {
            if (this->v[(instruction >> 8) & 0x000F] != (instruction & 0x00FF))
                this->pc += 2;
        }
        else if ((instruction >> 12) == 0x5) {
            if (this->v[(instruction >> 8) & 0x000F] == this->v[(instruction >> 4) & 0x000F])
                this->pc += 2;
        }
        else if ((instruction >> 12) == 0x6)
            this->v[(instruction >> 8) & 0x000F] = (instruction & 0x00FF);
        else if ((instruction >> 12) == 0x7)
            this->v[(instruction >> 8) & 0x000F] += (instruction & 0x00FF);
        else if ((instruction >> 12) == 0x8) {
            if ((instruction & 0x000F) == 0x0)
                this->v[(instruction >> 8) & 0x000F] = this->v[(instruction >> 4) & 0x000F];
            else if ((instruction & 0x000F) == 0x1)
                this->v[(instruction >> 8) & 0x000F] = this->v[(instruction >> 8) & 0x000F] | this->v[(instruction >> 4) & 0x000F];
            else if ((instruction & 0x000F) == 0x2)
                this->v[(instruction >> 8) & 0x000F] = this->v[(instruction >> 8) & 0x000F] & this->v[(instruction >> 4) & 0x000F];
            else if ((instruction & 0x000F) == 0x3)
                this->v[(instruction >> 8) & 0x000F] = this->v[(instruction >> 8) & 0x000F] ^ this->v[(instruction >> 4) & 0x000F];
            else if ((instruction & 0x000F) == 0x4) {
                if (this->v[(instruction >> 8) & 0x000F] + this->v[(instruction >> 4) & 0x000F] > 0xFF)
                    this->v[0xF] = 0x1;
                else
                    this->v[0xF] = 0x0;
                this->v[(instruction >> 8) & 0x000F] += this->v[(instruction >> 4) & 0x000F];
            }
            else if ((instruction & 0x000F) == 0x5) {
                if (this->v[(instruction >> 4) & 0x000F] <= this->v[(instruction >> 8) & 0x000F])
                    this->v[0xF] = 1;
                else
                    this->v[0xF] = 0;
                this->v[(instruction >> 8) & 0x000F] -= this->v[(instruction >> 4) & 0x000F];
            }
            else if ((instruction & 0x000F) == 0x6) {
                this->v[0xF] = (instruction & 0x0001);
                this->v[(instruction >> 8) & 0x000F] >>= 1;
            }
            else if ((instruction & 0x000F) == 0x7) {
                if (this->v[(instruction >> 4) & 0x000F] >= this->v[(instruction >> 8) & 0x000F])
                    this->v[0xF] = 1;
                else
                    this->v[0xF] = 0;
                this->v[(instruction >> 8) & 0x000F] = this->v[(instruction >> 4) & 0x000F] - this->v[(instruction >> 8) & 0x000F];
            }
            else if ((instruction & 0x000F) == 0xE) {
                this->v[0xF] = (instruction & 0x8000);
                this->v[(instruction >> 8) & 0x000F] <<= 1;
            }
        }
        else if ((instruction >> 12) == 0x9) {
            if (this->v[(instruction >> 8) & 0x000F] != this->v[(instruction >> 4) & 0x000F])
                this->pc += 2;
        }
        else if ((instruction >> 12) == 0xA)
            this->i = (instruction & 0x0FFF);
        else if ((instruction >> 12) == 0xB)
            this->pc = (instruction & 0x0FFF) + this->v[0x0] - 2;
        else if ((instruction >> 12) == 0xC)
            this->v[(instruction >> 8) & 0x000F] = ((rand() % 256) & (instruction & 0x00FF));
        else if ((instruction >> 12) == 0xD) {
            this->v[0xF] = this->draw_sprite(memory, this->v[(instruction >> 8) & 0x000F], this->v[(instruction >> 4) & 0x000F], (instruction & 0x000F));
            screen->update(memory);
        }
        else if ((instruction >> 12) == 0xE) {

        }
        else if ((instruction >> 12) == 0xF) {

        }

        SDL_Delay(this->delay);
        this->pc += 2;
    }
};
