#pragma once
#include <string>

using namespace std;

enum Tipo {
	variable, operador, numero, aleatorio,
	nulo, siguiente, nuevo, parentesis, repetir, heapback, mientras
};
struct Token {
	Tipo tipo;
	string tokenS;
	int num;
	char tokenC;

	Token(string st);
	Token(string st, Tipo k);
	Token(int st, Tipo k);
	Token(char s, Tipo k);
	Token(int n);
	Token();
	Token(Tipo k);
	Tipo demeTipo();
	friend ostream& operator<<(ostream &os, Token & t);
	~Token() {};
};
