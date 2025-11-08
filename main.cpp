#include <JuceHeader.h>
#include "MainComponent.h"

class EnhancedAudioPlayerApp : public juce::JUCEApplication
{
public:
    EnhancedAudioPlayerApp() {}
    const juce::String getApplicationName() override { return "Enhanced Audio Player"; }
    const juce::String getApplicationVersion() override { return "2.0"; }
    bool moreThanOneInstanceAllowed() override { return true; }
    void initialise(const juce::String& commandLine) override
    {
        try
        {
            mainWindow.reset(new MainWindow(getApplicationName()));
        }
        catch (const std::exception& e)
        {
            juce::AlertWindow::showMessageBoxAsync(
                juce::AlertWindow::WarningIcon,
                "Startup Error",
                juce::String("Failed to initialize: ") + e.what(),
                "OK");
            quit();
        }
    }
    void shutdown() override { mainWindow = nullptr; }
    void systemRequestedQuit() override { quit(); }
    void anotherInstanceStarted(const juce::String& commandLine) override {}
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(juce::String name)
            : DocumentWindow(name, juce::Colours::black, DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            mainComp = new MainComponent();
            setContentOwned(mainComp, true);
#if JUCE_IOS || JUCE_ANDROID
            setFullScreen(true);
#else
            setResizable(true, true);
            centreWithSize(1200, 700);
#endif
            setVisible(true);
        }
        void closeButtonPressed() override
        {
            if (mainComp)
                mainComp->saveAllStates();
            JUCEApplication::getInstance()->systemRequestedQuit();
        }
    private:
        MainComponent* mainComp;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };
private:
    std::unique_ptr<MainWindow> mainWindow;
};
START_JUCE_APPLICATION(EnhancedAudioPlayerApp)