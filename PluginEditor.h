/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class RingbufferAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    RingbufferAudioProcessorEditor (RingbufferAudioProcessor&);
    ~RingbufferAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    RingbufferAudioProcessor& audioProcessor;

    juce::ComboBox modeBox;
    std::unique_ptr<juce::ComboBoxParameterAttachment> modeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RingbufferAudioProcessorEditor)
};
