#pragma once
#pragma once
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <map>
#include <tuple>

class Color
{
public:
	virtual ~Color() = 0;
};

class HSL;
class HEX;

class RGB : virtual public Color
{
public:
	float red;
	float green;
	float blue;

	RGB(float r, float g, float b /* 0.f -> 1.0f */) : red(r), green(g), blue(b), Color() {}
	operator HSL() const;
};

class RGB8 : virtual public Color
{
public:
	int red;
	int green;
	int blue;

	RGB8(int r, int g, int b /* 0 -> 255 */) : red(r), green(g), blue(b), Color() {}
};

class HSL : virtual public Color
{
public:
	float hue;
	float saturation;
	float luminosity;

	HSL(float h /* 0 -> 360 */, float s/* 0.f -> 1.0f */, float l/* 0.f -> 1.0f */) : hue(h), saturation(s), luminosity(l), Color() {}
	operator RGB() const;
};

RGB::operator HSL() const
{
	float h;
	float s;
	float l;

	float max = std::max({ red, green, blue });
	float min = std::min({ red, green, blue });

	l = (min + max) / 2.f;

	if (max == min)
	{
		s = 0.f;
		h = 0.f;
	}
	else
	{
		if (l < 0.5f)
		{
			s = (max - min) / (max + min);
		}
		else if (l >= 0.5f)
		{
			s = (max - min) / (2.0 - max - min);
		}
	}

	if (max == red)
	{
		h = (green - blue) / (max - min);
	}
	else if (max == green)
	{
		h = 2.0 + (blue - red) / (max - min);
	}
	else if (max == blue)
	{
		h = 4.0 + (red - green) / (max - min);
	}

	h *= 60.f;

	if (std::signbit(h))
		h += 360.0;

	return { h, s, l };
}

HSL::operator RGB() const
{
	float r; // Red Out
	float g; // Green Out
	float b; // Blue Out
	float t_1; // Intermediate Var 1
	float t_2; // Intermediate Var 2
	float t_h; // 0.1 to 1.0 range Hue
	float t_r; // Temp Red
	float t_g; // Temp Green
	float t_b; // Temp Blue

	//If there is no Saturation it means that it�s a shade of grey.
	if (saturation == 0.f)
	{
		return { luminosity,luminosity,luminosity };
	}

	//Get first intermediate
	if (luminosity < 0.5f)
		t_1 = luminosity * (1.f + saturation);
	else if (luminosity >= 0.5f)
		t_1 = luminosity + saturation - (luminosity * saturation);

	//Get second intermediate
	t_2 = (2.f * luminosity) - t_1;

	//convert hue angle back to float 1.0 range
	t_h = hue / 360.f;

	//Red Temp
	t_r = hue + (1.f / 3.f);
	if (std::signbit(t_r))
		t_r += 1.f;
	if (t_r > 1.f)
		t_r -= 1.f;
	//Green Temp
	t_g = hue;
	if (std::signbit(t_g))
		t_g += 1.f;
	if (t_g > 1.f)
		t_g -= 1.f;

	//Blue Temp
	float t_b = hue - (1.f / 3.f);
	if (std::signbit(t_b))
		t_b += 1.f;
	if (t_b > 1.f)
		t_b -= 1.f;

	//Red Final
	if ((t_r * 6.f) < 1.f)
		r = t_2 + (t_1 - t_2) * 6 * t_r;
	else if ((t_r * 2.f) < 1.f)
		r = t_1;
	else if ((t_r * 3.f) < 2.f)
		r = t_2 + (t_1 - t_2) * ((2.f / 3.f) - t_r) * 6.f;
	else
		r = t_2;


	//Green Final
	if ((t_g * 6.f) < 1.f)
		g = t_2 + (t_1 - t_2) * 6 * t_g;
	else if ((t_g * 2.f) < 1.f)
		g = t_1;
	else if ((t_g * 3.f) < 2.f)
		g = t_2 + (t_1 - t_2) * ((2.f / 3.f) - t_g) * 6.f;
	else
		g = t_2;


	//Blue Final
	if ((t_b * 6.f) < 1.f)
		b = t_2 + (t_1 - t_2) * 6 * t_b;
	else if ((t_b * 2.f) < 1.f)
		b = t_1;
	else if ((t_b * 3.f) < 2.f)
		b = t_2 + (t_1 - t_2) * ((2.f / 3.f) - t_b) * 6.f;
	else
		b = t_2;

	return { r,g,b };
}

RGB complementary(const RGB& color)
{
	HSL t = color;
	t.hue -= 180;
	if (std::signbit(t.hue))
		t.hue += 360.0;
	return std::move(t);
}

// split(const RGB& color, float angle)
//{
//	HSL t = color;
//	t.hue -= 180;
//	if (std::signbit(t.hue))
//		t.hue += 360.0;
//
//	HSL t2 = color;
//	t.hue -= 180;
//	if (std::signbit(t.hue))
//		t.hue += 360.0;
//
//	return std::make_tuple(t ,t2);
//}

//class Gradient
//{
//public:
//	Gradient()
//	{
//	}
//
//	~Gradient()
//	{
//	}
//
//private:
//	std::map<float, color_rgb> data;
//
//	bool addColor(color_rgb color,float location /* 0.0 -> 1.0 */)
//	{
//		if (data.count(location) != 0)
//			return false;
//		else
//		{
//			data.emplace(std::make_pair(location,color));
//			return true;
//		}
//	}
//
//	size_t removeColor(const float & location)
//	{
//		return data.erase(location);
//	}
//	
//	
//};


enum class ColorAxis { RED, GREEN, BLUE, REDGREEN, REDBLUE, GREENBLUE, RGB, HUE, SATURATION, LUMINOSITY };