#pragma once
#include <complex>

/*
 * @class: Compressor
 * @namespace: ntdsp
 * @brief:
 *	// Models a compression algorithm applied to an audio signal given the coefficients:
 *		1. Threshold [ 0 to 1]
 *		2. Ratio [ -inf to inf ]
 *		TODO: 4. Knee  [ 0 to 1 ]
 *		3. Envelope [ 0 to 1 ]
 *		4. Parallel [ 0 to 1 ]
 *
 *	// The transfer in the amplitude domain is given by the function:
 *	//		(M(x) + (1-m)( x-((P(|x| - T ))+ ( 1 - P)(v((|x|- t )/R)))))
 *	// where
 *	//		x = amplitude value at current sample index ranging from -1 to + 1
 *	//	    V , T , P , and R are constant in the amplitude domain
 *	//			Therefore: must be reset in the time domain for a given sample index
 *	//			Note : V will increment by a set chunk size every sample index
 *	//				   T,P,R remain constant unless a user parameter change call occurs
 */

class Compressor {

	//parameters
	double threshold;
	double compress_ratio;
	double envelope;
	double limit_ratio;
	double mix_ratio;
	/*
	 * Processes the sample in the amplitude domain
	 */
	void process_attenuation(double& x_v)
	{
		x_v = (1 - mix_ratio) * (-((1 - limit_ratio) * envelope * (std::abs(x_v) - threshold) / mix_ratio) - limit_ratio * (abs(x_v) - threshold) + x_v) + mix_ratio * x_v;
	}

	void process_anvelope(double* x_p)
	{

	}
};