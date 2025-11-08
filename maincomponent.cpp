#include "MainComponent.h"

MainComponent::MainComponent()
{
    player1 = std::make_unique<PlayerGUI>("Player 1");
    addAndMakeVisible(player1.get());

    player2 = std::make_unique<PlayerGUI>("Player 2");
    player2->setVisible(false);
    addAndMakeVisible(player2.get());

    mixerModeToggle.setButtonText("Enable Mixer Mode");
    mixerModeToggle.setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    mixerModeToggle.setColour(juce::ToggleButton::tickColourId, juce::Colour(0xff00e5ff));
    mixerModeToggle.onClick = [this]() { toggleMixerMode(); };
    addAndMakeVisible(mixerModeToggle);

    setupMixerControls();
    applyModernStyling();

    setAudioChannels(0, 2);
    startTimer(50);
}

MainComponent::~MainComponent()
{
    stopTimer();
    shutdownAudio();
}

void MainComponent::saveAllStates()
{
    if (player1) player1->saveState();
    if (player2) player2->saveState();
}

void MainComponent::setupMixerControls()
{
    mixerLabel.setText("MIXER CONTROLS", juce::dontSendNotification);
    mixerLabel.setFont(juce::Font(18.0f, juce::Font::bold));
    mixerLabel.setColour(juce::Label::textColourId, juce::Colour(0xff00e5ff));
    mixerLabel.setJustificationType(juce::Justification::centred);
    mixerLabel.setVisible(false);
    addAndMakeVisible(mixerLabel);

    crossfadeSlider.setRange(0.0, 1.0, 0.01);
    crossfadeSlider.setValue(0.5);
    crossfadeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    crossfadeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    crossfadeSlider.onValueChange = [this]() { updateCrossfadeMix(); };
    crossfadeSlider.setVisible(false);
    addAndMakeVisible(crossfadeSlider);

    crossfadeLabel.setText("Crossfade: Player 1 <-> Player 2", juce::dontSendNotification);
    crossfadeLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    crossfadeLabel.setColour(juce::Label::textColourId, juce::Colour(0xffff00ff));
    crossfadeLabel.setJustificationType(juce::Justification::centred);
    crossfadeLabel.setVisible(false);
    addAndMakeVisible(crossfadeLabel);

    syncPlayButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff2196f3));
    syncPlayButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    syncPlayButton.onClick = [this]() { syncPlayBothPlayers(); };
    syncPlayButton.setVisible(false);
    addAndMakeVisible(syncPlayButton);

    autoCrossfadeButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff9c27b0));
    autoCrossfadeButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    autoCrossfadeButton.onClick = [this]()
        {
            if (autoCrossfadeActive) stopAutoCrossfade();
            else startAutoCrossfade();
        };
    autoCrossfadeButton.setVisible(false);
    addAndMakeVisible(autoCrossfadeButton);

    swapTracksButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xffff9800));
    swapTracksButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    swapTracksButton.onClick = [this]() { swapPlayerTracks(); };
    swapTracksButton.setVisible(false);
    addAndMakeVisible(swapTracksButton);

    crossfadeSpeedSlider.setRange(0.005, 0.05, 0.005);
    crossfadeSpeedSlider.setValue(0.01);
    crossfadeSpeedSlider.onValueChange = [this]() { crossfadeSpeed = static_cast<float>(crossfadeSpeedSlider.getValue()); };
    crossfadeSpeedSlider.setVisible(false);
    addAndMakeVisible(crossfadeSpeedSlider);

    crossfadeSpeedLabel.setText("Auto Fade Speed", juce::dontSendNotification);
    crossfadeSpeedLabel.setFont(juce::Font(12.0f));
    crossfadeSpeedLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    crossfadeSpeedLabel.setVisible(false);
    addAndMakeVisible(crossfadeSpeedLabel);
}

void MainComponent::applyModernStyling()
{
    juce::LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName("Arial");
}

void MainComponent::toggleMixerMode()
{
    mixerModeEnabled = !mixerModeEnabled;
    player2->setVisible(mixerModeEnabled);
    mixerLabel.setVisible(mixerModeEnabled);
    crossfadeSlider.setVisible(mixerModeEnabled);
    crossfadeLabel.setVisible(mixerModeEnabled);
    syncPlayButton.setVisible(mixerModeEnabled);
    autoCrossfadeButton.setVisible(mixerModeEnabled);
    swapTracksButton.setVisible(mixerModeEnabled);
    crossfadeSpeedSlider.setVisible(mixerModeEnabled);
    crossfadeSpeedLabel.setVisible(mixerModeEnabled);
    resized();
}

void MainComponent::syncPlayBothPlayers()
{
    if (!player1 || !player2) return;

    player1->stopPlayback();
    player2->stopPlayback();

    player1->setPlaybackPosition(0.0);
    player2->setPlaybackPosition(0.0);

    player1->startPlayback();
    player2->startPlayback();
}

void MainComponent::startAutoCrossfade()
{
    autoCrossfadeActive = true;
    crossfadeProgress = static_cast<float>(crossfadeSlider.getValue());
    crossfadeDirection = 1.0f;
    autoCrossfadeButton.setButtonText("Stop Auto Fade");
    autoCrossfadeButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xfff44336));
}

void MainComponent::stopAutoCrossfade()
{
    autoCrossfadeActive = false;
    autoCrossfadeButton.setButtonText("Auto Crossfade");
    autoCrossfadeButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff9c27b0));
}

void MainComponent::swapPlayerTracks()
{
    if (!player1 || !player2) return;
    player1->swapTrackWith(player2.get());
}

void MainComponent::updateCrossfadeMix()
{
    if (!player1 || !player2) return;

    float crossfade = static_cast<float>(crossfadeSlider.getValue());

    float gain1 = std::cos(crossfade * juce::MathConstants<float>::pi * 0.5f);
    float gain2 = std::sin(crossfade * juce::MathConstants<float>::pi * 0.5f);

    player1->setMixerGain(gain1);
    player2->setMixerGain(gain2);
}

void MainComponent::animateCrossfade()
{
    if (!autoCrossfadeActive) return;

    crossfadeProgress += crossfadeSpeed * crossfadeDirection;

    if (crossfadeProgress >= 1.0f)
    {
        crossfadeProgress = 1.0f;
        crossfadeDirection = -1.0f;
    }
    else if (crossfadeProgress <= 0.0f)
    {
        crossfadeProgress = 0.0f;
        crossfadeDirection = 1.0f;
    }

    crossfadeSlider.setValue(crossfadeProgress, juce::dontSendNotification);
    updateCrossfadeMix();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    if (player1) player1->prepareToPlay(samplesPerBlockExpected, sampleRate);
    if (player2) player2->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();

    if (mixerModeEnabled && player1 && player2)
    {
        juce::AudioBuffer<float> temp1(bufferToFill.buffer->getNumChannels(), bufferToFill.numSamples);
        juce::AudioBuffer<float> temp2(bufferToFill.buffer->getNumChannels(), bufferToFill.numSamples);

        temp1.clear();
        temp2.clear();

        juce::AudioSourceChannelInfo info1(&temp1, 0, bufferToFill.numSamples);
        juce::AudioSourceChannelInfo info2(&temp2, 0, bufferToFill.numSamples);

        if (player1) player1->getNextAudioBlock(info1);
        if (player2) player2->getNextAudioBlock(info2);

        for (int ch = 0; ch < bufferToFill.buffer->getNumChannels(); ++ch)
        {
            bufferToFill.buffer->addFrom(ch, bufferToFill.startSample, temp1, ch, 0, bufferToFill.numSamples);
            bufferToFill.buffer->addFrom(ch, bufferToFill.startSample, temp2, ch, 0, bufferToFill.numSamples);
        }
    }
    else if (player1)
    {
        player1->getNextAudioBlock(bufferToFill);
    }
}

void MainComponent::releaseResources()
{
    if (player1) player1->releaseResources();
    if (player2) player2->releaseResources();
}

void MainComponent::timerCallback()
{
    animateCrossfade();
    repaint();
}

void MainComponent::resized()
{
    auto area = getLocalBounds();
    auto topBar = area.removeFromTop(40);
    mixerModeToggle.setBounds(topBar.removeFromRight(180).reduced(5));

    if (mixerModeEnabled)
    {
        auto mixerArea = area.removeFromBottom(180);

        mixerLabel.setBounds(mixerArea.removeFromTop(30));

        auto controlsArea = mixerArea.removeFromTop(100);

        auto crossfadeArea = controlsArea.removeFromTop(50);
        crossfadeLabel.setBounds(crossfadeArea.removeFromTop(20));
        crossfadeSlider.setBounds(crossfadeArea.reduced(100, 5));

        auto buttonArea = controlsArea.removeFromTop(35);
        int buttonWidth = 140;
        int spacing = (getWidth() - buttonWidth * 3) / 4;
        syncPlayButton.setBounds(spacing, buttonArea.getY(), buttonWidth, 30);
        autoCrossfadeButton.setBounds(spacing * 2 + buttonWidth, buttonArea.getY(), buttonWidth, 30);
        swapTracksButton.setBounds(spacing * 3 + buttonWidth * 2, buttonArea.getY(), buttonWidth, 30);

        int speedWidth = 160;
        int speedHeight = 22;
        int autoX = spacing * 2 + buttonWidth;
        int autoY = buttonArea.getY();
        crossfadeSpeedLabel.setBounds(autoX + (buttonWidth - speedWidth) / 2, autoY + 36, speedWidth, 16);
        crossfadeSpeedSlider.setBounds(autoX + (buttonWidth - speedWidth) / 2, autoY + 52, speedWidth, speedHeight);

        int halfWidth = area.getWidth() / 2;
        player1->setBounds(area.removeFromLeft(halfWidth - 5));
        area.removeFromLeft(10);
        player2->setBounds(area);
    }
    else
    {
        player1->setBounds(area);
    }
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

    if (mixerModeEnabled)
    {
        g.setColour(juce::Colour(0xff00e5ff).withAlpha(0.3f));
        g.drawLine(static_cast<float>(getWidth() / 2), 40.0f,
            static_cast<float>(getWidth() / 2), static_cast<float>(getHeight() - 180), 2.0f);

        g.setColour(juce::Colour(0xffff00ff).withAlpha(0.4f));
        g.drawRect(10, getHeight() - 175, getWidth() - 20, 165, 2);
    }
}