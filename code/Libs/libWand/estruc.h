#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H
#include <string>
#include <stdio.h>
using namespace std;

class DatosQuery{
public:
   int a;
	int id_query;
	int n_terminos;
	unsigned long long *terminos;
	unsigned int *valores;
   string key;
	DatosQuery(){
		id_query=0;
		n_terminos=0;
		terminos=NULL;
		valores=NULL;
	}
	DatosQuery(int _id_query, int _n_terminos, unsigned long long *_terminos, unsigned int *_valores){
		id_query=_id_query;
		n_terminos=_n_terminos;
		terminos=new unsigned long long[n_terminos];
		valores=new unsigned int[n_terminos];
		for(int i=0; i<n_terminos; i++){
			terminos[i]=_terminos[i];
			valores[i]=_valores[i];
		}
	}

void llenarQ(int _id_query, int _n_terminos, unsigned long long *_terminos, unsigned int *_valores){
      id_query=_id_query;
      n_terminos=_n_terminos;

      liberarRam();
      terminos=new unsigned long long[n_terminos];
      valores=new unsigned int[n_terminos];
      for(int i=0; i<n_terminos; i++){
         terminos[i]=_terminos[i];
         valores[i]=_valores[i];
      }
}
   


void setKey(string var)
	{
   	  key = var;
   }

   string getKey()
   {
   return key;
   }
void lib()
{
a=1;
}

    void liberarRam()
    {
      if(terminos!=NULL){
         delete [] terminos;
         terminos=NULL;
      }
      if(valores!=NULL){
         delete [] valores;
         valores=NULL;
      }
    }
 
	~DatosQuery(){
		if(terminos!=NULL){
			delete [] terminos;
			terminos=NULL;
		}
		if(valores!=NULL){
			delete [] valores;
			valores=NULL;
		}
	}
};
/*
class Nodo{
public:
	unsigned int id_doc;
	unsigned int valor;
	Nodo(){
		id_doc=0;
		valor=0;
	}
	Nodo(unsigned int _id_doc, unsigned int _valor){
		id_doc=id_doc;
		valor=_valor;
	}
	//dejare el ordenamiento natural por doc_id (aqui no hay frecuencias)
	//tambien para el comparador de nodos
	bool operator> (class Nodo x) {return id_doc>x.id_doc;}
};
*/
/*
struct comp: binary_function<Nodo*,Nodo*,bool>{
bool operator() (Nodo* x,Nodo* y) 
{return x->id_doc > y->id_doc;}
};
*/

#endif


