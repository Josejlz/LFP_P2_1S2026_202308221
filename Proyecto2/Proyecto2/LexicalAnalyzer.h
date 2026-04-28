#pragma once
#include <vector>
#include <string>
#include "Token.h"
#include "ErrorManager.h"

class ErrorManager;

class LexicalAnalyzer
{
public:
	LexicalAnalyzer();
	void setFileContent(const std::string& content);
	void setErrorManager(ErrorManager* errMng);
	ErrorManager* getErrorManager();

	std::vector<Token> getTokens();
	void NextToken();
	char peek();
	char advance();

	int pos;
	int line;
	int column;
	std::string sourcetext;



private:
	std::string filecontent;
	std::vector<Token> tokens;
	ErrorManager* errorManager = nullptr;


};

