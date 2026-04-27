#include "ErrorManager.h"
#include <string>
#include <vector>
#include "ErrorToken.h"

void ErrorManager::addError(std::string, int l, int c, TipoError tE, Gravedad g, Tipo t) {

}

void ErrorManager::limpiarErrores() {
	errorList.clear();
}

std::vector<ErrorToken> ErrorManager::getErrorList(){
	return errorList;
}


