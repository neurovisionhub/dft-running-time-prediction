#include <sched.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "../Libs/libWand/bloques2013/WandIndexBlockMax.h"
#include "../Libs/libWand/estruc.h"
#include "math.h"
#include "../Libs/libWand/HeapM.h"
#include "./localHeapVar.h"
#include <string>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <algorithm>
#include <tr1/memory>
// Biblioteca STL
#include <map>
#include <vector>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <string>
#include <istream>
#include <ostream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <algorithm>

#include <time.h>
#include <sys/timeb.h>
#include <errno.h>

       #define handle_error_en(en, msg) \
               do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

       static void
       usage(char *prog_name, char *msg)
       {
           if (msg != NULL)
               fputs(msg, stderr);

           fprintf(stderr, "Usage: %s [options]\n", prog_name);
           fprintf(stderr, "Options are:\n");
       #define fpe(msg) fprintf(stderr, "\t%s", msg);          /* Shorter */
           fpe("-a<policy><prio> Set scheduling policy and priority in\n");
           fpe("                 thread attributes object\n");
           fpe("                 <policy> can be\n");
           fpe("                     f  SCHED_FIFO\n");
           fpe("                     r  SCHED_RR\n");
           fpe("                     o  SCHED_OTHER\n");
           fpe("-A               Use default thread attributes object\n");
           fpe("-i {e|s}         Set inherit scheduler attribute to\n");
           fpe("                 'explicit' or 'inherit'\n");
           fpe("-m<policy><prio> Set scheduling policy and priority on\n");
           fpe("                 main thread before pthread_create() call\n");
           exit(EXIT_FAILURE);
       }

       static int
       get_policy(char p, int *policy)
       {
           switch (p) {
           case 'f': *policy = SCHED_FIFO;     return 1;
           case 'r': *policy = SCHED_RR;       return 1;
           case 'o': *policy = SCHED_OTHER;    return 1;
           default:  return 0;
           }
       }

       static void
       display_sched_attr(int policy, struct sched_param *param)
       {
           printf("    policy=%s, priority=%d\n",
                   (policy == SCHED_FIFO)  ? "SCHED_FIFO" :
                   (policy == SCHED_RR)    ? "SCHED_RR" :
                   (policy == SCHED_OTHER) ? "SCHED_OTHER" :
                   "???",
                   param->sched_priority);
       }

       static void
       display_thread_sched_attr(char *msg)
       {
           int policy, s;
           struct sched_param param;

           s = pthread_getschedparam(pthread_self(), &policy, &param);
           if (s != 0)
               handle_error_en(s, "pthread_getschedparam");

           printf("%s\n", msg);
           display_sched_attr(policy, &param);
       }

       static void *
       thread_start(void *arg)
       {
           display_thread_sched_attr("Scheduler attributes of new thread");

           return NULL;
       }

void AsignaCPUThread(int cpuId);

int DOCS;
unsigned long int DOCS_PROCESADOS;
unsigned long int HEAP_UPDATES;
unsigned long int HEAP_SIZE;

 struct timespec latencia;
 //Tiempo llamada thread consulta global
struct timespec tiempo1_GlobalQ, tiempo2_GlobalQ; 
//Tiempo llamada thread consulta
struct timespec tiempo1_ThreadQ, tiempo2_ThreadQ; 
//Tiempo llamada thread consulta
struct timespec tiempo1_ThreadCierreQ, tiempo2_ThreadCierreQ; 
//Tiempo procesamiento consulta
struct timespec tiempo1_ProcQ, tiempo2_ProcQ; 
//Tiempo preprocesamiento consulta
struct timespec tiempo1_PreQ, tiempo2_PreQ;
//Tiempo completo Wand
struct timespec tiempo1_Wand, tiempo2_Wand;
//Tiempo preprocesamiento Wand
struct timespec tiempo1_PreWand, tiempo2_PreWand;
//Tiempo Iterador Wand
struct timespec tiempo1_IteWand, tiempo2_IteWand;
//Tiempo Merge-sort
struct timespec tiempo1_MergeSort, tiempo2_MergeSort;
//Tiempo Carga Lista
struct timespec tiempo1_Lista, tiempo2_Lista;
//Tiempo lote
struct timespec tiempo1_Lote, tiempo2_Lote;
struct timespec tiempo1_Padre, tiempo2_Padre;
const int NANO_SECONDS_IN_SEC = 1000000000;
 ofstream logT;
  ofstream thersholdLogConsulta;
  ofstream tiempoGlobal;
  ofstream thersholdLogUB;
  ofstream tiemposGlobales;
  ofstream porcentajesFile;
  long long int tiempoBloqueos;
  int contadorPaso,contadorPaso2,contadorInicio,contadorPasoWand;
    int contadorLocal,contadorFinWand,bandera,contadorPasoBarrera;
int K;
int SizeBloque;
int nPaticionIn;
//int contadorLocal;
//CHeapTree<unsigned int, unsigned int> *heapGlobal;// = new CHeapTree<unsigned int, unsigned int>(K, true);
long unsigned int sumaEC=0;
long unsigned int sumaAH=0;
long unsigned int sumaBloquesIN=0;
long unsigned int sumaBloquesOpen=0;
long int *EC,*AH,*bloquesComp,*bloquesDesc;
CBT *heapMGlobal;// = new CBT(topK);

vector < pair<unsigned int, unsigned int> >  HEAP; 
int nSeleccionadosG;
int contadorBandera;
unsigned int thresholdGlobal;
map < int, vector < pair<unsigned int, unsigned int> > > HEAP_LOCALES;
vector < unsigned int> HEAP_UBS;
unsigned int docUB;
int sumaHeapGlobal;
unsigned int minimoGlobal;
using namespace std;
int myglobal;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;;
pthread_barrier_t barrier;
pthread_barrierattr_t attr;
pthread_cond_t llegaron = PTHREAD_COND_INITIALIZER;
int cont_t;
int activos;
int *bufferEstados; // 0 : inactivo, 1:activo, 2:en espera
int contados;
//unsigned int *minimosLocales;
//vector<unsigned int> minimosLocalesVector;
double TIEMPO_TOTAL;
map<int,double> tiempos;
map<int, map<unsigned long long, PostingStream* > > indiceThreadGlobal; //Indices de  posting

vector < long unsigned int> nPrecalculados;
vector < long unsigned int> nActualizacionesVector;
vector < unsigned int> umbrales;
int opcionUB;//0 : sin, 1,
int opcionTiempo; //0: detalle completo, 1: tiempos maximos

unsigned int consultaInicial; //id partida consulta
unsigned int numeroConsultasProceso;//numero de consultas a procesar
unsigned int dimensionBloquesEscritura;

char *nombreArchivoTopk;
char *nombreArchivoConsultas;
char *nombreArchivoLista;
char *nombreArchivoSalida;
char *nombreThreshold;
FILE * pFileTopk;

char *PATH_LOGS;
map<int , vector<double> > logDataThresholdUB; //<idQ, vector <ub_t1, ub_t2,...,ub_tn > >
map<int , unsigned int > logDataThreshold;  // <idQ,threshold>

vector <int> UmbralesEstimados;
//map<int, vector<int> > docsTopkQ;
//map<int, vector<int> > puntajesDocsTopKQ;
map<int , vector< pair<unsigned int, unsigned int> > > puntajesDocsTopKQ;

struct timespec *TimeSpecDiff(struct timespec *ts1, struct timespec *ts2)
{

  static struct timespec ts;

  ts.tv_sec = ts1->tv_sec - ts2->tv_sec;

  ts.tv_nsec = ts1->tv_nsec - ts2->tv_nsec;

  if (ts.tv_nsec < 0) {

    ts.tv_sec--;

    ts.tv_nsec += NANO_SECONDS_IN_SEC;

  }

  return &ts;

}


void diff_time( timespec *t_fin, timespec *t_ini, timespec *delta )
{
  if( ( (*t_fin).tv_nsec - (*t_ini).tv_nsec ) < 0 )
  {
    if( (*t_fin).tv_sec == (*t_ini).tv_sec )
    {
      (*delta).tv_sec  = 0;
      (*delta).tv_nsec = 1000000000 + (*t_fin).tv_nsec - (*t_ini).tv_nsec;
    }
    else
    {
      (*delta).tv_sec  = (*t_fin).tv_sec - (*t_ini).tv_sec - 1;
      (*delta).tv_nsec = 1000000000 + (*t_fin).tv_nsec - (*t_ini).tv_nsec;
    }
  }
  else
  {
    if( (*t_fin).tv_sec == (*t_ini).tv_sec )
    {
      (*delta).tv_sec  = 0;
      (*delta).tv_nsec = (*t_fin).tv_nsec - (*t_ini).tv_nsec;
    }
    else
    {
      (*delta).tv_sec  = (*t_fin).tv_sec - (*t_ini).tv_sec;
      (*delta).tv_nsec = (*t_fin).tv_nsec - (*t_ini).tv_nsec;
    }
  }
}
//Fin Funcion Tiempo

class ListaC
{
  private:


   public:
   int tid;
   int nthreads;
  // map<unsigned long long, PostingStream* > indiceThreadGH;
  // map<unsigned long long, PostingStream* > indiceThreadGL;
  // map<unsigned long long, PostingStream* > indiceThread;
   unsigned int idQ;
   vector < DatosQuery* > consultas;
  // int prueba;
  // int umbralInicial;
 //  map<unsigned long long,unsigned int> ubLocales;
 //  map<unsigned long long,unsigned int> ubLocalesGH;
 //  map<unsigned long long,unsigned int> ubLocalesGL;
 //  int tipoDistribucion; //0 solo GH tiene datos, 1 solo GL tiene datos, 2 en ambas hay datos
   map<unsigned long long, PostingStream* > indiceThreadIntervalos;//lista completa
 //  map<unsigned long long, vector<unsigned int> > ubLocalesIntervalos;//ub bloques
 //  map<unsigned long long, int> numeroBloques;//datos locales
 //  map<unsigned long long, vector<unsigned int> > idDocsFronteras;//fronteras


   ListaC()
   {

   }
   ListaC(int p,int pr)
   {
     tid  = p;
     nthreads = pr;
   }
   
   void asignaQ(unsigned int id)
   {
	 idQ = id;  
   }
   unsigned int ub_local(PostingStream *entrada)
   {
       return 1;
   }

};

class datosQuerie
{
  private:


   public:
   int tid;
   int nthreads;
   unsigned int idQ;
   vector < DatosQuery* > consultas;
   DatosQuery*  consulta;
   int k;

   datosQuerie()
   {

   }
   datosQuerie(int p,int pr, unsigned int id,int k)
   {
     tid  = p;
     nthreads = pr;
     idQ = id;  
   }
   
 
};
datosQuerie dataQ[32];//lista de terminos

/**VARIABLES TRAZA ABRIL 2011**/
ListaC listaStruct[32];//lista de terminos

	  vector<DatosQuery*> querys;//consultas
	  vector<int> T1;
	  vector<int> T2;
	  vector<int> T3;
	  vector<int> T4;
	  vector<long unsigned int> T5;
	  vector<long unsigned int> T6;

void mostrarQueryes()
{


}

void mostrarVectorRespuesta(vector< pair<unsigned int, unsigned int> > respuesta, int dim)
{
    int cont=respuesta.size();
  //  int cont2=1;
    cout<<"    DIMENSION: "<<  cont  <<"   "<<endl;
    if(dim>cont)
    dim=cont;
    for(int i=0;i<dim;i++)
    {
cout << " "<< respuesta[cont - 1 - i].first <<" " << respuesta[cont - 1 - i].second;
  //  cout << "Respuesta[" << cont2 << "] = "<< respuesta[dim-1-i].first <<" - " << respuesta[dim-1-i].second << endl;
 //logT<< " "<< respuesta[cont - 1 - i].second <<" " << respuesta[cont - 1 - i].first;
    }
    
   // logT<<endl;

}


unsigned int retornaUmbral(vector< pair<unsigned int, unsigned int> > respuesta, int dim)
{
int cont=respuesta.size();

//unsigned int T = 0;
if(dim>cont)
{
    if(cont==0)
    return 0;
    
    dim=cont; 
    return respuesta[0].first;
}

int posT = cont - dim;
docUB = respuesta[posT].second;
return respuesta[posT].first;
}


void EscribirVectorRespuesta(vector< pair<unsigned int, unsigned int> > respuesta, int dim,int idQ)
{
    int cont=respuesta.size();
	if(dim>cont)
    dim=cont;
    for(int i=0;i<dim;i++)
    {

	if(i!=0)
	cout <<" "<<respuesta[cont - 1 - i].first;
	else
	cout << respuesta[cont - 1 - i].first;

	}
}

void leerUmbralesML(const char *archivo,int opcionT,int nQ)
{
	string line;
	int idDato;//,i;//,posicionConsulta;
	//char tmp;
	//char buff[256];
	int numero;
	//i=0;
        int idUmbral=0;

if(opcionT == 0)
{
for(int i=0;i<nQ;i++)
{
UmbralesEstimados.push_back(0);

}

}
else
{

 ifstream Umbrales(archivo, ifstream::in);

 while(getline(Umbrales,line))
   {
    numero= atoi(line.c_str());
	UmbralesEstimados.push_back(numero);
	idDato++;
	idUmbral++;
   }

Umbrales.close();
}
}


void mostrarVector(vector<unsigned int> entrada)
{

std::size_t N = entrada.size();
cout <<endl;
for(std::size_t i=0;i<N;++i)
cout << entrada[i] << endl;


}

void leerIndiceCharGrupos(const char *archivo/*,int nThreads,ListaC *listaStruct*/,int tidThread)
{
 //printf("Cargando archivo..\n");
 ifstream listaInvertida(archivo,ifstream::in);
  //  list< pair<unsigned int, unsigned int> > *lista[nThreads];
    list< pair<unsigned int, unsigned int> > *lista[1];
   // list< pair<unsigned int, unsigned int> > *lista[n;
    string line;
	char *h;
	unsigned int posicion;
    int idDato,conteoDatos,posicionLista=0;
    char tmp;
	char buff[256];
	int i=0,numero,bandera=0;
    unsigned long long idTerm=0;
    int idDoc=0,UB=0,pesoDoc=0;//,numDocs=0;
    unsigned int conteoLista=0;
    unsigned int dimLista;
    //bLocal[i]=0;
           int nLinea=0;

   // int UB_High;//, UB_Low;
    int contadorBloque;
    vector <unsigned int> pesosDocumentos;
    vector <unsigned int> fronterasIdIniBlock;
    vector <unsigned int> fronterasIdFinBlock;
    vector <unsigned int> ubParticiones;
    //vector <unsigned int> :: const_iterator UbTmp;
    unsigned int UbTmp;
    unsigned int docFin;
while(getline(listaInvertida,line))
{
nLinea++;
//cout<<" Lista: " <<nLinea<<endl;
   h = strdup(line.c_str());
   posicion=0;
   idDato=0;conteoDatos=0;posicionLista=0;
   lista[0]=new list< pair<unsigned int, unsigned int> >();
   contadorBloque=0;

   dimLista=strlen(h);
   while(posicion<dimLista)
		{
		        i=0;

				while(bandera==0)
				{
				   tmp=h[posicion];
				   if(isdigit(tmp))
				   {
					buff[i]=tmp;
					posicion++;
		            i++;
				   }
				   else
				   {
				   buff[i]='\0';
				   bandera=1;
		           posicion++;
				   }

				}
			bandera=0;




		    if(i!=0)
		    {
            numero= atoi(buff);
		        if(idDato==0)
				{
				idTerm=numero;
		        }
		        if(idDato==1)
		        {
		        UB=numero;

		        }

		   //     if(idDato==2)
		     //   {
		     //   numDocs=numero;

		    //    }
            //datos 1 y 2 no se consideran ya que se van a calcular nuevamente UB y #docs, solo en version multithreading
		    if(idDato>2)
		    {
					if(conteoDatos%2!=0)//frec
					{
						pesoDoc=numero;
						
						lista[0]->push_back(pair<unsigned int, unsigned int>(idDoc, pesoDoc)); //original



											if(posicionLista%SizeBloque==0&&posicionLista>0)
											{
                                                                                        fronterasIdFinBlock.push_back(docFin);//idDoc primer doc bloque
											                                                                                                        
											fronterasIdIniBlock.push_back(idDoc);//idDoc primer doc bloque
											UbTmp = *( std:: max_element(pesosDocumentos.begin(),pesosDocumentos.end()));
											contadorBloque++;

											pesosDocumentos.erase(pesosDocumentos.begin(),pesosDocumentos.end());
											pesosDocumentos.resize(0);
											ubParticiones.push_back(UbTmp);

											}
                                                                                        else if(posicionLista>0)
                                                                                        {
                                                                                        
                                                                                        docFin = idDoc;
                                                                                        
                                                                                        }
                                                
                                                pesosDocumentos.push_back(pesoDoc);
						posicionLista++;
					}
					else//doc
					{idDoc=numero;
				//	pesos[posicionConsulta]=valor;
                                       if(conteoDatos==0)//primer idDoc
                                        {
                                        fronterasIdIniBlock.push_back(idDoc);
                                        }


					}

		   // printf("[%d][%d]Numero:%d  \n",id_query,modulo,numero);
		    conteoDatos++;
		    }

			idDato++;
			}//fin if i!=0

	}
    conteoLista++;
   	free(h);
	h = NULL;
//bloque ultimo
if(posicionLista-contadorBloque*SizeBloque!=0)
{

UbTmp = *( std:: max_element(pesosDocumentos.begin(),pesosDocumentos.end()));
contadorBloque++;

                                        pesosDocumentos.erase(pesosDocumentos.begin(),pesosDocumentos.end());
                                        pesosDocumentos.resize(0);
					ubParticiones.push_back(UbTmp);
}

fronterasIdFinBlock.push_back(idDoc);
//fronterasIdIniBlock.push_back(idDoc);//Ultimo idDoc lista

//exit(1);

if(lista[0]->size()>0)
{
    
 pthread_mutex_lock( &lock );    
//listaStruct[0].ubLocales[idTerm] = UB;

//en la clase postingstream colocar punteros...
listaStruct[tidThread].indiceThreadIntervalos[idTerm] = new PostingStream(lista[0],ubParticiones,fronterasIdIniBlock,fronterasIdFinBlock,UB);
pthread_mutex_unlock( &lock );


}

//mostrarVector(ubParticiones);
//mostrarVector(fronterasIdFronteras);
  fronterasIdFinBlock.erase(fronterasIdFinBlock.begin(),fronterasIdFinBlock.end());
  fronterasIdFinBlock.resize(0);
  ubParticiones.erase(ubParticiones.begin(),ubParticiones.end());
  ubParticiones.resize(0);
  fronterasIdIniBlock.erase(fronterasIdIniBlock.begin(),fronterasIdIniBlock.end());
  fronterasIdIniBlock.resize(0);
  
  
  if(nLinea%100==0)
cout<<" Lista: " <<nLinea<<" size: "<<lista[0]->size()<<endl;
lista[0]->clear();

          }

		listaInvertida.close();
    //            exit(0);

}


void leerConsultasChar(const char *archivo,vector < DatosQuery* > *querys,int nQ)
{
	string line;
	char *h;
	unsigned int posicion;
	ifstream consultas(archivo, ifstream::in);
	int bandera=0;
	int idConsulta=0;
	int idDato,i,posicionConsulta;
	char tmp;
	char buff[256];
	int numero;
		unsigned int id_query=0;
		int  n_terms=0;
		unsigned long long id_term=0;
		unsigned int valor=0;
		unsigned long long *terms=NULL;
		unsigned int *pesos=NULL;
   int modulo,conteoDatos;
   unsigned int dimString;
   while(getline(consultas,line))
   {i=0;
   h = strdup(line.c_str());
   posicion=0;
   dimString=strlen(h);
   idDato=0;conteoDatos=0;posicionConsulta=0;
   while(posicion<dimString)
		{
		        i=0;

				while(bandera==0)
				{
				   tmp=h[posicion];
				   if(isdigit(tmp))
				   {
					buff[i]=tmp;
					posicion++;
				            i++;
				   }
				   else
				   {
				   buff[i]='\0';
				   bandera=1;
		                   posicion++;
				   }

				}
		         	bandera=0;




		    if(i!=0)
		    {
                         numero= atoi(buff);
		        if(idDato==0)
				{
				n_terms=numero;
				id_query = idConsulta;
		      //  printf("[%u]Numero terminos:%d \n",id_query,n_terms);
		            terms=new unsigned long long[n_terms];
			    pesos=new unsigned int[n_terms];
		        }
            //datos 1 y 2 no se consideran ya que se van a calcular nuevamente UB y #docs
		    if(idDato>0)
		    {
		    modulo = conteoDatos%2;
		    if(modulo==0)
		    {id_term=numero;
		    terms[posicionConsulta]=id_term;
		    }
		    else
		    {valor=numero;
		    pesos[posicionConsulta]=valor;
		    posicionConsulta++;
		    }

		 //   printf("[%d][%d]Numero:%d  \n",id_query,modulo,numero);
		    conteoDatos++;
		    }

			idDato++;
			}//fin if i!=0

	}
	if(i!=0)
	{
	  /*unsigned long long *terms2=NULL;
        unsigned int *pesos2=NULL;
        map<unsigned long long,unsigned int> termTmp;
        for(int j=0;j<n_terms;j++)
        {
        termTmp[terms[j]]=pesos[j];
        }
            
        terms2=new unsigned long long[termTmp.size()];
        pesos2=new unsigned int[termTmp.size()];
        int cont=0;
        
        
      
        
        
      
        
            for ( map<unsigned long long,unsigned int>:: iterator it= termTmp.begin(); it != termTmp.end(); it++)
    { 
terms2[cont]=it->first;
pesos2[cont]=it->second;
		
		cont++;
    }
      */  
            
//if(idConsulta%100==0)
//printf("Cargando Consulta %d\n",idConsulta);

querys->push_back(new DatosQuery(id_query, n_terms, terms, pesos)); // original
//querys->push_back(new DatosQuery(id_query, cont, terms2, pesos2));

    
      //   delete [] terms2;
	//delete [] pesos2;
    //    termTmp.clear();
        
        
        delete [] terms;
	delete [] pesos;
    }

   	free(h);
	h = NULL;


	idConsulta++;
if(idConsulta==nQ)
break;


 }

consultas.close();
//fflush(stdin);
}

void mostrarIndice(map<unsigned long long, PostingStream* > indiceThreadIn)
{

map<unsigned long long, PostingStream*>::iterator it = indiceThreadIn.begin();
pair<unsigned long long, PostingStream*> object;
cout<<"iterator points to a pair "<<"\n";
while( it != indiceThreadIn.end() ) {
object = *it;
// iterator points to a pair
cout << "mapper[" << object.first << "] = "<< endl;//<< object.second << endl;
it++;
}

 }
void mostrarCosulta(DatosQuery* consultaIn)
{
 printf("\nConsulta:");
       for(int j=0;j<consultaIn->n_terminos;j++)
        {
            printf(" [%d][%llu] -",consultaIn->valores[j],consultaIn->terminos[j]);
        }
        printf("\n");

}

void mostrarConsultas(vector < DatosQuery* > querys)
{
unsigned int contador=0;

 printf("\tMostrando Consulta:\n");

    while(contador<querys.size())
    {
        printf("Consulta[%d]:[%d]: ",contador,querys[contador]->n_terminos);
        mostrarCosulta(querys[contador]);
        printf("\n");
        contador++;
    }
}


 void wandHeapLocalesDinamico(
                                   DatosQuery* consulta, //Datos consulta
                                   int consultaId,
                                   int tid,  // P++ -> proc % threads 
                                   int topK, // 
                                   int nThreads,
                                   unsigned int umbral,
                                   int nSeleccionadosPaso,
                                   CBT*& heapPaso,//elementos almacenados en proceso anterior
                                   int &thresholdPaso,//nuevo threshold
                                   int &nSelPaso//nuevo conteo elementos en heap 
                                 //  unsigned int &umbralK_data//,
                                   )
  {

   //heapPaso->mostrarID_Prioridades();   
   //heapPaso->mostrarID_docs();  
         //     if(consultaId>42839)
       //  {      cout<< "TID:"<< tid  <<endl;
      //          cout<<"heapPaso->top: "<<heapPaso->top() <<endl;
      //   }
      unsigned int id_doc;
      unsigned long long id_term;
      unsigned int valor;
      unsigned int threshold = umbral;// UmbralesEstimados[consultaId];
      int nSeleccionadosTopK=nSeleccionadosPaso; //deberia ser igual al topk
      unsigned int min_thr=0;
      vector<QueryFeatureInfoT*> *query_terms_bloque=new vector<QueryFeatureInfoT*>();
      vector<PostingStream*> *streamsBloque=new vector<PostingStream*>();
      QueryFeatureInfoT *infoBloque;
      PostingStream *streamBloque;
   
      map<unsigned long long, PostingStream*>::iterator it;
		//llevo el conteo de los terms de consulta encontrados en el indice
		int n_terms=consulta->n_terminos;
		int n_terms_encontrados=0;

      map<int,unsigned long long> terminos;// = new map<int,unsigned long long>();
      map<int,unsigned int> pesos_terminos;

      typedef pair <int, unsigned long long> Int_Par_term;
      typedef pair <unsigned long long, int> Int_Par_docs;
      typedef pair <int,int> sizeListas;
      typedef pair <int,double> timeQ;
      typedef pair <int, sizeListas> tid_sizeListas;

		for(int i=0; i<n_terms; i++){
                        id_term=consulta->terminos[i];
                        valor=consulta->valores[i];
                        terminos.insert(Int_Par_term(i,id_term));
                        pesos_terminos.insert(Int_Par_term(i,valor));
			it=listaStruct[tid].indiceThreadIntervalos.find(id_term);
                        //Esto agrega un costo de computo al usar el peso de los terminos...
                  //      cout<<"idTerm: "<<id_term<<endl;
			if(it!=listaStruct[tid].indiceThreadIntervalos.end()){
			//	cout<<"makeRankingWand - encontrado\n";
				//termino encontrado
				n_terms_encontrados++;
				streamBloque=it->second;
				infoBloque=new QueryFeatureInfoT(id_term,valor,streamBloque->ub_inicial);//distintos ub
				infoBloque->ub=valor*streamBloque->ub_inicial;//idem
				query_terms_bloque->push_back(infoBloque);//dos estimadores
				streamsBloque->push_back(streamBloque);
                    //            cout<<"idTerm: "<<id_term<<"listaStruct[tid].size(): "<<streamBloque->n<<endl;
                                
			}//if... termino encontrado

		}//for... cada termino de la consulta
      //   if(consultaId>42839)
      //   {      
      //cout<< "TID:"<< tid  <<endl;
      //          cout<<"n_terms_encontrados: "<<n_terms_encontrados<<endl;
       //  }
       
        int nActualizaciones=0;
            if(n_terms_encontrados>0){
            //    cout<< "TID:"<< tid  <<endl;
            //    cout<<"antes wand: "<<n_terms_encontrados<<endl;
     
            Wand *wand=NULL;
            ScorerT *scorer=NULL;//exit(1);
	    scorer=new ScorerT(streamsBloque,query_terms_bloque);
	    wand=new Wand(n_terms_encontrados, scorer, streamsBloque);
            wand->init();
            //cout<< "TID:"<< tid  <<endl;
            //    cout<<"despues wand: "<<n_terms_encontrados<<endl;
     
         
            while((id_doc=wand->next(threshold,2)) != END_OF_STREAM)
            {//cout <<"+";
                                valor=wand->currentPayload;
                                //agregar al heap
                                if(nSeleccionadosTopK<topK)
                                {
                                heapPaso->insert(valor,id_doc); //Propuesta Marin
                                nSeleccionadosTopK++;
                                nActualizaciones++; //numero
            			}
				else{
                                        min_thr = heapPaso->top();
					if(min_thr<valor){
                                        heapPaso->insert(valor,id_doc);
                                        nActualizaciones++;
					}
				}
				//nuevo threshold
				if(nSeleccionadosTopK<topK){
				threshold=umbral;
				}
				else{
                                          threshold = heapPaso->top();
				}
	 }
            
            
         wand->~Wand();
       //       cout <<"*"<<endl;
         }
        
      //   cout <<"FUERA WAND* T:"<<threshold<<" S:"<<nSeleccionadosTopK<<endl;
         thresholdPaso = threshold;
         nSelPaso = nSeleccionadosTopK;
       //  cout <<"Despues FUERA WAND*"<<endl;
         
         //Fin llamada WAND % threads
  }

 void wandHeapLocales(
                                   DatosQuery* consulta, //Datos consulta
                                   int consultaId,
                                   int tid,
                                   int topK,
                                   int nThreads,
                                   int umbral
                                   )
  {
      //TOMAR TIEMPO DE CREACION CBT
      CBT *heapM = new CBT(topK);
      DOCS = 0;
     // mostrarCosulta(consulta);
      //ALMACENAR EL TIEMPO DEL CBT PARA EXCLUIRLO DE LA EJECUCION, 
      //OPTIMIZAR ESTA CREACION PREGUNTANDO SI LA SUMA DE DOCS A PROCESAR SUPERA K,
      //CON ESTO ULTIMO SE EVITAR CREAR EL VECTOR SI NO SERA RECORRIDO Y LLENADO POR WAND
      int nSeleccionadosTopK=0; //deberia ser igual al topk
      int nActualizaciones=0;
      unsigned int threshold = umbral;// UmbralesEstimados[consultaId];     
     /* if(nThreads<nPaticionIn)
      {   //for recursivo 
          int factor = nPaticionIn/nThreads;//potencia de dos y numero de grupos
          int tidDinamic;
          int thresholTmp;
          int nSelTmp;
        //  mostrarCosulta(consulta);
          for(int i=0;i<factor;i++)
          {          
              tidDinamic = tid + nThreads*i;
          //    cout <<"Llamada WAND : "<<tidDinamic<<" de tid: "<< tid <<endl;     
              
              wandHeapLocalesDinamico(
                                   consulta, //Datos consulta
                                   consultaId,
                                   tidDinamic,
                                   topK, // 
                                   nThreads,
                                   threshold,
                                   nSeleccionadosTopK,
                                   heapM,
                                   thresholTmp,
                                   nSelTmp
                                   // elementos almacenados en proceso anterior
                                 //  unsigned int &umbralK_data//,
                                   );
          
          nSeleccionadosTopK = nSelTmp;
          threshold = thresholTmp;  
          //if(consultaId>41305)
          //{
      //    cout << "nSeleccionadosTopK: "<<nSeleccionadosTopK<< " threshold: "<< threshold <<endl;
      
          
          //}
          }//fin for recursivo    
          //Creacion topk global
          int contador=1;
          
          pthread_mutex_lock( &lock );
          int dim = heapM->dimensionHeap();
                         if(dim>0)
                          while(contador<=dim)
                          {
                              HEAP.push_back(pair<unsigned int, unsigned int>(heapM->Prioridades[contador],heapM->ID_docs[contador] ));
                              contador++;
                          }
          
          pthread_mutex_unlock( &lock );       
          pthread_barrier_wait(&barrier);

          if(tid==0)
          {
              sort(HEAP.begin(), HEAP.end()); //ACA CAMBIAR EL SORT, POR UN SORT PARCIAL
              //umbralK_data=retornaUmbral(HEAP,topK);
              //logDataThreshold[consultaId]=retornaUmbral(HEAP,topK);
              clock_gettime(CLOCK_MONOTONIC, &tiempo2_ProcQ);
              //aca obtener vector de UBs
              //mostrarVectorRespuesta(HEAP,topK);
              HEAP.erase(HEAP.begin(), HEAP.end());
              HEAP.resize(0);
          }
           
          
      }*/
     // else
      //{    
      
      unsigned int id_doc;
      unsigned long long id_term;
      unsigned int valor;
      unsigned int min_thr=0;
      //long int sumaNumBloques=0;
      vector<QueryFeatureInfoT*> *query_terms_bloque=new vector<QueryFeatureInfoT*>();
      vector<PostingStream*> *streamsBloque=new vector<PostingStream*>();
      QueryFeatureInfoT *infoBloque;
      PostingStream *streamBloque;
      map<unsigned long long, PostingStream*>::iterator it;
      int n_terms=consulta->n_terminos;
      int n_terms_encontrados=0;
      map<int,unsigned long long> terminos;// = new map<int,unsigned long long>();
      map<int,unsigned int> pesos_terminos;

      typedef pair <int, unsigned long long> Int_Par_term;
      typedef pair <unsigned long long, int> Int_Par_docs;
      typedef pair <int,int> sizeListas;
      typedef pair <int,double> timeQ;
      typedef pair <int, sizeListas> tid_sizeListas;

      
       // Tiempo ini preprocesamiento querie
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo1_PreQ);
              
		for(int i=0; i<n_terms; i++){


                        id_term=consulta->terminos[i];
                        valor=consulta->valores[i];

                        terminos.insert(Int_Par_term(i,id_term));
                        pesos_terminos.insert(Int_Par_term(i,valor));


			it=listaStruct[tid].indiceThreadIntervalos.find(id_term);
                        //Esto agrega un costo de computo al usar el peso de los terminos...
			if(it!=listaStruct[tid].indiceThreadIntervalos.end()){
			//	cout<<"makeRankingWand - encontrado\n";
				//termino encontrado
				n_terms_encontrados++;

				streamBloque=it->second;
				DOCS = DOCS + streamBloque->n;
				infoBloque=new QueryFeatureInfoT(id_term,valor,streamBloque->ub_inicial);//distintos ub
//ACA SE DEBEN REALIZAR m PRODUCTOS POR CADA BLOQUE DE UB, DONDE m ES EL NUMERO DE BLOQUES DE CADA LISTA
//COMO ESTE PRODUCTO SE DEBE REALIZAR AL RECORRER LA LISTA, SE DEBERA CALCULAR INTERNAMENTE EN EL ITERADOR WAND
				infoBloque->ub=valor*streamBloque->ub_inicial;//idem
  //                              cout<<"LOW info["<<id_term<<"]->ub"<<infoBloque->ub<<"\n";
				query_terms_bloque->push_back(infoBloque);//dos estimadores
				streamsBloque->push_back(streamBloque); // esto al parece requiere mucho procesamiento o solo se envia el puntero?
                              //  sumaNumBloques = sumaNumBloques + streamBloque->nBloques;
                              
                              
                /*
 * LIMPIAR
 */
 
 //streamBloque->~PostingStream();
 
//or(i = 0; i < streamBloque->size(); i++)
//    delete streamBloque[i];
//streamBloque->clear();              
                              
			}
			//else
			//{
			//	delete * it;
				//it = listaStruct[tid].indiceThreadIntervalos.erase(it);
				
			//}
			
			
			//if... termino encontrado
            
            

		}
		//infoBloque=NULL;
		//streamBloque=NULL;
			
		//OPTIMIZAR DESTRUCCION VARIABLES
		
		
		
		//for... cada termino de la consulta
//Tiempo fin preprocesamiento pesos querie
clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo2_PreQ);


//int preCalculados=0;
         //tiempo ini WAND
            clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo1_Wand);
      
            if(n_terms_encontrados>0){
                
            //tiempo ini preprocesamiento Wand
         //   clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo1_PreWand);
            
            Wand *wand=NULL;
            ScorerT *scorer=NULL;//exit(1);
	    scorer=new ScorerT(streamsBloque,query_terms_bloque);
	    wand=new Wand(n_terms_encontrados, scorer, streamsBloque);
            wand->init();
            
            //tiempo fin preprocesamiento Wand
        //    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo2_PreWand);
            
          //  cout<<"COMENZANDO WAND"<<endl;
            
            //Tiempo ini iterador Wand
      //      clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo1_IteWand);
       
      	    while((id_doc=wand->next(threshold,2)) != END_OF_STREAM)
            {
                                
                                valor=wand->currentPayload;
				         //agregar al heap
                                if(nSeleccionadosTopK<topK)

                                {

                                heapM->insert(valor,id_doc);
                                 nSeleccionadosTopK++;
                               // nActualizaciones++; //numero
         			}
				else{
                                        min_thr = heapM->top();
					if(min_thr<valor){
                                        heapM->insert(valor,id_doc);
                                        nActualizaciones++;
					}
				}
				//nuevo threshold
				if(nSeleccionadosTopK<topK){
				threshold=umbral;
				}
				else{
                                          threshold = heapM->top();
   				}
			}
			
			
         //Tiempo final iterador Wand
//clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo2_IteWand);

	
         //   for (std::vector<PostingStream*>::iterator i(streamsBloque->begin()), i_end(streamsBloque->end()) ; i != i_end ; ++i)
		//	delete *i;
		//	delete streamsBloque;
		    DOCS_PROCESADOS = wand->evaluacionesCompletas;
		    
		
            wand->~Wand();
       
         }
         
    //  streamsBloque = NULL;    
         
         
    // Trazas de llamada recursiva...     
   //Fin recursion local de llamadas WAND % threads
         
 //Tiempo fin WAND
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo2_Wand);
 //Tiempo inicial Merge Sort
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo1_MergeSort);
    int contador=1;
  
    
   // pthread_mutex_lock( &lock );
    int dim = heapM->dimensionHeap();
    if(dim>0)
		while(contador<=dim)
                {
                    //cambiar por un heap Map para multiples threads en distintas queries
                    HEAP.push_back(pair<unsigned int, unsigned int>(heapM->Prioridades[contador],heapM->ID_docs[contador] ));
                    contador++;
		}
   // pthread_mutex_unlock( &lock ); 
    
  //  pthread_barrier_wait(&barrier);

if(tid==0)
{
    
    //Se tiene que cambiar el heap a un heap[int] para soportar uso de varios heap con multiple ejecucion multithread.
    
    sort(HEAP.begin(), HEAP.end()); //ACA CAMBIAR EL SORT, POR UN SORT PARCIAL
    //umbralK_data=retornaUmbral(HEAP,topK);
    //logDataThreshold[consultaId]=retornaUmbral(HEAP,topK);
  //  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo2_ProcQ);
   // mostrarVectorRespuesta(HEAP,topK);//file... 
    HEAP.erase(HEAP.begin(), HEAP.end());
    HEAP.resize(0);


}
    

    
//}
// Fin else Wand No Dinamico

//Tiempo final Merge Sort

clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo2_MergeSort);

//delete wand;
//heapM->~CBT();
delete heapM;


           //f(nSeleccionadosTopK<topK)
           HEAP_SIZE = nSeleccionadosTopK;
          // else
           HEAP_UPDATES = nActualizaciones;
		    
		           


}

/*
void *HeapLocalesPaso(void *arg)
{
	long int tiempoWand_procQ;
	
}*/
void *HeapLocales(void *arg)
{
	AsignaCPUThread(4);

	
//unsigned long int b, e, d;
//long int tiempoprocQueries;
unsigned long int tiempoWand_procQ1;
unsigned long int tiempoWand_procQ2;
unsigned long int tiempoWand_procQ3;
unsigned long int tiempoWand_procQ4;
unsigned long int tiempoWand_procQ5;

/*
long int tiempoWand_preQ;
long int tiempoWand;
long int tiempoWand_preWand;
long int tiempoWand_IteWand;
long int tiempoMergeSort;
long int diferencia;*/
//double mili_procQ,mili_preQ,mili_Wand,mili_preWand,mili_IteWand,mili_MergeSort;    
    
  datosQuerie *tmp;
  tmp = (datosQuerie*)arg;
  int i =  tmp->idQ; 
  int nT = tmp->nthreads;
  int tidLocal =tmp->tid;
  int ki =tmp->k;



//int *cpu_proc;

//cpu_proc = (int *) param;
//cpu_set_t cpuset;	
//cpu_proc = tidLocal;
cpu_set_t cpuset;	


if(tidLocal == 0){
CPU_SET(0,&cpuset);
//bind process to processor 0 
if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) <0) {
perror("pthread_setaffinity_np");
}
}

  
 // cout<<" i: "<<i<<" nT: "<<nT<<" tidLocal: "<<tidLocal<<" ki: "<<ki<<endl;
 // int nTerminosQ;
//  long int sumaBloques=0,sumaBloquesDesc=0,sumaUpdateHeap=0,sumaEvalComp=0;
//  long unsigned int  sumaBloquesTotal=0,sumaBloquesDescTotal=0,sumaUpdateHeapTotal=0,sumaEvalCompTotal=0;
//  long unsigned int  sumaBloquesTotalPromedio=0,sumaBloquesDescTotalPromedio=0,sumaUpdateHeapTotalPromedio=0,sumaEvalCompTotalPromedio=0;
  //char nombreArchivoLista [1024];
  //UK-SL 4Gb
  // sprintf (nombreArchivoLista, "/run/media/oscar/Datos/listas/4GB/thread%d/lista_C%d_Thread_%d.dat",nT,nT,tidLocal);
 //Gov2 25 Gb
 // sprintf (nombreArchivoLista, "/run/media/oscar/Datos/datos_gov2/log3mb/lista25Gb/gov2_%dthread/listaGov26G_C%d_Thread_%d.dat",nT,nT,tidLocal);
  
  // cout<<"leyendo lista"<<nombreArchivoLista<<" en tread : "<<tidLocal<<endl;
   
//leerIndiceCharGrupos(nombreArchivoLista/*,numThreads,listaStruct*/,tidLocal);
//Creacion de archivos


	
	
 //ACA INICIO DE THREAD	
    
    /*
 if(tidLocal==0)
 {
 contadorInicio=0;
 contadorPaso=0;
 nSeleccionadosG = 0;
 //heapMGlobal = new CBT(K);
 contadorPasoWand=0;
 contadorLocal=0;
 contadorFinWand=0;
        
      //   if(i%1==0)
      //     {cout<<" Q_ini: "<<i<<endl;    
      //     cout<<" ki_ini: "<<ki<<endl; 
      //     }   
        
   //Tiempo ini procesamiento consulta     
   //clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo1_ProcQ);  
 }
 */
 //pthread_barrier_wait(&barrier);
    //if(i==0)
  
  //  pthread_mutex_lock( &lock );
    //contadorLocal++;
   // if(contadorLocal==1)
   // {
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo1_ProcQ); //
        
   // }      
   // pthread_mutex_unlock( &lock );
  DOCS_PROCESADOS=0;
  HEAP_UPDATES=0;
 
  wandHeapLocales(//ListaDocumentos docConsulta, //Documentos seleccionados
                                   listaStruct[tidLocal].consultas[i], //Datos consulta
                                   //vector<PostingStream*> *streams, //Vector posting
                                   //int nThreads, //nThreads
                                   //Wand *wand, //WAND
                                   //threshold, //Umbral
                                   //listaStruct[0].indiceThread, //Lista de terminos - DocId
                                   i,
                                   tidLocal,
                                   ki,
                                   nT,
                                   0//,
                                   //umbralK//UmbralesEstimados[i]//thresholdi
        //                           tmp->ubLocales

                                );
                                
                              
// cout <<"llego tid: "<< tidLocal << endl; 
// pthread_barrier_wait(&barrier); //este barrier no afecta toma de tiempo en sort heap, pero en implementacion en motor de busqueda destruir el heap global una vez enviado querie al front
 clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo2_ProcQ);
 
 
 
 
          if(tidLocal==0)
          {  
              //Procesamiento Q
              thersholdLogConsulta<<i<<" "<<DOCS;  
              
			  diff_time(&tiempo2_Wand,&tiempo1_Wand,&latencia); 
              tiempoWand_procQ1=latencia.tv_sec*1000000000+latencia.tv_nsec;
              thersholdLogConsulta<<" "<<tiempoWand_procQ1;      		   //Procesaiento WAND
			       
			       
			  diff_time(&tiempo2_MergeSort,&tiempo1_MergeSort,&latencia); 
              tiempoWand_procQ2=latencia.tv_sec*1000000000+latencia.tv_nsec;
              thersholdLogConsulta<<" "<<tiempoWand_procQ2;      		   //Procesaiento Merge
			         
			       
			       
              diff_time(&tiempo2_PreQ,&tiempo1_PreQ,&latencia);            //Pre-procesamiento WAND
              tiempoWand_procQ3=latencia.tv_sec*1000000000+latencia.tv_nsec;
              thersholdLogConsulta<<" "<<tiempoWand_procQ3;

			  diff_time(&tiempo2_ProcQ,&tiempo1_ProcQ,&latencia); 
              tiempoWand_procQ4=latencia.tv_sec*1000000000+latencia.tv_nsec;
              thersholdLogConsulta<<" "<<tiempoWand_procQ4;        //Procesamiento Q
              
              
              
              
			       

      //              b = t->begin.tv_sec*1000000000 + t->begin.tv_nsec;
    //e = t->end.tv_sec  *1000000000 + t->end.tv_nsec;
    //d = t->delta.tv_sec*1000000000 + t->delta.tv_nsec;

    //printf("<%lu, %lu, %lu>", b, e, d);
             
    
           if(i%1==0)
           {cout<<" Q: "<<i<<endl;    
           cout<<" ki: "<<ki<<endl; 
           }
           
          }
          
          
  //ACA INICIO DE THREAD        
          

 // }//Fin for
       


//fin for k=10,100,1000,10000

//pthread_barrier_wait(&barrier);
pthread_exit(NULL);

}

void escribirLogs(int nT,int T)
{
     int ki;
for(int k0=0;k0<=3;k0++)//3
{   
	
	ki = pow(10,k0+1); 

   cout<<" :" <<ki<<endl;
   char nombreArchivoConsulta [1024];
   char nombreArchivo [1024];
 //  char nombreArchivoGlobales [1024];
  // char nombreArchivoPorcentajes [1024];

   // ACA DEFINIR RUTA DE LOGS
  //sprintf (nombreArchivoConsulta, "/work/x01/x01/hgvcosta/WAND/logs/c%d/log_Detalle_HeapLocales_Costos_T%d_Consultas%d_K%d.dat",nT,nT,T,K);
  //sprintf (nombreArchivoGlobales, "/work/x01/x01/hgvcosta/WAND/logs/c%d/log_Global_HeapLocales_Costos_T%d_Consultas%d_K%d.dat",nT,nT,T,K);

 // sprintf (nombreArchivoConsulta, "./logs/log_Umbral_HeapLocales_Costos_T%d_Consultas%d_K%ddFourier.dat",nT,T,K);
 // sprintf (nombreArchivoGlobales, "./logs/log_Global_HeapLocales_Costos_T%d_Consultas%d_K%d.dat",nT,T,K);
 sprintf (nombreArchivoConsulta, "/run/media/oscar/Informacion/universidad/Doctorado/2013/2014/funcionesDensidadGlobal/tiempos_T%d_Consultas%d_K%dBMW_Terms",nT,T,ki);
//printf ("/run/media/oscar/Informacion/universidad/Doctorado/2013/2014/funcionesDensidadGlobal/log_HeapLocales_Tiempos_T%d_Consultas%d_K%dBMW_UNICAS.dat",nT,T,ki);

  thersholdLogConsulta.open (nombreArchivoConsulta);
 // tiemposGlobales.open (nombreArchivoGlobales);


 // tiemposConsulta << "Evaluaciones\tUpdate Heap\tNumero Bloques\tDescompresion\tEvaluaciones Promedio\tUpdate Heap Promedio\tNumero Bloques Promedio\t Descompresion Promedio\n";
 // tiemposGlobales << "Evaluaciones\tUpdate Heap\tNumero Bloques\tDescompresion\tEvaluaciones Promedio\tUpdate Heap Promedio\tNumero Bloques Promedio\t Descompresion Promedio\n";

//       sprintf (nombreArchivo, "/home/oscar/Programacion/programacion2013/Wand2013/WandCache/2014/logs/logTopK_T%d_Consultas%d_K%d_localesdFourier.dat",nT,T,ki);
//  sprintf (nombreArchivoGlobales, "./logs/log_Global_HeapLocales_Costos_T%d_Consultas%d_K%d.dat",nT,T,K);

  logT.open (nombreArchivo);
      
}
}
void AsignaCPUThread(int cpuId)
{
		int s, j;
    cpu_set_t cpuset;
    pthread_t thread;
    int speid=cpuId;
    thread = pthread_self();
	CPU_ZERO(&cpuset);
    CPU_SET(speid, &cpuset);

    s = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
	}

void AsiganCPUPadre(int cpuId)
{
	
	int s, j;
    cpu_set_t cpuset;
    pthread_t thread;
    int speid=cpuId;
    thread = pthread_self();


    /* Set affinity mask to include CPUs 0 to 7 */

    CPU_ZERO(&cpuset);
    CPU_SET(speid, &cpuset);

    s = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
    if (s != 0)
        handle_error_en(s, "pthread_setaffinity_np");

    /* Check the actual affinity mask assigned to the thread */
    s = pthread_getaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
    if (s != 0)
        handle_error_en(s, "pthread_getaffinity_np");

    printf("Set returned by pthread_getaffinity_np() contained:\n");
    for (j = 0; j < CPU_SETSIZE; j++)
        if (CPU_ISSET(j, &cpuset))
            fprintf(stderr,"%d    CPU %d\n",speid, j);
	
	}

int main (int argc, char *argv[]) {

clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo1_Padre);
AsiganCPUPadre(0);




//exit(0);

int numThreads = atoi( argv[1]); //numero threads, por defecto 1
int tmpK = atoi( argv[2]); //numero top k
unsigned int numeroConsultasProceso = atoi( argv[3]);//numero de consultas
//int opcionWand = atoi( argv[4] );
SizeBloque = atoi( argv[4] );

nPaticionIn = atoi( argv[5]); //numero de particiones de listas a utilizar


int rc;
K=tmpK;
//printf("\n nT[%d],K[%d],nC[%d]\n",numThreads,K,numeroConsultasProceso);


//i7 - 32Gb terminos unicos
//leerConsultasChar("/run/media/oscar/Informacion/universidad/Doctorado/2013/2014/funcionesDensidadGlobal/queriesTermsGov2.txt",&querys,numeroConsultasProceso);
leerConsultasChar("/run/media/oscar/Datos/datos_gov2/queriesGovWandFrec1.dat",&querys,numeroConsultasProceso);
//mostrarConsultas(querys);
printf("Fin carga consultas...\n");
// exit(0);
//escribirLogs(numThreads,querys.size());
 
contadorBandera = 0;

for(int t=0; t<nPaticionIn; t++)
{
  listaStruct[t].tid=t;
  listaStruct[t].nthreads=numThreads;
  listaStruct[t].consultas=querys;
//  listaStruct[t].umbralInicial=umbral;
//  minimosLocalesVector.push_back(1000000);


}
char nombreArchivoLista [1024];

long int tiempoCargaLista;
struct timespec latencia;
clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo1_Lista);

//nPaticionIn : numero de particiones a utilizar
printf("Inicio carga lista...\n");
for(int t=0; t<nPaticionIn; t++)
{
printf("Cargando lista %d lista...\n",t);

//i7 - 25Gb
sprintf (nombreArchivoLista, "/run/media/oscar/Datos/datos_gov2/log3mb/lista25Gb/gov2_8thread/listaGov26G_C8_Thread_0.dat",nPaticionIn,nPaticionIn,t);
////printf ("/run/media/oscar/Datos/datos_gov2/log3mb/lista25Gb/gov2_%dthread/listaGov26G_C%d_Thread_%d.dat",nPaticionIn,nPaticionIn,t);
//sprintf (nombreArchivoLista, "/run/media/oscar/Datos/datos_gov2/log3mb/lista25Gb/gov2_1thread/listaGov26G_C1_Thread_%d.dat",t);
leerIndiceCharGrupos(nombreArchivoLista,t);
}

//Experimento traza lista pequenha
//i7 - 32Gb
//int t = 0;
//sprintf (nombreArchivoLista, "/run/media/oscar/Datos/datos_gov2/log3mb/lista25Gb/gov2_8thread/listaGov26G_C8_Thread_%d.dat",t);
//sprintf (nombreArchivoLista, "/run/media/oscar/Datos/datos_gov2/log3mb/lista25Gb/gov2_1thread/listaGov26G_C1_Thread_%d.dat",t);
//leerIndiceCharGrupos(nombreArchivoLista,t);

clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo2_Lista);
diff_time(&tiempo2_Lista,&tiempo1_Lista,&latencia); 

tiempoCargaLista=latencia.tv_sec*1000000000+latencia.tv_nsec;
printf("Fin carga lista...\n");
cout<<"\ntiempo lista: "<<tiempoCargaLista<<endl;



//
 return EXIT_SUCCESS;

pthread_t mythread[numThreads];
pthread_attr_t attr1;


//pthread_attr_t custom_sched_attr;	
int fifo_max_prio;	
struct sched_param fifo_param;

pthread_attr_init(&attr1);	
pthread_attr_setinheritsched(&attr1, PTHREAD_EXPLICIT_SCHED);	
//pthread_attr_setschedpolicy(&attr1, SCHED_FIFO); 
  pthread_attr_setscope(&attr1, PTHREAD_SCOPE_SYSTEM);
  fifo_max_prio = sched_get_priority_max(SCHED_FIFO);	
  fifo_param.sched_priority = fifo_max_prio;	
  fifo_param.sched_priority=100;
  pthread_attr_setschedparam(&attr1,&fifo_param);
  // pthread_create(&(threads[i]), &custom_sched_attr, ....);
      
 //pthread_attr_getschedparam(&attr1,&param);
   int ki;
//  unsigned int umbralK=0;
//for(int k0=0;k0<=0;k0++)
  unsigned int T;
  T=querys.size();
  unsigned long int tiempoWand_CargaIniQ;
  unsigned long int tiempoWand_FinIniQ;
  unsigned long int tiempoPadre;
for(int k0=0;k0<=3;k0++)//3
{   
	
	ki = pow(10,k0+1); 
   cout<<" :" <<ki<<endl;
   char nombreArchivoConsulta [1024];
sprintf (nombreArchivoConsulta, "/run/media/oscar/Informacion/universidad/Doctorado/2013/2014/funcionesDensidadGlobal/Tiempos_T%d_%d_K%dBMW_TERMS_DatosTotales25Gb_15Abril_OtrasCPU",numThreads,T,ki);
//printf ("/run/media/oscar/Informacion/universidad/Doctorado/2013/2014/funcionesDensidadGlobal/log_HeapLocales_Tiempos_T%d_Consultas%d_K%dBMW_UNICAS_Dincamic.dat",numThreads,T,ki);
thersholdLogConsulta.open (nombreArchivoConsulta);

char nombreArchivoTiempo [1024];
sprintf (nombreArchivoTiempo, "/run/media/oscar/Informacion/universidad/Doctorado/2013/2014/funcionesDensidadGlobal/Tiempos_T%d_%d_K%dGlobales",numThreads,T,ki);
//printf ("/run/media/oscar/Informacion/universidad/Doctorado/2013/2014/funcionesDensidadGlobal/log_HeapLocales_Tiempos_T%d_Consultas%d_K%dBMW_UNICAS_Dincamic.dat",numThreads,T,ki);
tiempoGlobal.open (nombreArchivoTiempo);



  //  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo1_Lote);
   struct sched_param param;
for( unsigned int i=0;i<T;i++)
{
	 
 //consulta
clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo1_ThreadQ);

pthread_mutex_init(&lock, NULL);
pthread_cond_init(&llegaron, NULL);
//pthread_attr_init(&attr1);
pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_JOINABLE);
pthread_barrier_init(&barrier, &attr, numThreads);
//pthread_attr_setscope(&attr1, PTHREAD_SCOPE_SYSTEM);
 

  
  
clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo2_ThreadQ); 
diff_time(&tiempo2_ThreadQ,&tiempo1_ThreadQ,&latencia); 
tiempoWand_CargaIniQ=latencia.tv_sec*1000000000+latencia.tv_nsec;
            

//Numero de threads a utilizar 
for(int t=0; t<numThreads; t++)
{

//CPU_SET( t, &set );
    //listaStruct[t].asignaQ(T);
    
    dataQ[t].idQ = i;
    dataQ[t].k = ki;
    dataQ[t].tid=t;
    dataQ[t].nthreads=numThreads;
    // pthread_setaffinity_np(mythread[t], sizeof(cpu_set_t), &set);
   // printf("Heap Locales\n");
    rc =  pthread_create(&mythread[t], &attr1,HeapLocales,(void *)&dataQ[t]); //se puede aplicar ub locales REVISAR ESTA LLAMADA PARA NO TENER QUE PASAR LA LISTA COMPLETA, OPCION CREAR OBJETO SOLO DE PARAMETROS
    
    if (rc)
    {
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      abort();//exit(-1);
    }

}

//pthread_barrier_wait(&barrier);



 // logT.close();
  //ESTO MIDE EL TIEMPO DE USO DEL THREAD!!!
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo1_ThreadCierreQ); 
  
 
   
   
  for(int t = 0; t < numThreads; t++ )
  {
    pthread_join( mythread[t], NULL );
  }
  
 //       pthread_mutex_destroy(&barrier[i]->mutex); oblivius
 //     pthread_cond_destroy(&barrier[i]->cond);
  
 // pthread_attr_destroy(&attr1);
 pthread_mutex_destroy(&lock);
 pthread_cond_destroy(&llegaron);
 clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo2_ThreadCierreQ); 
 
 
 diff_time(&tiempo2_ThreadCierreQ,&tiempo1_Padre,&latencia); 
tiempoPadre=latencia.tv_sec*1000000000+latencia.tv_nsec;

 
 diff_time(&tiempo2_ThreadCierreQ,&tiempo1_ThreadCierreQ,&latencia); 
tiempoWand_FinIniQ=latencia.tv_sec*1000000000+latencia.tv_nsec;
 
   thersholdLogConsulta<<" "<<tiempoWand_CargaIniQ;   
   thersholdLogConsulta<<" "<<tiempoWand_FinIniQ;   
   
    diff_time(&tiempo2_ThreadCierreQ,&tiempo1_ThreadQ,&latencia); 
   tiempoWand_FinIniQ=latencia.tv_sec*1000000000+latencia.tv_nsec;
   thersholdLogConsulta<<" "<<tiempoWand_FinIniQ; 
   
   
   thersholdLogConsulta<<" "<<DOCS_PROCESADOS;        //Evaluaciones completas
   thersholdLogConsulta<<" "<<HEAP_UPDATES;        //Evaluaciones completas
   thersholdLogConsulta<<" "<<HEAP_SIZE;          //size Heap
   
   
   
   
   thersholdLogConsulta<<endl;
   
}//fin for queries

cout<<"Tiemo Global: "tiempoPadre<<endl; 
tiempoGlobal<<tiempoPadre<<endl; 
tiempoGlobal.close();      
thersholdLogConsulta.close();
//logT.close();        

}//fin for variacion ki


//
 

 return EXIT_SUCCESS;
 //exit(0);

}
