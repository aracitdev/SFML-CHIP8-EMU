#include "Screen.h"
#include <iostream>
#include <cstring>


bool GetBit(unsigned char byte, int position) // position in range 0-7
{
    return (byte >> position) & 0x1;
}


Screen::Screen(void)
{
    screen.create(64,32);
    memset(ScreenBuffer, 0x00, 32 * 64);
}

bool Screen::SetPixel(uint16_t x, uint16_t y)
{
    x%=screen.getSize().x;
    y%=screen.getSize().y;

    bool returnV=ScreenBuffer[32 * x + y];
    ScreenBuffer[32 * x + y]^=1;
    return returnV;
}

bool Screen::DrawSprite(uint16_t x, uint16_t y, uint8_t* sprt, uint8_t byteCount)
{
    bool v = false;
    for(uint16_t countery=0; countery < byteCount; countery++)
        for(uint16_t counterx=0; counterx < 8; counterx++)
            if(GetBit(sprt[countery], 7-counterx))
                v = v || SetPixel(counterx + x, countery + y);
    return v;
}

void Screen::ClearScreen(void)
{
    for(uint32_t counter=0; counter < 64 * 32; counter++)
        ScreenBuffer[counter]=0;
}

void Screen::SaveToTexture(sf::Texture& out)
{
    for(uint32_t counterx=0; counterx < 64; counterx++)
        for(uint32_t countery=0; countery < 32; countery++)
        screen.setPixel(counterx, countery, ScreenBuffer[counterx * 32 + countery] ? sf::Color::White : sf::Color::Black);
    out.loadFromImage(screen);
}
