#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer
{
public:
    PlayerGUI(const juce::String& playerName);
    ~PlayerGUI() override;

    void resized() override;
    void paint(juce::Graphics& g) override;
    void timerCallback() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    void startPlayback();
    void stopPlayback();
    void pausePlayback();
    void setPlaybackPosition(double position); 
    double getPlaybackPosition() const;      



private:
    juce::String playerName;
    PlayerAudio playerAudio;

    juce::TextButton loadButton{ "LOAD" };
    juce::TextButton playButton{ "PLAY" };
    juce::TextButton pauseButton{ "PAUSE" };
    juce::TextButton stopButton{ "STOP" };
    juce::TextButton muteButton{ "MUTE" };
    juce::TextButton loopButton{ "LOOP" };

    juce::TextButton toStartButton{ "TO START" };
    juce::TextButton toEndButton{ "TO END" };
    juce::TextButton skipBackButton{ "-10s" };
    juce::TextButton skipForwardButton{ "+10s" };


    juce::Slider volumeSlider;


    juce::Label playerNameLabel;
    juce::Label titleLabel;
    juce::Label artistLabel;
    juce::Label albumLabel;
    juce::Label bitrateLabel;
    juce::Label volumeLabel;
    
    juce::TextButton loadPlaylistButton{ "Load Playlist" };
    juce::ListBox playlistBox;
    juce::StringArray playlist;
    std::vector<juce::File> playlistFiles;
    int currentPlaylistIndex{ -1 };


    std::unique_ptr<juce::FileChooser> fileChooser;
    juce::File currentLoadedFile;

    bool isMuted{ false };
    float previousVolume{ 0.5f };
    bool isLooping{ false }; 
    

    bool isFadingOut{ false };
    float fadeOutGain{ 1.0f };

    void styleModernButton(juce::TextButton& button, juce::Colour color);
    void styleModernSlider(juce::Slider& slider, juce::Colour color);
    void updateMetadataDisplay();
    
    void loadMultipleFiles();
    void playSelectedFile(int index);

    void startFadeOut();
    void processFadeOut();

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    class PlaylistModel : public juce::ListBoxModel
    {
    public:
        PlaylistModel(PlayerGUI& owner) : gui(owner) {}
        int getNumRows() override { return gui.playlist.size(); }
        void paintListBoxItem(int row, juce::Graphics& g, int width, int height, bool selected) override;
        void listBoxItemClicked(int row, const juce::MouseEvent&) override { gui.playSelectedFile(row); }
    private:
        PlayerGUI& gui;
    };

    PlaylistModel playlistModel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};