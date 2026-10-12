/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Lfo.h"
#include "RingBuffer.h"

//==============================================================================
/**
*/
class RingbufferAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    RingbufferAudioProcessor();
    ~RingbufferAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioParameterChoice& getModeParam() { return *modeParam; }

private:
    RingBuffer ringL, ringR;
    Lfo LfoL;
    
    struct ChorusMode
    {
        float rateHz, centerMs, depthMs;
    };
    static constexpr ChorusMode modes[] =
    {
        { 0.513f, 3.505f, 1.845f },   
        { 0.863f, 3.505f, 1.845f },   
        { 9.75f,  3.5f,   0.2f   },   
    };
    
    juce::AudioParameterChoice* modeParam = nullptr;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RingbufferAudioProcessor)
};
