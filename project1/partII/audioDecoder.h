#ifndef SOUND_DECODER_H
#define SOUND_DECODER_H

#include "audioDecoder.h"
#include <SFML/Audio.hpp>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <SFML/Graphics.hpp>
#include <cmath>
using namespace std;


int playAudio(string filename);
int getDetails(string filename);
int plotWaveform(string filename);
void plotTwoWaveforms(string filename, const std::vector<sf::Int16>& quantizedSamples);
int histogram(string filename);
std::vector<sf::Int16> quantize(string filename, int level);
#endif