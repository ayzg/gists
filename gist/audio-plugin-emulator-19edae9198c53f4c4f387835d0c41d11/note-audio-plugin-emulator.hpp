#pragma once
#pragma once
#include <boost/circular_buffer.hpp>
#include <vector>
#include <iostream>

class AudioPlugin
{
public:
	AudioPlugin();
	~AudioPlugin();

	using MultiChannelBuffer = std::vector<boost::circular_buffer<double>>;
	using MidiCommandBuffer = boost::circular_buffer<int>;


	MultiChannelBuffer input_buffer;
	MultiChannelBuffer output_buffer;
	double* source;
	size_t source_it;
	size_t source_size;
	double last_ret_val;
	size_t buffer_src_idx{};
	unsigned int sample_rate_;
	unsigned int num_in_channels;
	void set_src(double** source);

	void process();//MultiChannelBuffer& input_buffer/*,MidiCommandBuffer& midi_buffer*/);

	void set_source(double* src)
	{
		source = src;
	}

	void set_input_channels(unsigned int channels, unsigned int sampleRate)
	{
		sample_rate_ = sampleRate;
		num_in_channels = channels;

		while (input_buffer.size() < channels)
		{
			input_buffer.emplace_back(boost::circular_buffer<double>(sample_rate_));
		}

		for (auto& channel : input_buffer)
		{
			channel.set_capacity(sampleRate);
		}
	}

	void set_source_size(const size_t size)
	{
		source_size = size;
	}

	void get_next_out();

	void get_next_block();

	const double& get_next_sample()
	{
		//Last element
		if (source_it >= source_size)
		{
			source_it = 0;
			return source[source_it];
		}

		last_ret_val = source[source_it];
		source_it++;
		return last_ret_val;
	}
};



AudioPlugin::AudioPlugin()
{
}


AudioPlugin::~AudioPlugin()
{
}

void AudioPlugin::set_src(double** source)
{
}

void AudioPlugin::process()
{
	for (auto& channel : input_buffer)
	{
		for (auto& sample : channel)
		{
			sample *= 2;
		}
	}

}

void AudioPlugin::get_next_out()
{
}

void AudioPlugin::get_next_block()
{
	if (buffer_src_idx + sample_rate_ * num_in_channels > source_size)
	{
		for (unsigned int sample_idx = 0; sample_idx < sample_rate_; ++sample_idx)
		{
			for (unsigned int channel_idx = 0; channel_idx < num_in_channels; ++channel_idx)
			{
				input_buffer[channel_idx][sample_idx] = get_next_sample();
			}
		}
	}

	for (unsigned int channel_idx = 0; channel_idx < num_in_channels; ++channel_idx)
	{
		{
			for (unsigned int sample_idx = 0; sample_idx < sample_rate_; ++sample_idx)
			{
				input_buffer[channel_idx][sample_idx] = get_next_sample();
			}
		}
	}
}
