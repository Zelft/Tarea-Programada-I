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

class Procesar:public Tokenizador{
private:
	In_box * in;
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
	bool step;
	vector<NodoGUI*>nodos;
	void salir();
	Fl_Input *cuadro;
	Procesar();
	void iniciar(void*, void*, void*, void*);
	void tokenizar(string);
	vector<string> demInstrucciones(string);
	void clasificarInstrucciones();
	int genereAleatorio();
	string demeChar(Token);
	int calcularSiguientes(vector<Token>,int);
	string demeStr(vector<Token>, int);
	void limpiarHeap();
	int demeArco(int ,bool);

	//Funciones de Instrucciones
	void hagaHeapback(int);
	void imprimaError();
	void repita(int, string);
	void referenciaCampo(string, int);
	void asignarValor(string, int,int);
	void referenciaPuntero(string , int );
	void asignaPuntero(string, int, string, int);
	void eliminarReferencia(string, int);
	void referenciaCampo_referenciaCampo(string, int, string, int);
	void crearNodo(string, int, int);
	void hagaMientras(string, int, string);
	~Procesar();
};

