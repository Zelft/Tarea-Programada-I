#pragma once
typedef class Nodo *enlace;

class Nodo{
public:
    int v;
	int indice;
    enlace siguiente;
    Nodo(int v):v{v},siguiente{nullptr}, indice{ -1 }{}
	Nodo(int v, enlace siguiente) :v{ v }, siguiente{ siguiente }, indice{-1} {}
	Nodo(int v, enlace siguiente, int idx) :v{ v }, siguiente{ siguiente }, indice{idx}{}
};
