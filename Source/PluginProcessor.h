/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class EchoplexAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    EchoplexAudioProcessor();
    ~EchoplexAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    const String getInputChannelName (int channelIndex) const override;
    const String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool silenceInProducesSilenceOut() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    typedef struct {
        float *buffer;
        int current_index;
        int length;
    } RingBuffer;
    
    // Echoplex variables
    double feedback = 0.5;
    double mix = 1;
    double delayTime = 50;
    double filterCutoff = 1000;
    bool bypass = false;
    bool soundOnSound = false;
    
    

private:
    //==============================================================================
    
    int mod (int, int);
    
    // Ring buffer operation functions
    RingBuffer *ringbuffer_create (int);
    void ringbuffer_destroy (RingBuffer *);
    
    // Ring buffer declaration
    RingBuffer *ringBuf;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EchoplexAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
