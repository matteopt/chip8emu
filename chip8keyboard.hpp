class Chip8Keyboard {
private:
    bool* keystates;
    bool keypress;
    bool awaiting;
    unsigned char lastkey;
public:
    Chip8Keyboard() {
        this->keystates = new bool [16];
        this->keypress = false;
        this->awaiting = false;
        this->lastkey = 0;

        if (this->keystates == NULL)
            throw std::runtime_error("Failed to allocate memory!");

        for (size_t i = 0; i < 16; i++)
            this->keystates[i] = 0;
    }

    void notify(SDL_Event& e) {
        if (e.type == SDL_KEYDOWN)
            this->keypress = true;

        switch (e.key.keysym.sym) {
        case SDLK_1:
            this->keystates[0x0] = (e.type == SDL_KEYDOWN);
            break;
        case SDLK_2:
            this->keystates[0x1] = (e.type == SDL_KEYDOWN);
            break;
        case SDLK_3:
            this->keystates[0x2] = (e.type == SDL_KEYDOWN);
            break;
        case SDLK_4:
            this->keystates[0x3] = (e.type == SDL_KEYDOWN);
            break;
        case SDLK_q:
            this->keystates[0x4] = (e.type == SDL_KEYDOWN);
            break;
        case SDLK_w:
            this->keystates[0x5] = (e.type == SDL_KEYDOWN);
            break;
        case SDLK_e:
            this->keystates[0x6] = (e.type == SDL_KEYDOWN);
            break;
        case SDLK_r:
            this->keystates[0x7] = (e.type == SDL_KEYDOWN);
            break;
        case SDLK_a:
            this->keystates[0x8] = (e.type == SDL_KEYDOWN);
            break;
        case SDLK_s:
            this->keystates[0x9] = (e.type == SDL_KEYDOWN);
            break;
        case SDLK_d:
            this->keystates[0xA] = (e.type == SDL_KEYDOWN);
            break;
        case SDLK_f:
            this->keystates[0xB] = (e.type == SDL_KEYDOWN);
            break;
        case SDLK_z:
            this->keystates[0xC] = (e.type == SDL_KEYDOWN);
            break;
        case SDLK_x:
            this->keystates[0xD] = (e.type == SDL_KEYDOWN);
            break;
        case SDLK_c:
            this->keystates[0xE] = (e.type == SDL_KEYDOWN);
            break;
        case SDLK_v:
            this->keystates[0xF] = (e.type == SDL_KEYDOWN);
            break;
        }

        for (size_t i = 0; i <= 0xF; i++) {
            if (this->keystates[i] == 1 && this->lastkey != i) {
                this->lastkey = i;
                break;
            }
        }
    }

    bool get_key(unsigned char n) {
        if (n > 0xF)
            return false;

        return this->keystates[n];
    }

    char last_key() {
        return this->lastkey;
    }

    void await() {
        if (!this->awaiting)
            this->keypress = false;
        this->awaiting = true;
    }

    bool ack_key() {
        bool out = this->keypress;
        if (out)
            this->awaiting = false;
        this->keypress = false;
        return out;
    }
};
