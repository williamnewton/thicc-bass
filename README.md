# 🎵 THICC BASS

**A powerful bass synthesizer plugin for music producers**

> Vibrant street art design • Instant THICC bass sounds • Works in FL Studio, Logic Pro, Ableton, and more

![Version](https://img.shields.io/badge/version-1.0--beta-orange)
![Platform](https://img.shields.io/badge/platform-macOS%20%7C%20Windows-blue)
![License](https://img.shields.io/badge/license-MIT-green)

---

## 🚀 Quick Start for Users

### 📥 Download
**👉 [Download Latest Release](../../releases)** (macOS + Windows)

### ⚡ 3-Minute Install
1. Download for your OS (macOS or Windows)
2. Copy plugin to your plugins folder
3. Load in your DAW and make bass!

**📖 [Full Installation Guide](INSTALL.md)** | **📚 [User Guide](USER_GUIDE.md)**

### ✨ What is it?
Thicc Bass is a focused bass synthesizer designed to deliver powerful, punchy low-end with a vibrant street art aesthetic. Perfect for electronic music, hip-hop, trap, dubstep, and any genre that needs serious bass.

### 🎹 Works With
- **macOS**: Logic Pro, GarageBand, FL Studio, Ableton Live, Cubase, Reaper, Bitwig
- **Windows**: FL Studio, Ableton Live, Cubase, Studio One, Reaper, Bitwig

---

## 🎨 Features

### Street Art UI
- Vibrant graffiti-style design
- Bold pink knobs with cartoon shadows
- Real-time waveform visualizer
- Colorful output meter

### Instant Results
- **4 Factory Presets**: Phat Mono Bass, Sub Drop, Wobble Beast, Retro Synth Bass
- **6 Main Controls**: Cutoff, Resonance, Drive, Sub Mix, THICC, Glide
- **Advanced Panel**: 16 additional parameters for deep control

### Professional Sound
- Pure sawtooth oscillator with sub bass
- Classic Moog ladder filter
- Built-in saturation/drive
- Unison voices for width
- LFO modulation for wobbles

---

## 📸 Screenshots

*[Your screenshot here showing the plugin interface]*

---

## 🎵 Sound Examples

*[Link to audio demos or YouTube video]*

---

## 🔧 For Developers

### Overview

Thicc Bass is a professional bassline synthesizer built with JUCE 8.x following real-time safety practices.

## Features

### Phase 1: Core Synthesis (Complete ✓)
- **PolyBLEP Sawtooth Oscillator** - Band-limited anti-aliased sawtooth wave
- **Moog Ladder Filter** - Classic 24dB/octave lowpass filter
- **Dual ADSR Envelopes** - Separate envelopes for amplitude and filter modulation
- **8-Voice Polyphony** - Professional voice allocation
- **Parameter Smoothing** - Click-free parameter changes
- **State Save/Load** - Full preset recall via DAW

### Phase 2: Bass Enhancement (Complete ✓)
- **Sub-Oscillator** - Pure sine wave, selectable -1 or -2 octaves
- **Filter Envelope** - Full ADSR with amount control for filter modulation
- **LFO Modulation** - Sine wave LFO with rate (0.01-20 Hz) and amount controls
- **Drive/Saturation** - Soft clipping with 2x oversampling for clean harmonic generation
- **Custom UI** - Professional dark theme with gold accents, custom knob design
- **Organized Layout** - Logical section grouping with visual dividers

### Phase 3: Essential Playability (Complete ✓)
- **Exponential Envelopes** - Natural-sounding ADSR curves for punchy response
- **Glide/Portamento** - Smooth pitch transitions (0-2 seconds)
- **Velocity Sensitivity**
  - Velocity → Filter Cutoff (0-100%, default 50%)
  - Velocity → Amp Level (0-100%, default 70%)
- **Filter Key Tracking** - Prevents muddy high notes (0-100%, C4 reference)
- **Unison/Voice Spread** - 1-5 detuned voices with THICC control (up to ±100 cents)
- **Sub-Oscillator Octave Selector** - Choose between -1 or -2 octaves
- **Output Soft Clipper** - Always-on gentle limiting for safety and loudness

## Total Parameters: 22

### Oscillator Section
- Sub Mix (0-100%)
- Sub Octave (-1 Oct / -2 Oct)
- Glide Time (0-2s)

### Filter Section
- Cutoff (20 Hz - 20 kHz)
- Resonance (0-100%)
- Key Tracking (0-100%)

### Filter Envelope
- Attack (1ms - 5s)
- Decay (1ms - 5s)
- Sustain (0-100%)
- Release (1ms - 5s)
- Amount (0-100%)

### Amp Envelope
- Attack (1ms - 5s)
- Decay (1ms - 5s)
- Sustain (0-100%)
- Release (1ms - 5s)

### LFO
- Rate (0.01 - 20 Hz)
- Amount (0-100%)

### Modulation
- Velocity → Filter (0-100%)
- Velocity → Amp (0-100%)

### Unison
- Voices (1-5)
- THICC / Detune (0-100%)

### Drive
- Drive Amount (0-100%)

## Technical Highlights

### Real-Time Safety
- No memory allocation in audio thread
- `ScopedNoDenormals` for CPU spike prevention
- Pre-allocated buffers and voice structures
- Thread-safe parameter updates via atomic loads

### Audio Quality
- PolyBLEP anti-aliasing for oscillators
- 2x oversampling for drive/saturation
- Parameter smoothing (10ms ramps)
- Soft output limiting (always on)

### DSP Architecture
- Sample rate: Up to 192 kHz tested
- Buffer sizes: 64-4096 frames supported
- Processing: Mono voice rendering → filter → drive → soft clip
- Modulation: LFO, Filter Envelope, Velocity, Key Tracking

## Building

### Requirements
- CMake 3.25+
- JUCE 8.x (included as submodule)
- C++23 compiler
- macOS: Xcode Command Line Tools
- Homebrew (for CMake on macOS)

### Build Instructions

```bash
# Clone with submodules
git clone --recursive <repository-url>
cd thicc-bass

# Initialize submodules if not already done
git submodule update --init --recursive

# Configure and build
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug

# The plugin will be automatically installed to:
# ~/Library/Audio/Plug-Ins/Components/Thicc Bass.component (macOS AU)
# ~/Library/Audio/Plug-Ins/VST3/Thicc Bass.vst3 (VST3)
```

### Validation

```bash
# Validate Audio Unit
auval -v aumu Tbs1 Thic

# Result: AU VALIDATION SUCCEEDED
```

## Installation (For Users)

### macOS Installation

#### Step 1: Choose Your Plugin Format

Different DAWs support different plugin formats. Choose the one that works with your DAW:

| DAW | Recommended Format | Alternative |
|-----|-------------------|-------------|
| Logic Pro | AU (.component) | VST3 |
| GarageBand | AU (.component) | - |
| FL Studio | VST3 (.vst3) | - |
| Ableton Live | AU (.component) | VST3 |
| Cubase | VST3 (.vst3) | - |
| Studio One | VST3 (.vst3) | AU |
| Reaper | VST3 (.vst3) | AU, CLAP |
| Bitwig Studio | CLAP (.clap) | VST3 |
| Pro Tools | AU (.component) | - |

#### Step 2: Copy Plugin to the Correct Location

**For AU (Audio Units):**
```bash
# Copy to Components folder
cp -R "Thicc Bass.component" ~/Library/Audio/Plug-Ins/Components/
```

**For VST3:**
```bash
# Copy to VST3 folder
cp -R "Thicc Bass.vst3" ~/Library/Audio/Plug-Ins/VST3/
```

**For CLAP:**
```bash
# Copy to CLAP folder
cp -R "Thicc Bass.clap" ~/Library/Audio/Plug-Ins/CLAP/
```

**Accessing ~/Library folder in Finder:**
1. Open Finder
2. Press `Cmd + Shift + G`
3. Type: `~/Library/Audio/Plug-Ins/`
4. Press Enter

#### Step 3: macOS Security (Gatekeeper)

Since this is an unsigned plugin, macOS may block it:

1. Try to load the plugin in your DAW
2. If blocked, go to **System Settings > Privacy & Security**
3. Look for a message about "Thicc Bass" being blocked
4. Click **"Allow Anyway"**
5. Restart your DAW

### Windows Installation

**For VST3:**
```
Copy "Thicc Bass.vst3" to:
C:\Program Files\Common Files\VST3\
```

Restart your DAW after installation.

### DAW-Specific Instructions

#### FL Studio (Windows/macOS)
1. Install the VST3 version to the location above
2. Open FL Studio
3. Go to **Options > Manage Plugins**
4. Click **Find Plugins** or **Start Scan**
5. Thicc Bass should appear in your plugin list
6. Find it under **Generators** or **Instruments**

#### Logic Pro (macOS)
1. Install the AU version (.component)
2. Restart Logic Pro
3. Create a new Software Instrument track
4. Click the instrument slot
5. Go to **AU Instruments > Thic (Manufacturer) > Thicc Bass**

#### Ableton Live (macOS/Windows)
1. Install AU (macOS) or VST3 (Windows/macOS)
2. Restart Ableton Live
3. Ableton will automatically scan the plugin folders
4. Find Thicc Bass in the **Instruments** browser

#### Cubase/Nuendo (Windows/macOS)
1. Install VST3 version
2. Open Cubase
3. Go to **Studio > VST Plug-in Manager**
4. Click **Update Plug-in Information**
5. Thicc Bass will appear under **Instruments**

#### Reaper (Windows/macOS)
1. Install VST3, AU, or CLAP version
2. Open Reaper
3. Go to **Options > Preferences > Plugins > VST**
4. Add the plugin folder path if needed
5. Click **Re-scan**
6. Insert as instrument on a track: **Insert > Virtual Instrument**

#### Bitwig Studio (Windows/macOS)
1. Install CLAP or VST3 version
2. Restart Bitwig
3. Bitwig automatically scans plugin folders
4. Find Thicc Bass in the **Devices** browser

### Troubleshooting

**Plugin doesn't appear in DAW:**
- Restart your DAW after installation
- Manually rescan plugins in DAW preferences
- Check you installed to the correct folder
- On macOS, check System Settings for security blocks

**Plugin loads but shows security warning (macOS):**
- This is normal for unsigned plugins
- Follow the Gatekeeper instructions above

**Plugin crashes or doesn't load:**
- Make sure you're using the correct format for your DAW
- Check that your DAW is up to date
- Try both AU and VST3 versions (on macOS)

**No sound output:**
- Check MIDI is being sent to the plugin
- Check plugin output is routed correctly in your DAW
- Try adjusting the CUTOFF and SUB MIX controls

## Plugin Formats

- **Audio Unit (AU)** - macOS only (Logic Pro, GarageBand, Ableton Live, etc.)
- **VST3** - Cross-platform (FL Studio, Ableton Live, Cubase, Reaper, etc.)
- **Standalone** - Standalone application
- **CLAP** - Modern plugin format (Bitwig Studio, Reaper)

## Plugin Codes

- **Manufacturer Code**: `Thic` (ThiccBass Audio)
- **Plugin Code**: `Tbs1` (Thicc Bass v1)
- **Plugin Type**: `aumu` (Audio Unit Music Instrument)

## UI Layout

**Window Size**: 1200 x 600 pixels

**Row 1**: Oscillator | Filter | Filter Envelope | Amp Envelope
**Row 2**: LFO | Velocity | Drive
**Row 3**: Unison

All controls feature custom-designed rotary knobs with:
- Gold accent color (#FFCC66)
- Tick marks (11 positions)
- Value arc with glow effect
- Drop shadows for depth
- Gradient backgrounds

## Development Status

### Completed
✅ Phase 1: Core synthesis engine
✅ Phase 2: Bass-specific features
✅ Phase 3: Essential playability features
✅ Custom professional UI
✅ Real-time safety implementation
✅ Audio Unit validation passed

### Optional Future Enhancements
- Output level meter with peak hold
- Preset system (save/load to disk)
- Factory preset library
- Preset browser UI
- Parameter tooltips

## Architecture

### File Structure
```
source/
├── PluginProcessor.h/cpp    - Main audio processor, parameter management
├── PluginEditor.h/cpp        - UI layout and controls
├── SynthVoice.h/cpp          - Voice rendering and DSP
├── CustomLookAndFeel.h       - Custom UI styling
└── BinaryData.h              - Embedded resources
```

### Key Classes
- `PluginProcessor` - JUCE AudioProcessor, manages APVTS and voices
- `SynthVoice` - Extends juce::SynthesiserVoice, handles per-voice DSP
- `SynthSound` - Allows all MIDI notes (bass synth plays full range)
- `PluginEditor` - AudioProcessorEditor with custom layout
- `ThiccBassLookAndFeel` - Custom LookAndFeel_V4 for knobs

## Design Philosophy

**"Focused, not featureless"**

Thicc Bass prioritizes:
1. **Great defaults** - Sounds good immediately
2. **Essential controls** - No analysis paralysis
3. **Professional quality** - Real-time safe, validated, production-ready
4. **Bass-optimized** - Every feature serves the low-end

Not included by design:
- Multiple oscillator types (pure sawtooth is the bass foundation)
- Filter type selection (Moog ladder is optimal for bass)
- Complex modulation matrix (focused routing only)
- Unnecessary effects (drive is built-in)

## Credits

Built with:
- [JUCE 8.x](https://juce.com/) - Audio plugin framework
- [Pamplejuce](https://github.com/sudara/pamplejuce) - CMake template
- [Melatonin Inspector](https://github.com/sudara/melatonin_inspector) - UI debugging
- [Catch2](https://github.com/catchorg/Catch2) - Testing framework

## License

See LICENSE file for details.

## Development Notes

### Build Tested On
- macOS 14.6 (Darwin 24.6.0)
- CMake 4.2.1
- Apple Clang (Xcode Command Line Tools)

### Performance
- Validated at multiple sample rates (22-192 kHz)
- Buffer sizes from 64 to 4096 frames
- 8 voices of polyphony with unison up to 5x per voice
- CPU efficient with proper real-time practices

---

**Current Version**: 1.0-beta
**Last Updated**: December 29, 2024
**Status**: Beta Testing - Ready for Distribution

### What's New in v1.0-beta
- ✨ Street art/graffiti UI design
- 🎨 Vibrant red/orange/pink color scheme
- 🎛️ 6 primary controls + advanced panel with 16 additional parameters
- 🎵 4 factory presets included
- 📊 Real-time output meter and waveform visualizer
- 🎹 Full MIDI support with velocity sensitivity
- 🔊 Professional bass synthesis with sub oscillator, filter, and drive
