#include "audioDecoder.h"
#include <SFML/Audio.hpp>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>

using namespace std;




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

int main()
{  
    int option = 0;
    
    string filename ="";


    while(option != 4){
        std::cout<<std::endl;
        std::cout<<"-----------------------------------------------"<<std::endl;
        std::cout << "Please select an option"<<std::endl;
        std::cout << "1 - Choose the audio file"<<std::endl;
        std::cout << "2 - Play the audio file"<<std::endl;
        std::cout << "3 - Get details from audio file"<<std::endl;
        std::cout << "4 - Show file waveform"<<std::endl;
        std::cout << "5 - Quit"<<std::endl;
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
            case 5:
                std::cout<< "Bye Bye!"<<std::endl;
                break;
            default:
                std::cout<< "Choose a valid option"<<std::endl;

        }
    }
    
}