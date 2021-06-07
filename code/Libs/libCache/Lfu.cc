#include "Lfu.h"

//--------------------------------------------
//-------------LFU----------------------------
bool LFU::hit( string key )
{
  if( cache.empty( ) )
    return false;
  if( cache.find( key ) == cache.end( ) )
    return false;
  else
    return true;
}

//----------------------------------------------
/*
void LFU::leer_frecuentes( char *_file,
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
    util->preparar( linea, &entrada );

    //insertar
    cache.insert( entrada );
    i++;
  }
  cout << "LFU Rcache i=" << i << endl;
}
*/
//--------------------------------------------
//--------------------------------------------
//-------------LFU 2--------------------------

bool LFU2::hit( string key )
{
  if( cache.empty( ) )
    return false;
  if( cache.find( key ) == cache.end( ) )
    return false;
  else
    return true;
}

//--------------------------------------------
/*
void LFU2::leer_frecuentes( char *_file,
                            int N )
{
  string linea, entrada;
  int i;

  //archivo de entrada
  ifstream fin( _file );
//  ASSERT( fin.is_open( ) );
  
  //cargar "N" entradas
  i = 0;
  while( getline( fin, linea ) && i < N )
  {
    //la linea contiene los idts ordenados

    entrada.clear( );
    util->preparar( linea, &entrada );

    if( lfu->hit( entrada ) )
      continue; //verifica que no esta en la Rcache LFU

    //insertar 
    cache.insert( entrada );
    i++;
  }
  cout << "LFU Rcache i=" << i << endl;
}
*/
