#include "Graph.h"
#include "Simple_window.h"
#include "Procesar.h"
#include <stdio.h>   
#include <stdlib.h>
#include <map>

using namespace std;
using namespace Graph_lib;


void leerTexto(Fl_Widget*, void*);
void limpiarH(Fl_Widget*,void*);
void salida(Fl_Widget *, void *);
void pasoPaso(Fl_Widget *, void *);

Procesar p{};

int main()
{
	p.iniciar(leerTexto, limpiarH, salida, pasoPaso);
}


void leerTexto(Fl_Widget*, void*) {
	p.step = false;
	const char * text = p.cuadro->value();
	p.tokenizar(text);
	p.clasificarInstrucciones();
}
void limpiarH(Fl_Widget *, void *)
{
	p.limpiarHeap();

}
void salida(Fl_Widget *, void *) {
	p.salir();
}
void pasoPaso(Fl_Widget *, void *) {
	p.step = true;
	const char * text = p.cuadro->value();
	p.tokenizar(text);
	p.clasificarInstrucciones();
}