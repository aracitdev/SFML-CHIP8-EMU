#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED
#include <SFML/Graphics.hpp>

class Screen
{
public:
    Screen();
    void ClearScreen(void);
    bool SetPixel(uint16_t x, uint16_t y);
    bool DrawSprite(uint16_t x, uint16_t y, uint8_t* spritev, uint8_t byteCount);
    void SaveToTexture(sf::Texture& out);
    sf::Image screen;

    uint8_t ScreenBuffer[64 * 32];
private:
};

#endif // SCREEN_H_INCLUDED
