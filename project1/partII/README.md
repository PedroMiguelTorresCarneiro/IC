# IC - Information and Coding (2024/25)

--
# PROJECT 1 : Part II

- **Installing Boost.locale** (to work with UTF-8 encoding)
```bash
#macOs:
    Dowload the files from the SFML website, the cmake file will locate it 
#Linux:
    sudo apt-get update
    sudo apt-get install libsfml-dev
#Windows:
    Download the files directly from the SFML website
```

- **Build and Run**
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ./SoundDecoder
    ```

    Or, just:
    
    ```bash
    $ sh run.sh
    ```
## **Audio with SFML**
Sfml is a library that allows us to maipulate audio in a number of ways.
```md
    buffer.loadFromFile(filename) ->load the audio file
    sound.setBuffer(buffer)
    sound.play() ->play the sound
```
### **Raw audio data**
Sfml contains a variety of functions to extract the raw data from an audio file.
```md
    buffer.getSamples() -> gives us a pointer to an array with the samples that make up the audio file
    buffer.getSampleCount() -> gives us the number of samples
    buffer.getSampleRate() -> gives us the number of samples per second
    buffer.getChannelCount() ->gives us the number of channesl (ex. 1 is mono, 2 is stereo)
```

## **Waveforms**
The waveform of an audio file represents how the different amplitude values along the length of the file. We must be carefull however, in the samples array half the samples belong to the right channel and the other half belong to the left channel (for stereo audio), so we must skip every other sample.
```md
    for (std::size_t i = 0; i < sampleCount; i += channelCount) 
```

To draw the waveform, we used the sfml sf::VertexArray with the sf::LineStrip property to draw lines based on the amplitude values.

![Waveform](../partII/figures/Waveform.jpg) 
Example of a waveform being displayed

## **Quantization**

Quantization is the process with which we reduce the number of bits needed to represent an audio file by reducing the number of amplitude values the samples can have. This, however, results in loss of audio quality.