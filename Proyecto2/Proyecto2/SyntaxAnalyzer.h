#pragma once
#include <vector>
#include "Token.h"
#include "ErrorManager.h"

class SyntaxAnalyzer
{

private:
	std::vector<Token> tokens;
	int pos;
	ErrorManager* errorManager;

	Token peek();
	Token consume();
	bool match(TokenTypes Type);
	bool matchLexeme(const std::string& lexeme);
	Token expect(TokenTypes type);
	Token expectLexeme(const std::string& lexeme);
	bool isAtEnd();

	void parsePrograma();
	void parseColumnas();
	void parseColumna();
	void parseTablero();
	void parseTareas();
	void parseTarea();
	void parseAtributos();
	void parseAtributo();

public:
	SyntaxAnalyzer(std::vector<Token> t, ErrorManager* eM);
	void parse();

};

