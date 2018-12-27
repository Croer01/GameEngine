//
// Created by adria on 26/12/2018.
//

#ifndef SPACEINVADERS_AUDIOBUFFER_HPP
#define SPACEINVADERS_AUDIOBUFFER_HPP


#include <AL/al.h>
#include <sndfile.h>
#include <string>

class AudioBuffer {
    ALuint bufferId_;
    SNDFILE * file_;
    SF_INFO fileInfo_;
public:
    explicit AudioBuffer(const std::string &filePath);
    ~AudioBuffer();

    ALenum getFormatFromChannels(int channelCount);

    ALuint getId() const;
};


#endif //SPACEINVADERS_AUDIOBUFFER_HPP
