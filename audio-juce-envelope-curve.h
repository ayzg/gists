 /*
  ==============================================================================

	Envelope.h
	Created: 30 Jan 2019 1:04:20am
	Author:  zbook15

  ==============================================================================
*/

#pragma once

#include <array>
#include <cassert>
#include <exception>



// Represents a curve over a number of samples, a changeable equation, and range of 0.0 to 1.0
class AudioCurve
{
	unsigned int samples_;
	double increment_;
	double tension_; // 0.0 minimum

public:

	AudioCurve() : samples_(0), increment_(0.0), tension_(0.0) {};

	unsigned int samples() const { return samples_; }

	void reset(unsigned int samples, double curve_tension)
	{
		if (curve_tension < 0.0) { curve_tension = 0.0; }
		samples_ = samples;
		tension_ = curve_tension;
		increment_ = 1.0 / samples_;
	}

	void set_tension(double curve_tension)
	{
		if (curve_tension < 0.0) { curve_tension = 0.0; }
		tension_ = curve_tension;
	}

	double get(unsigned int sample_index) const { return std::pow((sample_index * increment_), tension_); }

};

class AudioEnvelope
{
	enum curve_id { attack, decay ,sustain, release };

	unsigned int samples_{};
	std::array<AudioCurve, 4> curves_;
	unsigned int attack_samples_{}, decay_samples_{}, sustain_samples_{}, release_samples_{}, sustain_volume_{};
	double attack_tension_{ 1.0 },decay_tension_{ 1.0 },release_tension_{ 1.0 };

public:
	AudioEnvelope() = default;

	void set_attack_tension(double tension)
	{
		if (attack_tension_ < 0.0) { attack_tension_ = 0.0; }
		curves_[attack].set_tension(attack_tension_);
	}
	void set_decay_tension(double tension)
	{
		if (decay_tension_ < 0.0) { decay_tension_ = 0.0; }
		curves_[decay].set_tension(decay_tension_);
	}
	void set_release_tension(double tension)
	{
		if (release_tension_ < 0.0) { release_tension_ = 0.0; }
		curves_[release].set_tension(attack_tension_);
	}

	void reset()
	{
			curves_[attack].reset(attack_samples_, attack_tension_);
			curves_[decay].reset(decay_samples_, decay_tension_);
			curves_[sustain].reset(sustain_samples_, 0.0 );
			curves_[release].reset(release_samples_, release_tension_);

			samples_ = attack_samples_ + decay_samples_ + sustain_samples_ + release_samples_;
	}

	void update_coefficients(unsigned int attack_samples, double attack_tension, unsigned int decay_samples,
		double decay_tension, unsigned int sustain_samples, double sustain_volume, unsigned int release_samples, double release_tension)
	{
		attack_samples_ = attack_samples;
		attack_tension_ = attack_tension;
		release_samples_ = release_samples;
		release_tension_ = release_tension;
		decay_samples_ = decay_samples;
		decay_tension_ = decay_tension;
		sustain_samples_ = sustain_samples;
	}

	double get(unsigned int sample_index)
	{
		if(sample_index < decay_sample_idx())
			return curves_[attack].get(sample_index);
		if(decay_sample_idx() <= sample_index < sustain_sample_idx())
			return curves_[decay].get(sample_index - decay_sample_idx());
		if (sustain_sample_idx() <= sample_index < release_sample_idx())
			return curves_[sustain].get(sample_index - sustain_sample_idx()) - (1.0 - sustain_volume_);
		if (release_sample_idx() <= sample_index < release_sample_idx() + release_samples_ )
			return curves_[release].get(sample_index - release_sample_idx());

		return 0.0;
	}

	unsigned int max_samples() const { return samples_; }
	unsigned int decay_sample_idx() const { return attack_samples_; }
	unsigned int sustain_sample_idx() const { return attack_samples_ + decay_samples_; }
	unsigned int release_sample_idx() const { return attack_samples_ + decay_samples_ + sustain_samples_; }
};