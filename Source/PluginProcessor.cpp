#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <BinaryData.h>

SippinOnTheBeatAudioProcessor::SippinOnTheBeatAudioProcessor()
    : AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "SOTB_STATE", createParameterLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout SippinOnTheBeatAudioProcessor::createParameterLayout()
{
    using P = juce::AudioParameterFloat;
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> p;
    p.push_back(std::make_unique<P>("blend", "SAMPLE / SYNTH", 0.0f, 1.0f, 0.55f));
    p.push_back(std::make_unique<P>("cutoff", "FILTER", juce::NormalisableRange<float>(80.0f, 18000.0f, 1.0f, 0.35f), 12000.0f));
    p.push_back(std::make_unique<P>("attack", "ATTACK", 0.001f, 2.0f, 0.015f));
    p.push_back(std::make_unique<P>("decay", "DECAY", 0.01f, 3.0f, 0.35f));
    p.push_back(std::make_unique<P>("sustain", "SUSTAIN", 0.0f, 1.0f, 0.72f));
    p.push_back(std::make_unique<P>("release", "RELEASE", 0.01f, 5.0f, 0.65f));
    p.push_back(std::make_unique<P>("reverb", "REVERB", 0.0f, 1.0f, 0.22f));
    p.push_back(std::make_unique<P>("delay", "DELAY", 0.0f, 1.0f, 0.12f));
    p.push_back(std::make_unique<P>("output", "OUTPUT", 0.0f, 1.5f, 0.8f));
    p.push_back(std::make_unique<juce::AudioParameterChoice>("presetBank", "SAMPLE BANK", juce::StringArray { "VELVET", "CHROME", "DARK" }, 0));
    return { p.begin(), p.end() };
}

void SippinOnTheBeatAudioProcessor::prepareToPlay(double sr, int)
{
    currentSampleRate = sr;
    filter.reset();
    juce::dsp::ProcessSpec spec { sr, (juce::uint32)512, 2 };
    filter.prepare(spec);
    reverb.prepare(spec);
    filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    filter.setCutoffFrequency(12000.0f);
    juce::dsp::Reverb::Parameters rp; rp.roomSize = 0.55f; rp.damping = 0.45f; rp.wetLevel = 0.2f; rp.dryLevel = 0.8f;
    reverb.setParameters(rp); reverb.reset();
    delayBuffer.setSize(2, (int)(sr * 1.2)); delayBuffer.clear(); delayWrite = 0;
    const int sizes[] = { BinaryData::velvet_pluck_wavSize, BinaryData::chrome_bell_wavSize, BinaryData::dark_keys_wavSize };
    const char* data[] = { BinaryData::velvet_pluck_wav, BinaryData::chrome_bell_wav, BinaryData::dark_keys_wav };
    for (int i = 0; i < 3; ++i)
    {
        juce::WavAudioFormat wav;
        std::unique_ptr<juce::AudioFormatReader> reader(wav.createReaderFor(new juce::MemoryInputStream(data[i], (size_t)sizes[i], false), true));
        if (reader)
        {
            sampleBank[i].setSize(1, (int)reader->lengthInSamples);
            reader->read(&sampleBank[i], 0, sampleBank[i].getNumSamples(), 0, true, false);
        }
    }
}

void SippinOnTheBeatAudioProcessor::releaseResources() {}
bool SippinOnTheBeatAudioProcessor::isBusesLayoutSupported(const BusesLayout& l) const
{ return l.getMainOutputChannelSet() == juce::AudioChannelSet::stereo() || l.getMainOutputChannelSet() == juce::AudioChannelSet::mono(); }

void SippinOnTheBeatAudioProcessor::noteOn(int note, float velocity)
{
    Voice* v = nullptr;
    for (auto& voice : voices) if (!voice.active) { v = &voice; break; }
    if (v == nullptr) v = &*std::min_element(std::begin(voices), std::end(voices), [](const Voice& a, const Voice& b) { return a.age > b.age; });
    *v = Voice{}; v->active = true; v->note = note; v->velocity = velocity; v->age = 0;
}
void SippinOnTheBeatAudioProcessor::noteOff(int note)
{ for (auto& v : voices) if (v.active && v.note == note) v.releasing = true; }

float SippinOnTheBeatAudioProcessor::renderVoice(Voice& v, float blend, float attack, float decay,
                                                  float sustain, float release, float, int)
{
    if (!v.active) return 0.0f;
    ++v.age;
    const float attackSamples = juce::jmax(1.0f, attack * (float)currentSampleRate);
    const float decaySamples = juce::jmax(1.0f, decay * (float)currentSampleRate);
    if (!v.releasing)
    {
        if (v.env < 1.0f) v.env = juce::jmin(1.0f, v.env + 1.0f / attackSamples);
        else v.env = juce::jmax(sustain, v.env - (1.0f - sustain) / decaySamples);
    }
    else
    {
        v.env -= 1.0f / juce::jmax(1.0f, release * (float)currentSampleRate);
        if (v.env <= 0.0001f) { v.active = false; return 0.0f; }
    }
    const float hz = 440.0f * std::pow(2.0f, (v.note - 69) / 12.0f);
    const float increment = hz / (float)currentSampleRate;
    v.phase += increment; if (v.phase >= 1.0f) v.phase -= 1.0f;
    const float sine = std::sin(juce::MathConstants<float>::twoPi * v.phase);
    const float saw = 2.0f * v.phase - 1.0f;
    const float synth = 0.72f * sine + 0.28f * saw;
    const int bankIndex = (int)parameters.getRawParameterValue("presetBank")->load();
    const auto& bank = sampleBank[juce::jlimit(0, 2, bankIndex)];
    float sample = 0.0f;
    if (bank.getNumSamples() > 1)
    {
        const float speed = hz / 261.6256f;
        const int pos = (int)v.samplePosition;
        if (pos >= 0 && pos < bank.getNumSamples() - 1)
        {
            const float frac = v.samplePosition - (float)pos;
            sample = bank.getSample(0, pos) * (1.0f - frac) + bank.getSample(0, pos + 1) * frac;
            v.samplePosition += speed;
        }
    }
    return ((1.0f - blend) * sample + blend * synth) * v.env * v.velocity * 0.22f;
}

void SippinOnTheBeatAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();
    for (const auto metadata : midi)
    {
        const auto msg = metadata.getMessage();
        if (msg.isNoteOn()) noteOn(msg.getNoteNumber(), msg.getFloatVelocity());
        else if (msg.isNoteOff()) noteOff(msg.getNoteNumber());
        else if (msg.isAllNotesOff() || msg.isAllSoundOff()) for (auto& v : voices) v.active = false;
    }
    const float blend = parameters.getRawParameterValue("blend")->load();
    const float attack = parameters.getRawParameterValue("attack")->load();
    const float decay = parameters.getRawParameterValue("decay")->load();
    const float sustain = parameters.getRawParameterValue("sustain")->load();
    const float release = parameters.getRawParameterValue("release")->load();
    const float cutoff = parameters.getRawParameterValue("cutoff")->load();
    const float out = parameters.getRawParameterValue("output")->load();
    const float rv = parameters.getRawParameterValue("reverb")->load();
    const float dl = parameters.getRawParameterValue("delay")->load();
    filter.setCutoffFrequency(cutoff);
    const int channels = buffer.getNumChannels(), n = buffer.getNumSamples();
    for (int i = 0; i < n; ++i)
    {
        float s = 0.0f;
        for (auto& v : voices) s += renderVoice(v, blend, attack, decay, sustain, release, cutoff, i);
        s = filter.processSample(0, s);
        const int delayLen = delayBuffer.getNumSamples();
        float delayed = delayLen > 0 ? delayBuffer.getSample(0, delayWrite) : 0.0f;
        if (delayLen > 0) { delayBuffer.setSample(0, delayWrite, s + delayed * 0.28f); delayWrite = (delayWrite + 1) % delayLen; }
        s = s * (1.0f - rv * 0.35f) + delayed * dl * 0.45f;
        for (int ch = 0; ch < channels; ++ch) buffer.setSample(ch, i, s * out);
    }
    if (channels == 2)
    {
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        reverb.setParameters({ 0.5f, 0.45f, rv * 0.55f, 1.0f - rv * 0.2f, 1.0f, 0.0f });
        reverb.process(context);
    }
}

void SippinOnTheBeatAudioProcessor::loadPreset(int index)
{
    auto set = [this](const char* id, float value) { if (auto* p = parameters.getParameter(id)) p->setValueNotifyingHost(p->getNormalisableRange().convertTo0to1(value)); };
    switch (index)
    {
        case 0: set("blend", 0.35f); set("cutoff", 6500); set("attack", 0.08f); set("decay", 0.8f); set("sustain", 0.5f); set("release", 1.8f); set("reverb", 0.55f); set("delay", 0.28f); break;
        case 1: set("blend", 0.78f); set("cutoff", 14500); set("attack", 0.005f); set("decay", 0.3f); set("sustain", 0.8f); set("release", 0.45f); set("reverb", 0.18f); set("delay", 0.1f); break;
        case 2: set("blend", 0.2f); set("cutoff", 4800); set("attack", 0.002f); set("decay", 1.2f); set("sustain", 0.0f); set("release", 0.8f); set("reverb", 0.35f); set("delay", 0.22f); break;
        case 3: set("blend", 0.9f); set("cutoff", 11000); set("attack", 0.001f); set("decay", 0.12f); set("sustain", 0.95f); set("release", 0.18f); set("reverb", 0.08f); set("delay", 0.05f); break;
        default: break;
    }
}

juce::AudioProcessorEditor* SippinOnTheBeatAudioProcessor::createEditor() { return new SippinOnTheBeatAudioProcessorEditor(*this); }
void SippinOnTheBeatAudioProcessor::getStateInformation(juce::MemoryBlock& dest) { auto state = parameters.copyState(); std::unique_ptr<juce::XmlElement> xml(state.createXml()); copyXmlToBinary(*xml, dest); }
void SippinOnTheBeatAudioProcessor::setStateInformation(const void* data, int size) { std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, size)); if (xml && xml->hasTagName(parameters.state.getType())) parameters.replaceState(juce::ValueTree::fromXml(*xml)); }

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new SippinOnTheBeatAudioProcessor(); }
