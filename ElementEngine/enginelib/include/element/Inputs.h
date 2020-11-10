#pragma once

#include "InputIndices.h"
#include "element/Maths/Vec2.h"

#include <deque>
#include <vector>
#include <functional>
#include <memory>

struct GLFWwindow;

namespace Element {

	class Inputs
	{
	public:

		struct InputEvent {
			virtual ~InputEvent() = default;
		};

		struct KeyEvent : public InputEvent{
			int key = -1;
			int action = -1;
			int scancode = -1;
			int mod = -1;
		};

		struct MouseButtonEvent : public InputEvent{
			int key = -1;
			int action = -1;
			int mod = -1;
		};

		struct ScrollEvent : public InputEvent {
			double xoffset = 0.0;
			double yoffset = 0.0;
		};


		Inputs() = default;
		~Inputs() = default;


		static Inputs& get() {
			static Inputs input;
			return input;
		}

		float getCursorPos(int axis);

		Vec2 getCursorPos();

		bool keyDown(int key, int mod = -1);

		bool keyUp(int key, int mod = -1);

		bool keyHeld(int key, int mod = -1);

		bool key(int key, int action, int mod);

		bool button(int key, int action);

		bool scrollUp(int axis);

		bool scrollDown(int axis);

		double scroll(int axis);

		bool buttonDown(int key);

		bool buttonUp(int key);

		bool buttonHeld(int key);

		void init(GLFWwindow* window);

		void sendEvent(std::shared_ptr<InputEvent> event);

		void clear();

	private:

		GLFWwindow* window;

		std::deque<std::shared_ptr<InputEvent>> events;
		
		void input(std::shared_ptr<InputEvent> event);

		using InputFncs = std::vector<std::function<void(std::shared_ptr<InputEvent>)>>;
		InputFncs callback_funcs;
	};
}