#include "ErrorToken.h"
#include <string>
#include <vector>

ErrorToken::ErrorToken(std::string lex, int l, int c, TipoError tE, Gravedad g, Tipo t) {
	lexeme = lex;
	line = l;
	column = c;
	tipoError = tE;
	gravedad = g;
	tipo = t;
}

std::string ErrorToken::toStringGravedad() const {
	switch (gravedad) {
		case Gravedad::REGULAR: return "REGULAR";
		case Gravedad::CRITICO: return "CRITICO";
		default: return "";
	}
}


std::string ErrorToken::toStringTipoError() const {
	switch (tipoError) {
		case TipoError::UNCLOSED_KEY: return "UNCLOSED_KEY";
		case TipoError::INVALID_TIME: return "INVALID_TIME";
		case TipoError::UNCLOSED_BRACKET: return "UNCLOSED_BRACKET";
		case TipoError::INVALID_DATE: return "INVALID_DATE";
		case TipoError::UNCLOSED_STRING: return "UNCLOSED_STRING";
		case TipoError::UNRECOGNIZED_CHAR: return "UNRECOGNIZED_CHAR";
		default: return "UNDEFINED";
	}
}

std::string ErrorToken::toStringTipoErrorSintLex() const{
	switch (tipo) {
		case Tipo::LEXICO: return "LEXICO";
		case Tipo::SINTACTICO: return "SINTACTICO";
		default: return "UNDEFINED";
	}
}


