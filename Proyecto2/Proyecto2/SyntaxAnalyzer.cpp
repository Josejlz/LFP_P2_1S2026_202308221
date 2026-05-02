#include "SyntaxAnalyzer.h"
#include <iostream>
#include <stdexcept>

SyntaxAnalyzer::SyntaxAnalyzer(std::vector<Token> t, ErrorManager* eM) {
	tokens = t;
	pos = 0;
	errorManager = eM;
};

bool SyntaxAnalyzer::isAtEnd() {
	return pos >= (int) tokens.size();
};

Token SyntaxAnalyzer::peek() {
	if (!isAtEnd()) {
		return tokens[pos];
	}

	return Token(TokenTypes::UNDEFINED, "", -1, -1);
};

Token SyntaxAnalyzer::consume() {
	if (!isAtEnd()) {
		return tokens[pos++];
	}
	else {
		return Token(TokenTypes::UNDEFINED, "", -1, -1);
	}
};

bool SyntaxAnalyzer::match(TokenTypes tType) {
	return (!isAtEnd()&&(peek().type == tType));
}

bool SyntaxAnalyzer::matchLexeme(const std::string& lexeme) {
	return(!isAtEnd ()&&(peek().lexema==lexeme));
};

Token SyntaxAnalyzer::expect(TokenTypes type){
	if (match(type)) {
		return consume();
	}

	Token malo = peek();
	errorManager->addError(malo.lexema, malo.line, malo.column, TipoError::UNEXPECTED_TOKEN, Gravedad::CRITICO, Tipo::SINTACTICO);

	throw SyntaxError("Error Sintáctico " + std::to_string(malo.line));
};

Token SyntaxAnalyzer::expectLexeme(const std::string& lexeme) {
	if (matchLexeme(lexeme)) {
		return consume();
	} 
	Token malo = peek();
	errorManager->addError(malo.lexema, malo.line, malo.column, TipoError::UNEXPECTED_TOKEN, Gravedad::CRITICO, Tipo::SINTACTICO);
	return Token(TokenTypes::UNDEFINED, malo.lexema, malo.line, malo.column);
};

//llamadas desde afuera

void SyntaxAnalyzer::parse() {
	try{
		parsePrograma();
	}
	catch (const SyntaxError&) {
	}
};

//reglas gramaticales

// <Programa> ::= TABLERO CADENA "{" <columnas> "}" ";"


void SyntaxAnalyzer::parsePrograma() {
	expect(TokenTypes::RESERVADA_MAIN);
	expect(TokenTypes::LIT_CADENA);
	expect(TokenTypes::LLAVE_ABRE);
	parseColumnas();
	expect(TokenTypes::LLAVE_CIERRA);
	expect(TokenTypes::PUNTO_Y_COMA);
}

//<Columnas> ::= <columna> <columnas> | <Columna>

void SyntaxAnalyzer::parseColumnas() {
	parseColumna();
	while (match(TokenTypes::RESERVADA_SUB)) {
		parseColumna();
	}
}

// <columna> ::= COLUMNA CADENA "{" <tareas> "}" ";"


void SyntaxAnalyzer::parseColumna() {
	expect(TokenTypes::RESERVADA_SUB);
	expect(TokenTypes::LIT_CADENA);
	expect(TokenTypes::LLAVE_ABRE);
	if (match(TokenTypes::RESERVADA_ELEMENTO)) {
		parseTareas();
	}
	expect(TokenTypes::LLAVE_CIERRA);
	expect(TokenTypes::PUNTO_Y_COMA);
};

//<tareas>::= <tarea> "," <tareas> | <tareas>

void SyntaxAnalyzer::parseTareas() {
	parseTarea();
	while (match(TokenTypes::COMA)) {
		consume();

		if (!match(TokenTypes::RESERVADA_ELEMENTO)) {
			break;
		}
		parseTarea();

	}

}

//<tarea>::=tarea ":" CADENA "[" <atributo> "]"

void SyntaxAnalyzer::parseTarea() {
	expect(TokenTypes::RESERVADA_ELEMENTO);
	expect(TokenTypes::DOS_PUNTOS);
	expect(TokenTypes::LIT_CADENA);
	expect(TokenTypes::CORCHETE_ABRE);
	parseAtributos();
	expect(TokenTypes::CORCHETE_CIERRA);
};

// <atributo> ::= <atributo> "," <atributos> | <atributo> "," | <atributo>

void SyntaxAnalyzer::parseAtributos() {

	parseAtributo();

	while (match(TokenTypes::COMA)) {
		consume();

		if (!match(TokenTypes::RESERVADA_ATRIBUTO)) {
			break;
		}

		parseAtributo();

	}

};

// <atributo>::= prioridad ";" <prioridad> | responsable ":" CADENA | fecha_limite ":" FECHA

void SyntaxAnalyzer::parseAtributo() {
	Token attrib = expect(TokenTypes::RESERVADA_ATRIBUTO);
	expect(TokenTypes::DOS_PUNTOS);

	if (attrib.lexema=="prioridad") {
		expect(TokenTypes::NIVEL_PRIORIDAD);
	}
	else if (attrib.lexema == "responsable") {
		expect(TokenTypes::LIT_CADENA);
	}
	else if (attrib.lexema == "fecha_limite") {
		expect(TokenTypes::LIT_FECHA);
	} else {
		if (!isAtEnd() && !match(TokenTypes::COMA) && !match(TokenTypes::CORCHETE_CIERRA)) {
			consume();
		}
	}
};

