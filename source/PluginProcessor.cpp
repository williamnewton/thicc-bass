#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthVoice.h"

//==============================================================================
PluginProcessor::PluginProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       apvts (*this, nullptr, "Parameters", createParameterLayout())
{
    // Add voices to the synthesizer
    for (int i = 0; i < NUM_VOICES; ++i)
        synth.addVoice (new SynthVoice());

    // Add sound - SynthSound allows all notes
    synth.addSound (new SynthSound());
}

PluginProcessor::~PluginProcessor()
{
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Filter Cutoff (20 Hz - 20 kHz, logarithmic scale)
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID (FILTER_CUTOFF_ID, 1),
        "Filter Cutoff",
        juce::NormalisableRange<float> (20.0f, 20000.0f, 0.1f, 0.3f),  // skew for logarithmic feel
        1000.0f,  // default value
        "Hz"));

    // Filter Resonance (0.0 - 1.0)
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID (FILTER_RESONANCE_ID, 1),
        "Filter Resonance",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.5f,  // default value
        ""));

    // Amp Envelope Attack (1ms - 5s, logarithmic)
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID (AMP_ATTACK_ID, 1),
        "Amp Attack",
        juce::NormalisableRange<float> (0.001f, 5.0f, 0.001f, 0.3f),
        0.01f,  // default 10ms
        "s"));

    // Amp Envelope Decay (1ms - 5s, logarithmic)
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID (AMP_DECAY_ID, 1),
        "Amp Decay",
        juce::NormalisableRange<float> (0.001f, 5.0f, 0.001f, 0.3f),
        0.1f,  // default 100ms
        "s"));

    // Amp Envelope Sustain (0.0 - 1.0)
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID (AMP_SUSTAIN_ID, 1),
        "Amp Sustain",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.8f,  // default 80%
        ""));

    // Amp Envelope Release (1ms - 5s, logarithmic)
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID (AMP_RELEASE_ID, 1),
        "Amp Release",
        juce::NormalisableRange<float> (0.001f, 5.0f, 0.001f, 0.3f),
        0.1f,  // default 100ms
        "s"));

    // Sub-Oscillator Mix (0.0 - 1.0)
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID (SUB_MIX_ID, 1),
        "Sub Mix",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.5f,  // default 50% mix
        ""));

    // Filter Envelope Attack (1ms - 5s, logarithmic)
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID (FILTER_ENV_ATTACK_ID, 1),
        "Filter Env Attack",
        juce::NormalisableRange<float> (0.001f, 5.0f, 0.001f, 0.3f),
        0.01f,  // default 10ms
        "s"));

    // Filter Envelope Decay (1ms - 5s, logarithmic)
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID (FILTER_ENV_DECAY_ID, 1),
        "Filter Env Decay",
        juce::NormalisableRange<float> (0.001f, 5.0f, 0.001f, 0.3f),
        0.2f,  // default 200ms
        "s"));

    // Filter Envelope Sustain (0.0 - 1.0)
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID (FILTER_ENV_SUSTAIN_ID, 1),
        "Filter Env Sustain",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.3f,  // default 30%
        ""));

    // Filter Envelope Release (1ms - 5s, logarithmic)
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID (FILTER_ENV_RELEASE_ID, 1),
        "Filter Env Release",
        juce::NormalisableRange<float> (0.001f, 5.0f, 0.001f, 0.3f),
        0.2f,  // default 200ms
        "s"));

    // Filter Envelope Amount (0.0 - 1.0)
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID (FILTER_ENV_AMOUNT_ID, 1),
        "Filter Env Amount",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.5f,  // default 50%
        ""));

    // LFO Rate (0.01 Hz - 20 Hz)
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID (LFO_RATE_ID, 1),
        "LFO Rate",
        juce::NormalisableRange<float> (0.01f, 20.0f, 0.01f, 0.3f),  // logarithmic
        2.0f,  // default 2 Hz
        "Hz"));

    // LFO Amount (0.0 - 1.0)
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID (LFO_AMOUNT_ID, 1),
        "LFO Amount",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.0f,  // default 0 (off)
        ""));

    // Drive Amount (0.0 - 1.0)
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID (DRIVE_AMOUNT_ID, 1),
        "Drive",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.0f,  // default 0 (clean)
        ""));

    return layout;
}

void PluginProcessor::updateVoiceParameters()
{
    // Get current parameter values
    float cutoff = apvts.getRawParameterValue (FILTER_CUTOFF_ID)->load();
    float resonance = apvts.getRawParameterValue (FILTER_RESONANCE_ID)->load();
    float attack = apvts.getRawParameterValue (AMP_ATTACK_ID)->load();
    float decay = apvts.getRawParameterValue (AMP_DECAY_ID)->load();
    float sustain = apvts.getRawParameterValue (AMP_SUSTAIN_ID)->load();
    float release = apvts.getRawParameterValue (AMP_RELEASE_ID)->load();
    float subMix = apvts.getRawParameterValue (SUB_MIX_ID)->load();
    float filterEnvAttack = apvts.getRawParameterValue (FILTER_ENV_ATTACK_ID)->load();
    float filterEnvDecay = apvts.getRawParameterValue (FILTER_ENV_DECAY_ID)->load();
    float filterEnvSustain = apvts.getRawParameterValue (FILTER_ENV_SUSTAIN_ID)->load();
    float filterEnvRelease = apvts.getRawParameterValue (FILTER_ENV_RELEASE_ID)->load();
    float filterEnvAmount = apvts.getRawParameterValue (FILTER_ENV_AMOUNT_ID)->load();
    float lfoRate = apvts.getRawParameterValue (LFO_RATE_ID)->load();
    float lfoAmount = apvts.getRawParameterValue (LFO_AMOUNT_ID)->load();
    float driveAmount = apvts.getRawParameterValue (DRIVE_AMOUNT_ID)->load();

    // Update all voices
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SynthVoice*> (synth.getVoice (i)))
        {
            voice->setFilterCutoff (cutoff);
            voice->setFilterResonance (resonance);
            voice->setAmpEnvelope (attack, decay, sustain, release);
            voice->setFilterEnvelope (filterEnvAttack, filterEnvDecay, filterEnvSustain, filterEnvRelease);
            voice->setFilterEnvAmount (filterEnvAmount);
            voice->setSubMix (subMix);
            voice->setLFORate (lfoRate);
            voice->setLFOAmount (lfoAmount);
            voice->setDriveAmount (driveAmount);
        }
    }
}

//==============================================================================
const juce::String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PluginProcessor::getCurrentProgram()
{
    return 0;
}

void PluginProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String PluginProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void PluginProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Prepare the synthesizer
    synth.setCurrentPlaybackSampleRate (sampleRate);

    // Prepare all voices
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SynthVoice*> (synth.getVoice (i)))
        {
            voice->prepareToPlay (sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }

    // Initialize all voices with current parameter values
    updateVoiceParameters();
}

void PluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool PluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void PluginProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    // Critical: Prevent denormal CPU spikes
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any output channels that don't have input
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Clear the buffer for synthesizer output (synth is additive)
    buffer.clear();

    // Update voice parameters (thread-safe via atomic loads)
    updateVoiceParameters();

    // Render synthesizer audio
    synth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor (*this);
}

//==============================================================================
void PluginProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Use APVTS to save state as XML
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Restore APVTS state from XML
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName (apvts.state.getType()))
        {
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
            updateVoiceParameters();  // Update voices with restored parameters
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
