#include "audioDecoder.h"
#include <SFML/Audio.hpp>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>

using namespace std;

int main(int argc, char* argv[]){
    
    std::string choice = argv[1];

    if(choice == "details"){

        if(argc < 2){
            std::cout<<"Not enough arguments"<<std::endl;
            return -1;
        }

        string filename = argv[2];
        getDetails(filename);

    }else if(choice == "waveform"){
        if(argc < 2){
            std::cout<<"Not enough arguments"<<std::endl;
            return -1;
        }
        
        string filename = argv[2];
        plotWaveform(filename);

    }else if(choice == "histogram"){
        if(argc < 2){
            std::cout<<"Not enough arguments"<<std::endl;
            return -1;
        }
        
        string filename = argv[2];
        histogram(filename);

    }else if(choice == "quantize"){
        if(argc < 3){
            std::cout<<"Not enough arguments"<<std::endl;
            return -1;
        }
        
        string filename = argv[2];
        int level = atoi(argv[3]);
        std::vector<sf::Int16> quantizedSamples = quantize(filename, level);
        plotTwoWaveforms(filename, quantizedSamples);

    }else if(choice == "MSE"){
        if(argc < 3){
            std::cout<<"Not enough arguments"<<std::endl;
            return -1;
        }
        
        string filename = argv[2];
        int level = atoi(argv[3]);
        std::vector<sf::Int16> quantizedSamples = quantize(filename, level);
        std::cout<< MSE(quantizedSamples, filename) <<std::endl;

    }else if(choice == "SNR"){
        if(argc < 3){
            std::cout<<"Not enough arguments"<<std::endl;
            return -1;
        }
        
        string filename = argv[2];
        int level = atoi(argv[3]);
        std::vector<sf::Int16> quantizedSamples = quantize(filename, level);
        double mse = MSE(quantizedSamples, filename);
        std::cout<<SNR(quantizedSamples, filename, mse)<<std::endl;

    }else if(choice == "qzPlay"){
        if(argc < 3){
            std::cout<<"Not enough arguments"<<std::endl;
            return -1;
        }
        
        string filename = argv[2];
        int level = atoi(argv[3]);
        std::vector<sf::Int16> quantizedSamples = quantize(filename, level);
        playFromSamples(quantizedSamples, filename);

    }else if(choice == "noiseWave"){
        if(argc < 3){
            std::cout<<"Not enough arguments"<<std::endl;
            return -1;
        }
        
        string filename = argv[2];
        int noise = atoi(argv[3]);
        std::vector<sf::Int16> noisySamples = addNoise(filename, noise);
        plotTwoWaveforms(filename, noisySamples);

    }else if(choice == "noisePlay"){
        if(argc < 3){
            std::cout<<"Not enough arguments"<<std::endl;
            return -1;
        }
        
        string filename = argv[2];
        int noise = atoi(argv[3]);
        std::vector<sf::Int16> noisySamples = addNoise(filename, noise);
        playFromSamples(noisySamples, filename);

    }else{
        std::cout<<"Invalid option";
        return -1;
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////Old menu interaction//////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
string getAudioFIle(){
    string start = "../../../datasets/audio/sample0";
    string finish =".wav";
    std::cout<<"Choose audio file (number from 1-7)\n";
    int num = 0;
    
    std::cin >> num;
    while(num<0 || num>7){
        std::cout<<"Invalid! Choose a number between 1 and 7\n";
        std::cin >> num;
    }

    string numstr = to_string(num);

    string path = start + numstr + finish;
    return path;
}

int main(){
    int option = 0;
    int level = 0;
    
    string filename ="";


    while(option != 4){
        std::cout<<std::endl;
        std::cout<<"-----------------------------------------------"<<std::endl;
        std::cout << "Please select an option"<<std::endl;
        std::cout << "1 - Choose the audio file"<<std::endl;
        std::cout << "2 - Play the audio file"<<std::endl;
        std::cout << "3 - Get details from audio file"<<std::endl;
        std::cout << "4 - Show file waveform"<<std::endl;
        std::cout << "5 - Show histogram of amplitudes"<<std::endl;
        std::cout << "6 - Quantize audio"<<std::endl;
        std::cout << "7 - Calculate and show MSE"<<std::endl;
        std::cout << "8 - Calculate and show SNR"<<std::endl;
        std::cout<<"-----------------------------------------------"<<std::endl;
        std::cin >> option;

        switch(option){
            case 1:
                filename = getAudioFIle();
                break;
            case 2:
                playAudio(filename);
                break;
            case 3:
                getDetails(filename);
                break;
            case 4:
                plotWaveform(filename);
                break;
            case 5:
                histogram(filename);
                break;
            case 6:
            {
                std::cout << "Number of quantization levels: "<<std::endl;
                std::cin >> level;
                std::vector<sf::Int16> quantizedSamples = quantize(filename, level);
                plotTwoWaveforms(filename, quantizedSamples);
                break;
            }
            case 7:
            {
                std::cout << "Number of quantization levels: "<<std::endl;
                std::cin >> level;
                std::vector<sf::Int16> quantizedSamples = quantize(filename, level);
                std::cout<< MSE(quantizedSamples, filename) <<std::endl;
                break;
            }
            case 8:
                {
                std::cout << "Number of quantization levels: "<<std::endl;
                std::cin >> level;
                std::vector<sf::Int16> quantizedSamples = quantize(filename, level);
                double mse = MSE(quantizedSamples, filename);
                std::cout<<SNR(quantizedSamples, filename, mse)<<std::endl;
                break;
            }

            default:
                std::cout<< "Choose a valid option"<<std::endl;

        }
    }
    
}
*/