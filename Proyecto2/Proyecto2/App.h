#pragma once
#include <wx/wx.h>
#include "MainFrame.h"
#include "LexicalAnalyzer.h"

class LexicalAnalyzer;

class App : public wxApp {
public:
	bool OnInit();
};