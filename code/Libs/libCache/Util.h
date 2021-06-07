#ifndef UTIL_H
#define UTIL_H
#include <string>

//#include <map>
//#include <vector>
//#include <set>

#include <iostream>
#include <assert.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <list>
#include <math.h>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <sys/resource.h>
#include <sys/time.h>
#include <queue>
#include <malloc.h>

using namespace std;


#include "./glob.h"
//#include "/home/gvcosta/SIMULADOR/libcppsim-0.2.1/MAQUINABSP/CASO12/glob.h"
class UTIL
{
private:
  double suma;
  vector<string>::iterator itv;
  set<int> procs;
  vector<string> tokens;
  vector<string> terminos;
  string vacio;
  int P;
  int proc;
public:
  UTIL(int P1)
  {
    vacio=string(" ");
    P=P1;
  }

  ~UTIL()
  {
  }

  /**
   * Convierte los elementos de un vector de strings a un string concatenado.
   *
   * @param *t es un puntero al vector strings.
   * @param *destino es el string donde se almacenan los strings anteriores concatenados.
   */
  void transform(vector<string> *, string *);

  /**
   * Prepara una consulta para ser procesada, para ello ordena sus terminos.
   *
   * @param query es donde estan los terminos de la consulta
   * @param *entrada es donde se almacenan los terminos ordenados de la consulta.
   */
  void preparar(string, string *);

  /**
   * Obtiene la suma total del costo de cada termino de la consulta segun los largos de
   * las posting lists de dichos terminos.
   *
   * @param query es la consulta que se estimara su costo.
   * @param *words es un mapa que contiene los terminos y los largos de sus respectivos
   *               posting lists.
   *
   * @return la suma de los costos de cada terminos.
   */
  double getCost(string,map<int,int>*);

  /**
   * Permite dividir un string en varios utilizando para ello un caracter delimitador.
   *
   * @param str es el string a dividir.
   * @param tokens es un vector donde se almacenan los substrings del termino dividido.
   * @param delimiters es un string que se utiliza para saber en donde se debe dividir
   *                   el string antes mencionado.
   */
  void Tokenize(const string&,
                vector<string>&,
                const string&);

  /**
   * Lee un archivo y llena un mapa con idt y el correspondiente largo.
   *
   * @param *_file es el nombre del archivo a leer.
   * @param *words es el puntero a mapa que sera llenado.
   */
  void leer_words(char *, map<int,int> *);

  /**
   * Lee un mapeo del tipo [ID_DOC PROCESADOR] desde archivo y lo carga en un map.
   *
   * @param *_file es el nombre de archivo que contiene el mapeo.
   * @param *mapeo es el mapa donde se almacena el mapeo.
   *
   * @warning al parece no esta en uso.
   */
  void leer_mapeo(char *, map<int,int> *);

  /**
   * De acuerdo a un conjunto de documentos retorna el numero de procesadores
   * en que se encuentran.
   *
   * @param docs
   * @param *mapeo
   *
   * @return el numero de procesadores.
   */
  int getProcs(string, map<int,int> *);

};
class Nodo
{
public:
  double prioridad;
  string key;
  bool marca;
  int tam;

  Nodo( double _prio, string _key, int _tam )
  {
    prioridad = _prio;
    key       = _key;
    tam       = _tam;
    marca     = true;
  }
};

//---------------------------------------------------

struct Compara: binary_function<Nodo*,Nodo*,bool>
{
  bool operator() (Nodo* a, Nodo* b)
  {
    return a->prioridad > b->prioridad;
  }
};

//---------------------------------------------------

struct Compara2: binary_function<Nodo*,Nodo*,bool>
{
  bool operator() (Nodo* a, Nodo* b)
  {
    return a->prioridad > b->prioridad;
  }
};

#endif
