#include "BeatDetector.h"
#include <iostream>

BeatDetector::BeatDetector() : fmodSystem(nullptr), sound(nullptr), channel(nullptr), lastBeatIntensity(0.f), fftDSP(nullptr)
{
    FMOD::System_Create(&fmodSystem);
    fmodSystem->init(512, FMOD_INIT_NORMAL, nullptr);
    fmodSystem->createDSPByType(FMOD_DSP_TYPE_FFT, &fftDSP);
    fftDSP->setParameterInt(FMOD_DSP_FFT_WINDOWSIZE, 512);
    fftDSP->setActive(true);
}

BeatDetector::~BeatDetector()
{
    if (sound) sound->release();
    if (fftDSP) fftDSP->release();
    if (fmodSystem) {
        fmodSystem->close();
        fmodSystem->release();
    }
}

bool BeatDetector::init(const std::string& audioFile, bool loop)
{
    if (!fmodSystem) return false;

    if (fmodSystem->createSound(audioFile.c_str(), loop ? FMOD_LOOP_NORMAL : FMOD_DEFAULT, nullptr, &sound) != FMOD_OK)
        return false;

    if (fmodSystem->playSound(sound, nullptr, false, &channel) != FMOD_OK)
        return false;

    setLoop(loop); 
    channel->addDSP(0, fftDSP);

    return true;
}



void BeatDetector::update()
{
    if (!fmodSystem || !channel || !fftDSP) return;

    fmodSystem->update();

    FMOD_DSP_PARAMETER_FFT* fftData = nullptr;
    unsigned int length = 0;

    if (fftDSP->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void**)&fftData, &length, nullptr, 0) != FMOD_OK || !fftData || fftData->numchannels <= 0) {
        return;
    }

    for (int i = 0; i < 512; ++i) {
        spectrum[i] = fftData->spectrum[0][i];
    }
}

float BeatDetector::getBeatIntensity()
{
    float bass = spectrum[5] * 50;
    return bass;
}

bool BeatDetector::isBeatDetected(float threshold)
{
    float currentIntensity = getBeatIntensity();
    bool detected = (currentIntensity - lastBeatIntensity) > threshold;
    lastBeatIntensity = currentIntensity;
    return detected;
}

void BeatDetector::setVolume(float volume)
{
    if (channel) {
        channel->setVolume(volume);
    }
}

void BeatDetector::setLoop(bool loop) 
{
    if (sound) {
        sound->setMode(loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
    }

    if (channel) {
        channel->setMode(loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
    }
}
