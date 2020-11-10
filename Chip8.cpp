#include "Chip8.h"
#include <fstream>
#include <iostream>
#include <cstring>

std::ifstream::pos_type filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

Chip8::Chip8() : Cpu(Memory,&Gpu)
{
    memset(Memory, 0x00,sizeof(Memory));
    uint8_t CharacterMemory[] =
    {
        0xF0,0x90,0x90,0x90,0xF0, // 0
        0x20,0x60,0x20,0x20,0x70, // 1
        0xF0,0x10,0xF0,0x80,0xF0, // 2
        0xF0,0x10,0xF0,0x10,0xF0, // 3
        0x90,0x90,0xF0,0x10,0x10, // 4
        0xF0,0x80,0xF0,0x10,0xF0, // 5
        0xF0,0x80,0xF0,0x90,0xF0, // 6
        0xF0,0x10,0x20,0x40,0x40, // 7
        0xF0,0x90,0xF0,0x90,0xF0, // 8
        0xF0,0x90,0xF0,0x10,0xF0, // 9
        0xF0,0x90,0xF0,0x90,0x90, // A
        0xE0,0x90,0xE0,0x90,0xE0, // B
        0xF0,0x80,0x80,0x80,0xF0, // C
        0xE0,0x90,0x90,0x90,0xE0, // D
        0xF0,0x80,0xF0,0x80,0xF0, // E
        0xF0,0x80,0xF0,0x80,0x80  // F
    };

    for(uint32_t counter=0; counter < sizeof(CharacterMemory); counter++)
        Memory[counter]=CharacterMemory[counter];
}


void Chip8::Run(void)
{
    sf::RenderWindow Window(sf::VideoMode(256,128),"Chip8 Emu");
    sf::Texture Tex;
    sf::Sprite Sprt;
    sf::Clock cpuClock;
    sf::Clock frameClock;
    Sprt.setTexture(Tex);
    Sprt.setScale(sf::Vector2f(4.0,4.0));
    Sprt.setPosition(0,0);
    while(Window.isOpen())
    {
        cpuClock.restart();
        sf::Event Ev;
        while(Window.pollEvent(Ev))
        {
            if(Ev.type == sf::Event::Closed)
                Window.close();
            if(Ev.type == sf::Event::KeyPressed)
                if(Ev.key.code == sf::Keyboard::F1)
                Gpu.ClearScreen();
        }
        Cpu.Run();
        Sound.SetPlaying(Cpu.SReg > 0);
        if(frameClock.getElapsedTime().asSeconds() > (1.0/60.0))
        {
            frameClock.restart();
            Gpu.SaveToTexture(Tex);
            Sprt.setTexture(Tex,true);
            Window.clear();
            Window.draw(Sprt);
            Window.display();
        }

        sf::sleep(sf::seconds(1.0/500.0) - cpuClock.getElapsedTime());
    }
}

bool Chip8::LoadRom(const std::string& filename)
{
    std::ifstream in(filename.c_str(), std::ofstream::binary);
    if(!in.is_open())
        return false;
    size_t fileSize =filesize(filename.c_str());
    in.read((char*)Memory + 0x200, fileSize);
    return true;
}



bool Chip8::DumpMemory(const std::string& outfile)
{
    std::ofstream out(outfile.c_str(), std::ofstream::out | std::ofstream::binary);
    if(!out.is_open())
        return false;
    out.write((char*)Memory + 0x200, sizeof(Memory) - 0x200);
    return true;
}
