//
// Created by adria on 26/12/2018.
//

#include <vector>
#include "AudioBuffer.hpp"
#include "../utils.hpp"


AudioBuffer::AudioBuffer(const std::string &filePath) {
    loadFileThread_ = std::async(std::launch::async, &AudioBuffer::loadFile, this, filePath);
}

ALenum AudioBuffer::getFormatFromChannels(int channelCount) const
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

void AudioBuffer::fillBuffer(ALuint buffer, int chunk) const {
    loadFileThread_.wait();

    //TODO: add mutex?
    std::vector<ALshort> chunkData = chunks_[chunk];
    alBufferData(
            buffer,
            getFormatFromChannels(fileInfo_.channels),
            &chunkData[0],
            chunkData.size() * sizeof(ALushort),
            fileInfo_.samplerate);
    CheckAlError();
}

int AudioBuffer::getChunkCount() const {
    loadFileThread_.wait();
    return chunks_.size();
}

void AudioBuffer::loadFile(const std::string &filePath) {
    // Open file and save initial data
    SNDFILE * file = sf_open(filePath.c_str(), SFM_READ, &fileInfo_);

    if(!file)
        throw std::runtime_error("Could not open audio file '" + filePath + "'");

    int sampleSecondCount = fileInfo_.channels * fileInfo_.samplerate;

    bool eof= false;
    while(!eof) {
        std::vector<ALshort> chunk(sampleSecondCount);
        sf_count_t readCount = sf_read_short(file, &chunk[0], sampleSecondCount);
        eof = readCount == 0;
        if (!eof)
            chunks_.push_back(chunk);
    }

    sf_close(file);
}
