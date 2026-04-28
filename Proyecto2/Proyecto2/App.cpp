#include <wx/wx.h>
#include "App.h"
#include "MainFrame.h"
#include "LexicalAnalyzer.h"
#include "ErrorManager.h"



wxIMPLEMENT_APP(App);

bool App::OnInit() {

	MainFrame* mainFrame = new MainFrame("Task Manager USAC");
	mainFrame->SetClientSize(800,600);
	mainFrame->Center();
	mainFrame->Show();
	mainFrame->setLexicalAnalyzer(new LexicalAnalyzer());
	mainFrame->getLexicalAnalyzer()->setErrorManager(new ErrorManager());
	return true;
};