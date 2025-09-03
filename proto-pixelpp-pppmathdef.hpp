#pragma once
#ifndef _PPP_MATHDEF
#define _PPP_MATHDEF

#include "pppcppdef.hpp"
namespace ppp {
	using byte_t = ::std::byte;
	using ::std::float_t; ///< Optimal system float.
	using ::std::double_t; ///< Optimal system double.

	///< Floating point evaluation method meta helper struct
	namespace mathdef::detail { 

		template<int FLOAT_EVAL_METHOD, typename = void>
		struct float_eval_method_helper {
			using system_float = float;
			using float_type = float;
			using double_type = double;
			using long_double_type = long double;

			float_eval_method_helper() = delete;
		};


		template<int FLOAT_EVAL_METHOD>
		struct float_eval_method_helper<FLOAT_EVAL_METHOD, std::enable_if_t<FLOAT_EVAL_METHOD == 1>> {
			using system_float = double;
			using float_type = double;
			using double_type = double;
			using long_double_type = long double;

			float_eval_method_helper() = delete;
		};


		template<int FLOAT_EVAL_METHOD>
		struct float_eval_method_helper<FLOAT_EVAL_METHOD, std::enable_if_t<FLOAT_EVAL_METHOD == 2>> {
			using system_float = long double;
			using float_type = long double;
			using double_type = long double;
			using long_double_type = long double;

			float_eval_method_helper() = delete;
		};


	} // end mathdef::detail

	///< Typedef which contains type info about the system's current floating point evaluation method.
	using float_eval_method = mathdef::detail::float_eval_method_helper<FLT_EVAL_METHOD>;

	///< Convenience enum for checking a floating point value's current state using std::fpclassify.
	const enum class float_state : int {
		INFINITE = FP_INFINITE
		, NANNUM = FP_NAN
		, NORNUM = FP_NORMAL
		, SNORNUM = FP_SUBNORMAL
		, ZERO = FP_ZERO
	};

	namespace mathdef {

		template<typename FloatT, typename = enable_if_t<std::is_floating_point_v<FloatT>>>
		struct pi_v {
			using value_type = FloatT;
			using type = pi_v;
			static constexpr value_type value = 3.141592653589793238462643383279502884197169399375105820974944592307816406286;
			constexpr operator value_type() const noexcept { return value; }
			constexpr value_type operator()() const noexcept { return value; }
		};

		template<>
		struct pi_v<float> {
			using value_type = float;
			using type = pi_v;
			static constexpr value_type value = 3.141592741f;
			constexpr operator value_type() const noexcept { return value; }
			constexpr value_type operator()() const noexcept { return value; }
		};

		template<>
		struct pi_v<double> {
			using value_type = double;
			using type = pi_v;
			static constexpr value_type value = 3.141592653589793116;
			constexpr operator value_type() const noexcept { return value; }
			constexpr value_type operator()() const noexcept { return value; }
		};

		template<>
		struct pi_v<long double> {
			using value_type = long double;
			using type = pi_v;
			static constexpr value_type value = 3.141592653589793238L;
			constexpr operator value_type() const noexcept { return value; }
			constexpr value_type operator()() const noexcept { return value; }
		};
	}

	///< Mathematical constants

	_CXSINLINE auto PI = mathdef::pi_v<double_t>::value;				///< Pi returned as the default system double
	_CXSINLINE float PI_F = mathdef::pi_v<float>::value;				///< Pi returned as a float
	_CXSINLINE double PI_D = mathdef::pi_v<double>::value;				///< Pi returned as a double
	_CXSINLINE long double PI_DL = mathdef::pi_v<long double>::value;	///< Pi returned as a long double

	_CXSINLINE float PPPM_2PI_F = PPPM_PI_F * 2.f;	///< 2*Pi
	_CXSINLINE double PPPM_2PI_D = PPPM_PI_D * 2.0;
	_CXSINLINE long double PPPM_2PI_DL = PPPM_PI_DL * 2.L;

	_CXSINLINE float PPPM_IPI_F = 1.f / PPPM_PI_F; ///< 1/Pi
	_CXSINLINE double PPPM_IPI_D = 1.0 / PPPM_PI_D;
	_CXSINLINE long double PPPM_IPI_DL = 1.l / PPPM_PI_DL;

} // end namespace ppp
#endif // _PPP_MATHDEF