#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED
#include <cstdint>
#include "Screen.h"

class CPU
{
public:
    CPU(uint8_t* mem, Screen* v);

    void Run(void);
    void ClearScreen(void);
    bool DrawSprite(uint8_t x, uint8_t y, uint8_t byteCount);
    uint16_t Compose12(uint8_t byte1, uint8_t byte2);
    bool IsPressed(uint8_t key);
    uint8_t VRegs[16];
    uint8_t TReg;
    uint16_t SReg;
    uint16_t IReg;
    uint16_t Stack[16];

    uint16_t PC;
    uint8_t SP;


    uint8_t* Memory;
    Screen* Scr;

    uint8_t timer;

    bool isRunning;
};

#endif // CPU_H_INCLUDED
