class Chip8Timer {
private:
    unsigned char value;
public:
    Chip8Timer() {
        this->value = 0;
    }

    void set(unsigned char value) {
        this->value = value;
    }

    void tick() {
        if (this->value != 0)
            this->value--;
    }

    unsigned char get_value() {
        return this->value;
    }
};
