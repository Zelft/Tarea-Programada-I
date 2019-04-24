#pragma once

#include <vector>
#include "Token.h"
#include <iostream>
#include <string>

using namespace std;

class Tokenizador {
private:
	int pos;
	vector<string>instrucciones;
public:
	string input;
	Tokenizador();
	Tokenizador(string entrada);
	Token demeToken();
};


