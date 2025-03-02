# Cocos2d-FMOD-AudioSync
Cocos2d-FMOD-AudioSync is a project that synchronizes graphics with audio in real-time using Cocos2d-x and FMOD. It implements beat detection and adjusts the scale of a sprite based on the intensity of the sound.

### Features

* **Real-time** audio analysis using **FMOD**.
* **Beat detection** to identify sound peaks.
* **Sprite scaling effect** based on beat intensity.
* **Loop and volume control** for audio playback.

### Requeriments

* **Cocos2d-x 4.0**
* **FMOD** Studio **API**
* **C++ 17**
* Compatible with Windows and other platforms supported by Cocos2d-x.

### Installation

1. Clone or download the repository.
2. Ensure you have **FMOD** integrated into your Cocos2d-x project.
3. Add the *.h* and *.cpp* files to your Cocos2d-x project.
4. Set the current path for your audio file in *GameScene.cpp*.
5. Compile and run the project.

### Usage

* Modify *GameScene.cpp* to set the desired audio file.
* Adjust parameters in *BeatDetector.cpp* to fine-tune beat detection.
* Use *setVolume(float) to control playback beahvior

## Example Code

```cpp
(!beatDetector->init("path/to/your/audio", true)) // false for not loop, true for enabled loop.
 beatDetector->setVolume(1.f); volume control.
```
### Demo Video

[![Cocos2d-FMOD-AudioSync](https://img.youtube.com/vi/YmK4mY0gAKlw/0.jpg)](https://youtu.be/mK4mY0gAKlw)

### License

This project is lincensed under the **MIT License**. See the *LICENSE* file for details.
