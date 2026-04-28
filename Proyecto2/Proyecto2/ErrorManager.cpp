#include "ErrorManager.h"
#include <string>
#include <vector>
#include "ErrorToken.h"

void ErrorManager::addError(std::string lex, int l, int c, TipoError tE, Gravedad g, Tipo t) {
	errorList.push_back(ErrorToken(lex, l, c, tE, g, t));
}

void ErrorManager::limpiarErrores() {
	errorList.clear();
}

std::vector<ErrorToken> ErrorManager::getErrorList(){
	return errorList;
}


