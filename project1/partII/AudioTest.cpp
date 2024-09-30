#include <SFML/Audio.hpp>
#include <iostream>
#include <stdio.h>
#include <unistd.h>





int playAudio(const char* filename) {
    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile(filename)){
        return -1;
    }


    sf::Sound sound;

    bool quit = 1;
    //PLAY THE SOUND:
    std::cout << "Playing the sound, press 0 to quit"<<std::endl;
    sound.setBuffer(buffer);
    while(quit != 0){
        sound.play();
        std::cin>>quit;
    }
    return 0;
    }

int getDetails(const char* filename){
    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile(filename)){
        return -1;
    }

    const sf::Int16* samples = buffer.getSamples(); //returns pointer to array of audio samples
    std::size_t sampleNum = buffer.getSampleCount();    //returns number of samples
    std::size_t sampleRate = buffer.getSampleRate();    //returns number of samples per second
    const sf::Int16 channels = buffer.getChannelCount(); //returns pointer to array of audio samples

    std::cout<<"DETAILS"<<std::endl;
    //get current sample
    std::cout << "Samples: " << samples << std::endl;
    //get the number of samples
    std::cout << "The sample count is: " << sampleNum << std::endl;
    //get the number of samples per second
    std::cout << "The sample rate is: " << sampleRate << std::endl;
    //get the number of samples per second
    std::cout << "The number of channels is: " << channels << std::endl;
    
    return 0;

}

int main()
{  
    int option = 0;
    const char* filename = "/home/joao/IC/IC/datasets/audio/sample01.wav";
    
    while(option != 3){
        std::cout<<std::endl;
        std::cout<<"-----------------------------------------------"<<std::endl;
        std::cout << "Please select an option"<<std::endl;
        std::cout << "1 - Play the audio file"<<std::endl;
        std::cout << "2 - Get details from audio file"<<std::endl;
        std::cout << "3 - Quit"<<std::endl;
        std::cout<<"-----------------------------------------------"<<std::endl;
        std::cin >> option;

        switch(option){
            case 1:
                playAudio(filename);
                break;
            case 2:
                getDetails(filename);
                break;
            case 3:
                std::cout<< "Bye Bye!"<<std::endl;
                break;
            default:
                std::cout<< "Choose a valid option"<<std::endl;

        }
    }
    
}