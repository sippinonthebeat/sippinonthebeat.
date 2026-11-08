#include "PluginEditor.h"

void SippinOnTheBeatAudioProcessorEditor::ChromeLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int w, int h, float pos, float start, float end, juce::Slider&)
{
    auto bounds = juce::Rectangle<float>((float)x, (float)y, (float)w, (float)h).reduced(8.0f);
    auto r = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.5f;
    auto centre = bounds.getCentre();
    g.setColour(juce::Colour(0xff11151b)); g.fillEllipse(bounds);
    g.setGradientFill(juce::ColourGradient(juce::Colour(0xfff5f7fa), bounds.getX(), bounds.getY(), juce::Colour(0xff555e6b), bounds.getRight(), bounds.getBottom(), false));
    g.drawEllipse(bounds, 2.0f);
    auto inner = bounds.reduced(r * 0.18f);
    g.setColour(juce::Colour(0xff242b35)); g.fillEllipse(inner);
    g.setColour(juce::Colour(0xffe7edf5)); g.drawEllipse(inner, 1.0f);
    const float angle = start + pos * (end - start);
    juce::Path p; p.addRoundedRectangle(-2.0f, -r * 0.68f, 4.0f, r * 0.32f, 2.0f);
    g.setColour(juce::Colour(0xffef3340)); g.fillPath(p, juce::AffineTransform::rotation(angle).translated(centre.x, centre.y));
    g.setColour(juce::Colour(0x557f8fa6)); g.drawEllipse(bounds.expanded(2.0f), 1.0f);
}

SippinOnTheBeatAudioProcessorEditor::SippinOnTheBeatAudioProcessorEditor(SippinOnTheBeatAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setLookAndFeel(&chrome); setSize(820, 520);
    logo.setText("SIPPINONTHEBEAT", juce::dontSendNotification); logo.setFont(juce::Font(juce::FontOptions(31.0f, juce::Font::bold))); logo.setColour(juce::Label::textColourId, juce::Colour(0xfff2f5fa)); addAndMakeVisible(logo);
    subtitle.setText("HYBRID INSTRUMENT  //  Y2K SOUND LAB", juce::dontSendNotification); subtitle.setFont(juce::Font(juce::FontOptions(11.0f, juce::Font::bold))); subtitle.setColour(juce::Label::textColourId, juce::Colour(0xff9da9b8)); addAndMakeVisible(subtitle);
    presetLabel.setText("PRESET", juce::dontSendNotification); presetLabel.setColour(juce::Label::textColourId, juce::Colour(0xffaeb9c7)); addAndMakeVisible(presetLabel);
    presets.addItem("01  VELVET AFTERHOURS", 1); presets.addItem("02  CHROME ANGEL", 2); presets.addItem("03  DARK CITY KEYS", 3); presets.addItem("04  PLUG STAR", 4);
    presets.setSelectedId(1); presets.onChange = [this] { processor.loadPreset(presets.getSelectedId() - 1); }; addAndMakeVisible(presets);
    const char* ids[] = { "blend", "cutoff", "attack", "decay", "sustain", "release", "reverb", "delay", "output" };
    const char* names[] = { "SAMPLE / SYNTH", "FILTER", "ATTACK", "DECAY", "SUSTAIN", "RELEASE", "REVERB", "DELAY", "OUTPUT" };
    juce::Slider* sliders[] = { &blend,&cutoff,&attack,&decay,&sustain,&release,&reverb,&delay,&output };
    for (int i=0;i<9;++i) { setupSlider(*sliders[i], i, names[i]); attachments[i] = std::make_unique<Attachment>(processor.parameters, ids[i], *sliders[i]); }
}

void SippinOnTheBeatAudioProcessorEditor::setupSlider(juce::Slider& s, int i, const juce::String& name)
{
    s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag); s.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 68, 18); s.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xffdbe3ed)); s.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack); s.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff151b23)); addAndMakeVisible(s);
    labels[i].setText(name, juce::dontSendNotification); labels[i].setJustificationType(juce::Justification::centred); labels[i].setFont(juce::Font(juce::FontOptions(10.0f, juce::Font::bold))); labels[i].setColour(juce::Label::textColourId, juce::Colour(0xffaeb9c7)); addAndMakeVisible(labels[i]);
}

void SippinOnTheBeatAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff090c11));
    auto area = getLocalBounds().toFloat();
    g.setGradientFill(juce::ColourGradient(juce::Colour(0xff252d38), 0, 0, juce::Colour(0xff10151d), (float)getWidth(), (float)getHeight(), false));
    g.fillRoundedRectangle(area.reduced(8), 18.0f);
    g.setColour(juce::Colour(0xff596575)); g.drawRoundedRectangle(area.reduced(8), 18.0f, 1.5f);
    g.setColour(juce::Colour(0xffe8edf4)); g.drawLine(24.0f, 91.0f, (float)getWidth()-24.0f, 91.0f, 1.0f);
    g.setColour(juce::Colour(0xffef3340)); g.fillRoundedRectangle(24.0f, 108.0f, 4.0f, 20.0f, 2.0f);
    g.setColour(juce::Colour(0xff7f8b9b)); g.setFont(juce::Font(juce::FontOptions(10.0f, juce::Font::bold))); g.drawText("SAMPLE ENGINE  +  SYNTH ENGINE", 38, 108, 250, 20, juce::Justification::centredLeft);
    g.setColour(juce::Colour(0x223be0ff)); g.drawRoundedRectangle(22.0f, 145.0f, (float)getWidth()-44.0f, 335.0f, 12.0f, 1.0f);
    g.setColour(juce::Colour(0xffef3340)); g.setFont(juce::Font(juce::FontOptions(9.0f, juce::Font::bold))); g.drawText("SOTB  //  VERSION 0.1", getWidth()-180, getHeight()-25, 145, 12, juce::Justification::centredRight);
}

void SippinOnTheBeatAudioProcessorEditor::resized()
{
    logo.setBounds(24, 18, 430, 43); subtitle.setBounds(27, 58, 360, 20); presetLabel.setBounds(535, 25, 65, 20); presets.setBounds(595, 23, 195, 27);
    juce::Slider* sliders[] = { &blend,&cutoff,&attack,&decay,&sustain,&release,&reverb,&delay,&output };
    const int startX=30, startY=165, cellW=145, cellH=145;
    for (int i=0;i<9;++i) { int row=i/5, col=i%5; int x=startX+col*150; int y=startY+row*150; sliders[i]->setBounds(x,y+17,130,112); labels[i].setBounds(x,y,130,18); }
}

SippinOnTheBeatAudioProcessorEditor::~SippinOnTheBeatAudioProcessorEditor() { setLookAndFeel(nullptr); }
