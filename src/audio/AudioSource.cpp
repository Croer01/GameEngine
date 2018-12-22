//
// Created by adria on 30/11/2018.
//

#include "AudioSource.hpp"
#include "../utils.hpp"
#include <exception>
#include <vector>


AudioSource::AudioSource(const std::string &filePath) {

    // Open file and save initial data
    file_ = sf_open(filePath.c_str(), SFM_READ, &fileInfo_);

    if(!file_)
    {
        throw std::runtime_error("Could not open audio file '" + filePath + "'");
    }

    alGenSources(1, &sourceId_);
    CheckAlError();

    ////////////////////////////////
    alGenBuffers(1, &bufferId_);
    readFileIntoBuffer(bufferId_);
    alSourceQueueBuffers(sourceId_, 1, &bufferId_);
    alSourcePlay(sourceId_);
}

AudioSource::~AudioSource() {
    if(file_)
    {
        sf_close(file_);
        alDeleteSources(1, &sourceId_);
        alDeleteBuffers(1, &bufferId_);
    }
}

void AudioSource::readFileIntoBuffer(ALuint buffer)
{
    // Read whole file
    int sampleCount = fileInfo_.frames * fileInfo_.channels;
    std::vector<ALshort> fileData(sampleCount);
    sf_read_short(file_, &fileData[0], sampleCount);

    alBufferData(
            buffer,
            AudioSource::getFormatFromChannels(fileInfo_.channels),
            &fileData[0],
            sampleCount*sizeof(ALushort),
            fileInfo_.samplerate);
    CheckAlError();
}

ALenum AudioSource::getFormatFromChannels(int channelCount)
{
    // Find audio format based on channel count
    switch (channelCount)
    {
        case 1 : return AL_FORMAT_MONO16;
        case 2 : return AL_FORMAT_STEREO16;
        case 4 : return alGetEnumValue("AL_FORMAT_QUAD16");
        case 6 : return alGetEnumValue("AL_FORMAT_51CHN16");
        case 7 : return alGetEnumValue("AL_FORMAT_61CHN16");
        case 8 : return alGetEnumValue("AL_FORMAT_71CHN16");
        default : return 0;
    }
}