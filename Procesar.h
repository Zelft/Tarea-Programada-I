#pragma once
#include <vector>
#include <string>
#include "Graph.h"
#include "Nodo.h"
#include "Simple_window.h"
#include "Tokenizador.h"
#include <FL/Fl_Input.H>
#include "Token.h"
using namespace std;

struct letra {
	string variable;
	enlace p;
	letra() :variable{ "" }, p{ nullptr }{};
	letra(string p, enlace q) : variable{ p }, p{ q }{};
	letra(string p) : variable{ p }, p{ NULL }{};
};

class Procesar :public Tokenizador {
private:
	In_box * in;
	Simple_window * win;
	Fl_Button *boton1;
	Fl_Button *boton2;
	Fl_Button *boton3;
	Fl_Button *boton4;
	letra variables[26];
	vector<vector<Token>> instrucciones;
	vector<string> strInstrucciones;
	Fl_Output *salida;
	vector<Celda *>celdas;
	vector<Text *>labels;
	vector<Text*>referencias;
	vector<Arco *> arcos;
	void cargarHeap();
	vector<Token>tokensAux;
	enlace demeNodo(string);
	void asignaNodo(enlace, string);
	int averiguaTipo(int, int);

	

public:
	Simple_window * win;
	~Procesar();
};

