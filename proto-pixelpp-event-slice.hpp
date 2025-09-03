#pragma once
#include <chrono> // time_point , steady_clock
#include <SFML/Window/Event.hpp> 

class Slice
{
	friend class Piece;
public:
	Slice() = default;
	~Slice() = default;
private:
	std::chrono::time_point<std::chrono::steady_clock> time_point_{};
	std::size_t sys_{ 0u };
	std::size_t key_{ 0u };
	int mousex_{ 0 };
	int mousey_{ 0 };
	unsigned int width_{ 0u };
	unsigned int height_{ 0u };
	bool alt_{ false };
	bool control_{ false };
	bool shift_{ false };
	bool system_{ false };
	unsigned int unicode_{ 0u };
	float wheeldelta_{ 0.f };

	const Slice & record(const sf::Event & event);
public:
	const auto time_point() { return time_point_; };
	const auto sys() const { return sys_; };				///< System event id aka sf::EventType
	const auto key() const { return key_; };				///< The key activated in this slice
	const auto mousex()	const { return mousex_; };			///< X position of the mouse pointer, relative to the left of the owner window
	const auto mousey() const { return mousey_; };			///< Y position of the mouse pointer, relative to the top of the owner window
	const auto width() const { return width_; };			///< New  window width, in pixels
	const auto height() const { return height_; };			///< New window height, in pixels
	const auto alt() const { return alt_; };				///< Is the Alt key pressed?
	const auto control() const { return control_; };		///< Is the Control key pressed?
	const auto shift() const { return shift_; };			///< Is the Shift key pressed?
	const auto system() const { return system_; };			///< Is the System key pressed?
	const auto unicode() const { return unicode_; };		///< UTF-32 Unicode value of the last entered character
	const auto wheeldelta() const { return wheeldelta_; };	///< Wheel offset (positive is up/left, negative is down/right). High-precision mice may use non-integral offsets.
	
};
