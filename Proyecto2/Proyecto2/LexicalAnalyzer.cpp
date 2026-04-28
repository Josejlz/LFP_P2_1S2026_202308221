#include "LexicalAnalyzer.h"
#include <iostream>
#include <cctype>
#include <string>
#include <stdexcept>
#include "ErrorManager.h"
#include "ErrorToken.h"

bool isDelimiter(const char& c) {
	return (c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ':'||c == ',' || c == ';');
}

char LexicalAnalyzer::peek() {
	if (pos < sourcetext.size()) {
		return sourcetext[pos];
	}
	return '\0'; // eof
}

char LexicalAnalyzer::advance() {
	if (pos < sourcetext.size()) {
		char currentChar = sourcetext[pos++];
		if (currentChar == '\n') {
			line++;
			column = 1;
		} else {
			column++;
		}
		return currentChar;
	}
	return '\0'; // eof
}

static TokenTypes clasificarPalabras(const std::string& p) {
	if (p=="TABLERO") {
		return TokenTypes::RESERVADA_MAIN;
	}
	else if (p=="COLUMNA") {
		return TokenTypes::RESERVADA_SUB;
	}
	else if (p=="tarea") {
		return TokenTypes::RESERVADA_ELEMENTO;
	}
	else if (p=="BAJA" || p == "MEDIA" || p == "ALTA") {
		return TokenTypes::NIVEL_PRIORIDAD;
	}
	else if (p == "fecha_limite"|| p == "prioridad" || p == "responsable") {
		return TokenTypes::RESERVADA_ATRIBUTO;
	}
	else {
		return TokenTypes::ATRIBUTO;
	}
}

void LexicalAnalyzer::setErrorManager(ErrorManager* errMng) {	
	errorManager = errMng;
}

ErrorManager* LexicalAnalyzer::getErrorManager() {
	return errorManager;
}

void LexicalAnalyzer::setFileContent(const std::string& content) {
	sourcetext = content;
	
}	

LexicalAnalyzer::LexicalAnalyzer() {

	sourcetext = "";
	pos = 0;
	line = 1;
	column = 1;
}

std::vector<Token> LexicalAnalyzer::getTokens() {
	return tokens;
}

void LexicalAnalyzer::NextToken() {
	pos = 0;
	line = 1;
	column = 1;
	tokens.clear();

	int state = 0;
	std::string lexeme = "";

	while (pos<sourcetext.size()) {
		char c = peek();

		switch (state) {
		case 0:
			if (isspace(c)){
				advance();
				break;
			}

			//numero, fechas, hora

			if (isdigit(c)) {
				lexeme += advance();
				state = 5;
				break;
			}

			//variables, reservadas, atributos, etc

			if (isalnum(c)||c=='_'){
				lexeme += advance();
				state = 1;
				break;
			}

			

			//delimitadores

			if (c== '(') {
				lexeme += advance();
				tokens.push_back(Token(TokenTypes::PARENTESIS_ABRE, lexeme, line, column));
				lexeme = "";
				break;
			} if (c == ')') {
				lexeme += advance();
				tokens.push_back(Token(TokenTypes::PARENTESIS_CIERRA, lexeme, line, column));
				lexeme = "";
				break;
			} if (c == '{') {
				lexeme += advance();
				tokens.push_back(Token(TokenTypes::LLAVE_ABRE, lexeme, line, column));
				lexeme = "";
				break;
			} if (c == '}') {
				lexeme += advance();
				tokens.push_back(Token(TokenTypes::LLAVE_CIERRA, lexeme, line, column));
				lexeme = "";
				break;
			} if (c == '[') {
				lexeme += advance();
				tokens.push_back(Token(TokenTypes::CORCHETE_ABRE, lexeme, line, column));
				lexeme = "";
				break;
			} if (c == ']') {
				lexeme += advance();
				tokens.push_back(Token(TokenTypes::CORCHETE_CIERRA, lexeme, line, column));
				lexeme = "";
				break;
			}
			if (c == '.') {
				lexeme += advance();
				tokens.push_back(Token(TokenTypes::PUNTO, lexeme, line, column));
				lexeme = "";
				break;
			} if (c == ',') {
				lexeme += advance();
				tokens.push_back(Token(TokenTypes::COMA, lexeme, line, column));
				lexeme = "";
				break;
			} if (c == ';') {
				lexeme += advance();
				tokens.push_back(Token(TokenTypes::PUNTO_Y_COMA, lexeme, line, column));
				lexeme = "";
				break;
			} if (c == ':') {
				lexeme += advance();
				tokens.push_back(Token(TokenTypes::DOS_PUNTOS, lexeme, line, column));
				lexeme = "";
				break;
			} if (c == '"') {
				advance();
				state = 3;
				break;
			}

			lexeme += advance();
			errorManager->addError(lexeme, line, column, TipoError::UNRECOGNIZED_CHAR, Gravedad::REGULAR, Tipo::LEXICO);
			lexeme = "";
			break;

			break;

			//variables, reservadas, atributos, etc.
		case 1:
			if (!(c == '"' || isspace(c) || isDelimiter(c))) {
				lexeme += advance();
				state = 1;
				break;
			}
			else {
				tokens.push_back(Token(clasificarPalabras(lexeme), lexeme, line, column));
				lexeme = "";
				state = 0;
				break;
			}

			break;
			//strings
		case 3:
			if (c == '"')
			{
				advance();
				tokens.push_back(Token(TokenTypes::LIT_CADENA, lexeme, line, column));
				lexeme = "";
				state = 0;
				break;
			}
			else if (c == '\0'|| c == '\n'){
				//para errores
				errorManager->addError(lexeme, line, column, TipoError::UNCLOSED_STRING, Gravedad::CRITICO, Tipo::LEXICO);
				lexeme = "";
				state = 0;
				advance();
				break;
			} else {
				lexeme += advance();
				break;
			}
			break;
			//numeros y guia a otros estados
		case 5:
			if (isdigit(c)) {
				lexeme += advance();
				break;
			}
			else if (c == '-' && lexeme.size()==4) {
				lexeme += advance(); 
				state = 10; //redirigir a mes
				break;
			}
			else if (c== ':'&&lexeme.size() <= 2) {
				lexeme += advance();
				state = 15;
				break;
			}
			else {
				if (isDelimiter(c) || isspace(c) || c== '\n') {
					tokens.push_back(Token(TokenTypes::LIT_NUMERO, lexeme, line, column));
					lexeme = "";
					state = 0;
					break;
				}
				else {
					lexeme += advance();
					state = 0;
					break;
				}
			}
			break;
		case 10:
			if (isdigit(c)) {
				lexeme += advance();
				break;
			} else if (c == '-' && lexeme.size() <= 7) {
				lexeme += advance();
				state = 11;
				break;
			}
			else {
				//error de fecha
				errorManager->addError(lexeme, line, column, TipoError::INVALID_DATE, Gravedad::CRITICO, Tipo::LEXICO);
				lexeme = "";
				state = 0;
				advance();
				break;
			}
			break;
		case 11:

			if (isdigit(c)) {
				lexeme += advance();
				if (isdigit(peek())) {
					lexeme += advance();
					tokens.push_back(Token(TokenTypes::LIT_FECHA, lexeme, line, column));
					lexeme = "";
					state = 0;
					break;
				}
				else {
					tokens.push_back(Token(TokenTypes::LIT_FECHA, lexeme, line, column));
					lexeme = "";
					state = 0;
					break;
				}
				break;
			}
			else {
				errorManager->addError(lexeme, line, column, TipoError::INVALID_DATE, Gravedad::CRITICO, Tipo::LEXICO);
				lexeme = "";
				state = 0;
				break;
			}
			break;
			
		case 15:
			if (isdigit(c)) {
				lexeme += advance();

				if (isdigit(peek())) {
					lexeme += advance();
					tokens.push_back(Token(TokenTypes::LIT_HORA, lexeme, line, column));
					lexeme = "";
					state = 0;
					break;
				}
				else {
					tokens.push_back(Token(TokenTypes::LIT_HORA, lexeme, line, column));
					lexeme = "";
					state = 0;
					break;
				}

				break;
			}
			else {
				errorManager->addError(lexeme, line, column, TipoError::INVALID_TIME, Gravedad::CRITICO, Tipo::LEXICO);
				lexeme = "";
				state = 0;
				break;
			}
			
			break;
		
		} 
	}

}
