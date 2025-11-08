#include "PlayerGUI.h"

namespace Icons
{
    static std::unique_ptr<juce::Drawable> createFromSVG(const juce::String& svgData)
    {
        std::unique_ptr<juce::XmlElement> xml = juce::XmlDocument::parse(svgData);
        if (xml != nullptr)
            return juce::Drawable::createFromSVG(*xml);
        return std::make_unique<juce::DrawableComposite>();
    }
    static juce::String getPlayIcon()
    {
        return R"svg(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M8 5v14l11-7z"/></svg>)svg";
    }
    static juce::String getPauseIcon()
    {
        return R"svg(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M6 19h4V5H6v14zm8-14v14h4V5h-4z"/></svg>)svg";
    }
    static juce::String getStopIcon()
    {
        return R"svg(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M6 6h12v12H6z"/></svg>)svg";
    }
    static juce::String getLoadIcon()
    {
        return R"svg(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M9 16h6v-6h4l-8-8-8 8h4v6zm-4 2h14v2H5v-2z"/></svg>)svg";
    }
    static juce::String getMuteIcon()
    {
        return R"svg(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M16.5 12c0-1.77-1.02-3.29-2.5-4.03v2.21l2.45 2.45c.03-.2.05-.41.05-.63zm2.5 0c0 .94-.2 1.82-.54 2.64l1.51 1.51C20.63 14.91 21 13.5 21 12c0-4.28-2.99-7.86-7-8.77v2.06c2.89.86 5 3.54 5 6.71zM4.27 3L3 4.27 7.73 9H3v6h4l5 5v-6.73l4.25 4.25c-.67.52-1.42.93-2.25 1.18v2.06c1.38-.31 2.63-.95 3.69-1.81L19.73 21 21 19.73l-9-9L4.27 3zM12 4L9.91 6.09 12 8.18V4z"/></svg>)svg";
    }
    static juce::String getUnmuteIcon()
    {
        return R"svg(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M3 9v6h4l5 5V4L7 9H3zm13.5 3c0-1.77-1.02-3.29-2.5-4.03v8.05c1.48-.73 2.5-2.25 2.5-4.02zM14 3.23v2.06c2.89.86 5 3.54 5 6.71s-2.11 5.85-5 6.71v2.06c4.01-.91 7-4.49 7-8.77s-2.99-7.86-7-8.77z"/></svg>)svg";
    }
    static juce::String getLoopOnIcon()
    {
        return R"svg(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M7 7h10v3l4-4-4-4v3H5v6h2V7zm10 10H7v-3l-4 4 4 4v-3h12v-6h-2v4z"/></svg>)svg";
    }
    static juce::String getLoopOffIcon()
    {
        return R"svg(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M7 7h10v3l4-4-4-4v3H5v6h2V7zm10 10H7v-3l-4 4 4 4v-3h12v-6h-2v4z"/></svg>)svg";
    }
    static juce::String getToStartIcon()
    {
        return R"svg(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M18 16l-6-6-6 6h12zM6 5h12v2H6z" transform="rotate(270 12 12)"/></svg>)svg";
    }
    static juce::String getToEndIcon()
    {
        return R"svg(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M6 8l6 6 6-6H6zM18 19H6v-2h12v2z" transform="rotate(270 12 12)"/></svg>)svg";
    }
    static juce::String getSkipBackIcon()
    {
        return R"svg(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M11.9 12c.5 0 .9-.4.9-.9V6.8c0-.4-.3-.8-.8-.8s-.8.3-.8.8v4.3c0 .5.4.9.9.9zm4.2 1.8c.2.1.5.1.7-.1l3.5-2.1c.2-.1.4-.4.4-.7s-.1-.5-.4-.7l-3.5-2.1c-.2-.1-.5-.1-.7.1-.2.1-.3.4-.3.6v4.2c0 .3.1.5.3.6zm-8.2-1.7c.2.1.5.1.7-.1l3.5-2.1c.2-.1.4-.4.4-.7s-.1-.5-.4-.7l-3.5-2.1c-.2-.1-.5-.1-.7.1-.2.1-.3.4-.3.6v4.2c0 .3.1.5.3.6zM12 19c-3.9 0-7-3.1-7-7s3.1-7 7-7 7 3.1 7 7-3.1 7-7 7zm0-12c-2.8 0-5 2.2-5 5s2.2 5 5 5 5-2.2 5-5-2.2-5-5-5z"/></svg>)svg";
    }
    static juce::String getSkipForwardIcon()
    {
        return R"svg(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M12.1 12c-.5 0-.9-.4-.9-.9V6.8c0-.4.3-.8.8-.8s.8.3.8.8v4.3c0 .5-.4.9-.9.9zM7.9 13.8c-.2.1-.5.1-.7-.1l-3.5-2.1c-.2-.1-.4-.4-.4-.7s.1-.5.4-.7l3.5-2.1c.2-.1.5-.1.7.1.2.1.3.4.3.6v4.2c0 .3-.1.5-.3.6zm8.2-1.7c-.2.1-.5.1-.7-.1l-3.5-2.1c-.2-.1-.4-.4-.4-.7s.1-.5.4-.7l3.5-2.1c.2-.1.5-.1.7.1.2.1.3.4.3.6v4.2c0 .3-.1.5-.3.6zM12 19c-3.9 0-7-3.1-7-7s3.1-7 7-7 7 3.1 7 7-3.1 7-7 7zm0-12c-2.8 0-5 2.2-5 5s2.2 5 5 5 5-2.2 5-5-2.2-5-5-5z"/></svg>)svg";
    }
    static juce::String getSetAIcon()
    {
        return R"svg(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M17.66 17.34l-2.43-4.21 2.43-4.21h-2.9l-2.43 4.21 2.43 4.21h2.9m.64-5.52l.9 1.56-.9 1.56h.9l.9-1.56-.9-1.56h-.9m-3.23-1.56l-.9-1.56h-1.8l-.9 1.56h3.6m-3.23 7.08l.9 1.56h1.8l.9-1.56h-3.6M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm0 18c-4.41 0-8-3.59-8-8s3.59-8 8-8 8 3.59 8 8-3.59 8-8 8z"/></svg>)svg";
    }
    static juce::String getSetBIcon()
    {
        return R"svg(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M15.14 14.86c.78-.73 1.29-1.74 1.29-2.86 0-1.12-.5-2.13-1.29-2.86-.78-.73-1.8-1.14-2.91-1.14H9v8h3.23c1.11 0 2.13-.41 2.91-1.14M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm-1.23 6h1.23c1.3 0 2.36.94 2.36 2.14 0 .6-.25 1.13-.67 1.49-.42.36-.97.57-1.58.57h-1.34V8zm0 6h1.45c1.28 0 2.3-.9 2.3-2.02 0-.6-.25-1.12-.67-1.48-.42-.36-.98-.58-1.63-.58H10.77v4.08z"/></svg>)svg";
    }
    static juce::String getClearABIcon()
    {
        return R"svg(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M12 2C6.47 2 2 6.47 2 12s4.47 10 10 10 10-4.47 10-10S17.53 2 12 2zm5 13.59L15.59 17 12 13.41 8.41 17 7 15.59 10.59 12 7 8.41 8.41 7 12 10.59 15.59 7 17 8.41 13.41 12 17 15.59z"/></svg>)svg";
    }

    static juce::String getAddMarkerIcon()
    {
        return R"svg(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M12 2C8.14 2 5 5.14 5 9c0 5.25 7 13 7 13s7-7.75 7-13c0-3.86-3.14-7-7-7zm0 2c1.1 0 2 .9 2 2s-.9 2-2 2-2-.9-2-2 .9-2 2-2zM11 10h2v3h3v2h-3v3h-2v-3H8v-2h3v-3z"/></svg>)svg";
    }
    static juce::String getTrashIcon()
    {
        return R"svg(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M6 19c0 1.1.9 2 2 2h8c1.1 0 2-.9 2-2V7H6v12zM19 4h-3.5l-1-1h-5l-1 1H5v2h14V4z"/></svg>)svg";
    }
    static juce::String getDeleteIcon()
    {
        return R"svg(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M19 6.41L17.59 5 12 10.59 6.41 5 5 6.41 10.59 12 5 17.59 6.41 19 12 13.41 17.59 19 19 17.59 13.41 12z"/></svg>)svg";
    }
}


PlayerGUI::PlayerGUI(const juce::String& name)
    : playerName(name),
    playlistModel(*this),
    markerModel(*this)
{
    waveformArea = juce::Rectangle<int>(0, 0, 100, 60);

    playerNameLabel.setText(playerName, juce::dontSendNotification);
    playerNameLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    playerNameLabel.setColour(juce::Label::textColourId, juce::Colour(0xff00e5ff));
    playerNameLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(playerNameLabel);

    addAndMakeVisible(albumArtComponent);
    albumArtComponent.setInterceptsMouseClicks(false, false);

    std::vector<juce::DrawableButton*> drawableButtons = {
        &loadButton, &playButton, &pauseButton, &stopButton,
        &muteButton, &loopButton, &toStartButton, &toEndButton,

        &addMarkerButton, &clearMarkersButton,
        &loadPlaylistButton, &clearPlaylistButton
    };
    for (auto* btn : drawableButtons)
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    std::vector<juce::TextButton*> textButtons = {
        &skipBackButton, &skipForwardButton,
        &abToggleButton, &clearABButton
    };
    for (auto* btn : textButtons)
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    styleModernIcon(loadButton, juce::Colour(0xff9c27b0), Icons::getLoadIcon());
    styleModernIcon(playButton, juce::Colour(0xff00e676), Icons::getPlayIcon());
    styleModernIcon(pauseButton, juce::Colour(0xffffab00), Icons::getPauseIcon());
    styleModernIcon(stopButton, juce::Colour(0xffff1744), Icons::getStopIcon());
    styleModernIcon(muteButton, juce::Colour(0xff607d8b), Icons::getMuteIcon());
    styleModernIcon(loopButton, juce::Colour(0xff607d8b), Icons::getLoopOffIcon());
    styleModernIcon(toStartButton, juce::Colour(0xff00b0ff), Icons::getToStartIcon());
    styleModernIcon(toEndButton, juce::Colour(0xff00b0ff), Icons::getToEndIcon());
    styleModernIcon(addMarkerButton, juce::Colour(0xffffd600), Icons::getAddMarkerIcon());
    styleModernIcon(clearMarkersButton, juce::Colour(0xff546e7a), Icons::getTrashIcon());
    styleModernIcon(loadPlaylistButton, juce::Colour(0xff7c4dff), Icons::getLoadIcon());
    styleModernIcon(clearPlaylistButton, juce::Colour(0xff546e7a), Icons::getTrashIcon());

    styleModernButton(skipBackButton, juce::Colour(0xff00bcd4));
    styleModernButton(skipForwardButton, juce::Colour(0xff00bcd4));
    styleModernButton(abToggleButton, juce::Colour(0xff64dd17));
    styleModernButton(clearABButton, juce::Colour(0xffff3d00));


    pauseButton.setVisible(false);

    volumeSlider.setRange(0.0, 1.0, 0.001);
    volumeSlider.setValue(0.5);
    styleModernSlider(volumeSlider, juce::Colour(0xff00e5ff));
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    speedSlider.setRange(0.25, 2.0, 0.01);
    speedSlider.setValue(1.0);
    styleModernSlider(speedSlider, juce::Colour(0xffff9800));
    speedSlider.addListener(this);
    addAndMakeVisible(speedSlider);

    positionSlider.setRange(0.0, 1.0, 0.0001);
    positionSlider.setValue(0.0);
    styleModernSlider(positionSlider, juce::Colour(0xff4caf50));
    positionSlider.addListener(this);
    addAndMakeVisible(positionSlider);

    positionSlider.onDragStart = [this] { isDraggingPosition = true; };
    positionSlider.onDragEnd = [this] {
        isDraggingPosition = false;
        double position = positionSlider.getValue() * playerAudio.getLength();
        playerAudio.setPosition(position);
        };

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
    setupLabel(timeLabel, "00:00 / 00:00", 15.0f, juce::Colour(0xff00e5ff), juce::Font::bold);
    setupLabel(volumeLabel, "Volume: 50%", 12.0f, juce::Colours::white);
    setupLabel(speedLabel, "Speed: 1.00x", 12.0f, juce::Colours::white);
    setupLabel(abLoopLabel, "A-B Loop: Off", 11.0f, juce::Colour(0xffffeb3b));
    titleLabel.setJustificationType(juce::Justification::centredLeft);
    artistLabel.setJustificationType(juce::Justification::centredLeft);
    albumLabel.setJustificationType(juce::Justification::centredLeft);
    bitrateLabel.setJustificationType(juce::Justification::centredLeft);

    playlistBox.setModel(&playlistModel);
    playlistBox.setColour(juce::ListBox::backgroundColourId, juce::Colour(0xff1a1a2e));
    playlistBox.setColour(juce::ListBox::outlineColourId, juce::Colour(0xff00e5ff));
    playlistBox.setOutlineThickness(2);
    playlistBox.setRowHeight(24);
    addAndMakeVisible(playlistBox);

    markerListBox.setModel(&markerModel);
    markerListBox.setColour(juce::ListBox::backgroundColourId, juce::Colour(0xff1a1a2e));
    markerListBox.setColour(juce::ListBox::outlineColourId, juce::Colour(0xffffd600));
    markerListBox.setOutlineThickness(2);
    markerListBox.setRowHeight(24);
    addAndMakeVisible(markerListBox);

    loadState();
    startTimer(50);
}

PlayerGUI::~PlayerGUI()
{
    stopTimer();
    fileChooser.reset();
}

void PlayerGUI::styleModernIcon(juce::DrawableButton& button, juce::Colour color, const juce::String& iconSVG)
{
    auto drawable = Icons::createFromSVG(iconSVG);
    drawable->replaceColour(juce::Colours::black, color);
    auto drawableOn = Icons::createFromSVG(iconSVG);
    drawableOn->replaceColour(juce::Colours::black, juce::Colours::black);

    button.setImages(drawable.get(), nullptr, nullptr, nullptr, drawableOn.get());
    button.setColour(juce::DrawableButton::backgroundColourId, juce::Colours::transparentBlack.darker(0.2f));
    button.setColour(juce::DrawableButton::backgroundOnColourId, color.withAlpha(0.6f));
    button.setClickingTogglesState(false);
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

juce::String PlayerGUI::formatTime(double seconds)
{
    int mins = static_cast<int>(seconds) / 60;
    int secs = static_cast<int>(seconds) % 60;
    return juce::String::formatted("%02d:%02d", mins, secs);
}

void PlayerGUI::updateTimeDisplay()
{
    double current = playerAudio.getPosition();
    double total = playerAudio.getLength();
    timeLabel.setText(formatTime(current) + " / " + formatTime(total), juce::dontSendNotification);
}

void PlayerGUI::updatePositionSlider()
{
    if (!isDraggingPosition)
    {
        double current = playerAudio.getPosition();
        double total = playerAudio.getLength();
        if (total > 0.0)
        {
            positionSlider.setValue(current / total, juce::dontSendNotification);
        }
    }
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
    auto art = playerAudio.getAlbumArt();
    if (art.isValid())
    {
        albumArtComponent.setImage(art);
    }
    else
    {
        albumArtComponent.setImage(juce::Image{});
    }
}

void PlayerGUI::drawWaveform(juce::Graphics& g)
{
    if (waveformArea.isEmpty()) return;
    g.setColour(juce::Colour(0xff0d0d15));
    g.fillRoundedRectangle(waveformArea.toFloat(), 4.0f);
    g.setColour(juce::Colour(0xff00e5ff).withAlpha(0.8f));
    double total = thumbnail.getTotalLength();
    if (total > 0.0)
    {
        thumbnail.drawChannels(g, waveformArea, 0.0, total, 1.0f);
    }
    double progress = 0.0;
    double totalLength = playerAudio.getLength();
    if (totalLength > 0.0)
        progress = playerAudio.getPosition() / totalLength;
    float playheadX = waveformArea.getX() + waveformArea.getWidth() * static_cast<float>(progress);
    g.setColour(juce::Colours::white);
    g.drawLine(playheadX, static_cast<float>(waveformArea.getY()),
        playheadX, static_cast<float>(waveformArea.getBottom()), 2.0f);
    if (pointA >= 0.0 && totalLength > 0.0)
    {
        float markerX = waveformArea.getX() + waveformArea.getWidth() * static_cast<float>(pointA / totalLength);
        g.setColour(juce::Colour(0xff64dd17));
        g.fillRect(markerX - 1.0f, static_cast<float>(waveformArea.getY()), 3.0f, static_cast<float>(waveformArea.getHeight()));
        g.drawText("A", static_cast<int>(markerX - 8), waveformArea.getY() - 18, 16, 16, juce::Justification::centred);
    }
    if (pointB >= 0.0 && totalLength > 0.0)
    {
        float markerX = waveformArea.getX() + waveformArea.getWidth() * static_cast<float>(pointB / totalLength);
        g.setColour(juce::Colour(0xff2979ff));
        g.fillRect(markerX - 1.0f, static_cast<float>(waveformArea.getY()), 3.0f, static_cast<float>(waveformArea.getHeight()));
        g.drawText("B", static_cast<int>(markerX - 8), waveformArea.getY() - 18, 16, 16, juce::Justification::centred);
    }
    for (const auto& marker : markers)
    {
        if (totalLength > 0.0)
        {
            float markerX = waveformArea.getX() + waveformArea.getWidth() * static_cast<float>(marker.position / totalLength);
            g.setColour(juce::Colour(0xffffd600));
            g.fillEllipse(markerX - 3.0f, static_cast<float>(waveformArea.getY()) - 5.0f, 6.0f, 6.0f);
        }
    }
}

void PlayerGUI::timerCallback()
{
    updateTimeDisplay();
    updatePositionSlider();
    if (isFadingOut)
    {
        processFadeOut();
    }
    if (thumbnail.isFullyLoaded())
    {
        repaint(waveformArea);
    }
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
        playerAudio.setGain(static_cast<float>(volumeSlider.getValue()) * mixerGain);
    }
    else
    {
        playerAudio.setGain(fadeOutGain * static_cast<float>(volumeSlider.getValue()) * mixerGain);
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

void PlayerGUI::setMixerGain(float gain)
{
    mixerGain = gain;
    if (!isFadingOut)
    {
        playerAudio.setGain(static_cast<float>(volumeSlider.getValue()) * mixerGain);
    }
}

void PlayerGUI::swapTrackWith(PlayerGUI* other)
{
    if (!other) return;
    auto tempFile = playerAudio.getLoadedFile();
    loadFile(other->playerAudio.getLoadedFile(), false);
    other->loadFile(tempFile, false);
}

void PlayerGUI::loadFile(const juce::File& file, bool autoPlay)
{
    if (playerAudio.loadFile(file))
    {
        thumbnail.setSource(new juce::FileInputSource(file));
        juce::MessageManager::callAsync([this, autoPlay]()
            {
                updateMetadataDisplay();
                if (autoPlay)
                    startPlayback();
            });
    }
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
        });
}

void PlayerGUI::playSelectedFile(int index)
{
    if (index < 0 || index >= static_cast<int>(playlistFiles.size()))
        return;

    currentPlaylistIndex = index;
    loadFile(playlistFiles[index], true);
    playlistBox.repaint();
}

void PlayerGUI::deletePlaylistItem(int index)
{
    if (index < 0 || index >= static_cast<int>(playlistFiles.size()))
        return;
    playlist.removeString(playlist[index]);
    playlistFiles.erase(playlistFiles.begin() + index);
    if (index == currentPlaylistIndex)
        currentPlaylistIndex = -1;
    else if (index < currentPlaylistIndex)
        currentPlaylistIndex--;
    playlistBox.updateContent();
}

void PlayerGUI::addMarkerAtCurrentPosition()
{
    double pos = playerAudio.getPosition();
    int markerNum = static_cast<int>(markers.size()) + 1;
    juce::String name = "Marker " + juce::String(markerNum);
    markers.emplace_back(pos, name);
    std::sort(markers.begin(), markers.end(), [](const Marker& a, const Marker& b) {
        return a.position < b.position;
        });
    markerListBox.updateContent();
}

void PlayerGUI::jumpToMarker(int index)
{
    if (index >= 0 && index < static_cast<int>(markers.size()))
    {
        playerAudio.setPosition(markers[index].position);
    }
}

void PlayerGUI::deleteMarkerItem(int index)
{
    if (index < 0 || index >= static_cast<int>(markers.size()))
        return;
    markers.erase(markers.begin() + index);
    markerListBox.updateContent();
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
                    loadFile(file, true);
                }
            });
    }
    else if (button == &loadPlaylistButton)
    {
        loadMultipleFiles();
    }
    else if (button == &clearPlaylistButton)
    {
        playlist.clear();
        playlistFiles.clear();
        currentPlaylistIndex = -1;
        playlistBox.updateContent();
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
        if (playerAudio.getPosition() > 1.0)
        {
            playerAudio.setPosition(0.0);
        }
        else
        {
            playSelectedFile(currentPlaylistIndex - 1);
        }
    }
    else if (button == &toEndButton)
    {
        playSelectedFile(currentPlaylistIndex + 1);
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
            styleModernIcon(muteButton, juce::Colour(0xffff1744), Icons::getUnmuteIcon());
        }
        else
        {
            volumeSlider.setValue(previousVolume);
            styleModernIcon(muteButton, juce::Colour(0xff607d8b), Icons::getMuteIcon());
        }
    }
    else if (button == &loopButton)
    {
        isLooping = !isLooping;
        playerAudio.setLooping(isLooping);
        if (isLooping)
        {
            styleModernIcon(loopButton, juce::Colour(0xff00e676), Icons::getLoopOnIcon());
        }
        else
        {
            styleModernIcon(loopButton, juce::Colour(0xff607d8b), Icons::getLoopOffIcon());
        }
    }
    else if (button == &abToggleButton)
    {
        if (abState == ABState::A)
        {
            pointA = playerAudio.getPosition();
            abLoopLabel.setText("A: " + formatTime(pointA), juce::dontSendNotification);
            abToggleButton.setButtonText("B"); 
            styleModernButton(abToggleButton, juce::Colour(0xff2979ff)); 
            abState = ABState::B;
        }
        else
        {
            pointB = playerAudio.getPosition();
            abToggleButton.setButtonText("A"); 
            styleModernButton(abToggleButton, juce::Colour(0xff64dd17));
            abState = ABState::A;
        }

        if (pointA >= 0.0 && pointB > pointA)
        {
            playerAudio.setLoopPoints(pointA, pointB);
            abLoopLabel.setText("A-B: " + formatTime(pointA) + " -> " + formatTime(pointB),
                juce::dontSendNotification);
        }
    }
    else if (button == &clearABButton)
    {
        playerAudio.clearLoopPoints();
        pointA = -1.0;
        pointB = -1.0;
        abToggleButton.setButtonText("A"); 
        styleModernButton(abToggleButton, juce::Colour(0xff64dd17)); 
        abState = ABState::A;
        abLoopLabel.setText("A-B Loop: Off", juce::dontSendNotification);
    }
    else if (button == &addMarkerButton)
    {
        addMarkerAtCurrentPosition();
    }
    else if (button == &clearMarkersButton)
    {
        markers.clear();
        markerListBox.updateContent();
    }
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        float volume = static_cast<float>(slider->getValue());
        if (!isFadingOut)
        {
            playerAudio.setGain(volume * mixerGain);
        }
        int volumePercent = static_cast<int>(volume * 100.0f);
        volumeLabel.setText("Volume: " + juce::String(volumePercent) + "%",
            juce::dontSendNotification);
    }
    else if (slider == &speedSlider)
    {
        double speed = slider->getValue();
        playerAudio.setSpeed(speed);
        speedLabel.setText("Speed: " + juce::String(speed, 2) + "x",
            juce::dontSendNotification);
    }
    else if (slider == &positionSlider)
    {
        if (isDraggingPosition)
        {
            double position = slider->getValue() * playerAudio.getLength();
            playerAudio.setPosition(position);
        }
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
    drawWaveform(g);
}

void PlayerGUI::resized()
{
    auto area = getLocalBounds().reduced(10);
    playerNameLabel.setBounds(area.removeFromTop(30));
    auto metadataArea = area.removeFromTop(60);
    auto artArea = metadataArea.removeFromLeft(60);
    artArea.reduce(2, 2);
    albumArtComponent.setBounds(artArea);
    metadataArea.removeFromLeft(5);
    titleLabel.setBounds(metadataArea.removeFromTop(20));
    artistLabel.setBounds(metadataArea.removeFromTop(18));
    albumLabel.setBounds(metadataArea.removeFromTop(16));
    bitrateLabel.setBounds(area.removeFromTop(14));
    area.removeFromTop(5);
    waveformArea = area.removeFromTop(60).reduced(5);
    area.removeFromTop(5);
    positionSlider.setBounds(area.removeFromTop(22).reduced(3, 0));
    timeLabel.setBounds(area.removeFromTop(20));
    area.removeFromTop(8);
    auto controlArea = area.removeFromTop(45);
    int buttonWidth = 45;
    int loadWidth = 60;
    int spacing = 5;
    int totalWidth = buttonWidth * 6 + loadWidth + spacing * 6;
    int startX = (getWidth() - totalWidth) / 2;
    toStartButton.setBounds(startX, controlArea.getY(), buttonWidth, 40);
    playButton.setBounds(startX + (buttonWidth + spacing), controlArea.getY(), buttonWidth, 40);
    pauseButton.setBounds(startX + (buttonWidth + spacing), controlArea.getY(), buttonWidth, 40);
    stopButton.setBounds(startX + (buttonWidth + spacing) * 2, controlArea.getY(), buttonWidth, 40);
    toEndButton.setBounds(startX + (buttonWidth + spacing) * 3, controlArea.getY(), buttonWidth, 40);
    muteButton.setBounds(startX + (buttonWidth + spacing) * 4, controlArea.getY(), buttonWidth, 40);
    loopButton.setBounds(startX + (buttonWidth + spacing) * 5, controlArea.getY(), buttonWidth, 40);
    loadButton.setBounds(startX + (buttonWidth + spacing) * 6, controlArea.getY(), loadWidth, 40);
    area.removeFromTop(8);
    auto skipArea = area.removeFromTop(30);
    int skipWidth = 50; 
    int skipSpacing = (getWidth() - skipWidth * 2) / 3;
    skipBackButton.setBounds(skipSpacing, skipArea.getY(), skipWidth, 26);
    skipForwardButton.setBounds(skipSpacing * 2 + skipWidth, skipArea.getY(), skipWidth, 26);
    area.removeFromTop(8);
    volumeLabel.setBounds(area.removeFromTop(18));
    volumeSlider.setBounds(area.removeFromTop(22).reduced(35, 0));
    area.removeFromTop(5);
    speedLabel.setBounds(area.removeFromTop(18));
    speedSlider.setBounds(area.removeFromTop(22).reduced(35, 0));
    area.removeFromTop(8);
    auto abArea = area.removeFromTop(28);
    int abWidth = 75; 
    int abSpacing = (getWidth() - abWidth * 2) / 3;
    abToggleButton.setBounds(abSpacing, abArea.getY(), abWidth, 24);
    clearABButton.setBounds(abSpacing * 2 + abWidth, abArea.getY(), abWidth, 24);
    abLoopLabel.setBounds(area.removeFromTop(18));
    area.removeFromTop(8);
    auto bottomArea = area;
    auto leftSide = bottomArea.removeFromLeft(getWidth() / 2 - 15);
    bottomArea.removeFromLeft(10);
    auto rightSide = bottomArea;
    auto playlistButtonArea = leftSide.removeFromTop(28);
    int listBtnWidth = (leftSide.getWidth() - 10) / 2;
    loadPlaylistButton.setBounds(playlistButtonArea.removeFromLeft(listBtnWidth));
    playlistButtonArea.removeFromLeft(10);
    clearPlaylistButton.setBounds(playlistButtonArea.removeFromLeft(listBtnWidth));
    leftSide.removeFromTop(3);
    playlistBox.setBounds(leftSide);
    auto markerButtonArea = rightSide.removeFromTop(28);
    int markerBtnWidth = (rightSide.getWidth() - 10) / 2;
    addMarkerButton.setBounds(markerButtonArea.removeFromLeft(markerBtnWidth));
    markerButtonArea.removeFromLeft(10);
    clearMarkersButton.setBounds(markerButtonArea.removeFromLeft(markerBtnWidth));
    rightSide.removeFromTop(3);
    markerListBox.setBounds(rightSide);
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
    g.setColour(juce::Colours::white.withAlpha(0.5f));
    auto bounds = juce::Rectangle<float>(width - 20, 2, 18, height - 4);
    auto icon = Icons::createFromSVG(Icons::getDeleteIcon());
    icon->drawWithin(g, bounds, juce::RectanglePlacement::centred, 1.0f);
}

void PlayerGUI::PlaylistModel::listBoxItemClicked(int row, const juce::MouseEvent& e)
{
    auto deleteBounds = juce::Rectangle<int>(gui.playlistBox.getWidth() - 22, 0, 22, gui.playlistBox.getRowHeight());
    if (deleteBounds.contains(e.getPosition()))
    {
        juce::AlertWindow::showOkCancelBox(
            juce::AlertWindow::WarningIcon,
            "Delete Track",
            "Are you sure you want to remove '" + gui.playlist[row] + "' from the playlist?",
            "Delete",
            "Cancel",
            nullptr,
            juce::ModalCallbackFunction::create([this, row](int result)
                {
                    if (result == 1)
                        gui.deletePlaylistItem(row);
                }));
    }
    else
    {
        gui.playSelectedFile(row);
    }
}

void PlayerGUI::MarkerModel::paintListBoxItem(int row, juce::Graphics& g, int width, int height, bool selected)
{
    if (selected)
    {
        g.fillAll(juce::Colour(0xffffd600).withAlpha(0.3f));
    }
    else
    {
        g.fillAll(juce::Colour(0xff1a1a2e));
    }
    g.setColour(juce::Colours::white);
    g.setFont(11.0f);
    if (row < static_cast<int>(gui.markers.size()))
    {
        const auto& marker = gui.markers[row];
        juce::String text = marker.name + " @ " + gui.formatTime(marker.position);
        g.drawText(text, 5, 0, width - 10, height, juce::Justification::centredLeft, true);
    }
    g.setColour(juce::Colours::white.withAlpha(0.5f));
    auto bounds = juce::Rectangle<float>(width - 20, 2, 18, height - 4);
    auto icon = Icons::createFromSVG(Icons::getDeleteIcon());
    icon->drawWithin(g, bounds, juce::RectanglePlacement::centred, 1.0f);
}

void PlayerGUI::MarkerModel::listBoxItemClicked(int row, const juce::MouseEvent& e)
{
    auto deleteBounds = juce::Rectangle<int>(gui.markerListBox.getWidth() - 22, 0, 22, gui.markerListBox.getRowHeight());
    if (deleteBounds.contains(e.getPosition()))
    {
        gui.deleteMarkerItem(row);
    }
    else
    {
        gui.jumpToMarker(row);
    }
}

juce::PropertiesFile::Options PlayerGUI::getPropertyFileOptions()
{
    juce::PropertiesFile::Options opts;
    opts.applicationName = "EnhancedAudioPlayer";
    opts.filenameSuffix = ".settings";
    opts.folderName = "EnhancedAudioPlayer";
    opts.osxLibrarySubFolder = "Application Support";
    opts.commonToAllUsers = false;
    return opts;
}

void PlayerGUI::saveState()
{
    auto props = std::make_unique<juce::PropertiesFile>(getPropertyFileOptions());
    props->setValue("lastTrack_" + playerName, playerAudio.getLoadedFile().getFullPathName());
    props->setValue("lastPos_" + playerName, playerAudio.getPosition());
}

void PlayerGUI::loadState()
{
    auto props = std::make_unique<juce::PropertiesFile>(getPropertyFileOptions());
    juce::File lastFile = props->getValue("lastTrack_" + playerName);
    double lastPos = props->getDoubleValue("lastPos_" + playerName);

    if (lastFile.existsAsFile())
    {
        loadFile(lastFile, false);
        juce::MessageManager::callAsync([this, lastPos]()
            {
                setPlaybackPosition(lastPos);
                updatePositionSlider();
                updateTimeDisplay();
            });
    }
}