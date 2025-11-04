#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer
{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void resized() override;
    void paint(juce::Graphics& g) override;
    void timerCallback() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

private:
    PlayerAudio playerAudio;

    juce::TextButton loadButton{ "LOAD" };
    juce::TextButton playButton{ "PLAY" };
    juce::TextButton pauseButton{ "PAUSE" };
    juce::TextButton stopButton{ "STOP" };
    juce::TextButton prevButton{ "<<" };
    juce::TextButton nextButton{ ">>" };
    juce::TextButton muteButton{ "MUTE" };
    juce::TextButton loopButton{ "LOOP" };

    juce::Slider volumeSlider;
    juce::Label volumeLabel;
    juce::Label titleLabel;
    juce::Label timeLabel;

    std::unique_ptr<juce::FileChooser> fileChooser;
    juce::String currentFileName;

    bool isMuted = false;
    float previousVolume = 0.5f;
    bool isLooping = false;

    void updateTimeDisplay();
    void styleButton(juce::TextButton& button, juce::Colour color);
    juce::String formatTime(double seconds);

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};