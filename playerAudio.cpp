#include "PlayerAudio.h"
PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
    transportSource = std::make_unique<juce::AudioTransportSource>();
    resampleSource = std::make_unique<juce::ResamplingAudioSource>(transportSource.get(), false);
}
PlayerAudio::~PlayerAudio()
{
    releaseResources();
}
bool PlayerAudio::loadFile(const juce::File& file)
{
    if (!file.existsAsFile())
    {
        transportSource->setSource(nullptr);
        readerSource.reset();
        currentFile = juce::File{};
        metadata = Metadata();
        albumArt = juce::Image{};
        return false;
    }
    auto* reader = formatManager.createReaderFor(file);
    if (reader == nullptr)
        return false;
    std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
    transportSource->setSource(newSource.get(), 0, nullptr, reader->sampleRate);
    readerSource.reset(newSource.release());
    currentFile = file;
    extractMetadata(file);
    return true;
}
void PlayerAudio::extractMetadata(const juce::File& file)
{
    metadata = Metadata();
    albumArt = juce::Image{};
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
    TagLib::MPEG::File mpegFile(file.getFullPathName().toStdString().c_str());
    if (mpegFile.isValid() && mpegFile.ID3v2Tag())
    {
        auto* id3v2Tag = mpegFile.ID3v2Tag();
        auto frameList = id3v2Tag->frameList("APIC");
        if (!frameList.isEmpty())
        {
            auto* picFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(frameList.front());
            if (picFrame)
            {
                albumArt = juce::ImageFileFormat::loadFrom(picFrame->picture().data(),
                    static_cast<size_t>(picFrame->picture().size()));
            }
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
    resampleSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    resampleSource->getNextAudioBlock(bufferToFill);
    checkABLoop();
}
void PlayerAudio::releaseResources()
{
    resampleSource->releaseResources();
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
void PlayerAudio::setSpeed(double ratio) { resampleSource->setResamplingRatio(ratio); }
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
void PlayerAudio::setLoopPoints(double start, double end)
{
    loopA = start;
    loopB = end;
    looping = true;
}
void PlayerAudio::clearLoopPoints()
{
    loopA = loopB = -1.0;
    looping = false;
}
void PlayerAudio::checkABLoop()
{
    if (!looping || loopA < 0.0 || loopB <= loopA)
        return;
    double pos = getPosition();
    if (pos >= loopB)
        setPosition(loopA);
}