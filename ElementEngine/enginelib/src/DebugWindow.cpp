#include <element/DebugWindow.h>
#include "ImGUI_.h"

#include <imgui.h>


Element::DebugWindow::DebugWindow(const std::string& label, WindowType type) : label(label)
{
	if (!ImGUI::Get().isPrepared())
		return;

	ImGui::Begin(label.c_str());
	started = true;
}

Element::DebugWindow::DebugWindow(WindowType type, const std::string& label) : label(label)
{
	if (!ImGUI::Get().isPrepared())
		return;

	if (type == WindowType::DEMO)
	{
		ImGui::ShowDemoWindow();
		return;
	}
	else if (type == WindowType::ABOUT)
	{
		ImGui::ShowAboutWindow();
		return;
	}

	ImGui::Begin(label.c_str());
	started = true;
}

Element::DebugWindow::~DebugWindow()
{
	if (!ImGUI::Get().isPrepared())
		return;
	if (started)
	{
		ImGui::End();
		started = false;
	}
}

void Element::DebugWindow::text(const std::string& label)
{
	if (!ImGUI::Get().isPrepared())
		return;

	ImGui::Text(label.c_str());
}

void Element::DebugWindow::data(const std::string& label, float data)
{
	auto tmp = label + ": %f";
	ImGui::Text(tmp.c_str(), data);
}

void Element::DebugWindow::checkBox(const std::string& label, bool* data)
{
	if (!ImGUI::Get().isPrepared())
		return;

	ImGui::Checkbox(label.c_str(), data);
}

void Element::DebugWindow::data(const std::string& label, bool data)
{
	if (!ImGUI::Get().isPrepared())
		return;
	auto tmp = label + (data ? ": true" : ": false");
	ImGui::Text(tmp.c_str());
}

void Element::DebugWindow::end()
{
	if (!ImGUI::Get().isPrepared())
		return;
	ImGui::End();
	started = false;
}

void Element::DebugWindow::createChild(const std::string& label)
{
	if (!ImGUI::Get().isPrepared())
		return;
	ImGui::BeginChild(label.c_str());
}

void Element::DebugWindow::endChild(const std::string& label)
{
	if (!ImGUI::Get().isPrepared())
		return;
	ImGui::EndChild();
}

void Element::DebugWindow::data(const std::string& label, int data)
{
	if (!ImGUI::Get().isPrepared())
		return;

	auto tmp = label + ": %i";
	ImGui::Text(tmp.c_str(), data);
}

void Element::DebugWindow::graph(const std::string& label, void* data, float min, float max, int plotCount, glm::vec2 size, const std::string& overlayText)
{
	if (!ImGUI::Get().isPrepared())
		return;

	ImGui::Text(label.c_str());
	ImGui::PlotLines("", (const float*)data, plotCount, 0, overlayText.c_str(), min, max, ImVec2(size.x, size.y));

}
