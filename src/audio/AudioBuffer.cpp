//
// Created by adria on 26/12/2018.
//

#include <vector>
#include "AudioBuffer.hpp"
#include "../utils.hpp"


AudioBuffer::AudioBuffer(const std::string &filePath) {

    // Open file and save initial data
    file_ = sf_open(filePath.c_str(), SFM_READ, &fileInfo_);

    if(!file_)
    {
        throw std::runtime_error("Could not open audio file '" + filePath + "'");
    }

    //TODO: load in multiple al buffers
    alGenBuffers(1, &bufferId_);
    CheckAlError();

    // Read whole file
    int sampleCount = fileInfo_.frames * fileInfo_.channels;
    std::vector<ALshort> fileData(sampleCount);
    sf_read_short(file_, &fileData[0], sampleCount);

    alBufferData(
            bufferId_,
            getFormatFromChannels(fileInfo_.channels),
            &fileData[0],
            sampleCount*sizeof(ALushort),
            fileInfo_.samplerate);
    CheckAlError();

}

AudioBuffer::~AudioBuffer() {
    if(file_)
    {
        //TODO: Remove the file reference after load into buffer if it is not streamed?
        sf_close(file_);
        alDeleteBuffers(1, &bufferId_);
    }
}


ALenum AudioBuffer::getFormatFromChannels(int channelCount)
{
    // Find audio format based on channel count
    int format = 0;
    switch (channelCount)
    {
        case 1 :
            format = AL_FORMAT_MONO16;
            break;
        case 2 :
            format = AL_FORMAT_STEREO16;
            break;
        case 4 :
            format = alGetEnumValue("AL_FORMAT_QUAD16");
            break;
        case 6 :
            format = alGetEnumValue("AL_FORMAT_51CHN16");
            break;
        case 7 :
            format = alGetEnumValue("AL_FORMAT_61CHN16");
            break;
        case 8 :
            format = alGetEnumValue("AL_FORMAT_71CHN16");
            break;
    }

    // bug OS X
    if(format == -1)
        format = 0;

    return format;
}

ALuint AudioBuffer::getId() const {
    return bufferId_;
}
