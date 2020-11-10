#pragma once

#include <map>
#include <string>
#include <chrono>


namespace Element {

	enum class Colour : int {
		BLACK = 0,
		BLUE = 1,
		GREEN = 2,
		CYAN = 3,
		RED = 4,
		MAGENTA = 5,
		YELLOW = 6,
		WHITE = 7,
		GREY = 8,
		BRIGHT_BLUE = 9,
		BRIGHT_GREEN = 10,
		BRIGHT_CYAN = 11,
		BRIGHT_RED = 12,
		BRIGHT_MAGENTA = 13,
		BRIGHT_YELLOW = 14,
		BRIGHT_WHITE = 15,
	};

	class Debugger
	{
	public:

		static Debugger& get() {
			static Debugger debugger;
			return debugger;
		}


		void log(const std::string& message, Colour forground = Colour::GREEN, Colour background = Colour::BLACK);
		void log(const std::string& message, float value, Colour forground = Colour::GREEN, Colour background = Colour::BLACK);
		void log(const std::string& message, int value, Colour forground = Colour::GREEN, Colour background = Colour::BLACK);
		void log(float value, Colour forground = Colour::GREEN, Colour background = Colour::BLACK);
		void log(int value, Colour forground = Colour::GREEN, Colour background = Colour::BLACK);
		void warning(const std::string& message, Colour forground = Colour::YELLOW, Colour background = Colour::BLACK);
		void error(const std::string& message, Colour forground = Colour::RED, Colour background = Colour::BLACK);

		void startTimer(const std::string& name);
		void endTimer(const std::string& name, Colour forground = Colour::CYAN, Colour background= Colour::BLACK);

		void log(const char* file, int line, const std::string& message);
		void warning(const char* file, int line, const std::string& message);
		void error(const char* file, int line, const std::string& message);

		void log(const char* file, int line, const char* message);
		void error(const char* file, int line, const char* message);
		void warning(const char* file, int line, const char* message);

#define LOG( message ) log( __FILE__, __LINE__, message )
#define LOG_ERROR( message ) error( __FILE__, __LINE__, message )
#define LOG_WARNING( message ) warning( __FILE__, __LINE__, message )

		Debugger() = default;
		~Debugger() = default;
	private:

		void print(const std::string& type, const std::string& message, float value, Colour forground, Colour background);
		void print(const std::string& type, const std::string& message, int value, Colour forground, Colour background);
		void print(const std::string& type, const std::string& message, Colour forground, Colour background);
		void print(const std::string& type, const char* file, int line, const char* message, Colour forground, Colour background);

		std::map<std::string, std::chrono::steady_clock::time_point> map;
	};
}