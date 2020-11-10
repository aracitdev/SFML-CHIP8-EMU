#ifndef BUZZER_H_INCLUDED
#define BUZZER_H_INCLUDED
#include <SFML/Audio.hpp>

class Buzzer
{
public:

    Buzzer();
    ~Buzzer();
    void SetPlaying(bool);

private:
    std::vector<sf::Int16> samples;
    sf::SoundBuffer* buffer;
    sf::Sound* sound;
};

#endif // BUZZER_H_INCLUDED
