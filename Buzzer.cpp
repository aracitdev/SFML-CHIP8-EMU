#include "Buzzer.h"
#include <math.h>
#include <iostream>

const size_t SAMPLE_RATE=8000;
const size_t TONE_FREQUENCY=262;
const size_t TONE_AMPLITUDE=11000;

Buzzer::Buzzer()
{
    sound = new sf::Sound();
    buffer= new sf::SoundBuffer();
    samples.resize(SAMPLE_RATE *2);
    for(size_t counter=0; counter < samples.size(); counter++)
    {
        samples[counter]= TONE_AMPLITUDE * sin(TONE_FREQUENCY * M_PI * 2.0 * counter / SAMPLE_RATE);
    }
    if(!buffer->loadFromSamples(&samples[0], samples.size(),1,SAMPLE_RATE))
        std::cout <<"Failed to load sound from samples.\n";
    sound->setBuffer(*buffer);
    sound->setLoop(true);
    sound->setVolume(100);
}

Buzzer::~Buzzer()
{
    delete sound;
    delete buffer;
}

void Buzzer::SetPlaying(bool v)
{
    if(v && sound->getStatus() != sf::Sound::Playing)
    {
        sound->play();
    }
    else
    if(!v && sound->getStatus() != sf::Sound::Stopped)
        sound->stop();
}
