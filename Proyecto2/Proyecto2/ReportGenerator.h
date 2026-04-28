#pragma once
#include "ErrorToken.h"
#include "Token.h"
#include <vector>
#include <string>

struct Tablero {
	std::string nombre;
	std::vector<Columna> columnas;
};

struct Columna {
	std::string nombre;
	std::vector<Tarea> tareas;
};

struct Tarea {
	std::string nombre;
	std::string prioridad;
	Persona responsable;
	std::string fechaLimite;
};

struct Persona {
	std::vector<Tarea> tareas;
};

class ReportGenerator
{
public:

	std::vector<Token> tokens;
	std::vector<ErrorToken> errores;
	std::vector<Tarea> tareas;
	std::vector<Tablero> tableros;
	std::vector<Persona> personas;

	void generateReports();
	void parsearTokens();
	void cruzarDatos();
	void genReporte1(const std::string& outputPath);
	void genReporte2(const std::string& outputPath);

private:



};

