#include "audioDecoder.h"
#include <SFML/Audio.hpp>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

//-----------------------------PLAY THE AUDIO FILE-----------------------------------------
int playAudio(string filename) {
    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile(filename)){
        std::cout<<"Invalid file!\n";
        return -1;
    }


    sf::Sound sound;

    char quit = ' ';
    //PLAY THE SOUND:
    std::cout << "Playing the sound, press q to quit\n"<<std::endl;
    sound.setBuffer(buffer);
    while(quit != 'q'){
        sound.play();
        std::cin>>quit;
    }
    return 0;
    }


//-----------------------------GET THE DETAILS OF THE FILE-----------------------------------------
int getDetails(string filename){
    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile(filename)){
        std::cout<<"Invalid file!\n";
        return -1;
    }

    const sf::Int16* samples = buffer.getSamples(); //returns pointer to array of audio samples
    std::size_t sampleNum = buffer.getSampleCount();    //returns number of samples
    std::size_t sampleRate = buffer.getSampleRate();    //returns number of samples per second
    const sf::Int16 channels = buffer.getChannelCount(); //returns pointer to array of audio samples
    char quit = ' ';

    while(quit != 'q'){
        std::cout<<"DETAILS"<<std::endl;
        //get current sample
        std::cout << "Samples: " << samples << std::endl;
        //get the number of samples
        std::cout << "The sample count is: " << sampleNum << std::endl;
        //get the number of samples per second
        std::cout << "The sample rate is: " << sampleRate << std::endl;
        //get the number of samples per second
        std::cout << "The number of channels is: " << channels << std::endl;
        std:cout<<"\nPress q to quit\n";
        std::cin>>quit;
    }
    
    
    return 0;

}


//---------------------------------SHOW THE WAVEFORM----------------------------------------
int plotWaveform(string filename){
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename))
    {
        std::cerr << "Error loading sound file" << std::endl;
        return -1;
    }

    const sf::Int16* samples = buffer.getSamples();
    std::size_t sampleCount = buffer.getSampleCount();

    unsigned int sampleRate = buffer.getSampleRate(); 
    unsigned int channelCount = buffer.getChannelCount();

    sf::RenderWindow window(sf::VideoMode(800, 600), "Waveform");

    sf::VertexArray waveform(sf::LinesStrip);

    // Resize elements to fit the window
    float timeScale = 800.0f / (sampleCount / channelCount); 
    float amplitudeScale = 300.0f / 32768.0f; // Map amplitude to half window height (300 pixels)

    for (std::size_t i = 0; i < sampleCount; i += channelCount) // skip half the channels becasue it's stereo
    {
        float x = i * timeScale / channelCount;

        float y = 300.0f - samples[i] * amplitudeScale; // 300.0f is half of window height

        waveform.append(sf::Vertex(sf::Vector2f(x, y), sf::Color::Green));
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        window.draw(waveform);

        window.display();
    }

    return 0;
}