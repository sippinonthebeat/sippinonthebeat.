#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class SippinOnTheBeatAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit SippinOnTheBeatAudioProcessorEditor(SippinOnTheBeatAudioProcessor&);
    ~SippinOnTheBeatAudioProcessorEditor() override;
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    struct ChromeLookAndFeel : juce::LookAndFeel_V4
    {
        void drawRotarySlider(juce::Graphics&, int, int, int, int, float, float, float, juce::Slider&) override;
    } chrome;
    SippinOnTheBeatAudioProcessor& processor;
    juce::ComboBox presets;
    juce::Label logo, subtitle, presetLabel;
    juce::Slider blend, cutoff, attack, decay, sustain, release, reverb, delay, output;
    juce::Label labels[9];
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> attachments[9];
    void setupSlider(juce::Slider&, int, const juce::String&);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SippinOnTheBeatAudioProcessorEditor)
};
