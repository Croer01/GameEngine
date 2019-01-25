//
// Created by adria on 26/12/2018.
//

#ifndef SPACEINVADERS_AUDIOBUFFER_HPP
#define SPACEINVADERS_AUDIOBUFFER_HPP


#include <AL/al.h>
#include <sndfile.h>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
namespace GameEngine {
namespace Internal {
    class AudioBuffer {
        SF_INFO fileInfo_;
        std::vector<std::vector<ALshort>> chunks_;
        std::thread loadFileThread_;
        std::atomic<bool> loadFileRunning_;
        bool loaded_;


        void loadFile(const std::string &filePath);
    public:
        explicit AudioBuffer(const std::string &filePath);
        virtual ~AudioBuffer();

        ALenum getFormatFromChannels(int channelCount) const;

        bool fillBuffer(ALuint buffer, int chunk) const;
    };
}
}


#endif //SPACEINVADERS_AUDIOBUFFER_HPP
