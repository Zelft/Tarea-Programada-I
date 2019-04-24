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


class Procesar:public Tokenizador{
private:
	In_box * in;
	Fl_Button *boton1;
	Fl_Button *boton2;
	Fl_Button *boton3;
	Fl_Button *boton4;
	Fl_Output *salida;
	

public:
	Simple_window * win;
	~Procesar();
};

