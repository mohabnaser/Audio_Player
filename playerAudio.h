#pragma once
#include <JuceHeader.h>
#define USE_TAGLIB
#ifdef USE_TAGLIB
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/audioproperties.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>
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
    void setSpeed(double ratio);
    void setPosition(double posInSeconds);
    double getPosition() const;
    double getLength() const;
    void setLooping(bool shouldLoop);
    void setLoopPoints(double start, double end);
    void clearLoopPoints();
    void checkABLoop();
    juce::String getTitle() const { return metadata.title; }
    juce::String getArtist() const { return metadata.artist; }
    juce::String getAlbum() const { return metadata.album; }
    juce::String getBitrate() const { return metadata.bitrate; }
    juce::Image getAlbumArt() const { return albumArt; }
    juce::File getLoadedFile() const { return currentFile; }
    juce::AudioFormatManager& getFormatManager() { return formatManager; }
private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioTransportSource> transportSource;
    std::unique_ptr<juce::ResamplingAudioSource> resampleSource;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::File currentFile;
    juce::Image albumArt;
    struct Metadata
    {
        juce::String title, artist, album, bitrate;
        juce::String duration;
    } metadata;
    double loopA{ -1.0 }, loopB{ -1.0 };
    bool looping{ false };
    double currentSpeed{ 1.0 };
    void extractMetadata(const juce::File& file);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};