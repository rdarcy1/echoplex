/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
EchoplexAudioProcessorEditor::EchoplexAudioProcessorEditor (EchoplexAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 450);
    
    // Feedback
    addAndMakeVisible (feedbackSlider);
    feedbackSlider.setSliderStyle (Slider::Rotary);
    feedbackSlider.setRange(0.0, 1.0);
    feedbackSlider.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
    feedbackSlider.setPopupDisplayEnabled (true, this);
    feedbackSlider.setTextValueSuffix (" feedback");
    feedbackSlider.setValue(1.0);
    feedbackSlider.addListener (this);
    
    addAndMakeVisible (feedbackLabel);
    feedbackLabel.setText ("Feedback", dontSendNotification);
    feedbackLabel.attachToComponent (&feedbackSlider, false);
    feedbackLabel.setJustificationType(Justification::centredTop);
    
    // Mix
    addAndMakeVisible (mixSlider);
    mixSlider.setSliderStyle (Slider::Rotary);
    mixSlider.setRange(0.0, 1.0);
    mixSlider.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
    mixSlider.setPopupDisplayEnabled (true, this);
    mixSlider.setTextValueSuffix (" mix");
    mixSlider.setValue(1.0);
    mixSlider.addListener (this);
    
    addAndMakeVisible (mixLabel);
    mixLabel.setText ("Mix", dontSendNotification);
    mixLabel.attachToComponent (&mixSlider, false);
    mixLabel.setJustificationType(Justification::centredTop);
    
    // Delay time
    addAndMakeVisible (delaySlider);
    delaySlider.setSliderStyle (Slider::Rotary);
    delaySlider.setRange(0.0, 100);
    delaySlider.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
    delaySlider.setPopupDisplayEnabled (true, this);
    delaySlider.setTextValueSuffix (" delay");
    delaySlider.setValue(1.0);
    delaySlider.addListener (this);
    
    addAndMakeVisible (delayLabel);
    delayLabel.setText ("Delay", dontSendNotification);
    delayLabel.attachToComponent (&delaySlider, false);
    delayLabel.setJustificationType(Justification::centredTop);
    
    // Filter cutoff
    addAndMakeVisible (cutoffSlider);
    cutoffSlider.setSliderStyle (Slider::Rotary);
    cutoffSlider.setRange(0.0, 1.0);
    cutoffSlider.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
    cutoffSlider.setPopupDisplayEnabled (true, this);
    cutoffSlider.setTextValueSuffix (" cutoff");
    cutoffSlider.setValue(1.0);
    cutoffSlider.addListener (this);
    
    addAndMakeVisible (cutoffLabel);
    cutoffLabel.setText ("Cutoff", dontSendNotification);
    cutoffLabel.attachToComponent (&cutoffSlider, false);
    cutoffLabel.setJustificationType(Justification::centredTop);
    
    // Bypass Button
    addAndMakeVisible (bypassButton);
    bypassButton.setClickingTogglesState(true);
    bypassButton.addListener (this);
    
    addAndMakeVisible (delayLabel);
    bypassLabel.setText ("Bypass", dontSendNotification);
    bypassLabel.attachToComponent (&bypassButton, false);
    bypassLabel.setJustificationType(Justification::centredTop);
    
    // Sound on Sound Button
    addAndMakeVisible (SoSButton);
    SoSButton.setClickingTogglesState(true);
    SoSButton.addListener (this);
    
    addAndMakeVisible (delayLabel);
    SoSLabel.setText ("SoS", dontSendNotification);
    SoSLabel.attachToComponent (&SoSButton, false);
    SoSLabel.setJustificationType(Justification::centredTop);
    
}

EchoplexAudioProcessorEditor::~EchoplexAudioProcessorEditor()
{
}

//==============================================================================
void EchoplexAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::white);

    g.setColour (Colours::black);
    g.setFont (25.0f);
    g.drawFittedText ("Echoplex", getLocalBounds(), Justification::centredTop, 1);
}

void EchoplexAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    feedbackSlider.setBounds(20, 80, 80, 80);
    mixSlider.setBounds(120, 80, 80, 80);
    delaySlider.setBounds(220, 80, 80, 80);
    cutoffSlider.setBounds(320, 80, 80, 80);
    
    bypassButton.setBounds(20, 180, 80, 80);
    SoSButton.setBounds(120, 180, 80, 80);
}

void EchoplexAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    if (slider == &feedbackSlider)
    {
        processor.feedback = feedbackSlider.getValue();
    }
    else if (slider == &mixSlider)
    {
        processor.mix = mixSlider.getValue();
    }
    else if (slider == &delaySlider)
    {
        processor.delayTime = delaySlider.getValue();
    }
    else if (slider == &cutoffSlider)
    {
        processor.filterCutoff = cutoffSlider.getValue();
    }
}

void EchoplexAudioProcessorEditor::buttonClicked (Button *button)
{
    if (button == &bypassButton)
    {
        processor.bypass = ! processor.bypass;
    }
    else if (button == &SoSButton)
    {
        processor.soundOnSound = ! processor.soundOnSound;
    }
}

