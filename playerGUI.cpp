#include "PlayerGUI.h"

PlayerGUI::PlayerGUI(const juce::String& name)
    : playerName(name),
    playlistModel(*this)
{
    playerNameLabel.setText(playerName, juce::dontSendNotification);
    playerNameLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    playerNameLabel.setColour(juce::Label::textColourId, juce::Colour(0xff00e5ff));
    playerNameLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(playerNameLabel);

    std::vector<juce::TextButton*> allButtons = {
        &loadButton, &playButton, &pauseButton, &stopButton,
        &muteButton, &loopButton, &toStartButton, &toEndButton,
        &skipBackButton, &skipForwardButton,
        &loadPlaylistButton
    };

    for (auto* btn : allButtons)
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    styleModernButton(loadButton, juce::Colour(0xff9c27b0));
    styleModernButton(playButton, juce::Colour(0xff00e676));
    styleModernButton(pauseButton, juce::Colour(0xffffab00));
    styleModernButton(stopButton, juce::Colour(0xffff1744));
    styleModernButton(muteButton, juce::Colour(0xff607d8b));
    styleModernButton(loopButton, juce::Colour(0xff607d8b)); 
    styleModernButton(toStartButton, juce::Colour(0xff00b0ff));
    styleModernButton(toEndButton, juce::Colour(0xff00b0ff));
    styleModernButton(skipBackButton, juce::Colour(0xff00bcd4));
    styleModernButton(skipForwardButton, juce::Colour(0xff00bcd4));
    styleModernButton(loadPlaylistButton, juce::Colour(0xff7c4dff));

    pauseButton.setVisible(false);

    volumeSlider.setRange(0.0, 1.0, 0.001);
    volumeSlider.setValue(0.5);
    styleModernSlider(volumeSlider, juce::Colour(0xff00e5ff));
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    auto setupLabel = [this](juce::Label& label, const juce::String& text,
        float fontSize, juce::Colour color, int style = juce::Font::plain)
        {
            label.setText(text, juce::dontSendNotification);
            label.setFont(juce::Font(fontSize, style));
            label.setColour(juce::Label::textColourId, color);
            label.setJustificationType(juce::Justification::centred);
            addAndMakeVisible(label);
        };

    setupLabel(titleLabel, "No Track Loaded", 18.0f, juce::Colours::white, juce::Font::bold);
    setupLabel(artistLabel, "", 14.0f, juce::Colour(0xffbdbdbd));
    setupLabel(albumLabel, "", 13.0f, juce::Colour(0xff9e9e9e));
    setupLabel(bitrateLabel, "", 11.0f, juce::Colour(0xff757575));
    setupLabel(volumeLabel, "Volume: 50%", 12.0f, juce::Colours::white);
    playlistBox.setModel(&playlistModel);
    playlistBox.setColour(juce::ListBox::backgroundColourId, juce::Colour(0xff1a1a2e));
    playlistBox.setColour(juce::ListBox::outlineColourId, juce::Colour(0xff00e5ff));
    playlistBox.setOutlineThickness(2);
    playlistBox.setRowHeight(24);
    addAndMakeVisible(playlistBox);

    startTimer(50); 
}

PlayerGUI::~PlayerGUI()
{
    stopTimer();
    fileChooser.reset();
}

void PlayerGUI::styleModernButton(juce::TextButton& button, juce::Colour color)
{
    button.setColour(juce::TextButton::buttonColourId, color.withAlpha(0.8f));
    button.setColour(juce::TextButton::buttonOnColourId, color);
    button.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    button.setColour(juce::TextButton::textColourOnId, juce::Colours::black);
}

void PlayerGUI::styleModernSlider(juce::Slider& slider, juce::Colour color)
{
    slider.setSliderStyle(juce::Slider::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider.setColour(juce::Slider::trackColourId, color);
    slider.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    slider.setColour(juce::Slider::backgroundColourId, color.darker(0.8f));
}


void PlayerGUI::updateMetadataDisplay()
{
    titleLabel.setText(playerAudio.getTitle(), juce::dontSendNotification);
    artistLabel.setText("Artist: " + playerAudio.getArtist(), juce::dontSendNotification);
    albumLabel.setText("Album: " + playerAudio.getAlbum(), juce::dontSendNotification);

    juce::String bitrateText = playerAudio.getBitrate();
    if (bitrateText.isNotEmpty())
        bitrateLabel.setText("Bitrate: " + bitrateText, juce::dontSendNotification);
    else
        bitrateLabel.setText("", juce::dontSendNotification); 

}

void PlayerGUI::timerCallback()
{
    if (isFadingOut)
    {
        processFadeOut();
    }
    repaint();
}

void PlayerGUI::startFadeOut()
{
    isFadingOut = true;
    fadeOutGain = 1.0f;
}

void PlayerGUI::processFadeOut()
{
    fadeOutGain -= 0.05f; 

    if (fadeOutGain <= 0.0f)
    {
        fadeOutGain = 0.0f;
        isFadingOut = false;
        playerAudio.stop();
        playerAudio.setGain(static_cast<float>(volumeSlider.getValue()));
    }
    else
    {
        playerAudio.setGain(fadeOutGain * static_cast<float>(volumeSlider.getValue()));
    }
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

void PlayerGUI::startPlayback()
{
    if (playerAudio.getLength() <= 0.0)
    {
        return;
    }

    fadeOutGain = 1.0f;
    isFadingOut = false;
    playerAudio.start();
    playButton.setVisible(false);
    pauseButton.setVisible(true);
}

void PlayerGUI::stopPlayback()
{
    startFadeOut();
    playButton.setVisible(true);
    pauseButton.setVisible(false);
}

void PlayerGUI::pausePlayback()
{
    playerAudio.pause();
}

void PlayerGUI::setPlaybackPosition(double position)
{
    playerAudio.setPosition(position);
}

double PlayerGUI::getPlaybackPosition() const
{
    return playerAudio.getPosition();
}


void PlayerGUI::loadMultipleFiles()
{
    fileChooser = std::make_unique<juce::FileChooser>(
        "Select audio files to add to playlist...",
        juce::File{},
        "*.wav;*.mp3;*.flac;*.ogg;*.aiff;*.m4a"
    );

    fileChooser->launchAsync(
        juce::FileBrowserComponent::openMode |
        juce::FileBrowserComponent::canSelectFiles |
        juce::FileBrowserComponent::canSelectMultipleItems,
        [this](const juce::FileChooser& fc)
        {
            auto files = fc.getResults();

            for (const auto& file : files)
            {
                if (file.existsAsFile())
                {
                    playlistFiles.push_back(file);
                    playlist.add(file.getFileNameWithoutExtension());
                }
            }

            playlistBox.updateContent();
            playlistBox.repaint();

            juce::AlertWindow::showMessageBoxAsync(
                juce::AlertWindow::InfoIcon,
                "Playlist Updated",
                "Files added to playlist successfully!"
            );
        });
}


void PlayerGUI::playSelectedFile(int index)
{
    if (index >= 0 && index < static_cast<int>(playlistFiles.size()))
    {
        currentPlaylistIndex = index;
        currentLoadedFile = playlistFiles[index];

        if (playerAudio.loadFile(playlistFiles[index]))
        {
            updateMetadataDisplay();
            startPlayback();
            playlistBox.repaint();
        }
    }
}


void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...", juce::File{}, "*.wav;*.mp3;*.flac;*.ogg;*.aiff;*.m4a");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    if (playerAudio.loadFile(file))
                    {
                        currentLoadedFile = file;
                        juce::MessageManager::callAsync([this]()
                            {
                                updateMetadataDisplay();
                                startPlayback();
                            });
                    }
                }
            });
    }
    else if (button == &loadPlaylistButton)
    {
        loadMultipleFiles();
    }
    else if (button == &playButton)
    {
        startPlayback();
    }
    else if (button == &pauseButton)
    {
        pausePlayback();
        pauseButton.setVisible(false);
        playButton.setVisible(true);
    }
    else if (button == &stopButton)
    {
        stopPlayback();
    }
    else if (button == &toStartButton)
    {
        playerAudio.setPosition(0.0); 
    }
    else if (button == &toEndButton)
    {
        double length = playerAudio.getLength(); 
        if (length > 0.0)
            playerAudio.setPosition(length - 0.1);
    }
    else if (button == &skipBackButton)
    {
        double current = playerAudio.getPosition(); 
        playerAudio.setPosition(juce::jmax(0.0, current - 10.0));
    }
    else if (button == &skipForwardButton)
    {
        double current = playerAudio.getPosition(); 
        double length = playerAudio.getLength();
        playerAudio.setPosition(juce::jmin(length, current + 10.0));
    }
    else if (button == &muteButton)
    {
        isMuted = !isMuted;
        if (isMuted)
        {
            previousVolume = static_cast<float>(volumeSlider.getValue());
            volumeSlider.setValue(0.0);
            muteButton.setButtonText("UNMUTE");
            styleModernButton(muteButton, juce::Colour(0xffff1744));
        }
        else
        {
            volumeSlider.setValue(previousVolume);
            muteButton.setButtonText("MUTE");
            styleModernButton(muteButton, juce::Colour(0xff607d8b));
        }
    }
    else if (button == &loopButton) 
    {
        isLooping = !isLooping;
        playerAudio.setLooping(isLooping);
        if (isLooping)
        {
            loopButton.setButtonText("LOOP: ON");
            styleModernButton(loopButton, juce::Colour(0xff00e676));
        }
        else
        {
            loopButton.setButtonText("LOOP");
            styleModernButton(loopButton, juce::Colour(0xff607d8b));
        }
    }

}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        float volume = static_cast<float>(slider->getValue());
        if (!isFadingOut)
        {
            playerAudio.setGain(volume); 
        }
        int volumePercent = static_cast<int>(volume * 100.0f);
        volumeLabel.setText("Volume: " + juce::String(volumePercent) + "%",
            juce::dontSendNotification);
    }

}

void PlayerGUI::paint(juce::Graphics& g)
{
    juce::ColourGradient gradient(
        juce::Colour(0xff0d0d15), 0.0f, 0.0f,
        juce::Colour(0xff1a1a2e), static_cast<float>(getWidth()), static_cast<float>(getHeight()),
        false
    );
    g.setGradientFill(gradient);
    g.fillAll();

    g.setColour(juce::Colour(0xff00e5ff).withAlpha(0.3f));
    g.drawRect(getLocalBounds(), 2);

}

void PlayerGUI::resized()
{
    auto area = getLocalBounds().reduced(10);

    playerNameLabel.setBounds(area.removeFromTop(30));

    titleLabel.setBounds(area.removeFromTop(24));
    artistLabel.setBounds(area.removeFromTop(18));
    albumLabel.setBounds(area.removeFromTop(16));
    bitrateLabel.setBounds(area.removeFromTop(14));

    area.removeFromTop(10); 


    auto controlArea = area.removeFromTop(45);
    int buttonWidth = 50;
    int spacing = 6;
    int totalWidth = buttonWidth * 8 + spacing * 7; 
    int startX = (getWidth() - totalWidth) / 2;

    toStartButton.setBounds(startX, controlArea.getY(), buttonWidth, 40);
    playButton.setBounds(startX + (buttonWidth + spacing), controlArea.getY(), buttonWidth, 40);
    pauseButton.setBounds(startX + (buttonWidth + spacing), controlArea.getY(), buttonWidth, 40);
    stopButton.setBounds(startX + (buttonWidth + spacing) * 2, controlArea.getY(), buttonWidth, 40);
    toEndButton.setBounds(startX + (buttonWidth + spacing) * 3, controlArea.getY(), buttonWidth, 40);
    muteButton.setBounds(startX + (buttonWidth + spacing) * 4, controlArea.getY(), buttonWidth, 40);
    loopButton.setBounds(startX + (buttonWidth + spacing) * 5, controlArea.getY(), buttonWidth, 40); // Task 4
    loadButton.setBounds(startX + (buttonWidth + spacing) * 6, controlArea.getY(), buttonWidth * 2 + spacing, 40);

    area.removeFromTop(8);

    auto skipArea = area.removeFromTop(30);
    int skipWidth = 60;
    int skipSpacing = (getWidth() - skipWidth * 2) / 3;
    skipBackButton.setBounds(skipSpacing, skipArea.getY(), skipWidth, 26);
    skipForwardButton.setBounds(skipSpacing * 2 + skipWidth, skipArea.getY(), skipWidth, 26);

    area.removeFromTop(8);

    volumeLabel.setBounds(area.removeFromTop(18));
    volumeSlider.setBounds(area.removeFromTop(22).reduced(35, 0));

    area.removeFromTop(5);


    
    area.removeFromTop(15); 


    loadPlaylistButton.setBounds(area.removeFromTop(26));
    area.removeFromTop(3);
    playlistBox.setBounds(area);

}


void PlayerGUI::PlaylistModel::paintListBoxItem(int row, juce::Graphics& g, int width, int height, bool selected)
{
    if (selected || row == gui.currentPlaylistIndex)
    {
        g.fillAll(juce::Colour(0xff00e5ff).withAlpha(0.3f));
    }
    else
    {
        g.fillAll(juce::Colour(0xff1a1a2e));
    }

    g.setColour(juce::Colours::white);
    g.setFont(12.0f);

    juce::String prefix = (row == gui.currentPlaylistIndex) ? "> " : "  ";
    g.drawText(prefix + gui.playlist[row], 5, 0, width - 10, height,
        juce::Justification::centredLeft, true);
}
