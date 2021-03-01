#include "element/Debugger.h"

#include <iostream>
#include <iomanip>

#ifdef _WIN32
#include <Windows.h>

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

void setConsoleTextColour(Element::Colour foreground, Element::Colour background) {
#ifdef _WIN32
	SetConsoleTextAttribute(hConsole, static_cast<int>(background) * 16 + static_cast<int>(foreground));
#endif
}


void resetConsoleTextColour() {
#ifdef _WIN32
	SetConsoleTextAttribute(hConsole, static_cast<int>(Element::Colour::WHITE));
#endif
}

void Element::Debugger::log(const std::string& message, Colour forground, Colour background)
{
	print("LOG", message, forground, background);
}

void Element::Debugger::log(const std::string& message, float value, Colour forground, Colour background)
{
	print("LOG", message, value, forground, background);
}

void Element::Debugger::log(const std::string& message, int value, Colour forground, Colour background)
{
	print("LOG", message, value, forground, background);
}

void Element::Debugger::log(float value, Colour forground, Colour background)
{
	print("LOG", "Value", value, forground, background);
}

void Element::Debugger::log(int value, Colour forground, Colour background)
{
	print("LOG", "Value", value, forground, background);
}

void Element::Debugger::error(const std::string& message, Colour forground, Colour background)
{
	print("ERROR", message, forground, background);
}

void Element::Debugger::warning(const std::string& message, Colour forground, Colour background)
{
	print("WARNING", message, forground, background);
}

void Element::Debugger::startTimer(const std::string& name)
{
	map[name] = std::chrono::high_resolution_clock::now();
}

void Element::Debugger::endTimer(const std::string& name, Colour forground, Colour background)
{
	auto time = std::chrono::high_resolution_clock::now();
	auto diff = std::chrono::duration<float, std::milli>(time - map[name]).count();
	setConsoleTextColour(forground, background);
	std::cout << "[" << name <<  "]: " << std::setprecision(6) << diff << "ms" << std::endl;
	resetConsoleTextColour();
}

void Element::Debugger::log(const char* file, int line, const std::string& message)
{
	print("LOG", file, line, message.c_str(), Colour::BRIGHT_GREEN, Colour::BLACK);
}

void Element::Debugger::warning(const char* file, int line, const  std::string& message)
{
	print("WARNING", file, line, message.c_str(), Colour::BRIGHT_YELLOW, Colour::BLACK);
}

void Element::Debugger::error(const char* file, int line, const std::string& message)
{
	print("ERROR", file, line, message.c_str(), Colour::BRIGHT_RED, Colour::BLACK);
}

void Element::Debugger::log(const char* file, int line, const char* message)
{
	print("LOG", file, line, message, Colour::BRIGHT_GREEN, Colour::BLACK);
}

void Element::Debugger::warning(const char* file, int line, const char* message)
{
	print("WARNING", file, line, message, Colour::BRIGHT_YELLOW, Colour::BLACK);
}

void Element::Debugger::error(const char* file, int line, const char* message)
{
	print("ERROR", file, line, message, Colour::BRIGHT_RED, Colour::BLACK);
}

void Element::Debugger::print(const std::string& type, const std::string& message, float value, Colour forground, Colour background)
{
	setConsoleTextColour(forground, background);
	std::cout << "[" << type << "]: " << message << ": " << value << std::endl;
	resetConsoleTextColour();
}

void Element::Debugger::print(const std::string& type, const std::string& message, int value, Colour forground, Colour background)
{
	setConsoleTextColour(forground, background);
	std::cout << "[" << type << "]: " << message << ": " << value << std::endl;
	resetConsoleTextColour();
}

void Element::Debugger::print(const std::string& type, const std::string& message, Colour forground, Colour background)
{
	setConsoleTextColour(forground, background);
	std::cout << "[" << type << "]: " << message << std::endl;
	resetConsoleTextColour();
}

void Element::Debugger::print(const std::string& type, const char* file, int line, const char* message, Colour forground, Colour background)
{
	setConsoleTextColour(forground, background);
	std::cout << "[" << type << "]: " << "[" << file << "]" << "[" << line << "]: " << message << std::endl;
	resetConsoleTextColour();
}