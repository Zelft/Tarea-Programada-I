//#include "pch.h"
#include "Procesar.h"
#include "Token.h"
#include "Tokenizador.h"
#include <ctime> 
#include <cstdlib>
#include <stdlib.h>
#include <dos.h>
#include <thread>

using std::cout;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds


void Procesar::cargarHeap()
{
	fl_color(Color::dark_blue);
	fl_line_style(Line_style::dash, 2);
	string ns[]{ "1","2","3","4","5","6","7","8","9","10","11","12","13","14"
		,"15","16","17","18","19","20","21","22","23","24","25","26","27","28" };
	int y = 15;
	int x = 15;
	for (int i = 0; i < 28; i++) {
		if (i % 7 == 0) {
			x = 15;
			y += 130;
		}
		Point xy{ x,y };
		Celda * celda = new Celda(xy, 140, 125);
		Text *t = new Text(Point(x + 5, y + 17), to_string(i + 1), 1, 15);
		labels.push_back(t);
		celdas.push_back(celda);
		Text *var = new Text(Point(x + 50, y + 17), "", 1, 15);
		referencias.push_back(var);
		NodoGUI *nodo = new NodoGUI(xy, i + 1);
		nodos.push_back(nodo);
		Arco *arco = new Arco(-1, -1, Point(0, 0), Point(1, 1), 0);
		arcos.push_back(arco);
		fl_line_style(Line_style::dash, 2);
		x += 145;
	}
}
enlace Procesar::demeNodo(string var)
{
	for (int i = 0; i < 26; i++) {
		if (variables[i].variable == var)
			return variables[i].p;
	}
	return 0;
}

void Procesar::asignaNodo(enlace q, string var)
{
	for (int i = 0; i < 26; i++) {
		if (variables[i].variable == var)
			variables[i].p = q;
	}
}

int Procesar::averiguaTipo(int origen, int destino)
{
	Celda *cOrigen = celdas[origen];
	Celda *cDestino = celdas[destino];
	if (cOrigen->getPos().x == cDestino->getPos().x) {
		//Caso 3
		return 3;
	}
	else if (cOrigen->getPos().y == cDestino->getPos().y)
	{
		//Casos 1 y 2
		bool intermedio = false;
		if (origen > destino) {
			//int temp = destino;
			//destino = origen;
			//origen = temp;
			return 2;
		}
		for (int i = origen + 1; i < destino; i++) {
			if (nodos[i]->activo) return 2;
		}
		return 1;
	}
	else if (cOrigen->getPos().x != cDestino->getPos().x && cOrigen->getPos().y != cDestino->getPos().y) {
		//Caso 4
		return 4;
	}
	return 0;
}


Procesar::Procesar()
{
	//Creando variables para Nodos
	char vars[]{ 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s'
	,'t','u','v','w','x','y','z' };
	for (int i = 0; i < 26; i++) {
		variables[i] = letra(to_string(vars[i]), 0);;
	}
	step = false;
}


void Procesar::iniciar(void* leerTexto, void*limpiarHeap, void* salir, void *pasoPaso)
{
	//Crear Ventana Principal
	Point tl(150, 150);
	win = new Simple_window(tl, 1400, 750, "Jerki Auir");
	//Asigna Color
	win->color(Color::white);
	//Inserta Imagen
	Image black{ Point(15,45),"black.jpg" };
	win->attach(black);

	Image ii{ Point{1039,0},"jerki.jpg" };
	ii.set_mask(Point(15, 95), 350, 600);
	win->attach(ii);

	Text t{ Point(15,35),"Bienvenido a Jerki Auir",1,45,Color::black };
	win->attach(t);

	//Inserta el Heap
	cargarHeap();

	for (int i = 0; i < celdas.size(); i++) { win->attach(*celdas[i]); };
	for (int i = 0; i < labels.size(); i++) { win->attach(*labels[i]); };

	cuadro = new Fl_Input(1050, 300, 315, 50);
	win->add(cuadro);

	salida = new Fl_Output(1050, 500, 315, 50);
	win->add(salida);

	//Creamos los botones

	boton1 = new Fl_Button(1120, 375, 135, 40, "Ejecutar");
	boton1->callback((Fl_Callback*)leerTexto);
	boton2 = new Fl_Button(1120, 440, 135, 40, "Paso a Paso");
	boton2->callback((Fl_Callback*)pasoPaso);
	boton3 = new Fl_Button(1200, 560, 135, 40, "Limpiar Heap");
	boton3->callback((Fl_Callback*)limpiarHeap);
	boton4 = new Fl_Button(1115, 610, 135, 40, "Salir");
	boton4->callback((Fl_Callback*)salir);


	//Plasmamos en la GUI los botones
	win->add(boton1);
	win->add(boton2);
	win->add(boton3);
	win->add(boton4);


	win->espera();
}

void Procesar::tokenizar(string s)
{
	//Tokeniza la entrada
	instrucciones = {};
	strInstrucciones = demInstrucciones(s);
	for (int i = 0; i < strInstrucciones.size(); i++) {
		Tokenizador tt{ strInstrucciones[i] };
		Token t = tt.demeToken();
		while (t.demeTipo() != nulo) {
			tokensAux.push_back(t);
			t = tt.demeToken();
		}
		instrucciones.push_back(tokensAux);
		tokensAux = {};
	}
}

vector<string> Procesar::demInstrucciones(string input)
{
	vector<string>instrucciones;
	string ins = "";
	for (int i = 0; i < input.length(); i++) {
		if (input[i] != ';') {
			ins += input[i];
		}
		else {
			instrucciones.push_back(ins);
			ins = "";
		}
	}
	instrucciones.push_back(ins);
	return instrucciones;
}


void Procesar::clasificarInstrucciones()
{
	for (int i = 0; i < instrucciones.size(); i++) {//Indexamos instrucciones
		Fl::wait();
		if (step && i < instrucciones.size()) { win->espera(); }
		bool entra = false;
		vector<Token> instruccion = instrucciones[i]; //vector con 1 instruccion
		//Instruccion Vacia Error
		if (instruccion.size() == 0) {
			return imprimaError();
		}
		//Evalua  :  Heapback
		if (instruccion[0].demeTipo() == heapback && (instruccion.size() == 2 || instruccion.size() == 1)) {
			if (instruccion.size() == 1) {
				hagaHeapback(28);
				entra = true;
			}
			else if (instruccion[1].demeTipo() == numero) {
				hagaHeapback(instruccion[1].num);
				entra = true;
			}
			else if (instruccion[1].demeTipo() == aleatorio) {
				int num = genereAleatorio();
				hagaHeapback(num);
				entra = true;
			}
			else {
				if (!entra) return imprimaError();
			}
		}
		//Evalua : repeat
		else if (instruccion[0].demeTipo() == repetir) {
			if (instruccion.size() > 4 && instruccion[1].demeTipo() == parentesis &&
				(instruccion[2].demeTipo() == numero || instruccion[2].demeTipo() == aleatorio)
				&& instruccion[3].demeTipo() == parentesis) {
				int condicion = instruccion[2].num;
				string tiraInstrucciones = "";
				for (int j = 4; j < instruccion.size(); j++) {
					tiraInstrucciones += demeChar(instruccion[j]);
				}
				tiraInstrucciones += ";";
				for (int x = i + 1; x < instrucciones.size(); x++) {
					i = instrucciones.size();
					instruccion = instrucciones[x];
					for (int y = 0; y < instruccion.size(); y++) {
						tiraInstrucciones += demeChar(instruccion[y]);
					}
					tiraInstrucciones += ";";
				}
				return repita(condicion, tiraInstrucciones);
			}
			else {
				if (!entra) return imprimaError();
			}

		}
		//Evalua while
		else if (instruccion.size() > 4 && instruccion[0].demeTipo() == mientras && instruccion.size() > 4) {
			if (instruccion[1].demeTipo() == parentesis && instruccion[2].demeTipo() == variable) {
				Token var = instruccion[2];
				int cantSig = 0;
				int j = 3;
				Token t = instruccion[j];
				while (t.demeTipo() == siguiente && j + 1 < instruccion.size()) {
					cantSig += 1;
					t = instruccion[++j];
				}
				if (instruccion[j].demeTipo() == siguiente) { cantSig++; j++; }
				if (j < instruccion.size() && (cantSig % 2 != 0 || instruccion[j].demeTipo() != parentesis)) {
					return imprimaError();
				}
				cantSig = cantSig / 2;
				string tiraInstrucciones = "";
				for (int z = j + 1; z < instruccion.size(); z++) {
					tiraInstrucciones += demeChar(instruccion[z]);
				}
				tiraInstrucciones += ";";
				for (int x = i + 1; x < instrucciones.size(); x++) {
					i = instrucciones.size();
					instruccion = instrucciones[x];
					for (int y = 0; y < instruccion.size(); y++) {
						tiraInstrucciones += demeChar(instruccion[y]);
					}
					tiraInstrucciones += ";";
				}
				return hagaMientras(var.tokenS, cantSig, tiraInstrucciones);
			}
		}//Empieza con variable a..*/*
		//Evalua Variable
		else if (instruccion[0].demeTipo() == variable) {
			//  p
			if (instruccion.size() == 1) {
				referenciaPuntero(instruccion[0].tokenS, 0);
				entra = true;
			}
			//Calculando cantidad de Siguientes
			int cantSig = 0;
			int j;
			(entra ? j = 0 : j = 1);
			Token t = instruccion[j];
			while (t.demeTipo() == siguiente && j + 1 < instruccion.size()) {
				cantSig += 1;
				t = instruccion[++j];
			}
			if (instruccion[j].demeTipo() == siguiente) cantSig++;
			//Termina de calcular siguientes
			if (cantSig % 2 != 0 && t.demeTipo() != variable) {
				return imprimaError();

			}
			else
			{
				if (cantSig % 2 != 0 && instruccion[j].demeTipo() == variable) {
					cantSig -= 1;
					if (j == instruccion.size() - 1) {
						//Evalua Referencia a Campo Instruccion 4
						cantSig = cantSig / 2;
						referenciaCampo(instruccion[0].tokenS, cantSig);
						entra = true;
					}

					if (instruccion.size() > j + 2 && instruccion[j + 1].demeTipo() == operador &&
						(instruccion[j + 2].demeTipo() == numero || instruccion[j + 2].demeTipo() == aleatorio)) {
						//Evalua Referencia a Campo = Valor
						int numero = 0;
						if (instruccion[j + 2].demeTipo() == 2) {
							numero = instruccion[j + 2].num;
						}
						else {
							numero = genereAleatorio();
						}
						cantSig = cantSig / 2;
						asignarValor(instruccion[0].tokenS, cantSig, numero);
						entra = true;
					}

					//Referencia Campo = Referencia Campo Instruccion 6
					if (!entra && instruccion[j + 1].demeTipo() == operador && (instruccion[j + 2].demeTipo() == variable
						&& j + 4 < instruccion.size())) {
						int cantSig2 = calcularSiguientes(instruccion, j + 3);
						if (cantSig2 % 2 == 0) {
							return imprimaError();
						}
						cantSig2 = (cantSig2 - 1) / 2;
						if (instruccion[instruccion.size() - 1].demeTipo() == variable) {
							cantSig = cantSig / 2;
							referenciaCampo_referenciaCampo(instruccion[0].tokenS, cantSig, instruccion[j + 2].tokenS, cantSig2);
							entra = true;
						}

					}

				}
				// Asignar Ref_ Puntero = Ref_Puntero  Instruccion 2
				if (instruccion[j].demeTipo() == operador && instruccion.size() > j + 1 && instruccion[j + 1].demeTipo() == variable) {
					cantSig = calcularSiguientes(instruccion, 1) / 2;
					int cantSig2 = 0;
					if (instruccion.size() > j + 2) { cantSig2 = calcularSiguientes(instruccion, j + 2); }
					if (cantSig2 % 2 == 0) {
						asignaPuntero(instruccion[0].tokenS, cantSig, instruccion[j + 1].tokenS, cantSig2 / 2);
						entra = true;
					}
					if (!entra) return imprimaError();
				}
				//Asignar referencia a puntero Instrucción 1
				if (cantSig + 1 == instruccion.size() && !entra) {
					cantSig = cantSig / 2;
					referenciaPuntero(instruccion[0].tokenS, cantSig);
					entra = true;
				}
				// Crear nuevo nodo
				if (j + 4 == instruccion.size() - 1 && instruccion[j].demeTipo() == operador && instruccion[j + 1].demeTipo() == nuevo && instruccion[j + 2].demeTipo() == parentesis
					&& (instruccion[j + 3].demeTipo() == numero || instruccion[j + 3].demeTipo() == aleatorio) &&
					instruccion[j + 4].demeTipo() == parentesis) {
					int numero = 0;
					if (instruccion[j + 3].demeTipo() == 2) {
						numero = instruccion[j + 3].num;
					}
					else {
						numero = genereAleatorio();
					}
					cantSig = cantSig / 2;
					crearNodo(instruccion[0].tokenS, cantSig, numero);
					entra = true;

				}

				//Eliminar referencia Instruccion 3
				if (!entra && instruccion.size() > j + 1 && instruccion[j].demeTipo() == operador && instruccion[j + 1].demeTipo() == numero && instruccion[j + 1].num == 0) {
					if (cantSig % 2 == 0) {
						cantSig = cantSig / 2;
						eliminarReferencia(instruccion[0].tokenS, cantSig);
						entra = true;
					}
				}
				if (!entra) return imprimaError();
			}
			if (!entra) return imprimaError();
		}
		else
		{
			if (!entra) return imprimaError();
		}
		if (!entra) return imprimaError();
	}
}

void Procesar::salir()
{
	win->hide();
}

void Procesar::limpiarHeap()
{
	enlace p;
	for (int i = 0; i < celdas.size(); i++) {
		referencias[i]->limpiar();
		nodos[i]->activo = false;
		win->detach(*arcos[i]);
		win->detach(*nodos[i]);
	}
	char vars[]{ 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s'
		,'t','u','v','w','x','y','z' };
	for (int j = 0; j < 26; j++) {
		variables[j] = letra(to_string(vars[j]), 0);;
	}
	salida->replace(0, 100, "", 0);
	cuadro->replace(0, 100000, "", 0);
	win->add(salida);
	win->add(cuadro);
	win->redraw();
}

int Procesar::demeArco(int idx, bool destino)
{
	if (destino) {
		for (int i = 0; i < arcos.size(); i++) {
			if (arcos[i]->idxDestino == idx) return i;
		}
		return -1;
	}
	else {
		for (int i = 0; i < arcos.size(); i++) {
			if (arcos[i]->idxOrigen == idx) return i;
		}
		return -1;
	}
}

int Procesar::genereAleatorio()
{
	return rand() % 99;
}

string Procesar::demeChar(Token auxTokens)
{
	switch (auxTokens.demeTipo())
	{
	case numero: {
		return to_string(auxTokens.num);
	}
	case variable: {
		return auxTokens.tokenS;
	}
	case operador: {
		return "=";
	}

	case aleatorio: {
		return to_string(auxTokens.tokenC);
	}
	case siguiente: {
		return auxTokens.tokenS;
	}
	case nuevo: {
		return auxTokens.tokenS;
	}
	case parentesis: {
		return to_string(auxTokens.tokenC);
	}
	case repetir: {
		return auxTokens.tokenS;
	}

	case heapback: {
		return auxTokens.tokenS;
	}
	case mientras: {
		return auxTokens.tokenS;
	}
	default:
		return 0;
	}
}

int Procesar::calcularSiguientes(vector<Token> instruccion, int index)
{
	int cantSig = 0;
	int j = index;
	Token t = instruccion[j];
	while (t.demeTipo() == siguiente && j + 1 < instruccion.size()) {
		cantSig += 1;
		t = instruccion[++j];
	}
	if (instruccion[j].demeTipo() == siguiente) cantSig++;

	return cantSig;
}

string Procesar::demeStr(vector<Token> instruccion, int index)
{
	string tiraInstrucciones = "";
	for (int j = index; j < instruccion.size(); j++) {
		tiraInstrucciones += demeChar(instruccion[j]);
	}
	return tiraInstrucciones;
}

//Funciones de Instrucciones
void Procesar::hagaHeapback(int x)
{
	cout << "Haciendo Heapback: " << x << " veces.\n\n";
	for (int i = 0; i < x; i++) {

	}
	salida->replace(0, 1000, "");
	string out = to_string(x);
	const char* pOutput = (char*)out.c_str();
	salida->insert(pOutput);
}

void Procesar::imprimaError()
{
	cout << "Error\n";
	salida->replace(0, 100, "", 0);
	this->salida->insert(" Error!");
}

void Procesar::repita(int condicion, string tira)
{
	cout << "\t** Repitiendo**\n";
	cout << "Repitiendo " << condicion << " las instrucciones: " << tira << "\n\n";
	for (int i = 0; i < condicion; i++)
	{
		leerInput(tira);
		instrucciones.pop_back();
		clasificarInstrucciones();
	}
	salida->replace(0, 100, "", 0);
	string out = to_string(condicion);
	const char* pOutput = (char*)out.c_str();
	salida->insert(pOutput);
}

void Procesar::referenciaCampo(string variable, int cantSig)
{
	cout << "\t** Haciendo una Referencia a Campo**\n";
	cout << "Imprimiendo la variable " << variable << " con cant sig = " << cantSig << "\n\n";
	enlace p = demeNodo(variable);
	if (p) {
		while (cantSig != 0) {
			if (p->siguiente) {
				p = p->siguiente;
				cantSig--;
			}
			else
			{
				return imprimaError();
			}

		}
		salida->replace(0, 100, "", 0);
		cout << "Imprimiendo el p->v: " << p->v;
		string out = to_string(p->v);
		const char* pOutput = (char*)out.c_str();
		salida->insert(pOutput);
	}
	else {
		imprimaError();
	}
}

void Procesar::asignarValor(string variable, int cantSig, int valor)
{
	cout << "\t** Asignando un Valor**\n";
	cout << "Asignando " << valor << " a la variable " << variable << " con cant sig = " << cantSig << "\n\n";
	enlace p = demeNodo(variable);
	enlace copyp = p;
	if (p) {
		while (cantSig != 0) {
			if (p->siguiente) {
				p = p->siguiente;
				cantSig--;
			}
			else
			{
				return imprimaError();
			}

		}
		salida->replace(0, 100, "", 0);
		p->v = valor;
		int idx = p->indice;
		nodos[idx]->setValor(valor);
		win->redraw();
		asignaNodo(copyp, variable);
		cout << "Ok \n\n";
		salida->insert("Ok");
	}
	else {
		imprimaError();
	}
}

void Procesar::referenciaPuntero(string variable, int cantSig) {
	cout << "\t** ReferenciaPuntero **\n";
	cout << "Referenciando Puntero " << variable << " con cantidad de veces: " << cantSig << "\n\n";
	enlace p = demeNodo(variable);
	if (!p) return imprimaError();
	while (cantSig > 0) {
		if (p->siguiente) {
			p = p->siguiente;
			cantSig--;
		}
		else {
			return imprimaError();
		}
	}
	cout << "\nLa direccion es: " << &p << " y se encuentra en la celda " << p->indice << endl;
	string out = to_string(&p);
	const char* pOutput = (char*)out.c_str();
	salida->replace(0, 100, "", 0);
	salida->insert(pOutput);
}

void Procesar::asignaPuntero(string var1, int sig1, string var2, int sig2)
{
	cout << "\t** Asigna Puntero **\n";
	cout << "Asignando a la variable " << var1 << " con " << sig1 << " siguientes , la variable " << var2 << " con " << sig2 << " siguientes" << "\n\n";
	enlace p = demeNodo(var1);
	enlace q = demeNodo(var2);
	enlace copyP = p;
	cout << "Imprimiendo " << copyP->v;
	if (p && q) {
		int indViejo;
		int indActual;
		while (sig2 != 0) {
			if (q->siguiente) {
				q = q->siguiente;
				sig2--;
			}
			else
			{
				return imprimaError();
			}
		}
		indActual = q->indice;
		if (sig1 == 0) {
			salida->replace(0, 100, "", 0);
			indViejo = p->indice;
			indActual = q->indice;
			p = q;
			referencias[indViejo]->eliminaLetra(*var1.c_str());
			referencias[indActual]->agregarLetra(var1);
			win->attach(*referencias[indActual]);
			win->attach(*referencias[indViejo]);
			win->redraw();
			asignaNodo(q, var1);
			salida->insert(" Ok");
			cout << "Ok";
		}
		else {
			while (sig1 != 1) {
				if (p->siguiente) {
					p = p->siguiente;
					sig1--;
				}
				else
				{
					return imprimaError();
				}
			}
			indViejo = p->indice;
			salida->replace(0, 100, "", 0);
			int indOrigen = p->indice;
			int indDestino = q->indice;
			int tipoArco = averiguaTipo(indOrigen, indDestino);
			p->siguiente = q;
			//referencias[indViejo]->eliminaLetra(*var1.c_str());
			//referencias[indActual]->agregarLetra(var1);
			if (arcos[indOrigen]->getOrigen().x != 0) {
				win->detach(*arcos[indOrigen]);
				win->redraw();
			}
			arcos[indOrigen] = new Arco(indOrigen, indDestino, celdas[indOrigen]->getPos(), celdas[indDestino]->getPos(), tipoArco);;
			win->attach(*arcos[indOrigen]);
			//win->attach(*referencias[indActual]);
			//win->attach(*referencias[indViejo]);
			win->redraw();
			asignaNodo(copyP, var1);
			salida->insert(" Ok");
			cout << "Ok";
		}
	}
	else {
		cout << "FML";
		imprimaError();
	}

}

void Procesar::eliminarReferencia(string var, int cantSig)
{
	cout << "\t** Elimina referencia **\n";
	cout << "Eliminando la variable " << var << " con " << cantSig << " siguientes" << "\n\n";
	enlace p = demeNodo(var);
	enlace copyP = p;

	if (!p) {
		return imprimaError();
	}
	if (cantSig == 0) {
		int idx = p->indice;
		delete p;
		copyP = NULL;
		asignaNodo(NULL, var);
		nodos[idx]->activo = false;
		win->detach(*nodos[idx]);
		//referencias[idx]->eliminaLetra(*var.c_str());
		referencias[idx]->set_label("");
		win->redraw();
		salida->replace(0, 100, "Ok", 0);
		cout << "Nodo eliminado\n";
		int indiceArco = demeArco(idx, false);
		if (indiceArco != -1) {
			win->detach(*arcos[indiceArco]);
			arcos[indiceArco] = new Arco(-1, -1, Point{ 0,0 }, Point{ 0,0, }, 0);
			win->redraw();
		}
	}
	else {
		while (cantSig != 1) {
			if (p->siguiente) {
				p = p->siguiente;
				cantSig--;
			}
			else
			{
				return imprimaError();
			}
		}
		int idx = p->siguiente->indice;
		int indiceArco = demeArco(idx, true);
		if (indiceArco != -1) {
			win->detach(*arcos[indiceArco]);
			arcos[indiceArco] = new Arco(-1, -1, Point{ 0,0 }, Point{ 0,0, }, 0);
			win->redraw();
		}
		indiceArco = demeArco(idx, false);
		if (indiceArco != -1) {
			win->detach(*arcos[indiceArco]);
			arcos[indiceArco] = new Arco(-1, -1, Point{ 0,0 }, Point{ 0,0, }, 0);
			win->redraw();
		}
		delete p->siguiente;
		p->siguiente = nullptr;
		asignaNodo(p, var);
		nodos[idx]->activo = false;
		referencias[idx]->set_label("");
		win->detach(*nodos[idx]);
		win->redraw();
		cout << "Nodo eliminado\n";
		salida->replace(0, 100, "", 0);
		salida->insert(" Ok");
	}

}

void Procesar::referenciaCampo_referenciaCampo(string var1, int cantSig, string var2, int cantSig2)
{
	cout << "\t** Referencia Campo = Referencia Campo**\n";
	cout << "Asignando a la variable " << var1 << " con cantidad siguiente " << cantSig
		<< " la variable " << var2 << " con cant siguientes " << cantSig2 << "\n\n";
	enlace p = demeNodo(var1);
	enlace q = demeNodo(var2);
	enlace copyP = p;
	if (p && q) {
		while (cantSig2 != 0) {
			if (q->siguiente) {
				q = q->siguiente;
				cantSig2--;
			}
			else
			{
				return imprimaError();
			}
		}
		while (cantSig != 0) {
			if (p->siguiente) {
				p = p->siguiente;
				cantSig--;
			}
			else
			{
				return imprimaError();
			}

		}
		salida->replace(0, 100, "", 0);
		p->v = q->v;
		int idx = p->indice;
		nodos[idx]->setValor(q->v);
		win->redraw();
		asignaNodo(copyP, var1);
		salida->insert(" Ok");
		cout << "Ok";
	}
	else {
		imprimaError();
	}
}

void Procesar::crearNodo(string var, int cantSig, int x)
{
	cout << "\t** Creando Nuevo Nodo**\n";
	cout << "Creando un nuevo nodo con el valor " << x << " en la variable " << var << " con la cantidad de siguientes " << cantSig << "\n\n";
	if (x < 0 || x>99) {
		return imprimaError();
	}
	bool full = true;
	for (int i = 0; i < nodos.size(); i++) {
		if (!nodos[i]->activo) full = false;
	}
	if (full) {
		cout << "Overflow\n";
		salida->replace(0, 100, "", 0);
		salida->insert("Overflow");
		return;
	}
	enlace p = demeNodo(var);
	enlace copyP = p;
	enlace q = new Nodo(x);
	if (cantSig == 0 && !p) {
		asignaNodo(q, var);
		for (int i = 0; i < nodos.size(); i++) {
			if (!nodos[i]->activo) {
				nodos[i]->activo = true;
				nodos[i]->setValor(x);
				q->indice = i;
				referencias[i]->agregarLetra(var);
				win->attach(*referencias[i]);
				win->attach(*nodos[i]);
				win->redraw();
				i = nodos.size();
				cout << "Nodo creado\n";
				salida->replace(0, 100, "", 0);
				salida->insert("Ok");
			}
		}
	}
	else if (cantSig != 0 && p)
	{
		while (cantSig != 1) {
			if (p->siguiente) {
				p = p->siguiente;
				cantSig--;
			}
			else
			{
				return imprimaError();
			}
		}
		for (int i = 0; i < nodos.size(); i++) {
			if (!nodos[i]->activo) {
				nodos[i]->activo = true;
				nodos[i]->setValor(x);
				win->attach(*nodos[i]);
				win->redraw();
				q->indice = i;
				cout << "Nodo creado\n";
				salida->replace(0, 100, "", 0);
				salida->insert(" Ok");
				i = nodos.size();
			}
		}
		int indOrigen = p->indice;
		int indDestino = q->indice;
		int tipoArco = averiguaTipo(indOrigen, indDestino);
		p->siguiente = q;
		//referencias[indViejo]->eliminaLetra(*var1.c_str());
		//referencias[indActual]->agregarLetra(var1);
		if (arcos[indOrigen]->getOrigen().x != 0) {
			win->detach(*arcos[indOrigen]);
			win->redraw();
		}
		arcos[indOrigen] = new Arco(indOrigen, indDestino, celdas[indOrigen]->getPos(), celdas[indDestino]->getPos(), tipoArco);;
		win->attach(*arcos[indOrigen]);

		p->siguiente = q;
		asignaNodo(copyP, var);
	}
	else if (cantSig == 0 && p)
	{
		cout << "entra";
		int indicePasado = p->indice;
		referencias[indicePasado]->eliminaLetra(*var.c_str());
		win->attach(*referencias[indicePasado]);
		win->redraw();
		for (int i = 0; i < nodos.size(); i++) {
			if (!nodos[i]->activo) {
				nodos[i]->activo = true;
				nodos[i]->setValor(x);
				q->indice = i;
				win->attach(*nodos[i]);
				referencias[i]->agregarLetra(var);
				win->attach(*referencias[i]);
				win->redraw();
				i = nodos.size();
			}
		}
		asignaNodo(q, var);
		cout << "Nodo creado\n";
		salida->replace(0, 100, "", 0);
		salida->insert(" Ok");
		cout << "Imprimiendo p->indice " << p->indice;
	}
	else
	{
		imprimaError();
	}
}

void Procesar::hagaMientras(string var, int cantSig, string tira)
{
	cout << "\t** Haciento un While**\n";
	cout << "Mientras la variable " << var << " con cantidad de siguientes " << cantSig << " haciendo la tira : " << tira;
	enlace p = demeNodo(var);
	if (!p) return imprimaError();
	while (cantSig != 1) {
		if (p->siguiente) {
			p = p->siguiente;
			cantSig--;
		}
		else
		{
			return imprimaError();
		}
	}
	int iteraciones = 0;
	while (p) {
		leerInput(tira);
		instrucciones.pop_back();
		clasificarInstrucciones();
	}
	cout << "Ok, mientras realizado: " << iteraciones << "veces";
	salida->replace(0, 100, "", 0);
	string out = to_string(iteraciones);
	const char* pOutput = (char*)out.c_str();
	salida->insert(pOutput);

}

Procesar::~Procesar()
{
	delete win;
}

