//
// Created by adria on 26/12/2018.
//

#ifndef SPACEINVADERS_AUDIOBUFFER_HPP
#define SPACEINVADERS_AUDIOBUFFER_HPP


#include <AL/al.h>
#include <sndfile.h>
#include <string>
#include <vector>
#include <future>


class AudioBuffer {
    SF_INFO fileInfo_;
    std::vector<std::vector<ALshort>> chunks_;
    std::future<void> loadFileThread_;

    void loadFile(const std::string &filePath);
public:
    explicit AudioBuffer(const std::string &filePath);

    ALenum getFormatFromChannels(int channelCount) const;

    void fillBuffer(ALuint buffer, int chunk) const;

    int getChunkCount() const;
};


#endif //SPACEINVADERS_AUDIOBUFFER_HPP
