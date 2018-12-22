//
// Created by adria on 30/11/2018.
//

#ifndef SPACEINVADERS_AUDIOSOURCE_HPP
#define SPACEINVADERS_AUDIOSOURCE_HPP


#include <AL/al.h>
#include <string>
#include <memory>
#include <sndfile.h>

class AudioSource {

    ALuint sourceId_;
    ALuint bufferId_;
    SNDFILE * file_;
    SF_INFO fileInfo_;

public:
    explicit AudioSource(const std::string &filePath);
    ~AudioSource();

    void readFileIntoBuffer(ALuint buffer);

protected:
    static ALenum AudioSource::getFormatFromChannels(int channelCount);
};


#endif //SPACEINVADERS_AUDIOSOURCE_HPP
