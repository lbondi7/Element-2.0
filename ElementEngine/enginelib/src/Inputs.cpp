#include <element/Inputs.h>
#include <element/Debugger.h>

#include <GLFW/glfw3.h>

#include <memory>

float Element::Inputs::getCursorPos(int axis)
{
	double value;
	glfwGetCursorPos(window, axis == Element::AXIS::X ? &value : nullptr,
		axis == Element::AXIS::Y ? nullptr: &value);
	return static_cast<float>(value);
}

glm::vec2 Element::Inputs::getCursorPos()
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	return glm::vec2(static_cast<float>(x), static_cast<float>(y));
}

bool Element::Inputs::keyDown(int _key, int mod)
{
	return key(_key, GLFW_PRESS, mod);
}

bool Element::Inputs::keyUp(int _key, int mod)
{
	return key(_key, GLFW_RELEASE, mod);
}

bool Element::Inputs::keyHeld(int _key, int mod)
{
	return key(_key, GLFW_REPEAT, mod);
}

bool Element::Inputs::key(int key, int action, int mod)
{
	for (int i  = 0; i < events.size(); ++i)
	{
		const auto& event = dynamic_cast<KeyEvent*>(events[i].get());
		if (!event || (event->key != key || event->action != action))
			continue;

		return true;
	}
	return false;
}

bool Element::Inputs::buttonDown(int key)
{
	return button(key, GLFW_PRESS);
}

bool Element::Inputs::buttonUp(int key)
{
	return button(key, GLFW_RELEASE);
}

bool Element::Inputs::buttonHeld(int key)
{
	return glfwGetMouseButton(window, key) != GLFW_RELEASE;
}

bool Element::Inputs::button(int key, int action)
{
	for (int i = 0; i < events.size(); ++i)
	{
		const auto& event = dynamic_cast<MouseButtonEvent*>(events[i].get());
		if (!event || (event->key != key || event->action != action))
			continue;

		return true;
	}
	return false;
}

bool Element::Inputs::scrollUp(int axis)
{
	return scroll(axis) > 0.0;
}

bool Element::Inputs::scrollDown(int axis)
{
	return scroll(axis) < 0.0;
}

double Element::Inputs::scroll(int axis)
{
	for (int i = 0; i < events.size(); ++i)
	{
		const auto& event = dynamic_cast<ScrollEvent*>(events[i].get());
		if (!event)
			continue;

 		return axis == 0 ? event->xoffset : event->yoffset;
	}
	return 0.0;
}

void Element::Inputs::init(GLFWwindow* _window)
{
	callback_funcs.push_back(std::bind(&Inputs::input, this, std::placeholders::_1));
	window = _window;

	glfwSetWindowUserPointer(window, this);
	auto kFunc = [](GLFWwindow* w, int key, int scancode, int action, int mod)
	{
		auto event = std::make_shared<KeyEvent>();
		event->key = key;
		event->scancode = scancode;
		event->action = action;
		event->mod = mod;
		static_cast<Inputs*>(glfwGetWindowUserPointer(w))->sendEvent(event);
	};
	auto mbFunc = [](GLFWwindow* w, int key, int action, int mod)
	{
		auto event = std::make_shared<MouseButtonEvent>();
		event->key = key;
		event->action = action;
		event->mod = mod;
		static_cast<Inputs*>(glfwGetWindowUserPointer(w))->sendEvent(event);
	};
	auto scrollFunc = [](GLFWwindow* w, double xoffset, double yoffset)
	{
		auto event = std::make_shared<ScrollEvent>();
		event->xoffset = xoffset;
		event->yoffset = yoffset;
		static_cast<Inputs*>(glfwGetWindowUserPointer(w))->sendEvent(event);
	};
	glfwSetKeyCallback(window, kFunc);
	glfwSetMouseButtonCallback(window, mbFunc);
	glfwSetScrollCallback(window, scrollFunc);
}

void Element::Inputs::sendEvent(std::shared_ptr<InputEvent> event)
{
	for (const auto& callback : callback_funcs)
	{
		callback(event);
	}
}

void Element::Inputs::clear()
{
	events.clear();
}

void Element::Inputs::input(std::shared_ptr<InputEvent> event)
{
	if (events.size() > 20)
		events.pop_front();

	events.emplace_back(event);
}
