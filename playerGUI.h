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

    void setMixerGain(float gain);
    void swapTrackWith(PlayerGUI* other);
    void saveState();

private:
    juce::String playerName;
    PlayerAudio playerAudio;
    float mixerGain{ 1.0f };

    juce::DrawableButton loadButton{ "load", juce::DrawableButton::ImageOnButtonBackground };
    juce::DrawableButton playButton{ "play", juce::DrawableButton::ImageOnButtonBackground };
    juce::DrawableButton pauseButton{ "pause", juce::DrawableButton::ImageOnButtonBackground };
    juce::DrawableButton stopButton{ "stop", juce::DrawableButton::ImageOnButtonBackground };
    juce::DrawableButton muteButton{ "mute", juce::DrawableButton::ImageOnButtonBackground };
    juce::DrawableButton loopButton{ "loop", juce::DrawableButton::ImageOnButtonBackground };

    juce::DrawableButton toStartButton{ "toStart", juce::DrawableButton::ImageOnButtonBackground };
    juce::DrawableButton toEndButton{ "toEnd", juce::DrawableButton::ImageOnButtonBackground };

    juce::TextButton skipBackButton{ "-10s" };
    juce::TextButton skipForwardButton{ "+10s" };

    enum class ABState { A, B };
    ABState abState{ ABState::A };
    juce::TextButton abToggleButton{ "A" };
    juce::TextButton clearABButton{ "Clear A-B" };

    juce::Slider volumeSlider;
    juce::Slider speedSlider;
    juce::Slider positionSlider;

    juce::Label playerNameLabel;
    juce::Label titleLabel;
    juce::Label artistLabel;
    juce::Label albumLabel;
    juce::Label bitrateLabel;
    juce::Label timeLabel;
    juce::Label volumeLabel;
    juce::Label speedLabel;
    juce::Label abLoopLabel;

    juce::ImageComponent albumArtComponent;
    juce::Rectangle<int> waveformArea;
    juce::AudioThumbnailCache thumbnailCache{ 5 };
    juce::AudioThumbnail thumbnail{ 512, playerAudio.getFormatManager(), thumbnailCache };

    juce::DrawableButton loadPlaylistButton{ "loadPlaylist", juce::DrawableButton::ImageOnButtonBackground };
    juce::DrawableButton clearPlaylistButton{ "clearPlaylist", juce::DrawableButton::ImageOnButtonBackground };
    juce::ListBox playlistBox;
    juce::StringArray playlist;
    std::vector<juce::File> playlistFiles;
    int currentPlaylistIndex{ -1 };

    juce::DrawableButton addMarkerButton{ "addMarker", juce::DrawableButton::ImageOnButtonBackground };
    juce::DrawableButton clearMarkersButton{ "clearMarkers", juce::DrawableButton::ImageOnButtonBackground };
    juce::ListBox markerListBox;

    struct Marker
    {
        double position{ 0.0 };
        juce::String name;
        Marker() = default;
        Marker(double pos, const juce::String& n) : position(pos), name(n) {}
    };
    std::vector<Marker> markers;

    std::unique_ptr<juce::FileChooser> fileChooser;

    bool isMuted{ false };
    float previousVolume{ 0.5f };
    bool isLooping{ false };
    bool isDraggingPosition{ false };
    double pointA{ -1.0 };
    double pointB{ -1.0 };

    bool isFadingOut{ false };
    float fadeOutGain{ 1.0f };

    void styleModernIcon(juce::DrawableButton& button, juce::Colour color, const juce::String& iconSVG);
    void styleModernButton(juce::TextButton& button, juce::Colour color); 
    void styleModernSlider(juce::Slider& slider, juce::Colour color);
    void updateTimeDisplay();
    void updatePositionSlider();
    void updateMetadataDisplay();
    void drawWaveform(juce::Graphics& g);
    juce::String formatTime(double seconds);

    void loadFile(const juce::File& file, bool autoPlay);
    void loadMultipleFiles();
    void playSelectedFile(int index);
    void deletePlaylistItem(int index);
    void addMarkerAtCurrentPosition();
    void jumpToMarker(int index);
    void deleteMarkerItem(int index);

    void startFadeOut();
    void processFadeOut();

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    juce::PropertiesFile::Options getPropertyFileOptions();
    void loadState();

    class PlaylistModel : public juce::ListBoxModel
    {
    public:
        PlaylistModel(PlayerGUI& owner) : gui(owner) {}
        int getNumRows() override { return gui.playlist.size(); }
        void paintListBoxItem(int row, juce::Graphics& g, int width, int height, bool selected) override;
        void listBoxItemClicked(int row, const juce::MouseEvent& e) override;
    private:
        PlayerGUI& gui;
    };

    class MarkerModel : public juce::ListBoxModel
    {
    public:
        MarkerModel(PlayerGUI& owner) : gui(owner) {}
        int getNumRows() override { return static_cast<int>(gui.markers.size()); }
        void paintListBoxItem(int row, juce::Graphics& g, int width, int height, bool selected) override;
        void listBoxItemClicked(int row, const juce::MouseEvent& e) override;
    private:
        PlayerGUI& gui;
    };

    PlaylistModel playlistModel;
    MarkerModel markerModel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};