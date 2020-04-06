class Chip8Cpu {
private:
    unsigned char* v;
    unsigned short int i;
    unsigned short int pc;
public:
    Chip8Cpu() {
        // allocate registers
        this->v = new unsigned char [16];
        this->i = 0x0;
        this->pc = 0x200;

        // check for allocation errors
        if (this->v == NULL)
            throw std::runtime_error("Failed to allocate memory!");
    }

    void clear_screen(unsigned char*& memory) {
        for (size_t i = 3840; i < 4096; i++)
            memory[i] = 0x00;
    }

    void cycle(unsigned char*& memory) {
        unsigned char instruction = (memory[this->pc] << 8) | (memory[this->pc + 1]);
        if (instruction == 0xE0)
            this->clear_screen(memory);
        if ((instruction >> 12) == 0x1)
            this->pc = (instruction & 0x0FFF);
        if ((instruction >> 12) == 0x3) {
            if (this->v[(instruction >> 8) & 0x000F] == (instruction & 0x00FF))
                this->pc++;
        }
        if ((instruction >> 12) == 0x4) {
            if (this->v[(instruction >> 8) & 0x000F] != (instruction & 0x00FF))
                this->pc++;
        }
        if ((instruction >> 12) == 0x5) {
            if (this->v[(instruction >> 8) & 0x000F] == this->v[(instruction >> 4) & 0x000F])
                this->pc++;
        }
        if ((instruction >> 12) == 0x6)
            this->v[(instruction >> 8) & 0x000F] = (instruction & 0x00FF);
        if ((instruction >> 12) == 0x7)
            this->v[(instruction >> 8) & 0x000F] += (instruction & 0x00FF);
        if ((instruction >> 12) == 0x8) {
            if ((instruction & 0x000F) == 0x0)
                this->v[(instruction >> 8) & 0x000F] = this->v[(instruction >> 4) & 0x000F];
            else if ((instruction & 0x000F) == 0x1)
                this->v[(instruction >> 8) & 0x000F] = this->v[(instruction >> 8) & 0x000F] | this->v[(instruction >> 4) & 0x000F];
            else if ((instruction & 0x000F) == 0x2)
                this->v[(instruction >> 8) & 0x000F] = this->v[(instruction >> 8) & 0x000F] & this->v[(instruction >> 4) & 0x000F];
            else if ((instruction & 0x000F) == 0x3)
                this->v[(instruction >> 8) & 0x000F] = this->v[(instruction >> 8) & 0x000F] ^ this->v[(instruction >> 4) & 0x000F];
            else if ((instruction & 0x000F) == 0x4) {
                size_t expected = this->v[(instruction >> 8) & 0x000F] + this->v[(instruction >> 4) & 0x000F];
                this->v[(instruction >> 8) & 0x000F] += this->v[(instruction >> 4) & 0x000F];
                if (this->v[(instruction >> 8) & 0x000F] != expected)
                    this->v[0xF] = 0x1;
                else
                    this->v[0xF] = 0x0;
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
        }

    }
};
