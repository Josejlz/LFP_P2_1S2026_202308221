#pragma once
#include <string>
#include <vector>

enum class TipoError {
	UNRECOGNIZED_CHAR,
	UNCLOSED_STRING,
	INVALID_DATE,
	INVALID_TIME,
	UNCLOSED_KEY,
	UNCLOSED_BRACKET,
};

enum class Gravedad {
	REGULAR,
	CRITICO
};

enum class Tipo {
	LEXICO, 
	SINTACTICO
};

class ErrorToken
{
public:
	std::string lexeme;
	int line;
	int column;
	TipoError tipoError;
	Gravedad gravedad;
	Tipo tipo;

	ErrorToken(std::string lex, int l, int c, TipoError tE, Gravedad g, Tipo t);

	std::string toStringGravedad() const;
	std::string toStringTipoError() const;
	std::string toStringTipoErrorSintLex() const;



};

