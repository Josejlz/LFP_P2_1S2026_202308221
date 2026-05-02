#include "MainFrame.h"
#include <wx/wx.h>
#include <string>
#include <wx/string.h>
#include <wx/filedlg.h>
#include <wx/log.h>
#include <fstream>
#include <sstream>
#include "LexicalAnalyzer.h"
#include "ErrorManager.h"
#include "ReportGenerator.h"
#include <stdexcept>

void MainFrame::setLexicalAnalyzer(LexicalAnalyzer* analyzer) {
	lexicalAnalyzer = analyzer;
}

LexicalAnalyzer* MainFrame::getLexicalAnalyzer() {
	return lexicalAnalyzer;
}


wxString filePath = "";

enum BTN_ID {
	LOAD_ID = 1,
	GENREPORTES_ID = 2,
	ANALYZE_ID = 3,
	TEXTAREA_ID = 4,
	PNGHANDLER = 5

};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(LOAD_ID, MainFrame::OnButtonLoadClicked)
EVT_BUTTON(GENREPORTES_ID, MainFrame::OnButtonGenReportesClicked)
EVT_BUTTON(ANALYZE_ID, MainFrame::OnButtonAnalyzeClicked)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 350));

	btnLoad = new wxButton(panel, LOAD_ID, "Cargar archivo", wxPoint(155, 50), wxSize(150, 50));

	btnGenReportes = new wxButton(panel, GENREPORTES_ID, "Generar reportes", wxPoint(325, 50), wxSize(150, 50));

	btnAnalyze = new wxButton(panel, ANALYZE_ID, "Analizar", wxPoint(495, 50), wxSize(150, 50));

	textArea = new wxTextCtrl(panel, TEXTAREA_ID, "", wxPoint(50, 120), wxSize(700, 200), wxTE_MULTILINE | wxTE_RICH);

	wxInitAllImageHandlers();

	scrolledWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL);
	scrolledWindow->SetScrollRate(10, 10);
	scrolledWindow->SetBackgroundColour(*wxLIGHT_GREY); // anotacion del area de dibujo

	wxBoxSizer* imageSizer = new wxBoxSizer(wxVERTICAL);
	scrolledWindow->SetSizer(imageSizer);

	// organizacion de los elementos
	mainSizer->Add(panel, 0, wxEXPAND | wxALL, 5);
	mainSizer->Add(scrolledWindow, 1, wxEXPAND | wxALL, 5); // El 1 hace que use el resto del espacio

	this->SetSizer(mainSizer);

	wxInitAllImageHandlers();
	image = nullptr;
	reportGenerator = new ReportGenerator();

};

//carga de files

void MainFrame::OnButtonLoadClicked(wxCommandEvent &evt) {
	
	wxFileDialog openFileDialog(this, "Seleccionar archivo", "", "", "Archivos tipo task (*.task)|*.task", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL) {
		return;
	}

	filePath = openFileDialog.GetPath();

	wxLogMessage("Archivo seleccionado: %s", filePath );
	
	std::string filename = filePath.ToStdString();

	if (filename.find(".task") == std::string::npos) {
		wxLogMessage("El archivo no es válido, verifique que tenga la extensión correcta.");
		return;
	}

	std::ifstream archivo(filename);

	if (!archivo.is_open()) {
		wxLogMessage("No se pudo abrir el archivo.");
		return;
	}

	std::stringstream buffer;
	buffer << archivo.rdbuf();

	wxString fileContent(buffer.str());

	textArea->SetValue("=====================CONTENIDO DEL ARCHIVO===================== \n\n");
	textArea->AppendText(wxString(buffer.str().c_str(), wxConvUTF8));
	lexicalAnalyzer->setFileContent(wxString(buffer.str().c_str(), wxConvUTF8).ToStdString());
	analized = false;
	sintaxisOK = false;

};

//Analisis



void MainFrame::OnButtonAnalyzeClicked(wxCommandEvent& evt) {
	if (lexicalAnalyzer == nullptr) {
		wxLogMessage("El analizador léxico no ha sido configurado.");
		return;
	}
	if (filePath.IsEmpty()) {
		wxLogMessage("No se ha cargado ningún archivo para analizar.");
		return;
	}
	else {
			//lexico
			
		analized = true;
			lexicalAnalyzer->getErrorManager()->limpiarErrores();
			lexicalAnalyzer->NextToken();
			std::vector<Token> tokens = lexicalAnalyzer->getTokens();
			textArea->AppendText("\n\n=====================TOKENS IDENTIFICADOS===================== \n\n");


			for (const auto& token : tokens) {
				textArea->AppendText(wxString::Format(
					wxString::FromUTF8("Tipo: %s, Lexema: %s, Línea: %d, Columna: %d\n"),
					token.typeToString(), token.lexema, token.line, token.column));
			}

			//sintactico

			if (syntaxAnalyzer) delete syntaxAnalyzer;
			syntaxAnalyzer = new SyntaxAnalyzer(tokens, lexicalAnalyzer->getErrorManager());
			syntaxAnalyzer->parse();

			//errores

			std::vector<ErrorToken> errors = lexicalAnalyzer->getErrorManager()->getErrorList();
			textArea->AppendText("\n\n=====================ERRORES IDENTIFICADOS===================== \n\n");
			bool hayErrorSintactico = false;
			int i = 1;
			for (const auto& error : errors) {

				if (error.tipo == Tipo::SINTACTICO) {
					hayErrorSintactico = true;
				}

				textArea->AppendText(wxString::Format(wxString::FromUTF8("No.: %d, Lexema: %s, Línea: %d, Columna: %d, Gravedad: %s, TipoError: %s, Tipo: %s\n"), 
					i++, error.lexeme, error.line, error.column, error.toStringGravedad(), error.toStringTipoError(), error.toStringTipoErrorSintLex()));
			}

			if (errors.empty()) {
				textArea->AppendText("Sin errores.\n");
			}
		
			sintaxisOK = !hayErrorSintactico;

			if (sintaxisOK) {
				textArea->AppendText("\nAnalisis sintactico: OK. Puede generar los reportes.\n");
				reportGenerator->tokens = tokens;
			}
			else {
				textArea->AppendText("\nAnalisis sintactico: FALLIDO. Corrija los errores antes de generar reportes.\n");
			}
		
	}
};


void MainFrame::setImage(const wxString& imagePath)
{
	wxImage imagen;
	if (!imagen.LoadFile(imagePath, wxBITMAP_TYPE_PNG))
	{
		return;
	}

	wxBitmap bitmap(imagen);

	if (image == nullptr)
	{
		// creacion sino esta
		image = new wxStaticBitmap(scrolledWindow, wxID_ANY, bitmap);
		scrolledWindow->GetSizer()->Add(image, 0, wxALIGN_CENTER | wxALL, 10);
	}
	else
	{
		// actualizacion
		image->SetBitmap(bitmap);
	}

	// para actuallizar el scroll
	scrolledWindow->FitInside();
	scrolledWindow->Layout();
};


void MainFrame::OnButtonGenReportesClicked(wxCommandEvent& evt) {
	


	if (!sintaxisOK) {
		wxLogMessage("No se pueden generar reportes: el archivo tiene errores sintacticos.");

		if (analized) {
			wxLogMessage("Se generará un reporte con los Tokens.");

			try {



				wxDirDialog dirDialog(this, "Seleccionar carpeta para guardar los reportes", "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

				if (dirDialog.ShowModal() == wxID_CANCEL) {
					return;
				}

				std::string outputDir = dirDialog.GetPath().ToStdString();
				reportGenerator->tokens = syntaxAnalyzer->getTokens();
				reportGenerator->errores = lexicalAnalyzer->getErrorManager()->getErrorList();


				reportGenerator->genReporteTokens(outputDir + "/reporte_tokens.html");

				return;
			}
			catch (const std::exception& e) {
				return;
			}
			
		}

		return;
	}

	wxDirDialog dirDialog(this, "Seleccionar carpeta para guardar los reportes", "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

	if (dirDialog.ShowModal()==wxID_CANCEL) {
		return;
	}

	std::string outputDir = dirDialog.GetPath().ToStdString();
	reportGenerator->tokens = syntaxAnalyzer->getTokens();
	reportGenerator->errores = lexicalAnalyzer->getErrorManager()->getErrorList();
	bool ok = reportGenerator->generateReports(outputDir);

	if (ok) {
		wxLogMessage("Reportes generados exitosamente en:\n" + outputDir);
	}
	else {
		wxLogMessage("Ocurrio un error al generar los reportes.");
	}

};
