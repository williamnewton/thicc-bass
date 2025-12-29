# THICC BASS

A professional bassline synthesizer plugin for Logic Pro and other DAWs, built with JUCE 8.x.

## Overview

Thicc Bass is a focused, bass-optimized synthesizer designed to deliver powerful, punchy low-end with essential playability features. Built following professional audio plugin development practices with real-time safety and performance in mind.

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

## Plugin Formats

- **Audio Unit (AU)** - Primary target for Logic Pro
- **VST3** - Cross-platform DAW support
- **Standalone** - Standalone application
- **CLAP** - Modern plugin format

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

**Current Version**: 0.0.1
**Last Updated**: December 29, 2025
**Status**: Phase 3 Complete - Production Ready
