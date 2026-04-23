#pragma once
#include <wx/wx.h>
#include <string>
#include <wx/string.h>
class MainFrame : public wxFrame 
{
private:
	wxPanel* panel;
	wxButton* btnLoad;
	wxButton* btnGenReportes;
	wxButton* btnAnalyze;
	wxTextCtrl* textArea;
	wxStaticBitmap* image = nullptr;
	wxScrolledWindow* scrolledWindow = nullptr;

	void OnButtonLoadClicked(wxCommandEvent& evt);
	void OnButtonGenReportesClicked(wxCommandEvent& evt);
	void OnButtonAnalyzeClicked(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();

	wxString filepath;
public:
	MainFrame(const wxString& title);
	void setImage(const wxString& imagePath);
};

