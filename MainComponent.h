#pragma once
#include <JuceHeader.h>
#include "PlayerGUI.h"
class MainComponent : public juce::AudioAppComponent,
    public juce::Timer
{
public:
    MainComponent();
    ~MainComponent() override;
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void resized() override;
    void paint(juce::Graphics& g) override;
    void timerCallback() override;
    void saveAllStates();
private:
    std::unique_ptr<PlayerGUI> player1;
    std::unique_ptr<PlayerGUI> player2;
    juce::ToggleButton mixerModeToggle;
    bool mixerModeEnabled{ false };
    juce::Label mixerLabel;
    juce::Slider crossfadeSlider;
    juce::Label crossfadeLabel;
    juce::TextButton syncPlayButton{ "Sync Play" };
    juce::TextButton autoCrossfadeButton{ "Auto Crossfade" };
    juce::TextButton swapTracksButton{ "Swap Tracks" };
    juce::Slider crossfadeSpeedSlider;
    juce::Label crossfadeSpeedLabel;
    bool autoCrossfadeActive{ false };
    float crossfadeProgress{ 0.0f };
    float crossfadeSpeed{ 0.01f };
    float crossfadeDirection{ 1.0f };
    void setupMixerControls();
    void toggleMixerMode();
    void syncPlayBothPlayers();
    void startAutoCrossfade();
    void stopAutoCrossfade();
    void swapPlayerTracks();
    void updateCrossfadeMix();
    void animateCrossfade();
    void applyModernStyling();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};