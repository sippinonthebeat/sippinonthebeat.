#pragma once
#include <JuceHeader.h>

class SippinOnTheBeatAudioProcessor final : public juce::AudioProcessor
{
public:
    SippinOnTheBeatAudioProcessor();
    ~SippinOnTheBeatAudioProcessor() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return "SIPPINONTHEBEAT"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 3.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;

    juce::AudioProcessorValueTreeState parameters;
    void loadPreset(int index);

private:
    struct Voice
    {
        bool active = false, releasing = false;
        int note = 60;
        float velocity = 0.0f, phase = 0.0f, samplePosition = 0.0f;
        float env = 0.0f, releaseStep = 0.0f;
        int age = 0;
    };
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void noteOn(int note, float velocity);
    void noteOff(int note);
    float renderVoice(Voice& voice, float blend, float attack, float decay,
                      float sustain, float release, float cutoff, int sampleIndex);

    static constexpr int maxVoices = 16;
    Voice voices[maxVoices];
    juce::AudioBuffer<float> sampleBank[3];
    double currentSampleRate = 44100.0;
    juce::dsp::StateVariableTPTFilter<float> filter;
    juce::dsp::Reverb reverb;
    juce::AudioBuffer<float> delayBuffer;
    int delayWrite = 0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SippinOnTheBeatAudioProcessor)
};
