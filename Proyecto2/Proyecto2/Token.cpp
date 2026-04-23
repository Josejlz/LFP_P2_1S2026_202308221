#include "Token.h"

Token::Token(TokenType type, std::string lexema, int line, int column) {
	this->type = type;
	this->lexema = lexema;
	this->line = line;
	this->column = column;
}

std::string Token::typeToString() const {
	switch (type) {
	case TokenType::RESERVADA_MAIN:
		return "RESERVADA_MAIN";
	case TokenType::RESERVADA_SUB:
		return "RESERVADA_SUB";
	case TokenType::RESERVADA_ELEMENTO:
		return "RESERVADA_ELEMENTO";
	case TokenType::RESERVADA_ATRIBUTO:
		return "RESERVADA_ATRIBUTO";
	case TokenType::NIVEL_PRIORIDAD:
		return "NIVEL_PRIORIDAD";
	case TokenType::LIT_FECHA:
		return "LIT_FECHA";
	case TokenType::LIT_HORA:
		return "LIT_HORA";
	case TokenType::LIT_NUMERO:
		return "LIT_NUMERO";
	case TokenType::LIT_CADENA:
		return "LIT_CADENA";
	case TokenType::LLAVE_ABRE:
		return "LLAVE_ABRE";
	case TokenType::LLAVE_CIERRA:
		return "LLAVE_CIERRA";
	case TokenType::CORCHETE_ABRE:
		return "CORCHETE_ABRE";
	case TokenType::CORCHETE_CIERRA:
		return "CORCHETE_CIERRA";
	case TokenType::PARENTESIS_ABRE:
		return "PARENTESIS_ABRE";
	case TokenType::PARENTESIS_CIERRA:
		return "PARENTESIS_CIERRA";
	case TokenType::DOS_PUNTOS:
		return "DOS_PUNTOS";
	case TokenType::PUNTO:
		return "PUNTO";
	case TokenType::PUNTO_Y_COMA:
		return "PUNTO_Y_COMA";
	default:
		return "UNKNOWN";
	}
}
