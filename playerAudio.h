#pragma once
#include <JuceHeader.h>
#define USE_TAGLIB
#ifdef USE_TAGLIB
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/audioproperties.h>
#endif

class PlayerAudio
{
public:
    PlayerAudio();
    ~PlayerAudio();

    bool loadFile(const juce::File& file);
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    void start();
    void stop();
    void pause();
    void setGain(float gain);
    void setPosition(double posInSeconds); 
    double getPosition() const;
    double getLength() const;
    void setLooping(bool shouldLoop); 

    // Metadata
    juce::String getTitle() const { return metadata.title; }
    juce::String getArtist() const { return metadata.artist; }
    juce::String getAlbum() const { return metadata.album; }
    juce::String getBitrate() const { return metadata.bitrate; }

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioTransportSource> transportSource;
    std::unique_ptr<juce::ResamplingAudioSource> resampleSource;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;

    struct Metadata
    {
        juce::String title, artist, album, bitrate;
        juce::String duration;
    } metadata;

    bool looping{ false };

    void extractMetadata(const juce::File& file);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};