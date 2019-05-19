#pragma once

#ifndef __APPLICATION_LOG_H__
#define __APPLICATION_LOG_H__

#include <imgui.h>

enum LOG_LEVEL
{
	LOG_INFO = 1,
	LOG_WARNING,
	LOG_ERROR,

};

class Application_Log
{

protected:
	Application_Log();
	~Application_Log();
public:
	static Application_Log* Create();
	static void Destroy();

	static Application_Log* Get();

	void showLog(bool* a_Open);
	void clear();
	void addLog(LOG_LEVEL a_logLevel, const char* fmt, ...) IM_FMTARGS(2);
	void render(const char* title, bool* p_open = NULL);

private:
	ImGuiTextBuffer     Buf;
	ImGuiTextFilter     Filter;
	ImVector<int>       LineOffsets;        // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
	bool                ScrollToBottom;
	static Application_Log* m_instance;
};


#endif //__APPLICATION_LOG_H__