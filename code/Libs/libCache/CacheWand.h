#if !defined(_CacheW_H)
#define _CacheW_H
#include <stdio.h>
#include <string>
using namespace std;
class CacheW
{
private:
   int N; // tamanho actual del CBT

  // int NC;

  // int P; 
   
 //  set<string> cacheLfu;

public:

//# procesadores
int procs;

//#queries
int nQueries;

//size Caches
int sizeCacheL1;

int sizeCacheL2;

int sizeCacheLFU;

int sizeCacheLF;




map<unsigned int, unsigned> map_mis_idx; 
map<unsigned int, unsigned int> terminos;

//map<string,unsigned int> querieThershold;

map<string,int> querieAlpha;

//cache querie
map<string,unsigned int> querieCache;
//cache alpha
map<string,vector<int> > cacheAlpha;
//cache dependencias
map<string,vector<int> > cacheDeps;
//cache thershold
map<string,unsigned int> querieThreshold;


map<string,int> cacheActive;

map<unsigned int,int> cacheFlag;

//cache comun
//map<TIPO,vector<int> > cacheAlphaL2: //size >
//map<TIPO,vector<int> > cacheAlphaL1: //size <

  /*  unsigned int *Prioridades;
    unsigned int    *ID_docs;
    int    *cbt; //posiciones
 */
  int *idNodos;
 
    int K; // tamanho final del CBT
  
CacheW()
{

}

 void  Procs(int p)
  {
	procs=p;
    
  }

  ~CacheW( )
  {
/*  delete[]cbt;
  delete[]Prioridades;
  delete[]ID_docs;
*/ 
 // cacheLfu.clear();
 }
/*
void iniCacheLfu(int _nc,int _p)
{
NC=_nc;
P=_p;
}

bool hitLfu(string);

void getSizeLfu( )
  {
    cout << "cache_lfu.size()=" << cache.size( ) << endl;
  }
*/
void buildCacheAlpha()
{

}

bool searchAlphaQ(string key)
{
	if(cacheAlpha[key].size()>0)
	return true;
	else
	return false;
}

/*bool searchCacheActive(string key)
{


}*/

bool searchDepQ(string key)
{
   if(cacheDeps[key].size()>0)
   return true;
   else
   return false;
}
void insertAlphaQ(string key,vector<int> var)
{
	cacheAlpha[key] = var;
}

void insertDepNodos(string key,vector<int> var)
{
   cacheDeps[key] = var;
}

vector<int> depNodos(string key)
{
return cacheDeps[key];
}

void insertThresholdQ(string key,unsigned int threshold/*,unsigned int id*/)
{
	querieThreshold[key]=threshold;
   cacheActive[key]=1; // 1 esta en cache
//   cacheFlag[id]=1;
}

//posible uso de incrementos...
void activarCache(unsigned int id)
{
cacheFlag[id]=2; // activo uso en paso de mensajes
}

void desactivarCacheFlag(unsigned int id)
{
cacheFlag.erase(id);

}

int sizeCacheDeps()
{
return cacheDeps.size();
}

int sizeCacheFlag()
{
return cacheFlag.size();
}


int sizeCacheActive()
{
return cacheActive.size();
}


int searchCacheFlag(unsigned int id)
{

 map<unsigned int,int>::iterator iter = cacheFlag.find(id);
  if( iter != cacheFlag.end() )
  return cacheFlag[id];
  else
  return 0;


return cacheFlag[id];
}

// Obtiene dato de cache querie, 0 si no existe, 1 si fue detectada, 2 si esta activa.
int searchCache(string key)
{
   if(searchCacheQuerie(key))
   return cacheActive[key];
   else
   return 0;
}

bool searchCacheQuerie(string key)
{
  map<string,int>::iterator iter = cacheActive.find(key);
  if( iter != cacheActive.end() )
  return true;
  else
  return false;
}



unsigned int retornaThreshold(string key)
{
	return querieThreshold[key];
}

void mostrarAlphaQs()
{
int cont=0;
   for (std::map<string,vector<int> >:: iterator it= cacheAlpha.begin(); it!= cacheAlpha.end(); it++ )
   {
      printf("\n Q: ");
   cout<<it->first<<":";
   int t = it->second.size();
   
   for(int i=0;i<t;i++)
	{
	   int idpAlpha = it->second[i];
      //unsigned int frec = it->second;
      printf(" [%d,%d]",i,idpAlpha);
	}
//      unsigned int idp = it->first;
  //    unsigned int frec = it->second;
    //  printf("T[%lu]:[%lu]",idp,frec);
      cont++;
   }
cout<<"Numero de consultas: "<<cont<<endl;
}

void mostrarDepQs()
{
int cont=0;
   for (std::map<string,vector<int> >:: iterator it= cacheDeps.begin(); it!= cacheDeps.end(); it++ )
   {
   printf("\n Q Deps: ");
   cout<<it->first<<": "<< querieThreshold[it->first] <<" :";
   int t = it->second.size();

   for(int i=0;i<t;i++)
   {
      int idp = it->second[i];
      //unsigned int frec = it->second;
      printf(" [%d,%d]",i,idp);
   }
//      unsigned int idp = it->first;
  //    unsigned int frec = it->second;
    //  printf("T[%lu]:[%lu]",idp,frec);
      cont++;
   }
cout<<"Numero de consultas deps: "<<cont<<endl;
}




void insertTermino(unsigned int entrada)
{

 map<unsigned int,unsigned int>::iterator it;
 it=terminos.find(entrada);
 int a=0;
 if(it->second>0)
{
unsigned int tmp = it->second;
tmp++;
terminos[entrada]=tmp;
}
else
terminos[entrada]=1;

}

void insertQuerie(string Q)
{
map<string,unsigned int>::iterator it;
it=querieCache.find(Q);

if(it->second>0)
{unsigned int tmp = it->second;
tmp++;
querieCache[Q]=tmp;
}
else
querieCache[Q]=1;
}


void mostrarCacheConsultas()
{
int cont=0;
   for (std::map<string,unsigned int>:: iterator it= querieCache.begin(); it!= querieCache.end(); it++ )
   {
      printf("\n");
     // ostringstream convert = it->first;
     // string idq = convert.str();      
      string idq = it->first;
      unsigned int frec = it->second;
      //printf("T[%s]:[%lu]",idq,frec);
      cout<< idq <<" : "<<frec;
      cont++;
   }

cout<<"Consultas en cache: "<<cont<<endl;
}

int numTerms()
{
return terminos.size();
}

int numQueriesDist()
{
return querieCache.size();
}

void mostrarCacheTerminos()
{
int cont=0;
	for (std::map<unsigned int,unsigned int>:: iterator it= terminos.begin(); it!= terminos.end(); it++ )
	{
      printf("\n");
      unsigned int idp = it->first;
      unsigned int frec = it->second;
		printf("T[%lu]:[%lu]",idp,frec);
      cont++;
	}
cout<<"Numero de terminos: "<<cont<<endl;

}



unsigned int id()
{

return 10;
}

/*
  unsigned int top()
  {
     return Prioridades[cbt[1]]; //saca el minimo
  }



  void insert( unsigned int prio, unsigned int id_doc)
  {
     int n; //condicion N=1 
     if (N<K)
     {  
         N++; n= N;
     } //Se aumenta el contador de elementos del heap y se iguala a n ese contador
     else
     {
         n = cbt[1]; //posicion del minimo en el arbol
     } //obs n pasa a ser un valor muy grande, lo cual no es posicion menor o igual q  k

     Prioridades[n] = prio; //puntajes
     ID_docs[n]     = id_doc; //id docs
     updateCBT(K - 1 + n );

  }

  void mostrarHeap()
  {



  }
  void mostrarID_Prioridades()
  {
       printf("\n Prioridades \n");
  for(int n=1; n<=N; n++)
     {
         printf(" Prioridades[%d]:%d \n",n,Prioridades[n]);
     }


  }
    void mostrarID_docs()
  {
       printf("\n ID_docs \n");
  for(int n=0; n<K+1; n++)
     {
         printf(" ID_docs[%d]:%d ",n,ID_docs[n]);
     }


  }
 */

};
#endif

