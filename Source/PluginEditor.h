/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class EchoplexAudioProcessorEditor  :   public AudioProcessorEditor,
                                        private Slider::Listener,
                                        private Button::Listener
{
public:
    EchoplexAudioProcessorEditor (EchoplexAudioProcessor&);
    ~EchoplexAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged (Slider *slider) override;
    void buttonClicked (Button *button) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    EchoplexAudioProcessor& processor;
    
    Slider feedbackSlider;
    Label feedbackLabel;
    
    Slider mixSlider;
    Label mixLabel;
    
    Slider delaySlider;
    Label delayLabel;
    
    Slider cutoffSlider;
    Label cutoffLabel;
    
    TextButton bypassButton;
    Label bypassLabel;

    TextButton SoSButton;
    Label SoSLabel;
    
    TextButton filterButton;
    Label filterLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EchoplexAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
