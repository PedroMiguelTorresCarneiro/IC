#include "audioDecoder.h"
#include <SFML/Audio.hpp>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>
#include <vector>


//-----------------------------PLAY THE AUDIO FILE-----------------------------------------
int playAudio(string filename) {
    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile(filename)){
        std::cerr << "Invalid file!" << std::endl;
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
        std::cerr << "Invalid file!" << std::endl;
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
    if (!buffer.loadFromFile(filename)){
        std::cerr << "Invalid file!" << std::endl;
        return -1;
    }

    const sf::Int16* samples = buffer.getSamples();
    std::size_t sampleCount = buffer.getSampleCount();

    unsigned int sampleRate = buffer.getSampleRate(); 
    unsigned int channelCount = buffer.getChannelCount();

    sf::RenderWindow window(sf::VideoMode(800, 600), "Waveform");

    sf::VertexArray waveform(sf::LinesStrip);

    // Resize elements to fit the window
    float timeScale = 800.0f / (sampleCount / channelCount); //map the x axis
    float amplitudeScale = 300.0f / 32768.0f; // map the y axis to half the window height(because of positive and negative amplitudes)

    for (std::size_t i = 0; i < sampleCount; i += channelCount){ // skip half the channels becasue it's stereo
        float x = i * timeScale / channelCount;

        float y = 300.0f - samples[i] * amplitudeScale; // 300.0f is half of window height

        waveform.append(sf::Vertex(sf::Vector2f(x, y), sf::Color::Green));
    }

    while (window.isOpen()){
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



//---------------------------------PLOT TWO WAVEFORMS FOR COMPARISON----------------------------------------
void plotTwoWaveforms(string filename, const std::vector<sf::Int16>& quantizedSamples) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename)) {
        std::cerr << "Invalid file!" << std::endl;
        return;
    }

    const sf::Int16* originalSamples = buffer.getSamples();
    std::size_t sampleCount = buffer.getSampleCount();
    unsigned int channelCount = buffer.getChannelCount();
    
    sf::RenderWindow window1(sf::VideoMode(800, 600), "Original samples");

    sf::RenderWindow window2(sf::VideoMode(800, 600), "Modified Samples");

    sf::VertexArray originalWaveform(sf::LinesStrip);
    sf::VertexArray quantizedWaveform(sf::LinesStrip);

    // Resize elements
    float timeScale = 800.0f / (sampleCount / channelCount); //map the x axis
    float amplitudeScale = 300.0f / 32768.0f; // map the y axis to half the window height(because of positive and negative amplitudes)


    // Plot original samples 
    for (std::size_t i = 0; i < sampleCount; i += channelCount) {
        float x = i * timeScale / channelCount;
        float y = 300.0f - originalSamples[i] * amplitudeScale; 
        originalWaveform.append(sf::Vertex(sf::Vector2f(x, y), sf::Color::Green));
    }

    // Plot quantized samples 
    for (std::size_t i = 0; i < sampleCount; i += channelCount) {
        float x = i * timeScale / channelCount;
        float y = 300.0f + quantizedSamples[i] * amplitudeScale;
        quantizedWaveform.append(sf::Vertex(sf::Vector2f(x, y), sf::Color::Red));
    }

    while (window1.isOpen()) {
        sf::Event event1;
        while (window1.pollEvent(event1)) {
            if (event1.type == sf::Event::Closed)
                window1.close();
        }

        window1.clear(sf::Color::Black);

        window1.draw(originalWaveform);


        window1.display();

        sf::Event event2;
        while (window2.pollEvent(event2)) {
            if (event2.type == sf::Event::Closed)
                window2.close();
        }

        window2.clear(sf::Color::Black);


        window2.draw(quantizedWaveform);

        window2.display();
    
    }

}

//---------------------------------SHOW THE HISTOGRAM----------------------------------------
int histogram(string filename){
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename)){
        std::cerr << "Invalid file!" << std::endl;
        return -1;
    }

    const sf::Int16* samples = buffer.getSamples();
    std::size_t sampleCount = buffer.getSampleCount();

    int binCount = 30; // number of bins
    int amplMax = 32767;
    int amplMin = -32768;
    int binSize = (amplMax - amplMin) / binCount;

    // Vectors to hold mid and side channels
    std::vector<int> midChannel(sampleCount / 2, 0);
    std::vector<int> sideChannel(sampleCount / 2, 0);
    std::vector<int> midHistogram(binCount, 0);
    std::vector<int> sideHistogram(binCount, 0);


    for (std::size_t i = 0; i < sampleCount; i += 2) {
        // MID channel: (L + R) / 2
        midChannel[i / 2] = (samples[i] + samples[i + 1]) / 2;
        // SIDE channel: (L - R) / 2 
        sideChannel[i / 2] = (samples[i] - samples[i + 1]) / 2;
    }


    for (std::size_t i = 0; i < sampleCount / 2; i++) {
        int midIndex = (midChannel[i] - amplMin) / binSize;

        // Ensure that the bin index is within range
        if (midIndex < 0) {
            midIndex = 0;
        } else if (midIndex >= binCount) {
            midIndex = binCount - 1;
        }

        midHistogram[midIndex]++;
    }


    for (std::size_t i = 0; i < sampleCount / 2; i++) {
        int sideIndex = (sideChannel[i] - amplMin) / binSize;

        // Ensure that the bin index is within range
        if (sideIndex < 0) {
            sideIndex = 0;
        } else if (sideIndex >= binCount) {
            sideIndex = binCount - 1;
        }

        sideHistogram[sideIndex]++;
    }

    

    //print mid histogram
    std::cout<<"MID Channel"<<std::endl;
    for (int i = 0; i < binCount; ++i) {
        int binMin = amplMin + i * binSize;
        int binMax = binMin + binSize;

        std::cout << "[" << binMin << " to " << binMax << "] : ";
        std::cout << " (" << midHistogram[i] << ")\n";
    }
    
    std::cout<<"\n"<<std::endl;
    
    //print side histogram
    std::cout<<"SIDE Channel"<<std::endl;
    for (int i = 0; i < binCount; ++i) {
        int binMin = amplMin + i * binSize;
        int binMax = binMin + binSize;

        std::cout << "[" << binMin << " to " << binMax << "] : ";
        std::cout << " (" << sideHistogram[i] << ")\n";
    }

    return 0;
}

//--------------------------------- QUANTIZE THE AUDIO ----------------------------------------

std::vector<sf::Int16> quantize(string filename, int level){
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename)){
        std::cerr << "Invalid file!" << std::endl;
    }

    const sf::Int16* samples = buffer.getSamples();
    std::size_t sampleCount = buffer.getSampleCount();

    int amplMax = 32767;
    int amplMin = -32768;

    //define step size
    float stepSize = (amplMax - amplMin) / float(level);

    std::vector<sf::Int16> quantizedSamples(sampleCount);

    for (std::size_t i=0; i<sampleCount; ++i){
        int quantizedLevel = std::round((samples[i] - amplMin) / stepSize);
        quantizedSamples[i] = amplMin + quantizedLevel * stepSize;
    }

    //compare the original and quantized audio
    return quantizedSamples;
}

//--------------------------------- CALCULATE MSE ----------------------------------------
//remember, higher MSE = bigger difference between audio

double MSE(const std::vector<sf::Int16>& quantizedSamples, string filename){
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename)){
        std::cerr << "Invalid file!" << std::endl;
        return -1.0;
    }

    const sf::Int16* originalSamples = buffer.getSamples();
    std::size_t sampleCount = buffer.getSampleCount();

    double mse = 0.0;

    for(std::size_t i = 0; i<sampleCount; ++i){

        //difference betwenn original and quantized samples, then squared
        double difference = static_cast<double>(originalSamples[i] - quantizedSamples[i]);
        mse += pow(difference, 2); 
    }

    //calculate the average
    mse /= static_cast<double>(sampleCount);

    return mse;
}

//--------------------------------- CALCULATE SNR ----------------------------------------
//this time is different, the higher the SNR the less difference there is

double SNR(const std::vector<sf::Int16>& quantizedSamples, string filename, double mse){
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename)){
        std::cerr << "Invalid file!" << std::endl;
        return -1.0;
    }

    const sf::Int16* originalSamples = buffer.getSamples();
    std::size_t sampleCount = buffer.getSampleCount();

    double signalPower = 0.0;
    double noisePower = 0.0;
    double SNR = 0.0;

    for(std::size_t i = 0; i<sampleCount; ++i){
        signalPower += std::pow(static_cast<double>(originalSamples[i]), 2);
    }

    //we must multiply by sampleCount becasue mse is an average and we want total noise power
    noisePower = mse * sampleCount;
    

    SNR = 10.0 * std::log10(signalPower / noisePower);

    return SNR;
} 

//---------------------------------PLAY QUANTIZED AUDIO----------------------------------------

void playFromSamples(const std::vector<sf::Int16>& samples, string filename) {

    sf::SoundBuffer buff;

    if (!buff.loadFromFile(filename)){
        std::cerr << "Invalid file!" << std::endl;
    }

    std::size_t sampleCount = buff.getSampleCount();
    std::size_t channelCount = buff.getChannelCount();
    std::size_t sampleRate = buff.getSampleRate();


    sf::SoundBuffer buffer;
    if (!buffer.loadFromSamples(samples.data(), samples.size(), channelCount, sampleRate)) {
        std::cerr << "Buffer error!" << std::endl;
        return;
    }

    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();

    while (sound.getStatus() == sf::Sound::Playing) {
        sf::sleep(sf::milliseconds(100));
    }
}

//---------------------------------ADD NOISE TO AUDIO----------------------------------------

std::vector<sf::Int16> addNoise(const std::string& filename, float noiseLevel) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename)) {
        std::cerr << "Error loading sound file" << std::endl;
        return {};
    }

    const sf::Int16* samples = buffer.getSamples();
    std::size_t sampleCount = buffer.getSampleCount();

    std::vector<sf::Int16> noisySamples(sampleCount);

    std::default_random_engine randomGenerator; //random noise generator
    std::uniform_real_distribution<float> noiseDistribution(-noiseLevel, noiseLevel);

    for (std::size_t i = 0; i < sampleCount; ++i) {
        float noise = noiseDistribution(randomGenerator);

        int noisySample = samples[i] + static_cast<int>(noise);

        // Clamp to valid range
        if (noisySample < -32768) noisySample = -32768;
        if (noisySample > 32767) noisySample = 32767;

        noisySamples[i] = static_cast<sf::Int16>(noisySample);
    }

    return noisySamples;
}