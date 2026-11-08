cmake_minimum_required(VERSION 3.22)
project(SIPPINONTHEBEAT VERSION 0.1.0)

include(FetchContent)
FetchContent_Declare(JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.8
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(JUCE)

juce_add_binary_data(SOTBAssets SOURCES
    Assets/velvet_pluck.wav
    Assets/chrome_bell.wav
    Assets/dark_keys.wav)

juce_add_plugin(SIPPINONTHEBEAT
    COMPANY_NAME "SippinOnTheBeat"
    BUNDLE_ID "com.sippinonthebeat.instrument"
    PLUGIN_MANUFACTURER_CODE Sipn
    PLUGIN_CODE SOTB
    FORMATS VST3
    PRODUCT_NAME "SIPPINONTHEBEAT"
    DESCRIPTION "Hybrid Y2K instrument"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD FALSE)

target_sources(SIPPINONTHEBEAT PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h)

target_compile_features(SIPPINONTHEBEAT PRIVATE cxx_std_17)
target_compile_definitions(SIPPINONTHEBEAT PUBLIC
    JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0)
target_link_libraries(SIPPINONTHEBEAT PRIVATE
    SOTBAssets
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_audio_processors
    juce::juce_gui_extra
    PUBLIC juce::juce_recommended_config_flags
           juce::juce_recommended_lto_flags
           juce::juce_recommended_warning_flags)
