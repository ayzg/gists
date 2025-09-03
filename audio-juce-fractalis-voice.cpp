/*
  ==============================================================================

	FractalisVoice.cpp
	Created: 14 Mar 2019 2:38:58pm
	Author:  zbook15

  ==============================================================================
*/

#include "FractalisVoice.h"
#include "../JuceLibraryCode/JuceHeader.h"

struct Wave
{
	AudioBuffer<float> data;
	
};


class Wavetable
{
	const size_t size_;
	HeapBlock<Wave> waves_;
public:
	Wavetable(size_t size):size_(size), waves_(size_){}
};
