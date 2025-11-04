#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    for (auto* btn : { &loadButton, &playButton, &pauseButton, &stopButton,
                       &prevButton, &nextButton, &muteButton, &loopButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }
    styleButton(playButton, juce::Colours::green);
    styleButton(pauseButton, juce::Colours::orange);
    styleButton(stopButton, juce::Colours::red);
    styleButton(prevButton, juce::Colours::lightblue);
    styleButton(nextButton, juce::Colours::lightblue);
    styleButton(loadButton, juce::Colours::purple);
    styleButton(muteButton, juce::Colours::grey);
    styleButton(loopButton, juce::Colours::grey);
    pauseButton.setVisible(false);
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    volumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    volumeSlider.setColour(juce::Slider::trackColourId, juce::Colours::lightblue);
    volumeSlider.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);
    volumeLabel.setText("Volume: 50%", juce::dontSendNotification);
    volumeLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    volumeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    volumeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(volumeLabel);
    titleLabel.setText("No Track Loaded", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    titleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleLabel);
    timeLabel.setText("00:00 / 00:00", juce::dontSendNotification);
    timeLabel.setFont(juce::Font(18.0f, juce::Font::bold));
    timeLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    timeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(timeLabel);
    startTimer(100);
}

PlayerGUI::~PlayerGUI()
{
    stopTimer();
}

void PlayerGUI::styleButton(juce::TextButton& button, juce::Colour color)
{
    button.setColour(juce::TextButton::buttonColourId, color.darker(0.3f));
    button.setColour(juce::TextButton::buttonOnColourId, color);
    button.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    button.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
}

juce::String PlayerGUI::formatTime(double seconds)
{
    int mins = (int)seconds / 60;
    int secs = (int)seconds % 60;
    return juce::String::formatted("%02d:%02d", mins, secs);
}

void PlayerGUI::updateTimeDisplay()
{
    double current = playerAudio.getPosition();
    double total = playerAudio.getLength();

    timeLabel.setText(formatTime(current) + " / " + formatTime(total),
        juce::dontSendNotification);
}

void PlayerGUI::timerCallback()
{
    updateTimeDisplay();
}

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
    playerAudio.releaseResources();
}

void PlayerGUI::paint(juce::Graphics& g)
{
    juce::ColourGradient gradient(
        juce::Colour(0xff1a1a2e), 0, 0,
        juce::Colour(0xff16213e), getWidth(), getHeight(),
        false
    );
    g.setGradientFill(gradient);
    g.fillAll();
    g.setColour(juce::Colours::cyan.withAlpha(0.3f));
    g.drawRect(getLocalBounds(), 3);
    g.setColour(juce::Colours::cyan.withAlpha(0.1f));
    g.drawLine(20, 80, getWidth() - 20, 80, 2.0f);
    g.drawLine(20, getHeight() - 80, getWidth() - 20, getHeight() - 80, 2.0f);
}

void PlayerGUI::resized()
{
    auto area = getLocalBounds().reduced(20);
    titleLabel.setBounds(area.removeFromTop(50));
    area.removeFromTop(10);
    timeLabel.setBounds(area.removeFromTop(30));
    area.removeFromTop(20);
    auto controlArea = area.removeFromTop(80);
    int buttonSize = 70;
    int spacing = 10;
    int totalWidth = buttonSize * 5 + spacing * 4;
    int startX = (getWidth() - totalWidth) / 2;

    prevButton.setBounds(startX, controlArea.getY(), buttonSize, buttonSize);
    playButton.setBounds(startX + buttonSize + spacing, controlArea.getY(), buttonSize, buttonSize);
    pauseButton.setBounds(startX + buttonSize + spacing, controlArea.getY(), buttonSize, buttonSize);
    stopButton.setBounds(startX + (buttonSize + spacing) * 2, controlArea.getY(), buttonSize, buttonSize);
    nextButton.setBounds(startX + (buttonSize + spacing) * 3, controlArea.getY(), buttonSize, buttonSize);
    loopButton.setBounds(startX + (buttonSize + spacing) * 4, controlArea.getY(), buttonSize, buttonSize);

    area.removeFromTop(20);
    auto volumeArea = area.removeFromTop(60);
    volumeLabel.setBounds(volumeArea.removeFromTop(25));
    volumeSlider.setBounds(volumeArea.reduced(50, 5));
    area.removeFromTop(20);
    auto bottomArea = area.removeFromTop(50);
    int bottomButtonWidth = 120;
    int bottomSpacing = (getWidth() - bottomButtonWidth * 2) / 3;
    loadButton.setBounds(bottomSpacing, bottomArea.getY(), bottomButtonWidth, 40);
    muteButton.setBounds(bottomSpacing * 2 + bottomButtonWidth, bottomArea.getY(), bottomButtonWidth, 40);
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File{},
            "*.wav;*.mp3;*.flac;*.ogg;*.aiff");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    if (playerAudio.loadFile(file))
                    {
                        currentFileName = file.getFileNameWithoutExtension();
                        titleLabel.setText(currentFileName, juce::dontSendNotification);
                        playerAudio.start();
                        playButton.setVisible(false);
                        pauseButton.setVisible(true);
                    }
                }
            });
    }
    else if (button == &playButton)
    {
        playerAudio.start();
        playButton.setVisible(false);
        pauseButton.setVisible(true);
    }
    else if (button == &pauseButton)
    {
        playerAudio.pause();
        pauseButton.setVisible(false);
        playButton.setVisible(true);
    }
    else if (button == &stopButton)
    {
        playerAudio.stop();
        playerAudio.setPosition(0.0);
        pauseButton.setVisible(false);
        playButton.setVisible(true);
        updateTimeDisplay();
    }
    else if (button == &prevButton)
    {
        playerAudio.setPosition(0.0);
    }
    else if (button == &nextButton)
    {
        double length = playerAudio.getLength();
        if (length > 0.0)
        {
            playerAudio.setPosition(length - 0.1);
        }
    }
    else if (button == &muteButton)
    {
        isMuted = !isMuted;

        if (isMuted)
        {
            previousVolume = volumeSlider.getValue();
            volumeSlider.setValue(0.0);
            playerAudio.setGain(0.0f);
            muteButton.setButtonText("UNMUTE");
            volumeLabel.setText("Volume: MUTED", juce::dontSendNotification);
            styleButton(muteButton, juce::Colours::red);
        }
        else
        {
            volumeSlider.setValue(previousVolume);
            playerAudio.setGain(previousVolume);
            muteButton.setButtonText("MUTE");
            int volumePercent = (int)(previousVolume * 100);
            volumeLabel.setText("Volume: " + juce::String(volumePercent) + "%",
                juce::dontSendNotification);
            styleButton(muteButton, juce::Colours::grey);
        }
    }
    else if (button == &loopButton)
    {
        isLooping = !isLooping;
        playerAudio.setLooping(isLooping);

        if (isLooping)
        {
            styleButton(loopButton, juce::Colours::green);
            loopButton.setButtonText("LOOP: ON");
        }
        else
        {
            styleButton(loopButton, juce::Colours::grey);
            loopButton.setButtonText("LOOP");
        }
    }
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        float volume = static_cast<float>(slider->getValue());
        playerAudio.setGain(volume);
        int volumePercent = (int)(volume * 100);
        volumeLabel.setText("Volume: " + juce::String(volumePercent) + "%",
            juce::dontSendNotification);
        if (volume > 0.0f && isMuted)
        {
            isMuted = false;
            muteButton.setButtonText("MUTE");
            styleButton(muteButton, juce::Colours::grey);
        }
    }
}