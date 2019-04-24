#include "Token.h"

Token::Token(Tipo k)
{
	tipo = k;
}

Tipo Token::demeTipo()
{
	return tipo;
}


Token::Token(string st)
{
	tipo = nulo;
	tokenS = st;
}

Token::Token(string st, Tipo k)
{
	tipo = k;
	tokenS = st;
}

Token::Token(int st, Tipo k)
{
	tipo = k;
	num = st;
}

Token::Token(char s, Tipo k)
{
	tipo = k;
	tokenC = s;
}

Token::Token(int n)
{
	tipo = numero;
	num = n;
}

Token::Token() {
	tipo = nulo;
}

ostream& operator <<(ostream &os, Token & t) {
	switch (t.tipo) {
	case variable: {
		os << "[" << t.tokenS << "]" << "  variable\n";
		break;
	}
	case operador:
		os << "[" << t.tokenC << "]" << "   operador\n";
		break;
	case numero:
		os << "[" << t.num << "]" << "   numero\n";
		break;
	case siguiente:
		os << "[" << t.tokenS << "]" << " siguiente\n";
		break;
	case aleatorio:
		os << "[" << t.tokenC << "]" << " aleatorio\n";
		break;
	case nuevo:
		os << "[" << t.tokenS << "]" << " nuevo\n";
		break;
	case parentesis:
		os << "[" << t.tokenC << "]" << " parentesis\n";
		break;
	case repetir:
		os << "[" << t.tokenS << "]" << " repetir\n";
		break;
	case mientras:
		os << "[" << t.tokenS << "]" << " mientras\n";
		break;
	case heapback:
		os << "[" << t.tokenS << "]" << " heapback\n";
		break;
	case nulo:
		os << "[ ]" << "  nulo\n";
		break;
	}
	return os;
}
