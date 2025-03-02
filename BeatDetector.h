#pragma once
#include <fmod.hpp>
#include <string>

class BeatDetector
{
public:
    BeatDetector();
    ~BeatDetector();

    bool init(const std::string& audioFile, bool loop);
    void update();
    float getBeatIntensity();
    bool isBeatDetected(float threshold);

    void setVolume(float volume);
    void setLoop(bool loop);

private:
    FMOD::System* fmodSystem;
    FMOD::Sound* sound;
    FMOD::Channel* channel;
    FMOD::DSP* fftDSP;
    float spectrum[512];
    float lastBeatIntensity;
};
