#include "losslessEncoder.h"
#include "losslessDecoder.h"
#include "BitStream.h"

int main(int argc, char* argv[]){

        std::string choice = argv[1];
        if(argc != 5){
            std::cerr<<"Invalid number of arguments!"<<"\n";
        }

        if(choice == "lossless"){
            std::string inputWav = argv[2];
            std::string outputCodec = argv[3];
        
            std::string inputCodec = argv[3];
            std::string outputWav = argv[4];
        
        
            //Encode
            finalEncode(inputWav, outputCodec);
            std::cout << "Encoding complete, output written to " << outputCodec << std::endl;

        
            //Decode
            finalDecode(inputCodec, outputWav);
            std::cout << "Decoding complete, output written to " << outputWav << std::endl;

        }else if(choice == "lossy"){
            std::string inputWav = argv[2];
            std::string outputCodec = argv[3];
        
            std::string inputCodec = argv[3];
            std::string outputWav = argv[4];
        
        
            //Encode
            lossyEncode(inputWav, outputCodec);
            std::cout << "Encoding complete, output written to " << outputCodec << std::endl;

        
            //Decode
            finalDecode(inputCodec, outputWav);
            std::cout << "Decoding complete, output written to " << outputWav << std::endl;
        }else{
            std::cerr<<"Invalid choice!";
        }

        /* FOR DEBUGGING
        std::string inputWav = "../../../datasets/audio/sample05.wav";
        std::string outputCodec = "../audio.mac";
        
        std::string inputCodec = "../audio.mac";
        std::string outputWav = "../test.wav";
        
        
        //Encode
        finalEncode(inputWav, outputCodec);
        std::cout << "Encoding complete, output written to " << outputCodec << std::endl;

        
        //Decode
        finalDecode(inputCodec, outputWav);
        std::cout << "Decoding complete, output written to " << outputWav << std::endl;
        */
}


