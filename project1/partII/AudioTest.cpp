#include <SFML/Audio.hpp>
#include <iostream>
#include <stdio.h>
#include <unistd.h>

int main()
{
    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile("/home/joao/IC/IC/datasets/audio/sample01.wav")){
        return -1;
    }


    sf::Sound sound;

    //PLAY THE SOUND:
    //sound.setBuffer(buffer);
    //sound.play();
    //sleep(9999);

    const sf::Int16* samples = buffer.getSamples(); //returns pointer to array of audio samples
    std::size_t sampleNum = buffer.getSampleCount();    //returns number of samples
    std::size_t sampleRate = buffer.getSampleRate();    //returns number of samples per second

    //get current sample
    std::cout << "Samples: " << samples << std::endl;
    //get the number of samples
    std::cout << "The sample count is: " << sampleNum << std::endl;
    //get the number of samples per second
    std::cout << "The sample rate is: " << sampleRate << std::endl;
    return 0;
}