# Enhanced Audio Player (CS213 OOP Project)

This is a comprehensive audio player built in C++ using the **JUCE framework**. It was developed as the second assignment for the CS213: Object Oriented Programming course at Cairo University, Faculty of Computing and AI.

The project implements a modular, feature-rich audio player that adheres to OOP principles, particularly **Separation of Concerns (SoC)**. The architecture is divided into a back-end audio engine (`PlayerAudio`), a front-end UI component (`PlayerGUI`), and a main host (`MainComponent`) that manages the decks and bonus mixer functionality.

**Video Demo:** [https://youtu.be/bNOVf5L2Tys](https://youtu.be/bNOVf5L2Tys)

<img width="1919" height="1007" alt="Image" src="https://github.com/user-attachments/assets/aefc7dfa-19ee-4257-b637-2479db6c0cdf" />

---

## 🚀 Features

This player supports a wide range of features, from basic playback to advanced DJ mixing.

### Core Player Features (Task 3)
* **Modular Design:** Code is separated into `PlayerAudio` (engine) and `PlayerGUI` (interface) components.
* **Playback Controls:** Play, Pause, Stop, Go to Start, and Go to End.
* **Mute/Unmute:** Toggle audio output on or off.
* **Volume Control:** A linear slider to adjust gain.
* **Speed Control:** A slider to adjust playback speed (0.25x to 2.0x).
* **Continuous Loop:** Toggle looping for the entire track.
* **Position Slider:** A slider to scrub through the track's timeline.
* **Waveform Display:** Renders the audio waveform using `juce::AudioThumbnail` and shows a real-time playhead.
* **Metadata Display:** Uses **TagLib** to read and display:
    * Track Title
    * Artist
    * Album
    * Bitrate
    * Embedded Album Art
* **A-B Segment Loop:** Set a start point (A) and an end point (B) to loop a specific segment of the track.
* **Playlist Support:**
    * Load multiple files into a playlist.
    * Play tracks by clicking them in the `juce::ListBox`.
    * Remove individual tracks or clear the entire list.
* **10s Skip:** Skip 10 seconds backward or forward.
* **Save/Load Last Session:** Automatically saves the last played track and its position and reloads it on startup (using `juce::PropertiesFile`).
* **Track Markers:**
    * Add timestamped markers at the current playhead position.
    * Jump to any marker by clicking it in the list.
    * Sorts markers by time.
    * Clear all markers.

### 🌟 Bonus Features (Task 4)
* **Dual Decks (Mixer Mode):** A toggle button enables a second `PlayerGUI` deck, allowing two tracks to be loaded simultaneously.
* **Crossfader:** A horizontal slider to mix the audio output between Deck 1 and Deck 2.
* **Sync Play:** A button to stop both tracks, reset their positions to the start, and play them simultaneously.
* **Swap Tracks:** A button to instantly swap the tracks (and their states) loaded in Deck 1 and Deck 2.
* **Auto-Crossfade:** An "Auto Crossfade" button with a speed slider for automatic, oscillating fades between the two decks.

---

## 🛠️ Technologies Used

* **C++ (17/20)**
* **JUCE Framework:** For all UI, audio processing, and application handling.
* **TagLib:** For advanced audio file metadata and album art extraction.

---

## 🏛️ Project Structure

The application follows the **Separation of Concerns (SoC)** principle:

* **`Main.cpp`:** The entry point for the JUCE application. It creates and manages the main application window.
* **`MainComponent`:** The main host component.
    * Inherits from `juce::AudioAppComponent` to handle the final audio output.
    * Contains two `PlayerGUI` instances.
    * Manages the "Mixer Mode" state, showing/hiding the second deck and mixer controls.
    * Performs the audio mixing from both decks in its `getNextAudioBlock()` method.
* **`PlayerGUI`:** The front-end component for a single deck.
    * Inherits from `juce::Component`, `juce::Timer`, `juce::Button::Listener`, etc.
    * Holds all UI elements (buttons, sliders, labels, list boxes).
    * Manages the `AudioThumbnail` for waveform drawing.
    * Handles all user input and delegates audio commands to its `PlayerAudio` instance.
    * Manages the data models for the Playlist and Marker list boxes.
* **`PlayerAudio`:** The back-end audio engine.
    * Contains no UI code.
    * Manages the core JUCE audio graph: `AudioFormatManager`, `AudioTransportSource`, and `ResamplingAudioSource`.
    * Handles file loading, metadata extraction (with TagLib), and playback logic (position, speed, gain, looping).

---

## 📦 How to Build

1.  **Clone the Repository:**
    ```bash
    git clone [https://github.com/mohabnaser/Audio_Player.git](https://github.com/mohabnaser/Audio_Player.git)
    ```
2.  **Prerequisites:**
    * Have the **JUCE framework** installed and set up.
    * Have the **TagLib** library compiled and its headers/libraries accessible to your linker.
3.  **Projucer Setup:**
    * Open the `.jucer` file in the Projucer.
    * Ensure the paths to your JUCE modules are correct.
    * In the Exporters (e.g., Visual Studio, Xcode), add the necessary Header Search Paths and Library Search Paths for **TagLib**.
4.  **Build:**
    * Save the project in Projucer to generate the IDE solution files.
    * Open the project in your IDE (e.g., Visual Studio) and build in `Release` mode.

---

## 🧑‍💻 Team Members & Contributions

* **Task 1 (Modularization) & Bonus Features:** All Team
* **Seif:** Tasks 2 (Playback), 5 (Metadata), 8 (Playlist), 12 (10s Skip)
* **Omar:** Tasks 3 (Mute), 6 (Speed), 9 (Waveform), 13 (Save/Load State)
* **Mohab:** Tasks 4 (Loop), 7 (Position), 10 (A-B Loop), 14 (Track Markers)
