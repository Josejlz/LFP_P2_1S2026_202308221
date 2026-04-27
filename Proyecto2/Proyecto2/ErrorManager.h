#pragma once
#include <string>
#include <vector>
#include "ErrorToken.h"

class ErrorManager {
public:
	std::vector<ErrorToken> getErrorList();
	void limpiarErrores();
	void addError(std::string lex, int l, int c, TipoError tE, Gravedad g, Tipo t);
private:
	std::vector<ErrorToken> errorList;
};

