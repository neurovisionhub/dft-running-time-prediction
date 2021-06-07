#ifndef LFU_H
#define LFU_H
/*#include <map>
#include <vector>
#include <set>
#include <string>
#include <iostream>
*/
#include "global.h"
#include "Util.h"
#include "Lru.h"
using namespace std;
/**
 * Clase utilizada por la parte estatica de la RCache.
 */
class LFU
{
private:
  UTIL *util; ///< Rutinas utilizadas por todas las clases

  //--cache misma
  set<string/*key*/> cache;

  int NC;   ///< Numero de entradas de la cache

  int P;    ///< Numero de procesadores (columna de la matriz)

public:
  LFU( int _nc,
       UTIL* _util,
       int _p
     //  char* _archivo_frecuentes
     )
  {
    NC   = _nc;
    P    = _p;
    util = _util;

    /**< al leer las mas frecuentes tambien debo llenar la cache dinamica */
  //  this->leer_frecuentes( _archivo_frecuentes, NC );
  }
    LFU(
     )
  {
  
  
    /**< al leer las mas frecuentes tambien debo llenar la cache dinamica */
  //  this->leer_frecuentes( _archivo_frecuentes, NC );
  }
  

  ~LFU( )
  {
    cache.clear( );
  }

  /**
   * Verifica si la consulta esta en la cache
   *
   * @param key es la consulta.
   * @return true si la consulta existe en la cache.
   */
  
  void ini( int _nc,
       UTIL* _util,
       int _p
     //  char* _archivo_frecuentes
     )
  {
    NC   = _nc;
    P    = _p;
    util = _util;

    /**< al leer las mas frecuentes tambien debo llenar la cache dinamica */
  //  this->leer_frecuentes( _archivo_frecuentes, NC );
  }
  
 void insertCache(string entrada)
 {
        cache.insert( entrada );
 } 
  
 bool hit(string);/* key)
  {

   if( cache.empty( ) )
    return false;
  if( cache.find( key ) == cache.end( ) )
    return false;
  else
    return true;


  }
*/
  /**
   * Lee las consultas con mayor prioridad desde el archivo para la parte estatica
   *
   * @param *_file es el archivo.
   * @param N es la cantidad de entradas a leer.
   */
/*  void leer_frecuentes( char *,int);
//char *_file,
                           int N )
{
  string linea, entrada;
  int i;

  //archivo de entrada
  ifstream fin( _file );
  ASSERT( fin.is_open( ) );

  //cargar "N" entradas
  i = 0;
  while( getline( fin, linea ) && ( i < N ) )
  {
    //la linea contiene los idts ordenados

    entrada.clear( );
//-------    util->preparar( linea, &entrada );

    //insertar
    cache.insert( entrada );
    i++;
  }
  cout << "LFU Rcache i=" << i << endl;
}

*/
  /**
   * Obtiene el tamanio de la cache y lo muestra por pantalla.
   *
   */
  void getSize( )
  {
    cout << "cache_lfu.size()=" << cache.size( ) << endl;
  }
  
   int getSizeCache()
  {
      int tam = cache.size( );
      return tam;
  }
};

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------

/**
 * @warning Esta clase no esta en uso dado que no hay LCache en esta version del simulador.
 *
 * Implementa la politica "LandLord" para representar una memoria LCache.
 *
 */
class LFU2 //LandLord-Lcache
{
private:
  UTIL *util;
  LFU *lfu;

  //--cache misma
  set<string/*key*/> cache;  ///< cache misma.

  int NC;         ///< Numero de entradas en la cache

  int P;    ///< Numero de procesadores (columnas de la matriz)

public:
  LFU2( int _nc,
  //     UTIL* _util,
        int _p,
    //    char* _archivo_frecuentes,
        LFU *_lfu
      )
  {
    NC   = _nc;
    P    = _p;
   // util = _util;
    lfu  = _lfu;

    /**< al leer las mas frecuentes tambien debo
         llenar la cache dinamica */
   // this->leer_frecuentes( _archivo_frecuentes, NC );
  }

  ~LFU2( )
  {
    cache.clear( );
  }

  /**
   * .
   */
  void set_LFU( LFU* _lfu )
  {
    if( _lfu != NULL )
    lfu = _lfu;
  }

 void insertCache(string entrada)
 {
        cache.insert( entrada );
 } 
  
  
  /**
   * Verifica que la consulta exista en LCache.
   *
   * @param key es la consulta.
   * @return true si la consulta existe.
   */
  bool hit(string);/* key )
{
  if( cache.empty( ) )
    return false;
  if( cache.find( key ) == cache.end( ) )
    return false;
  else
    return true;
}
*/
  /**
   * Lee las consultas frecuentes desde un archivo.
   *
   * @param *_file es el archivo desde donde leer los datos.
   * @param N es el numero de entradas a leer.
   */
/*  void leer_frecuentes(  char *_file,
                            int N )
{
  string linea, entrada;
  int i;

  //archivo de entrada
  ifstream fin( _file );
  ASSERT( fin.is_open( ) );

  //cargar "N" entradas
  i = 0;
  while( getline( fin, linea ) && i < N )
  {
    //la linea contiene los idts ordenados

    entrada.clear( );
   // util->preparar( linea, &entrada );

    if( lfu->hit( entrada ) )
      continue; //verifica que no esta en la Rcache LFU

    //insertar 
    cache.insert( entrada );
    i++;
  }
cout << "LFU Rcache i=" << i << endl;
}
*/

  /**
   * Muestra por pantalla el tamanio de la LCache.
   *
   */
  void getSize( )
  {
    cout << "cache_lfu.size()=" << cache.size( ) << endl;
  }
  
  int getSizeCache()
  {
      int tam = cache.size( );
      return tam;
  }
};
#endif
