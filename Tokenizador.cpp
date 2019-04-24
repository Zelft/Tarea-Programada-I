//#include "pch.h"
#include "Tokenizador.h"


Tokenizador::Tokenizador()
{
	int pos = 0;
	string input = "";
}

Tokenizador::Tokenizador(string entrada) {
	cout << "La entrada fue: " << entrada << endl;
	input = entrada;
	pos = 0;
}

Token Tokenizador::demeToken() {
	string tira = input;
	if (pos >= tira.length())
		return Token(nulo);

	while (pos < tira.length() && tira[pos] == ' ') pos++; // Omite blancos

	if (isalpha(tira[pos])) {
		string s = "";
		while (isalpha(tira[pos])) s.push_back(tira[pos++]);
		int cont = 5;
		if (s == "sig") {
			return Token(s, siguiente);
		}
		else if (s == "new") {
			while (pos < tira.length() && cont>0) {
				s += tira[pos++];
				cont -= 1;
			}
			if (s == "new Nodo") { return Token(s, nuevo); }
			pos -= 5;
			return Token(s, nulo);
		}
		else if (s == "repeat" || s == "Repeat") {
			return Token(s, repetir);
		}
		else if (s == "while" || s == "While") {
			return Token(s, mientras);
		}
		else if (s == "heapback" || s == "Heapback") {
			return Token(s, heapback);
		}
		else if(s.length()==1){
			return Token(s, variable);
		}
		return Token(s, nulo);
	}
	if (isdigit(tira[pos])) {
		int v = 0;
		while (isdigit(tira[pos])) v = v * 10 + tira[pos++] - '0';
		return Token(v, numero);
	}
	if (tira[pos] == '=') {
		return Token(tira[pos++], operador);
	}
	if (tira[pos] == '_') {
		return Token(tira[pos++], aleatorio);
	}
	if (tira[pos] == '-') {
		string s = "->";
		if (pos + 1 < tira.length()) {
			if (tira[pos + 1] == '>') {
				pos += 2;
				return Token(s, siguiente);
			}
		}
		pos++;
		return Token(s, nulo);
	}
	if (tira[pos] == '(' || tira[pos] == ')') {
		return Token(tira[pos++], parentesis);
	}
	pos++;
	return Token(nulo);
}