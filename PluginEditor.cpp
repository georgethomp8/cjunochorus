/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RingbufferAudioProcessorEditor::RingbufferAudioProcessorEditor (RingbufferAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    auto& modeParam = audioProcessor.getModeParam();
    modeBox.addItemList (modeParam.choices, 1);
    addAndMakeVisible (modeBox);
    modeAttachment = std::make_unique<juce::ComboBoxParameterAttachment> (modeParam, modeBox);

    setSize (300, 150);
}

RingbufferAudioProcessorEditor::~RingbufferAudioProcessorEditor()
{
}

//==============================================================================
void RingbufferAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("Juno Chorus", getLocalBounds().removeFromTop (50), juce::Justification::centred, 1);
}

void RingbufferAudioProcessorEditor::resized()
{
    modeBox.setBounds (getLocalBounds().withSizeKeepingCentre (160, 30).translated (0, 10));
}
