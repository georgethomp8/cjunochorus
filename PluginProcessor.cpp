/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Lfo.h"

//==============================================================================
RingbufferAudioProcessor::RingbufferAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter (modeParam = new juce::AudioParameterChoice ({ "mode", 1 }, "Chorus Mode", { "I", "II", "I+II" }, 0));
}

RingbufferAudioProcessor::~RingbufferAudioProcessor()
{
}

//==============================================================================
const juce::String RingbufferAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RingbufferAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool RingbufferAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool RingbufferAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double RingbufferAudioProcessor::getTailLengthSeconds() const
{
    return 0.01;
}

int RingbufferAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int RingbufferAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RingbufferAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String RingbufferAudioProcessor::getProgramName (int index)
{
    return {};
}

void RingbufferAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void RingbufferAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    ringL.clear();
    ringR.clear();
    
    LfoL.prepare (sampleRate, modes[0].rateHz);
    
}

void RingbufferAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RingbufferAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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
#endif

void RingbufferAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    

    
    const auto& m = modes[modeParam->getIndex()];
    const float msToSamples = (float) getSampleRate() * 0.001f;
    LfoL.setFrequency (m.rateHz);
    const float center = m.centerMs * msToSamples;
    const float depth  = m.depthMs  * msToSamples;
    
    auto* left  = buffer.getWritePointer (0);
    float* right = nullptr;

    if (totalNumOutputChannels > 1)
    {
        right = buffer.getWritePointer (1);
    }

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        float dry;

        if (right != nullptr && totalNumInputChannels > 1)
        {
            dry = 0.5f * (left[i] + right[i]);
        }
        else
        {
            dry = left[i];
        }
        const float lfoVal = LfoL.nextSample();

        ringL.push (dry);
        ringR.push (dry);
        const float wetL = ringL.read (center + lfoVal * depth);
        const float wetR = ringR.read (center - lfoVal * depth);

        left[i] = 0.5f * (dry + wetL);
        if (right) right[i] = 0.5f * (dry + wetR);
    }
}

//==============================================================================
bool RingbufferAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* RingbufferAudioProcessor::createEditor()
{
    return new RingbufferAudioProcessorEditor (*this);
}

//==============================================================================
void RingbufferAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream (destData, true).writeInt (modeParam->getIndex());
}

void RingbufferAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (sizeInBytes >= 4)
        *modeParam = juce::MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readInt();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RingbufferAudioProcessor();
}
