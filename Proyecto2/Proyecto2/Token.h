#pragma once
#include <string>


enum class TokenType {
	RESERVADA_MAIN,
	RESERVADA_SUB,
	RESERVADA_ELEMENTO,
	RESERVADA_ATRIBUTO,
	NIVEL_PRIORIDAD,
	LIT_FECHA,
	LIT_HORA,
	LIT_NUMERO,
	LIT_CADENA,
	LLAVE_ABRE,
	LLAVE_CIERRA,
	CORCHETE_ABRE,
	CORCHETE_CIERRA,
	PARENTESIS_ABRE,
	PARENTESIS_CIERRA,
	DOS_PUNTOS,
	PUNTO,
	PUNTO_Y_COMA,

};

class Token
{
public:
	TokenType type;
	std::string lexema;
	int line;
	int column;

	Token(TokenType type, std::string lexema, int line, int column);

	std::string typeToString() const;


};

