#ifndef LRU_H
#define LRU_H

//#include <map>
#include "./global.h"
#include "Util.h"
#include <queue>
#include <iostream>
//#include "/home/gvcosta/SIMULADOR/libcppsim-0.2.1/MAQUINABSP/CASO12/glob.h"
class UTIL;
using namespace std;
/**
 * Esta clase implementa una politica de reemplazo LRU. Es utilizada en las Cache
 * L1 y L2 de los procesadores, en las cache de interseccion y tambien en RCache.
 *
 */
class LRU
{
private:
  UTIL *util;

  map<string/*key*/,Nodo* /*ptr*/> cache;  ///< Representan la cache.
  int *NC;
  int libre; 

  priority_queue< Nodo*, vector<Nodo*>, Compara > cola_prioridad; ///< Cola de prioridad.
  int TS;

  //--parametros
  int *P;

  //--temporales
  Nodo *ptr;
  unsigned int prio;
  vector<string> v;
  vector<string>::iterator itv;
  
  bool impreso;

public:
  LRU( int *_nc,
       UTIL *_util,
       int *_p )
  {
    NC   = _nc;
    P    = _p;
    util = _util;
    TS   = 0;
    libre = *_nc;
    impreso = false;
  }

  LRU( )
  {

  }
  
  ~LRU( )
  {
    cache.clear( );
    //limpiar cola
  }

  
  void ini( int *_nc,
       UTIL *_util,
       int *_p )
  {
    NC   = _nc;
    P    = _p;
    util = _util;
    TS   = 0;
    libre = *_nc;
    impreso = false;
  }
  /**
   * Inserta elementos en la LRU.
   *
   * @param TSL es el timestamp de los elementos insertados.
   * @param key es la clave o elemento insertado.
   * @param tam es el tamanio del elemento insertado.
   */
  void insert_Load( int, string, int  );
  
  /**
   * Recorre la cola de prioridad e imprime sus
   *
   */
  void imprimir()
  {
    while( !cola_prioridad.empty() )
    {
       ptr = cola_prioridad.top();

       cola_prioridad.pop();
       if ( ptr->marca != false )  
       cout<<ptr->key<<endl;  
    }
  }

  /**
   * Retorna el valor de la variable "impreso".
   *
   * @return el valor de la variable "impreso".
   */
  bool get_impr() {return impreso;}

  /**
   * Asigna un nuevo valor a la variable "impreso".
   *
   * @param x es el valor a asignar.
   */
  void set_imp( bool x ){ impreso = x;}

  /**
   * .
   *
   * @return .
   */
  int getNC()
  {
    return *NC;
  }

  /**
   * Indica si la entrada existe en la Cache (Hit) o no.
   *
   * @param key es la entrada a consultar por Hit.
   */
  bool hit( string/*key*/ );

  /**
   * 
   *
   */
  bool warm_up( string/*key*/ );

  /**
   * Inserta una entrada en la Cache.
   *
   * @param key es la entrada a insertar.
   * @param tam es el tamanio de la entrada.
   */
  void insert( string/*key*/, int/*tam*/ );

  /**
   * Aumenta (o actualiza) el timestamp de la entrada en la Cache.
   *
   * @param key es la entrada a actualizar su timestamp.
   */
  void update( string/*key*/ );

  /**
   * Indica si en la Cache existe el espacio libre indicado.
   *
   * @param tam es el espacio.
   * @return true si queda espacio.
   */
  bool queda_espacio( int/*tam*/ );

  /**
   * Elimina entradas hasta generar espacio suficiente.
   *
   * @param tam es la cantidad de espacio requerida.
   */
  void generar_espacio( int/*tam*/ );

  /**
   * Muestra por pantalla el tamanio de la cache (el numero de entradas).
   */
  void getSize( )
  {
    cout << "cache.size()=" << cache.size( ) << endl;
  }
};


//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------
/**
 * Esta clase implementa una politica de reemplazo Landlord utilizada en la
 * parte dinamica de los servicios RCache.
 *
 */
class LRUB  //LandLord
{
private:
 // UTIL *util;

  //--cache misma
  map<string/*key*/,Nodo* /*ptr*/> cache;
  int NC;
  int libre;

  priority_queue< Nodo*, vector<Nodo*>, Compara2 > cola_prioridad;

  //--parametros
  int P;
   
  //--temporales
  Nodo *ptr;
  unsigned int prio;
  vector<string> v;
  vector<string>::iterator itv;
  double suma_costo;

public:
  LRUB( int _nc,
   //     UTIL *_util,
        int _p )
  {
    NC         = _nc;
    P          = _p;
    //util       = _util;
    suma_costo = 0.0;
    libre      = _nc;
  }

  ~LRUB( )
  {
    cache.clear( );
    //limpiar cola
  }

  /**
   * Verifica si la consulta esta en cache   
   *
   * @param key es la entrada a consultar.
   * @return true si hubo hit.
   */
  bool hit( string );

  /**
   * Inserta una consulta en cache
   *
   * @param key es la entrada (consulta) a insertar en la Cache.
   * @param TS el timestamp de la consulta.
   * @param tam es el tamanio de la nueva entrada.
   */
  void insert( string, double, int );

  /**
   * Verifica si hay espacio libre en cache
   *
   * @param tam es el tamanio minimo libre.
   * @return true si hay espacio libre. 
   */
  bool queda_espacio( int );

  /**
   * Genera espacio en la cache
   *
   * @param tam es el espacio a liberar.
   * @return true si fue posible generar la memoria requerida.
   */
  bool generar_espacio( int );

  /**
   * Obtiene la menor prioridad de los elementos en cache
   *
   * @return retorna la prioridad minima
   */
  double min( );

  /**
   * Actualiza la prioridad de una consulta en cache
   *
   * @param key es la entrada a actualizar si prioridad.
   */
  void update( string );

  /**
   * Obtiene la variable que mantiene el paso del tiempo en la cache
   *
   * @return valor de la variable que mantiene el paso del tiempo de la cache.
   */
  double getsuma( )
  {
    return suma_costo;
  }

  /**
   * Verifica si queda espacio libre minimo.
   *
   * @param tam el espacio minimo libre.
   * @return true si queda espacio libre.
   */
  bool space( int tam)
  {
    return ( queda_espacio( tam ) );
  }

  /**
   * Obtiene el tamanio de la cache y lo muestra por pantalla.
   *
   */
  void getSize( )
  {
    cout << "cache.size()=" << cache.size( ) << endl;
  }
  
  /**
   * Reduce el tamanio de la cache eliminando entradas si fuera necesario.
   *
   * @param es el nuevo tamanio de la cache.
   */
  void reduce_size( int );

  /**
   * Actualiza el valor de la variable que mantiene el paso del tiempo en la cache.
   *
   * @param x es el valor a aumentar.
   */
  void actualizar_suma( double x ){ suma_costo+= x; }
};

#endif
