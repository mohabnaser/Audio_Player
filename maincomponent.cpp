#include "MainComponent.h"

MainComponent::MainComponent()
{
    player = std::make_unique<PlayerGUI>("Player 1");
    addAndMakeVisible(player.get());

    applyModernStyling();

    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::applyModernStyling()
{
    juce::LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName("Arial");
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    if (player) player->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();

    if (player)
    {
        player->getNextAudioBlock(bufferToFill);
    }
}

void MainComponent::releaseResources()
{
    if (player) player->releaseResources();
}

void MainComponent::resized()
{
    player->setBounds(getLocalBounds());
}

void MainComponent::paint(juce::Graphics& g)
{
    juce::ColourGradient gradient(
        juce::Colour(0xff0a0a0f), 0.0f, 0.0f,
        juce::Colour(0xff151520), static_cast<float>(getWidth()), static_cast<float>(getHeight()),
        false
    );
    g.setGradientFill(gradient);
    g.fillAll();
}