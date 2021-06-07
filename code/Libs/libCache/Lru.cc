//#include "Util.h"
#include "Lru.h"
#include "global.h"
#include <queue>
bool LRU::hit( string key )
{
  if( cache.empty( ) )
    return false;

  if( cache.count( key ) == 0 )
    return false;
  else
    return true;
}

//---------------------------------------------------

bool LRU::warm_up( string key )
{
  //si queda mas espacio
  if( (int)cache.size( ) < (*NC) )
    return false;
  else
    return true;
}

//---------------------------------------------------
void LRU::insert_Load( int TSL, string key, int tam )
{
  if ( cache.count( key )!= 0 )
   {
      update( key );
      return;
   }
  if( !this->queda_espacio( tam ) )
  {
    this->generar_espacio( tam );
  }

  //poner en cache nuevo nodo
  ptr = new Nodo( TSL, key, tam );
  cola_prioridad.push( ptr );

  libre -= tam;

  //enlazar
  cache[key] = ptr;

  //aumentar timestamp
  if ( TS < TSL ) TS = TSL;
}

//---------------------------------------------------
void LRU::insert( string key, int tam )
{
  if ( cache.count( key )!= 0 )
   {
      update( key );   
      return;  
   } 
  //verificar que no este
  if( cache.count( key ) == 0 )
  {
  if( !this->queda_espacio( tam ) )
  {
    this->generar_espacio( tam );
  }

  //poner en cache nuevo nodo
  ptr = new Nodo( TS, key, tam );
  cola_prioridad.push( ptr );
  
  libre -= tam;

  //enlazar
  cache[key] = ptr;

  //aumentar timestamp
  TS++;
  }
}

//---------------------------------------------------

void LRU::update( string key )
{
  //marcar la entrada
  cache[key]->marca = false;
  int tam = cache[key]->tam;

  //agregar una nueva
  ptr = new Nodo( TS , key, tam );
  cola_prioridad.push( ptr );

  //datos
  cache[key] = ptr;

  //aumentar timestamp
  TS++;
}

//---------------------------------------------------

bool LRU::queda_espacio( int tam )
{
  if( libre >= tam )
    return true;
  return false;
}

//---------------------------------------------------

void LRU::generar_espacio( int tam )
{
  do
  {
    ptr = cola_prioridad.top( );
    while( ptr->marca == false )
    {
      cola_prioridad.pop( );
      delete ptr;
      ptr = cola_prioridad.top( );
    }
    cola_prioridad.pop( );
 
    libre += ptr->tam;
  
    cache.erase( ptr->key );
 
    delete ptr;
  }while ( libre < tam );
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

void LRUB::reduce_size( int newNC )
{

   if ( this->libre < ((NC) - newNC) )
   {
      this->generar_espacio( (NC - newNC) - libre );     
   } 

   NC = newNC;
   this->libre -= ( NC - newNC );   
}

bool LRUB::hit( string key )
{
  if( cache.empty( ) )
    return false;
  if( cache.count( key ) == 0 )
    return false;
  else
    return true;
}

//---------------------------------------------------

void LRUB::update( string key )
{
  //marcar la entrada
  cache[key]->marca = false;
  int tam = cache[key]->tam;
 
  double prioridad  = cache[key]->prioridad;
  
  ptr=new Nodo( prioridad + suma_costo , key, tam );    
  cola_prioridad.push( ptr );

  //datos
  cache[key] = ptr;
}

//---------------------------------------------------

void LRUB::insert( string key, double TS, int tam )
{
  bool space = true;
  if( !this->queda_espacio( tam ) )
  {
    space = this->generar_espacio( tam );
  }

  //verificar que no este
  if( cache.count( key ) == 0 )
{
  if ( space )
  {
    //poner en cache nuevo nodo
    ptr = new Nodo( TS, key, tam );
    cola_prioridad.push( ptr );

    libre -= tam;

    //datos
    cache[key] = ptr;
  }

}
}

//---------------------------------------------------

bool LRUB::queda_espacio( int tam )
{
  if( libre >= tam )
    return true;
  return false;
}

//---------------------------------------------------

bool LRUB::generar_espacio( int tam )
{
  // ERROR: si esta vacia la cola
//  ASSERT( !cola_prioridad.empty( ) );

  do
  {
    if (cola_prioridad.empty())
    {
       cout<<"NO HAY SUFICIENTE MEMORIA: PEDIDOS "<<tam<< "Libres " <<libre<<endl;
       fflush(stdout);
       return false;
    }
     ptr = cola_prioridad.top( );
     while( ptr->marca == false )
     {
       cola_prioridad.pop( );
       delete ptr;
       ptr = cola_prioridad.top( );
     }
     cola_prioridad.pop( );

     cache.erase( ptr->key );

     suma_costo += ptr->prioridad;

     libre += ptr->tam;

     delete ptr;
   }while( libre < tam );

   return true;
}

//---------------------------------------------------

double LRUB::min( )
{
  if( cola_prioridad.size( ) > 0 )
  {
    ptr = cola_prioridad.top( );
    while( ptr->marca == false )
    {
      cola_prioridad.pop( );
      delete ptr;
      ptr = cola_prioridad.top( );
    }
    if( cola_prioridad.size( ) > 0 )
      return ( ptr->prioridad );
    else
      return 0.0;
  }
  else
    return 0.0;
}

