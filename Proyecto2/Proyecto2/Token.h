#pragma once
#include <string>


enum class TokenTypes {
	RESERVADA_MAIN,
	RESERVADA_SUB,
	RESERVADA_ELEMENTO,
	RESERVADA_ATRIBUTO,
	NIVEL_PRIORIDAD,
	ATRIBUTO,
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
	COMA,
	COMILLA,
	UNDEFINED

};

class Token
{
public:
	TokenTypes type;
	std::string lexema;
	int line;
	int column;

	Token(TokenTypes type, std::string lexema, int line, int column);

	std::string typeToString() const;


};

