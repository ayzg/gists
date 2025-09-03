/*
  ==============================================================================

	FractalisVoice.h
	Created: 14 Mar 2019 2:38:58pm
	Author:  zbook15

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "FractalisSound.h"

class NtOscilator
{
	double frequency;

};

class MidiEnvelope
{
	enum stage{rest,knee, attack1, attack2, attack3, decay, sustain, release1, release2, release3};
};

class FractalisVoice : public SynthesiserVoice
{
public:
	FractalisVoice(double offset):offset(offset){}
	double currentAngle{};
	double level{};
	double tailOff{};
	double angleDelta{};
	double frequency{};
	double offset{};
public:
	bool canPlaySound(SynthesiserSound* sound) override
	{
		return dynamic_cast<FractalisSound*>(sound) != nullptr;
	}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override
	{
		frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);

		currentAngle = 0.0;
		level = velocity * 0.15;
		tailOff = 0.0;
		auto cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber)+offset;
		auto cyclesPerSample = cyclesPerSecond / getSampleRate();
		angleDelta = cyclesPerSample * 2.0 * MathConstants<double>::pi;
	}


	void stopNote(float velocity, bool allowTailOff) override
	{
		if (allowTailOff)
		{
			if (tailOff == 0.0)
				tailOff = 1.0;
		}
		else
		{
			clearCurrentNote();
			angleDelta = 0.0;
		}
	}

	void pitchWheelMoved(int newPitchWheelValue) override{};
	void controllerMoved(int controllerNumber, int newControllerValue) override{};
	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
	{
		if (angleDelta != 0.0)
		{
			if (tailOff > 0.0) // [7]
			{
				while (--numSamples >= 0)
				{
					auto currentSample = (float)(std::sin(currentAngle) * level * tailOff);
					for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
						outputBuffer.addSample(i, startSample, currentSample);
					currentAngle += angleDelta;
					++startSample;
					tailOff *= 0.99; // [8]
					if (tailOff <= 0.005)
					{
						clearCurrentNote(); // [9]
						angleDelta = 0.0;
						break;
					}
				}
			}
			else
			{
				while (--numSamples >= 0) // [6]
				{
					auto currentSample = (float)(std::sin(currentAngle) * level);
					for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
						outputBuffer.addSample(i, startSample, currentSample);
					currentAngle += angleDelta;
					++startSample;
				}
			}
		}
	};
};

class FractalisSynth : public Synthesiser
{
public:

	FractalisSynth() 
	{
		clearVoices();
		for (int i = 0; i < 200; ++i)
		{
			addVoice(new FractalisVoice());
		}
		clearSounds();
		addSound(new FractalisSound);
	}
};