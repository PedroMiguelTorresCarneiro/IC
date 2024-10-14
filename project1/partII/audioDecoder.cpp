#include "audioDecoder.h"
#include <SFML/Audio.hpp>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <SFML/Graphics.hpp>
#include <cmath>


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

//---------------------------------SHOW THE WAVEFORM BUT USING SAMPLES----------------------------------------
int plotWaveform(string filename, std::vector<sf::Int16> quantizedSamples){
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename))
    {
        std::cerr << "Error loading sound file" << std::endl;
        return -1;
    }

    std::vector<sf::Int16>  samples = quantizedSamples;
    std::size_t sampleCount = buffer.getSampleCount();

    //unsigned int sampleRate = buffer.getSampleRate(); 
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

//---------------------------------SHOW THE HISTOGRAM----------------------------------------
int histogram(string filename){
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename))
    {
        std::cerr << "Error loading sound file" << std::endl;
        return -1;
    }

    const sf::Int16* samples = buffer.getSamples();
    std::size_t sampleCount = buffer.getSampleCount();


    int binCount = 30; //number of bins, could be changed
    int amplMax = 32767;
    int amplMin = -32768;
    std::vector<int> bins(binCount, 0); //range of the bins
    int binSize = (amplMax - amplMin)/binCount;

    for (std::size_t i=0; i<sampleCount; i++){
        //haven't differentiated between L and R channels, need to get on that
        int binIndex = (samples[i] - amplMin)/binSize; 

        //ensure that the bin is withing the specified range
        if(binIndex<0){
            binIndex = 0;
        }else if(binIndex >= binCount){
            binIndex = binCount - 1;
        }

        bins[binIndex]++;
    }

    int binMaxCount = *std::max_element(bins.begin(), bins.end());

    //print the histogram

    for(int i = 0; i<binCount; ++i){
        int binMin = amplMin + i * binSize;
        int binMax = binMin + binSize;

        std::cout<<"["<<binMin<<" to "<<binMax<<"] : ";
        std::cout << " (" << bins[i] << ")\n";
    }

    return 0;
}

//--------------------------------- QUANTIZE THE AUDIO ----------------------------------------

int quantize(string filename, int level){
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename))
    {
        std::cerr << "Error loading sound file" << std::endl;
        return -1;
    }

    const sf::Int16* samples = buffer.getSamples();
    std::size_t sampleCount = buffer.getSampleCount();

    int amplMax = 32767;
    int amplMin = -32768;

    //define step size
    float stepSize = (amplMax - amplMin) / float(level);

    //vector to keep quantized samples
    std::vector<sf::Int16> quantizedSamples(sampleCount);

    for (std::size_t i=0; i<sampleCount; ++i){
        float normalizedSample = (samples[i] - amplMin) / float(amplMax - amplMin);

        int quantizedLevel = std::round(normalizedSample * (level - 1));

        quantizedLevel = std::max(0, std::min(quantizedLevel, level - 1));


        quantizedSamples[i] = amplMin + quantizedLevel * stepSize;
    }

    plotWaveform(filename);
    plotWaveform(filename, quantizedSamples);

    return 0;
}