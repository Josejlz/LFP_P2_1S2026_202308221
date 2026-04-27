#include <wx/wx.h>
#include "App.h"
#include "MainFrame.h"
#include "LexicalAnalyzer.h"



wxIMPLEMENT_APP(App);

bool App::OnInit() {

	MainFrame* mainFrame = new MainFrame("test");
	mainFrame->SetClientSize(800,600);
	mainFrame->Center();
	mainFrame->Show();
	mainFrame->setLexicalAnalyzer(new LexicalAnalyzer());

	return true;
};