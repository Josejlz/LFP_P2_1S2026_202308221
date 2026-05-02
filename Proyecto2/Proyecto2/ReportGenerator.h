#pragma once
#include "ErrorToken.h"
#include "Token.h"
#include <vector>
#include <string>
#include <map>

struct Tarea {
	std::string nombre;
	std::string prioridad;
	std::string responsable;
	std::string fechaLimite;
};

struct Columna {
	std::string nombre;
	std::vector<Tarea> tareas;
};

struct Tablero {
	std::string nombre;
	std::vector<Columna> columnas;
};

struct PersonaRes {
	std::string nombre;
	int totalTareas = 0;
	int tareasAlta = 0;
	int tareasMedia = 0;
	int tareasBaja = 0;
};

class ReportGenerator
{
public:

	std::vector<Token> tokens;
	std::vector<ErrorToken> errores;
	void genReporteTokens(const std::string& outputPath);
	bool generateReports(const std::string& outputDir);

	

private:
	Tablero tablero;
	std::vector<Tarea> tareas;
	std::vector<PersonaRes> personas;

	void parsearTokens();
	void cruzarDatos();
	void genReporte1(const std::string& outputPath);
	void genReporte2(const std::string& outputPath);

	int pos = 0;
	bool isAtEnd() const;
	Token peekToken() const;
	Token consumeToken();
	bool matchToken(TokenTypes type) const;
	Token expectToken(TokenTypes type);

};

