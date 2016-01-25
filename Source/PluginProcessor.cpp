/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
EchoplexAudioProcessor::EchoplexAudioProcessor()
{
}

EchoplexAudioProcessor::~EchoplexAudioProcessor()
{
}

//==============================================================================
const String EchoplexAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

const String EchoplexAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String EchoplexAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool EchoplexAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool EchoplexAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool EchoplexAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool EchoplexAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool EchoplexAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double EchoplexAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EchoplexAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int EchoplexAudioProcessor::getCurrentProgram()
{
    return 0;
}

void EchoplexAudioProcessor::setCurrentProgram (int index)
{
}

const String EchoplexAudioProcessor::getProgramName (int index)
{
    return String();
}

void EchoplexAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void EchoplexAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    max_delay_time = 2 * getSampleRate();
    
    // Create a ring buffer for each input channel
    int input_chans = getNumInputChannels();
    ringBuf.resize(input_chans);

    for (int channel = 0; channel < input_chans; ++channel)
    {
        ringBuf[channel].length = max_delay_time + 1;
        ringBuf[channel].buffer.resize(ringBuf[channel].length, 0.0);

        if (ringBuf[channel].buffer.size() < ringBuf[channel].length)
        {
            NativeMessageBox::showMessageBox (AlertWindow::AlertIconType::WarningIcon, "Error", "Unable to allocate buffer memory.");

        }
    }
    
    // Calculate inital filter coefficients
    calculate_filter_coeffs(filterCutoff);
}

void EchoplexAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void EchoplexAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // I've added this to avoid people getting screaming feedback
    // when they first compile the plugin, but obviously you don't need to
    // this code if your algorithm already fills all the output channels.
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    float delayed_sample_idx;
    float delayed_sample;
    float current_sample;
    float output_sample;
    float output_sample_feedback;
    int samples_to_interpolate = 4;
    
    static int delay_change_counter = 0;
    int param_change_sample_delay = 50000;
    

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int channel = 0; channel < getNumInputChannels(); ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);

        for (int frame = 0; frame < buffer.getNumSamples(); ++frame)
        {
            
            // Detect change in parameters
            if (actual_delay != delay_in_samples)
            {
                if (delay_change_counter >= param_change_sample_delay)
                {
                    actual_delay = delay_in_samples;
                    delay_change_counter = 0;
                    printf("%f\n",actual_delay);
                }
                else
                {
                    delay_change_counter++;
                }
            }

            // Read the delayed sampled from the circular buffer
            delayed_sample_idx = mod (round((ringBuf[channel].current_index)-(actual_delay)),ringBuf[channel].length);
            
            // SORT THIS OUT
            delayed_sample = lagrange_interpolate(channel, (int)delayed_sample_idx + 1, samples_to_interpolate, 3+fmod(delayed_sample_idx,1));

            
            // Read the current sample from input buffer
            current_sample = channelData[frame];
            
            // If sound on sound mode is off, clear the current buffer sample
            if (! soundOnSound)
            {
                (ringBuf[channel].buffer)[ringBuf[channel].current_index] = 0;
            }
            else
            {
                current_sample += (ringBuf[channel].buffer)[ringBuf[channel].current_index] * 0.1;
            }
            
            // Calculate the output sample value
            output_sample = (dry_mix*current_sample) + (wet_mix*delayed_sample);
            
            // Tape filter
            if (filterIsIn)
            {
                // Apply filter to delayed sample
                // First coeff is a special case as the delayed sample is interpolated
                delayed_sample *= filter_coeffs[0];
                for (int x = 1; x <= filterOrder; x++) {
                    delayed_sample += filter_coeffs[x] * ringBuf[channel].buffer[mod(delayed_sample_idx - x, ringBuf[channel].length)];
                }
            }
            
            output_sample_feedback = (dry_mix*current_sample) + (wet_mix*delayed_sample);
            
            // Write current sample to ring buffer with feedback
            (ringBuf[channel].buffer)[ringBuf[channel].current_index] += current_sample + (feedback * output_sample_feedback);
            
            if (! bypass)
            {
                // If not bypassed, send output sample to audio output
                channelData[frame] = output_scale_factor * output_sample;
            }
            
            // Increment the circular buffer index
            ringBuf[channel].current_index = ((ringBuf[channel].current_index)+1)%ringBuf[channel].length;
        }
    }
}

//==============================================================================
bool EchoplexAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* EchoplexAudioProcessor::createEditor()
{
    return new EchoplexAudioProcessorEditor (*this);
}

//==============================================================================
void EchoplexAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void EchoplexAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

int EchoplexAudioProcessor::mod(int a, int b)
{
    int r = a%b;
    return (r<0 ? r+b : r);
}

float EchoplexAudioProcessor::lagrange_interpolate (int channel, int idx, int n, float p)
{
    
    // y is a pointer to the latest sample in the buffer
    // n is the number of samples to interpolate over (from y[-n] to y[0])
    // p is the x value to obtain the value of
    
    float sum = 0;
    
    for (int i=0; i < n; i++)
    {
        double temp = 1;
        
        for (int j=0; j < n; j++)
        {
            if (i==j)
                continue;
            else
                temp *= ((p-j)/(i-j));
        }

        sum += ringBuf[channel].buffer[mod(idx + i - n, ringBuf[channel].length)] * temp;
    }
    
    return sum;
}

void EchoplexAudioProcessor::calculate_filter_coeffs (float cutoff)
{
    float window_coefficient;
    float fourier_coefficient;
    double sample_rate = getSampleRate();

    for (int x = 0; x < filterOrder; x++)
    {
        // Hamming window
        window_coefficient = 0.54 - 0.46 * cos((2*M_PI*x)/filterOrder);
        
        fourier_coefficient = ((2*cutoff)/sample_rate) * sinc(((2*x - filterOrder)*cutoff)/sample_rate);
        
        filter_coeffs[x] = window_coefficient * fourier_coefficient;
        
    }
}

float EchoplexAudioProcessor::sinc (float x)
{
    return x ? sin(M_PI*x)/(M_PI*x) : 1;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EchoplexAudioProcessor();
}
