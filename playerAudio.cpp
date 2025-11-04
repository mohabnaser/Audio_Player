#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
}

PlayerAudio::~PlayerAudio()
{
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (readerSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    transportSource.getNextAudioBlock(bufferToFill);
    if (looping && !transportSource.isPlaying())
    {
        transportSource.setPosition(0.0);
        transportSource.start();
    }
}

void PlayerAudio::releaseResources()
{
    transportSource.releaseResources();
}

bool PlayerAudio::loadFile(const juce::File& file)
{
    if (!file.existsAsFile()) return false;
    auto* reader = formatManager.createReaderFor(file);
    if (reader == nullptr) return false;
    transportSource.stop();
    transportSource.setSource(nullptr);
    readerSource.reset();
    readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
    transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);
    return true;
}

void PlayerAudio::start()
{
    transportSource.start();
}

void PlayerAudio::stop()
{
    transportSource.stop();
}

void PlayerAudio::pause()
{
    if (transportSource.isPlaying())
        transportSource.stop();
    else
        transportSource.start();
}

void PlayerAudio::setGain(float gain)
{
    transportSource.setGain(gain);
}

void PlayerAudio::setPosition(double pos)
{
    if (readerSource.get() != nullptr)
    {
        transportSource.setPosition(pos);
    }
}

void PlayerAudio::setLooping(bool shouldLoop)
{
    looping = shouldLoop;
}

double PlayerAudio::getPosition() const
{
    return transportSource.getCurrentPosition();
}

double PlayerAudio::getLength() const
{
    if (readerSource.get() != nullptr)
        return transportSource.getLengthInSeconds();
    return 0.0;
}

bool PlayerAudio::isPlaying() const
{
    return transportSource.isPlaying();
}