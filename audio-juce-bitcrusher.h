/*
  ==============================================================================

	DownSampler.h
	Created: 12 May 2019 2:55:02pm
	Author:  Anton Yahchenko

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

template<class in_it, class out_it>
out_it copy_every_n(in_it b, in_it e, out_it r, size_t n) {
	for (size_t i = distance(b, e) / n; i--; advance(b, n))
		*r++ = *b;
	return r;
}

class Crusher : public dsp::ProcessorBase
{
	using FIRFilterCascade = dsp::ProcessorDuplicator<dsp::FIR::Filter <float>, dsp::FIR::Coefficients <float>>;

	FIRFilterCascade ds_halfband_lp_;
	FIRFilterCascade rs_interp_lp_;
	AudioBuffer<float> ds_buffer_;
	AudioBuffer<float> wet_buffer_;
	//-------------------------------------------------------
	int ratio_;
	void setRatio(float ratio_);
	//-------------------------------------------------------

	double lsamplerate_;
	dsp::ProcessSpec rs_spec;
	dsp::ProcessSpec ds_spec;

	//-------------------------------------------------------
	void prepare(const dsp::ProcessSpec&spec) override
	{
		lsamplerate_ = spec.sampleRate;

		ds_buffer_.setSize(spec.numChannels, spec.maximumBlockSize);
		wet_buffer_.setSize(spec.numChannels, spec.maximumBlockSize);

		rs_spec = spec;
		ds_spec.sampleRate = spec.sampleRate / double(ratio_);
		ds_spec.maximumBlockSize = spec.maximumBlockSize / ratio_;
		ds_spec.numChannels = spec.numChannels;

		//Prepare filters
		ds_halfband_lp_.prepare(ds_spec);
		rs_interp_lp_.prepare(spec);
	};

	void process(const dsp::ProcessContextReplacing<float>& context) override
	{
		dsp::AudioBlock<float> dry_buffer(context.getInputBlock());

		const auto ds_nyquist = ds_spec.sampleRate / 2.0;
		const auto rs_nyquist = rs_spec.sampleRate / 2.0;

		for (int n = 0; n < rs_spec.numChannels; ++n)
		{
			wet_buffer_.copyFrom(n, 0, dry_buffer.getChannelPointer(n), 0, rs_spec.maximumBlockSize);
			copy_every_n(wet_buffer_.getReadPointer(n), wet_buffer_.getReadPointer(n) + wet_buffer_.getNumSamples(), ds_buffer_.getWritePointer(n), ratio_);

			ds_halfband_lp_.process(ds_buffer_);


		}

		
	};
};
class Downsampler : public dsp::ProcessorBase
{
	AudioBuffer<float> downsample_buffer_;
	dsp::FIR::Filter<float> downsample_lp_;
	LagrangeInterpolator interpolator_;
	double sampleRate;
public:
	float ratio_; // 0.0 - 1.0 normalized : 0 means no downsampling, 1 means 128 times downsampling
	void prepare(const dsp::ProcessSpec& spec) override
	{
		sampleRate = spec.sampleRate;
		downsample_buffer_.setSize(spec.numChannels, spec.maximumBlockSize);
		downsample_lp_.prepare(spec);
	};
	void process(const dsp::ProcessContextReplacing<float>& context) override
	{
		dsp::AudioBlock<float> input(context.getInputBlock());
		dsp::AudioBlock<float> testing(test);
		const auto n_in_channels = input.getNumChannels();
		const auto n_in_samples = input.getNumSamples();
		const auto in_sample_rate = sampleRate;
		const auto in_nyquist = sampleRate / 2.0;

		const auto ds_ratio = int(128.f * ratio_); // Number of input samples per output samples : Ranges from 1(No Downsampling) to 128 times downsampling.
													// Cannot == 0
		const auto ds_buff_size = n_in_samples / ds_ratio;	// Number of expected samples to keep in downsample buffer
		const auto ds_sample_rate = sampleRate / double(ds_ratio);
		const auto ds_nyquist = ds_sample_rate / 2.0;

		dsp::ProcessSpec ds_spec;
		ds_spec.numChannels = n_in_channels;
		ds_spec.sampleRate = ds_sample_rate;
		ds_spec.maximumBlockSize = ds_buff_size;

		downsample_lp_.prepare(ds_spec);
		downsample_lp_.coefficients = dsp::FilterDesign<float>::designFIRLowpassHalfBandEquirippleMethod(0.25f, -8.f);



		//Fill downsample buffer with every ds_ratio sample from input
		for (int channel = 0; channel < n_in_channels; channel++)
		{
			auto ds_buff = downsample_buffer_.getWritePointer(channel);
			auto in_buff = input.getChannelPointer(channel);

			for (auto s_i = 0, d_i = 0; d_i < ds_buff_size;)
			{
				ds_buff[d_i] = in_buff[s_i];
				s_i += ds_ratio;
				d_i++;
			}
		}

		//Perform Poly Phase Half Band Low Pass Filter at the Nyquist Frequency of the downsample sample rate
		downsample_lp_.process(downsample_buffer_);

		//Iterpolate back to original sample rate


		//Perform second Polyphase HalfBand at Nyquist of input





		//For each chunk
		//	//Fake Downsample - Sample Rate Remains
		//	//	-Run Low Pass at samplerate/ratio/2
		//	//	-Get initial sample
		//	//	-Set next (ratio - 1) samples to 0
		//	// 3 Options:
		//	//	-Return zeroed array [ i_s, 0, 0 , 0, i_s + chk_s, ...]
		//	//	-Return dropped sample [ i_s, i_s, i_s, i_s, i_s +chk]
		//	//  -Return interpolated sample [ i_s, i_s + k, i_s + 2k, i_s + 3k , i_s + chk_s, ... ] k = 
		//					// Linearly interpolate samples 
		//		// x = x0(t1 - t) + x1(t - t0) / t1 - t0
		//		// t0 = 0
	};
	void reset() override
	{
		downsample_lp_.reset();
	};
};
