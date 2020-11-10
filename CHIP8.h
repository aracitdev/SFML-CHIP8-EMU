#ifndef CHIP8_H_INCLUDED
#define CHIP8_H_INCLUDED
#include "Cpu.h"
#include "Buzzer.h"

class Chip8
{
public:
    Chip8();

    bool LoadRom(const std::string& in);
    void Run(void);
    bool DumpMemory(const std::string& outfile);

private:
    uint8_t Memory[0x1000];
    Buzzer Sound;
    CPU Cpu;
    Screen Gpu;
};

#endif // CHIP8_H_INCLUDED
