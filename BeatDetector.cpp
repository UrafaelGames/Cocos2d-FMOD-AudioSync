#include "BeatDetector.h"
#include "cocos2d.h"
#include <iostream>
#include "UrafaelxAudioManager.h"

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

bool BeatDetector::init( const std::string& audioFile, bool loop )
{
    if (!fmodSystem) {
        CCLOG( "Error: FMOD system not initialized." );
        return false;
    }

    channel = UrafaelxAudioManager::getInstance()->playBgForBeatDetector( audioFile, loop );
    if (!channel) {
        CCLOG( "Error: Failed to get a valid FMOD channel for BeatDetector!" );
        return false;
    }

    setLoop( loop );

    if (fftDSP) {
        FMOD_RESULT result = channel->addDSP( 0, fftDSP );
        if (result != FMOD_OK) {
            CCLOG( "FMOD Error (addDSP): %d", result );
            return false;
        }
        fftDSP->setActive( true );
    }
    else {
        CCLOG( "Error: fftDSP is nullptr!" );
        return false;
    }

    return true;
}



void BeatDetector::update()
{
    if (!fmodSystem) {
        CCLOG( "Error: fmodSystem is nullptr in BeatDetector::update" );
        return;
    }

    if (!channel) {
        CCLOG( "Error: channel is nullptr in BeatDetector::update" );
        return;
    }

    if (!fftDSP) {
        CCLOG( "Error: fftDSP is nullptr in BeatDetector::update" );
        return;
    }

    fmodSystem->update();

    FMOD_DSP_PARAMETER_FFT* fftData = nullptr;
    unsigned int length = 0;

    FMOD_RESULT result = fftDSP->getParameterData( FMOD_DSP_FFT_SPECTRUMDATA, (void**)&fftData, &length, nullptr, 0 );
    if (result != FMOD_OK) {
        CCLOG( "Error: getParameterData failed in BeatDetector::update, FMOD result: %d", result );
        return;
    }

    if (!fftData) {
        CCLOG( "Error: FFT data is nullptr in BeatDetector::update" );
        return;
    }

    if (fftData->numchannels <= 0) {
        CCLOG( "Error: No channels in FFT data in BeatDetector::update" );
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

void BeatDetector::release()
{
    if (sound) {
        sound->release();
        sound = nullptr;
    }

    if (fftDSP) {
        fftDSP->release();
        fftDSP = nullptr;
    }

    if (channel) {
        channel->stop(); 
        channel = nullptr;
    }
}
