#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
    transportSource = std::make_unique<juce::AudioTransportSource>();
}

PlayerAudio::~PlayerAudio()
{
    releaseResources();
}

bool PlayerAudio::loadFile(const juce::File& file)
{
    if (!file.existsAsFile()) return false;

    auto* reader = formatManager.createReaderFor(file);
    if (reader == nullptr)
        return false;

    std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
    transportSource->setSource(newSource.get(), 0, nullptr, reader->sampleRate);
    readerSource.reset(newSource.release());

    extractMetadata(file);
    return true;
}

void PlayerAudio::extractMetadata(const juce::File& file)
{
    metadata = Metadata();

#ifdef USE_TAGLIB
    TagLib::FileRef fileRef(file.getFullPathName().toStdString().c_str());
    if (!fileRef.isNull() && fileRef.tag())
    {
        TagLib::Tag* tag = fileRef.tag();
        metadata.title = juce::String(tag->title().toCString(true));
        metadata.artist = juce::String(tag->artist().toCString(true));
        metadata.album = juce::String(tag->album().toCString(true));

        if (fileRef.audioProperties())
        {
            TagLib::AudioProperties* props = fileRef.audioProperties();
            int len = props->lengthInSeconds();
            metadata.duration = juce::String::formatted("%02d:%02d", len / 60, len % 60);
            metadata.bitrate = juce::String(props->bitrate()) + " kbps";
        }
    }
#endif

    if (metadata.title.isEmpty())  metadata.title = file.getFileNameWithoutExtension();
    if (metadata.artist.isEmpty()) metadata.artist = "Unknown Artist";
    if (metadata.album.isEmpty())  metadata.album = "Unknown Album";
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    transportSource->getNextAudioBlock(bufferToFill); 
}

void PlayerAudio::releaseResources()
{
    transportSource->releaseResources();
    transportSource->setSource(nullptr);
    readerSource.reset();
}

void PlayerAudio::start()
{
    if (readerSource != nullptr)
        readerSource->setLooping(looping); 
    transportSource->start();
}
void PlayerAudio::stop() { transportSource->stop(); }
void PlayerAudio::pause() { transportSource->stop(); }

void PlayerAudio::setGain(float gain) { transportSource->setGain(gain); }

void PlayerAudio::setPosition(double posInSeconds)
{
    transportSource->setPosition(posInSeconds);
}
double PlayerAudio::getPosition() const { return transportSource->getCurrentPosition(); }
double PlayerAudio::getLength() const { return transportSource->getLengthInSeconds(); }

void PlayerAudio::setLooping(bool shouldLoop) 
{
    looping = shouldLoop;
    if (readerSource != nullptr)
        readerSource->setLooping(looping);
}