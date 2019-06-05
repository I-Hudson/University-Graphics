#pragma once
#include "Application_Log.h"

Application_Log* Application_Log::m_instance = nullptr;

Application_Log* Application_Log::Create()
{
	if (m_instance == nullptr)
	{
		m_instance = DEBUG_NEW  Application_Log();
	}
	return m_instance;
}

Application_Log* Application_Log::Get()
{
	return m_instance;
}

void Application_Log::Destroy()
{
	if (m_instance != nullptr)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

Application_Log::Application_Log()
{
}

Application_Log::~Application_Log()
{

}

void Application_Log::showLog(bool* a_Open)
{
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 window_pos = ImVec2(0, io.DisplaySize.y - (io.DisplaySize.y * 0.1f));
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, (io.DisplaySize.y * 0.1f)), ImGuiCond_FirstUseEver);
	ImGui::Begin("Output Log", a_Open, ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | 
		ImGuiWindowFlags_AlwaysAutoResize | 
		ImGuiWindowFlags_NoSavedSettings | 
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_NoCollapse);
	ImGui::End();
	render("Output Log", a_Open);

}
void Application_Log::clear()
{
	Buf.clear();
	LineOffsets.clear();
	LineOffsets.push_back(0);
}


void Application_Log::addLog(LOG_LEVEL a_logLevel, const char* fmt, ...) IM_FMTARGS(2)
{

	int old_size = Buf.size();
	Buf.appendf("[%05d] [%i] ", ImGui::GetFrameCount(), a_logLevel);
	va_list args;
	va_start(args, fmt);
	Buf.appendfv(fmt, args);
	va_end(args);
	Buf.appendf("\n");
	for (int new_size = Buf.size(); old_size < new_size; old_size++)
	{
		if (Buf[old_size] == '\n')
		{
			LineOffsets.push_back(old_size + 1);
		}
	}
	ScrollToBottom = true;
}

void Application_Log::render(const char* title, bool* p_open)
{
	if (!ImGui::Begin(title, p_open))
	{
		ImGui::End();
		return;
	}
	//Implement a clear button to clear the log of any data
	if (ImGui::Button("Clear"))
	{
		clear();
	}
	ImGui::SameLine();
	bool copy = ImGui::Button("Copy");
	ImGui::SameLine();
	Filter.Draw("Filter", -100.0f);
	ImGui::Separator();
	ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	
	if (copy)
	{
		ImGui::LogToClipboard(); //ImGui has a function to copy any text data to a clipboard
	}
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	const char* buf = Buf.begin();
	const char* buf_end = Buf.end();
	if (Filter.IsActive())
	{
		int offset = 0;
		for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
		{
			const char* line_start = buf + offset;
			offset = LineOffsets[line_no];
			const char* line_end = buf + LineOffsets[line_no];
			if (Filter.PassFilter(line_start, line_end))
				ImGui::TextUnformatted(line_start, line_end);
		}
	}
	else
	{
		ImGui::TextUnformatted(buf, buf_end);
	}
	ImGui::PopStyleVar();

	if (ScrollToBottom)
		ImGui::SetScrollHereY(1.0f);
	ScrollToBottom = false;
	ImGui::EndChild();
	ImGui::End();
}
