#include "CPU.h"
#include "Keybinds.h"
#include <iostream>

#include <random>
#include <time.h>

CPU::CPU(uint8_t* mem, Screen* v): Memory(mem)
{
    srand (time(NULL));
    timer=0;
    Scr=v;
    isRunning=true;
    PC=0x200;
    for(uint32_t counter=0; counter < sizeof(VRegs); counter++)
        VRegs[counter]=0;
    IReg=0;
    SReg=0;
    TReg=0;
    SP=0;
}


void CPU::Run(void)
{
    timer++;
    if(timer >= 9)
    {
        TReg -= (TReg > 0) ? 1 : 0;
        SReg -= (SReg > 0) ? 1 : 0;
        timer=0;
    }

    if(!isRunning)
    {
        for(uint32_t counter=0; counter < 16; counter++)
            isRunning = isRunning || IsPressed(counter);
    }
    if(!isRunning)
    {
        return;
    }

    if(PC%2 != 0)
        std::cout <<"ERROR: CHIP8 expect program counter to be aligned to 2 bytes.\n";
    uint8_t instrByte1=Memory[PC];
    uint8_t instrByte2=Memory[PC+1];
    //std::cout <<"PC " <<std::hex<<PC <<" Opcode " <<std::hex << ((uint16_t)(instrByte1)) <<" "<<std::hex <<((uint16_t)(instrByte2))<<"\n";
    //std::cout <<"Regs " << std::hex <<uint16_t(VRegs[0]) << " "<<std::hex <<uint16_t(VRegs[1])<<"\n";

    PC+=2;

    switch(instrByte1 & 0xF0)
    {
    case 0x00:
        if(instrByte2 == 0xE0 && instrByte1 == 0x00)
        {
            ClearScreen();
        }
        if(instrByte2 == 0xEE && instrByte1 == 0x00)
        {
            SP--;
            PC = Stack[SP]; //Return
        }
        break;
    case 0x10:
        PC = Compose12(instrByte1,instrByte2);  //JMP
        break;
    case 0x20:
        Stack[SP]=PC;
        SP++;
        PC = Compose12(instrByte1, instrByte2); //CALL
        break;
    case 0x30:
        PC += VRegs[instrByte1 & 0x0f] == instrByte2 ? 2 : 0;
        break;
    case 0x40:
        PC += VRegs[instrByte1 & 0x0f] != instrByte2 ? 2 : 0;
        break;
    case 0x50:
        PC += VRegs[instrByte1 & 0x0f] == VRegs[((instrByte2 & 0xf0) >> 4)] ? 2 : 0;
        break;
    case 0x60:
        VRegs[instrByte1 & 0x0f] = instrByte2;
        break;
    case 0x70:
        VRegs[instrByte1 & 0x0f] += instrByte2;
        break;
    case 0x80:
        switch(instrByte2 & 0x0f)
        {
            case 0x00:
                VRegs[instrByte1 & 0x0f] = VRegs[((instrByte2 & 0xf0) >> 4)];
            break;
            case 0x01:
                VRegs[instrByte1 & 0x0f] |= VRegs[((instrByte2 & 0xf0) >> 4)];
            break;
            case 0x02:
                VRegs[instrByte1 & 0x0f] &= VRegs[((instrByte2 & 0xf0) >> 4)];
            break;
            case 0x03:
                VRegs[instrByte1 & 0x0f] ^= VRegs[((instrByte2 & 0xf0) >> 4)];
            break;
            case 0x04:
                {
                    uint8_t InitialV =VRegs[instrByte1 & 0x0f];
                    VRegs[instrByte1 & 0x0f] += VRegs[((instrByte2 & 0xf0) >> 4)];
                    VRegs[0x0f]= (VRegs[instrByte1 & 0x0f] < InitialV) ? 1 : 0;
                }
            break;
            case 0x05:
                VRegs[0x0f]= (VRegs[instrByte1 & 0x0f] > VRegs[((instrByte2 & 0xf0) >> 4)]) ? 1 : 0;
                VRegs[instrByte1 & 0x0f] -= VRegs[((instrByte2 & 0xf0) >> 4)];
            break;
            case 0x06:
                VRegs[0x0f] = (VRegs[0x0f] & 0x01);
                VRegs[instrByte1 & 0x0f]/=2;
            break;
            case 0x07:
                VRegs[0x0f]=(VRegs[((instrByte2 & 0xf0) >> 4)] > VRegs[instrByte1 & 0x0f]) ? 1 : 0;
                VRegs[instrByte1 & 0x0f] = VRegs[((instrByte2 & 0xf0) >> 4)] - VRegs[instrByte1 & 0x0f];
                break;
            case 0x0E:
                VRegs[0x0f]=(VRegs[instrByte1 & 0x0f] & 0x80) ? 1 : 0;
                VRegs[instrByte1 & 0x0f]*=2;
                break;
        default:
            std::cout <<"Unknown 0xE opcode\n";
            std::cout <<"Opcode was " <<std::hex <<uint16_t(instrByte1) <<" " <<std::hex << uint16_t(instrByte2)<<"\n";
            break;
        }
    break;
    case 0xA0:
        IReg = Compose12(instrByte1,instrByte2);
    break;
    case 0xB0:
        PC = Compose12(instrByte1,instrByte2) + VRegs[0];
    break;
    case 0xC0:
        VRegs[instrByte1 & 0x0f] = (rand() % 255) & instrByte2;
    break;
    case 0xD0:
        VRegs[0x0f]=(DrawSprite( VRegs[instrByte1 & 0x0f], VRegs[((instrByte2 & 0xf0) >> 4)], instrByte2 & 0x0f) ? 1 : 0);
    break;
    case 0xE0:
        if(instrByte2 == 0x9E)
            PC += IsPressed(VRegs[instrByte1 & 0x0f]) ? 2 : 0;
        else if(instrByte2 == 0xA1)
            PC += (!IsPressed(VRegs[instrByte1 & 0x0f])) ? 2 : 0;
    break;
    case 0xF0:
        switch(instrByte2)
        {
        case 0x07:
            VRegs[instrByte1 & 0x0f]=TReg;
            break;
        case 0x0A:
            isRunning=false;
            break;
        case 0x15:
            TReg=VRegs[instrByte1 & 0x0f];
            break;
        case 0x18:
            SReg=VRegs[instrByte1 & 0x0f];
            break;
        case 0x1E:
            IReg+=VRegs[instrByte1 & 0x0f];
            break;
        case 0x29:
            IReg = 0x05 * VRegs[instrByte1 & 0x0f];
            break;
        case 0x33:
            {
                uint8_t tmp =VRegs[instrByte1 & 0x0f];
                Memory[IReg] = tmp / 100;
                Memory[IReg+1] = (tmp % 100) / 10;
                Memory[IReg+2] = (tmp % 10);
            }
            break;
        case 0x55:
            for(uint32_t counter=0; counter <= (instrByte1 & 0x0f); counter++)
                Memory[IReg+counter]=VRegs[counter];
            break;
        case 0x65:
            for(uint32_t counter=0; counter <= (instrByte1 & 0x0f); counter++)
                VRegs[counter]=Memory[IReg+counter];
            break;
        default:
            std::cout <<"Unknown 0xf opcode\n";
            std::cout <<"Opcode was " <<std::hex <<uint16_t(instrByte1) <<" " <<std::hex << uint16_t(instrByte2)<<"\n";
            break;

        }

    break;
        default:
            std::cout <<"Unknown opcode\n";
            std::cout <<"Opcode was " <<std::hex <<uint16_t(instrByte1) <<" " <<std::hex << uint16_t(instrByte2)<<"\n";
            break;
    }
}



uint16_t CPU::Compose12(uint8_t byte1, uint8_t byte2)
{
    uint16_t returnV = byte1 & 0x0f;
    returnV = (returnV << 8) + byte2;

    //std::cout <<"Composed " <<std::hex <<returnV<<"\n";
    return returnV;
}

bool CPU::IsPressed(uint8_t v)
{
    return sf::Keyboard::isKeyPressed(Keys[v]);
}

void CPU::ClearScreen(void)
{
    Scr->ClearScreen();
}

bool CPU::DrawSprite(uint8_t x, uint8_t y, uint8_t byteCount)
{
    uint8_t* bytes = Memory + IReg;
    return Scr->DrawSprite(x,y,bytes, byteCount);
}
