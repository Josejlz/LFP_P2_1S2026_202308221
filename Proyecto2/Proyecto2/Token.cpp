#include "Token.h"

Token::Token(TokenTypes type, std::string lexema, int line, int column) {
	this->type = type;
	this->lexema = lexema;
	this->line = line;
	this->column = column;
}

std::string Token::typeToString() const {
	switch (type) {
	case TokenTypes::RESERVADA_MAIN:
		return "RESERVADA_MAIN";
	case TokenTypes::RESERVADA_SUB:
		return "RESERVADA_SUB";
	case TokenTypes::RESERVADA_ELEMENTO:
		return "RESERVADA_ELEMENTO";
	case TokenTypes::RESERVADA_ATRIBUTO:
		return "RESERVADA_ATRIBUTO";
	case TokenTypes::NIVEL_PRIORIDAD:
		return "NIVEL_PRIORIDAD";
	case TokenTypes::LIT_FECHA:
		return "LIT_FECHA";
	case TokenTypes::LIT_HORA:
		return "LIT_HORA";
	case TokenTypes::LIT_NUMERO:
		return "LIT_NUMERO";
	case TokenTypes::LIT_CADENA:
		return "LIT_CADENA";
	case TokenTypes::LLAVE_ABRE:
		return "LLAVE_ABRE";
	case TokenTypes::LLAVE_CIERRA:
		return "LLAVE_CIERRA";
	case TokenTypes::CORCHETE_ABRE:
		return "CORCHETE_ABRE";
	case TokenTypes::CORCHETE_CIERRA:
		return "CORCHETE_CIERRA";
	case TokenTypes::PARENTESIS_ABRE:
		return "PARENTESIS_ABRE";
	case TokenTypes::PARENTESIS_CIERRA:
		return "PARENTESIS_CIERRA";
	case TokenTypes::DOS_PUNTOS:
		return "DOS_PUNTOS";
	case TokenTypes::PUNTO:
		return "PUNTO";
	case TokenTypes::PUNTO_Y_COMA:
		return "PUNTO_Y_COMA";
	case TokenTypes::COMA:
		return "COMA";
	case TokenTypes::COMILLA: 
			return "COMILLA";
	default:
		return "UNKNOWN";
	}
}
