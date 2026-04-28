#include "SyntaxAnalyzer.h"
#include <iostream>

SyntaxAnalyzer::SyntaxAnalyzer(std::vector<Token> t, ErrorManager* eM) {
	tokens = t;
	pos = 0;
	errorManager = eM;
};

bool SyntaxAnalyzer::isAtEnd() {
	return pos >= tokens.size();
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
	return(!isAtEnd()&&(peek().lexema==lexeme));
};

Token SyntaxAnalyzer::ExpectLexeme(TokenTypes type) {
	if (match(type)) {
		return consume();
	} 
};


