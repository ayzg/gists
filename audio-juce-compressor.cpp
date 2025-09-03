/*
  ==============================================================================

	Compressor.cpp
	Created: 10 Feb 2019 3:04:24am
	Author:  zbook15

  ==============================================================================
*/

#include "Compressor.h"
#include "ntUtil.h"

// !class: CompressorState
Compressor::CompressorState::CompressorState(const float& sample_rate) : sample_rate_(sample_rate)
{
	state_ = c_state::rest;		//Initial State: Resting
	threshold_ = 1;				// 1  = 0db threshold
	ratio_ = 1;					// 1:1 Ratio = No Compression
	envelope_ = 1;				// 1.0 Envelope = Full Mask (original signal only)
	envelope_increment_ = 0;	// -1 : Unactivated Envelope | out of range exception if called
	po_intersect_idx_ = 0;		// off
	po_interrupt_idx_ = 0;		// off
	attack_.set_samples(0);		// samples 0 = instant
	attack_.set_tension(1.f);	// tension 1 = flat,
	release_.set_samples(0);	// samples 0 = instant
	release_.set_tension(2.f);	// tension = 2 (second order)
}
void Compressor::CompressorState::begin_a_envelope()
{
	envelope_ = attack_.get_fast_linear(0);
	envelope_increment_ = 0;
	is_atk_ = true;
}
bool Compressor::CompressorState::increment_a_envelope()
{
	if (envelope_increment_ < attack_.samples())
	{
		envelope_increment_++;
		envelope_ = attack_.get_fast_linear(envelope_increment_);
		return true;
	}
	else
	{
		is_atk_ = false;
		state_ = c_state::release;
		return false;
	}
}
void Compressor::CompressorState::begin_r_envelope()
{
	envelope_ = release_.get(release_.samples());
	envelope_increment_ = 0;
	is_rls_ = true;
}
bool Compressor::CompressorState::increment_r_envelope()
{
	if (envelope_increment_ < release_.samples())
	{
		envelope_increment_++;
		envelope_ = release_.get(release_.samples() - envelope_increment_);
		return true;
	}
	else
	{
		is_rls_ = false;
		state_ = c_state::rest;
		return false;
	}
}
void Compressor::CompressorState::reset(const float& sample_rate)
{
	set_srate(sample_rate);

	//Initial State: Resting
	state_ = c_state::rest;
	threshold_ = 1; // 1  = 0db threshold
	ratio_ = 1; // 1:1 Ratio = No Compression
	envelope_ = 1; // 1.0 Envelope = Full Mask (original signal only)
	envelope_increment_ = 0; // -1 : Unactivated Envelope | out of range exception if called
	po_intersect_idx_ = 0; // off
	po_interrupt_idx_ = 0; // off


	//Attack inital state: tension 1 = flat, samples 0 = instant
	attack_.set_samples(0);
	attack_.set_tension(1.f);

	//Release initial state: tension = 2 (second order), samples 0 = instant
	attack_.set_samples(0);
	attack_.set_tension(2.f);
}

// !class: Compressor
void Compressor::process_sample(float& sample) const
{
	// Amplitude Domain Algorithm // Output = sign(x) * ((abs(Input) - Threshold)/Ratio + Threshold)
	sample = sign(sample)*																								// Sign of the sample
		((std::abs(sample) - curr_state_.get_thresh()) / curr_state_.get_ratio() * (1 - curr_state_.get_envelope())		// Compressed Value above threshold
			+ (abs(sample) - curr_state_.get_thresh()) * curr_state_.get_envelope()										// Uncompressed value above threshold
			+ curr_state_.get_thresh());																				// Threshold value
}
void Compressor::reset(const float& sample_rate)
{
	curr_state_.reset(sample_rate);
}
void Compressor::prepare(const dsp::ProcessSpec& spec) noexcept
{
	curr_state_.set_srate(spec.sampleRate);
}
void Compressor::update_coef(const float& threshold_db,const float& ratio, const float& attack_ms, const float& release_ms)
{
	curr_state_.set_thresh(Decibels::decibelsToGain(threshold_db));
	curr_state_.set_ratio(ratio);
	curr_state_.set_attack_ms(attack_ms);
	curr_state_.set_release_ms(release_ms);
}
void Compressor::process(dsp::ProcessContextReplacing<float> context, dsp::AudioBlock<float>& in_signal)
{
	//Assume 2 channels //
	auto& in_signal = context.getInputBlock();
	float* leftChannelData = in_signal.getChannelPointer(0);
	float* rightChannelData = in_signal.getChannelPointer(1);

	for (unsigned int this_sample_idx = 0; this_sample_idx < in_signal.getNumSamples();) {

		switch (curr_state_.state_)
		{
		case CompressorState::c_state::rest:
		{
			//While In Signal < T : Bypass Process, Search for next Point of Collision
			if (leftChannelData[this_sample_idx] > curr_state_.get_thresh() | rightChannelData[this_sample_idx] > curr_state_.get_thresh())
				{
					//Hold a sample
					curr_state_.set_pois(this_sample_idx); // Point of intersect
					curr_state_.set_state(CompressorState::c_state::attack);
					break;
				}

			//No collision occurs : bypass audio process : continue rest
			this_sample_idx++;
			break;
		}
		case CompressorState::c_state::knee: break;
		case CompressorState::c_state::attack:
		{
			if (!curr_state_.is_attack())
			{
				curr_state_.begin_a_envelope();
			}
			else
			{
				if (!curr_state_.increment_a_envelope()) //Returns false, sets state to release once envelope reaches max
				{
					break; // Do not process max value, do not increment, envelope is not set
				}
			}
			process_sample(leftChannelData[this_sample_idx]);
			process_sample(rightChannelData[this_sample_idx]);
			this_sample_idx++;
			break;
		}
		case CompressorState::c_state::release:
		{
			if (!curr_state_.is_release())
			{
				curr_state_.begin_r_envelope();
			}
			else
			{
				if (!curr_state_.increment_r_envelope()) //Returns false, sets state to rest once envelope reaches max
				{
					break; // Do not process max value, do not increment, envelope is not set
				}
			}
			process_sample(leftChannelData[this_sample_idx]);
			process_sample(rightChannelData[this_sample_idx]);
			this_sample_idx++;
			break;
		}
		case CompressorState::c_state::makeup: break;
		}
	}
}
