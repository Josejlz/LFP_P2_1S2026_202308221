#include "ReportGenerator.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <iomanip>

bool ReportGenerator::isAtEnd() const {
	return (pos >= (int)tokens.size());
}

Token ReportGenerator::peekToken() const {
	if (!isAtEnd()) {
		return tokens[pos];
	}
	return Token(TokenTypes::UNDEFINED, "", -1, -1);
}

Token ReportGenerator::consumeToken() {
	if (!isAtEnd()) {
		return tokens[pos++];
	} 
	return Token(TokenTypes::UNDEFINED, "", -1, -1);
}

bool ReportGenerator::matchToken(TokenTypes type) const {
	return (!isAtEnd() && (peekToken().type == type));
}

//exception

Token ReportGenerator::expectToken(TokenTypes type){
	if (matchToken(type)) {
		return consumeToken();
	}
	Token malo = peekToken();
	throw std::runtime_error("ReportGenerator: token inesperado '" + malo.lexema +"' en linea " + std::to_string(malo.line));
}

//llamada

bool ReportGenerator::generateReports(const std::string& outputDir) {
	try {
		pos = 0;
		tablero = Tablero{};
		personas.clear();

		parsearTokens();
		cruzarDatos();
		genReporte1(outputDir + "/reporte_tablero.html");;
		genReporte2(outputDir + "/reporte_responsables.html");
		genReporteTokens(outputDir + "/reporte_tokens.html");
		return true;
	}
	catch(const std::exception& e){
		return false;
	}
}


void ReportGenerator::parsearTokens() {
	// <programa> ::= TABLERO CADENA "{" <columnas> "}" ";"
	expectToken(TokenTypes::RESERVADA_MAIN);
	tablero.nombre = expectToken(TokenTypes::LIT_CADENA).lexema;
	expectToken(TokenTypes::LLAVE_ABRE);

	//para columnas
	while (matchToken(TokenTypes::RESERVADA_SUB)) {
		Columna col;

		expectToken(TokenTypes::RESERVADA_SUB);
		col.nombre = expectToken(TokenTypes::LIT_CADENA).lexema;
		expectToken(TokenTypes::LLAVE_ABRE);

		//tareas 

		while (matchToken(TokenTypes::RESERVADA_ELEMENTO)) {
			Tarea t;

			expectToken(TokenTypes::RESERVADA_ELEMENTO);
			expectToken(TokenTypes::DOS_PUNTOS);
			t.nombre = expectToken(TokenTypes::LIT_CADENA).lexema;
			expectToken(TokenTypes::CORCHETE_ABRE);
			
			//atributos
			bool luisPrimero = true;
			while (luisPrimero || matchToken(TokenTypes::COMA)) {
				if (!luisPrimero) {
					consumeToken();
					if (!matchToken(TokenTypes::RESERVADA_ATRIBUTO)) {
						break;
					}
				}
				luisPrimero = false;
				Token attrib = expectToken(TokenTypes::RESERVADA_ATRIBUTO);
				expectToken(TokenTypes::DOS_PUNTOS);

				if (attrib.lexema=="prioridad") {
					t.prioridad = expectToken(TokenTypes::NIVEL_PRIORIDAD).lexema;
				}
				else if (attrib.lexema=="responsable") {
					t.responsable = expectToken(TokenTypes::LIT_CADENA).lexema;
				}
				else if (attrib.lexema == "fecha_limite") {
					t.fechaLimite = expectToken(TokenTypes::LIT_FECHA).lexema;
				}
			}

			expectToken(TokenTypes::CORCHETE_CIERRA);
			col.tareas.push_back(t);

			if (matchToken(TokenTypes::COMA)) {
				consumeToken();
			}

		}

		expectToken(TokenTypes::LLAVE_CIERRA);
		expectToken(TokenTypes::PUNTO_Y_COMA);
		tablero.columnas.push_back(col);
	}

	expectToken(TokenTypes::LLAVE_CIERRA);
	expectToken(TokenTypes::PUNTO_Y_COMA);
};

// referencias cruzadas basadas

void ReportGenerator::cruzarDatos() {
	std::map<std::string, PersonaRes> mapa;
	for (const auto& col : tablero.columnas) {
		for (const auto& t: col.tareas) {
			if (t.responsable.empty()) {
				continue;
			}

			PersonaRes& p = mapa[t.responsable];
			p.nombre = t.responsable;
			p.totalTareas++;

			if (t.prioridad=="ALTA") {
				p.tareasAlta++;
			}
			else if (t.prioridad=="MEDIA") {
				p.tareasMedia++;
			}
			else  if (t.prioridad=="BAJA") {
				p.tareasBaja++;
			}
		}

	}

	for (auto& par : mapa) {
		personas.push_back(par.second);
	}
};

// recursos HTML

static std::string htmlHeader(const std::string& titulo) {
	return R"(<!DOCTYPE html>
<html lang="es">
<head>
  <meta charset="UTF-8">
  <title>)" + titulo + R"(</title>
  <style>
    body { font-family: Arial, sans-serif; margin: 32px; background: #f4f4f4; color: #222; }
    h1   { text-align: center; margin-bottom: 24px; }
    /* --- Kanban --- */
    .tablero { display: flex; gap: 16px; flex-wrap: wrap; justify-content: center; }
    .columna { background: #fff; border-radius: 8px; padding: 16px; min-width: 200px;
               flex: 1; box-shadow: 0 2px 4px rgba(0,0,0,.1); }
    .columna h2 { font-size: 1rem; margin-bottom: 12px; border-bottom: 2px solid #ddd;
                  padding-bottom: 6px; }
    .tarea { background: #f9f9f9; border: 1px solid #ddd; border-radius: 6px;
             padding: 8px 10px; margin-bottom: 8px; font-size: .875rem; }
    .tarea .nombre { font-weight: bold; margin-bottom: 4px; }
    .tarea .meta   { color: #666; font-size: .8rem; }
    .ALTA  { border-left: 4px solid #e74c3c; }
    .MEDIA { border-left: 4px solid #f39c12; }
    .BAJA  { border-left: 4px solid #2ecc71; }
    /* --- Tabla responsables --- */
    table { width: 100%; border-collapse: collapse; background: #fff;
            box-shadow: 0 2px 4px rgba(0,0,0,.1); border-radius: 8px; overflow: hidden; }
    th { background: #3498db; color: #fff; padding: 10px 14px; text-align: left; }
    td { padding: 9px 14px; border-bottom: 1px solid #eee; }
    tr:last-child td { border-bottom: none; }
    tr:nth-child(even) td { background: #f9f9f9; }
  </style>
</head>
<body>
)";
};

static std::string htmlFooter() {
	return "</body>\n</html>\n";
};

// generación de reportes

//el uno

void ReportGenerator::genReporte1(const std::string& outputPath) {
	std::ofstream f(outputPath);
	if (!f.is_open()) throw std::runtime_error("No se pudo crear " + outputPath);

	f << htmlHeader("Tablero: " + tablero.nombre);
	f << "<h1>" << tablero.nombre << "</h1>\n";
	f << "<div class=\"tablero\">\n";

	for (const auto& col : tablero.columnas) {
		f << "  <div class=\"columna\">\n";
		f << "    <h2>" << col.nombre << "</h2>\n";

		for (const auto& t : col.tareas) {
			f << "    <div class=\"tarea " << t.prioridad << "\">\n";
			f << "      <div class=\"nombre\">" << t.nombre << "</div>\n";
			f << "      <div class=\"meta\">";
			if (!t.responsable.empty())  f << "Responsable: " << t.responsable << "<br>";
			if (!t.fechaLimite.empty())  f << "Fecha l&iacute;mite: " << t.fechaLimite << "<br>";
			if (!t.prioridad.empty())    f << "Prioridad: " << t.prioridad;
			f << "</div>\n";
			f << "    </div>\n";
		}

		f << "  </div>\n";
	}

	f << "</div>\n";
	f << htmlFooter();
};

//el dos

void ReportGenerator::genReporte2(const std::string& outputPath) {
	// Total de tareas en todo el tablero
	int totalGlobal = 0;
	for (const auto& col : tablero.columnas)
		totalGlobal += (int)col.tareas.size();

	std::ofstream f(outputPath);
	if (!f.is_open()) throw std::runtime_error("No se pudo crear " + outputPath);

	f << htmlHeader("Reporte por Responsable");
	f << "<h1>Reporte por Responsable</h1>\n";
	f << "<table>\n";
	f << "  <thead><tr>"
		<< "<th>Responsable</th>"
		<< "<th>Tareas asignadas</th>"
		<< "<th>Prioridad ALTA</th>"
		<< "<th>Prioridad MEDIA</th>"
		<< "<th>Prioridad BAJA</th>"
		<< "<th>% del total</th>"
		<< "</tr></thead>\n";
	f << "  <tbody>\n";

	for (const auto& p : personas) {
		double pct = totalGlobal > 0 ? (p.totalTareas * 100.0 / totalGlobal) : 0.0;

		f << "    <tr>"
			<< "<td>" << p.nombre << "</td>"
			<< "<td>" << p.totalTareas << "</td>"
			<< "<td>" << p.tareasAlta << "</td>"
			<< "<td>" << p.tareasMedia << "</td>"
			<< "<td>" << p.tareasBaja << "</td>"
			<< "<td>" << std::fixed << std::setprecision(1) << pct << "%</td>"
			<< "</tr>\n";
	}

	f << "  </tbody>\n</table>\n";
	f << htmlFooter();
};

//el errores y tokesn (el tres)

void ReportGenerator::genReporteTokens(const std::string& outputPath) {
	std::ofstream f(outputPath);
	if (!f.is_open()) throw std::runtime_error("No se pudo crear " + outputPath);

	f << htmlHeader("Reporte de Tokens y Errores");
	f << "<h1>Reporte de Análisis</h1>\n";

	// -para la tabla de tokens
	f << "<h2>Tokens Identificados</h2>\n";
	f << "<table>\n";
	f << "  <thead><tr>"
		<< "<th>#</th><th>Tipo</th><th>Lexema</th><th>Línea</th><th>Columna</th>"
		<< "</tr></thead>\n";
	f << "  <tbody>\n";

	int i = 1;
	for (const auto& t : tokens) {
		f << "    <tr>"
			<< "<td>" << i++ << "</td>"
			<< "<td>" << t.typeToString() << "</td>"
			<< "<td>" << t.lexema << "</td>"
			<< "<td>" << t.line << "</td>"
			<< "<td>" << t.column << "</td>"
			<< "</tr>\n";
	}

	f << "  </tbody>\n</table>\n";

	// para la tabla de errores
	f << "<h2 style='margin-top:40px'>Errores Identificados</h2>\n";
	f << "<table>\n";
	f << "  <thead><tr>"
		<< "<th>#</th><th>Tipo</th><th>Lexema</th>"
		<< "<th>Línea</th><th>Columna</th><th>Error</th><th>Gravedad</th>"
		<< "</tr></thead>\n";
	f << "  <tbody>\n";

	if (errores.empty()) {
		f << "    <tr><td colspan='7' style='text-align:center'>Sin errores</td></tr>\n";
	}
	else {
		int j = 1;
		for (const auto& e : errores) {
			f << "    <tr>"
				<< "<td>" << j++ << "</td>"
				<< "<td>" << e.toStringTipoErrorSintLex() << "</td>"
				<< "<td>" << e.lexeme << "</td>"
				<< "<td>" << e.line << "</td>"
				<< "<td>" << e.column << "</td>"
				<< "<td>" << e.toStringTipoError() << "</td>"
				<< "<td>" << e.toStringGravedad() << "</td>"
				<< "</tr>\n";
		}
	}

	f << "  </tbody>\n</table>\n";
	f << htmlFooter();
}