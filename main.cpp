#include <iostream>
#include "Chip8.h"


int main(int argc, char* argv[])
{
    std::string filename;
    if(argc >= 2)
        filename = argv[1];
    else
    {
        std::cout <<"Please enter the ROM filename.\n";
        getline(std::cin, filename);
    }
    Chip8 Interpreter;
    if(!Interpreter.LoadRom(filename))
    {
        std::cout <<"Failed to find the rom " <<filename<<"\n";
        return 0;
    }
    Interpreter.Run();
    return 0;
}
