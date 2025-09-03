//#pragma once
//enum comenum { test = 'A', one, three };
//
//struct Command
//{
//	std::string name;
//	std::vector<char> args;
//	std::vector<int> values;
//};
//
//static const std::hash<std::string> g_hasher;
//static const std::map<size_t, std::string> g_commands;
//
//size_t encode(Command command)
//{
//	std::stringstream encoding;
//
//	/*	for (auto& command : commands)
//	{	*/
//	encoding << "*";
//
//	for (auto& con : command.args)
//	{
//		encoding << con << "|";
//	}
//
//	encoding << "#";
//
//
//	for (auto& con : command.values)
//	{
//		encoding << con << "|";
//	}
//
//	std::cout << encoding.str() << std::endl;
//	const size_t result = g_hasher(encoding.str());;
//	return result;
//}
//
//struct Message
//{
//	size_t content;
//public:
//	Message() = default;
//	Message(const size_t content) : content(content) {}
//};
//
//
//class Envelope
//{
//	Message msg_;
//public:
//	size_t from;
//	size_t to;
//
//	Message& msg() { return  msg_; }
//	Envelope(/*size_t from,*/ size_t to, Message msg) : from(from), to(to), msg_(msg)
//	{
//	}
//};
//
//class Channel;
//
//class Antenna
//{
//public:
//	size_t channel;
//
//public:
//	Antenna() {}
//	virtual ~Antenna() {};
//	//base of any radio device
//public:
//	virtual bool send(Channel& target, Message msg) = 0;
//	virtual bool recieve(Message msg) = 0;
//};
//
//class Channel
//{
//public:
//	size_t id;
//	Channel(size_t id) : id(id) {}
//
//	std::vector<Antenna*> devices_;
//
//	bool attach(Antenna* device)
//	{
//		devices_.emplace_back(device);
//		device->channel = id;
//		return true;
//	}
//};

#include <stack>
#include <queue>
#include <list>
#include <vector>
#include <iostream>
#include <ctime>
#include <array>
#include <chrono>
#include <map>
#include <cassert>
#include <sstream>
#include <istream>
#include <boost\mpl\list.hpp>
#include <unordered_map>

template<class ...Us> void f(Us... pargs) {
	for (auto&& arg : pargs)
	{
		std::cout << arg << std::endl;
	}
}
template<class ...Ts> void g(Ts... args) { f(&args...); }

template
<class Content>
class Signal
{
	Content content_;
public:
	Content const& get() { return content_; }

	Signal(const Content content) : content_(content) {}
	~Signal() = default;
};

class IntegerSignal : public Signal<int>
{
public:
	IntegerSignal(const int content)
		: Signal<int>(content)
	{
	}
};


/*!
 * @class: Radio
 * @purpose:
 */
class Radio
{

	//types

	//attributes

	//access

	//modify

	//methods


};
