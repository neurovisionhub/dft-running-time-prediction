//Crear wand para todo tipo de experimentos
//Ordenar las librerias para distintos tipos de listas
//incorporar cualquier tipo de calgulo de modelo alpha*w(d,t), derivado de modelo tf-idf   tf=f/sizeDoc, tfBM25, otros...

//HITSCACHE
//perf stat -e L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses
#include <papi.h>


//Disenar la otra version multithread separa pero con identicas llamads y estructuras...
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
//Libreria WAND-Index Block Max
#include "./Libs/libWand/bloques2013/WandLibreria2014.h"
#include <math.h>
//Libreria WAND-Baseline
//#include "Wand.h"
#include "./Libs/libExtras/estructfloat.h"
#include "math.h"
//Libreria que almacena el heap-topk
#include "./Libs/libWand/HeapMfloat.h"
#include <string.h>
#include <math.h>
// Biblioteca STL
#include <map>
#include <vector>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <istream>
#include <ostream>
#include <iterator>
#include <sstream>
//Variables
#include "variables.h"
#include "./Libs/libExtras/libData.h"
//#include "./multithread/libThreadsWand.h"


bool esperando;
int threadsLibres;
int activos[8];
int consultasBandera[20000];
int consultasBanderaInicial[20000];
int contQueries;
struct timespec _timesMERGELatencia[8];

struct timespec _timesIniQBroker[8];


struct timespec _timesMERGE[8];
struct timespec _timesPreQ[8];
int _timesLatencia[8];
//struct timespec _timesWandInicio[8];
//struct timespec _timesWandInicio[8];
struct timespec _timesQ[8];
	int procesosTraza;
 #define handle_error_en(en, msg) \
               do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

       #define handle_error(msg) \
               do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define THREADS 8 //MAX num thread architect
using namespace std;
//lista de terminos
ListaInvertida listaStruct[THREADS];				//lista de terminos
//void procesingQueries(int opcionW);
//unsigned int UMBRAL,HEAP_UPDATES,DOCS_PROCESADOS;
int K;
int procId;
int tipoMetodo; //1-->baseline, 2-->BMW
int tipoRank;   //1-->TF,2-->BM25,3-->TF-IDF
int busquedaAND=0;//suma UB
int busquedaOR;//min UN o bien threshold estatico en 0
int particionIni,particionFin;
int numThreadsWand;
map<int,int> contPasoThread;
int sumaPasoThread[8];
int banderas[8];
set<int> queriesListas;
//map<int,CBT*> CBT_WandDinamic;//multiples cbt?
int termOrQueries;
int MAXARCH;
CBT* CBT_WandDinamic[8];
class datosQuerie
{
   public:
   int tid;
   int nthreads;
   int idQ;
   int coreProc;
   int corePadre;
   //vector < DatosQuery* > consultas;
   DatosQuery* consulta;
   vector<int> vBanderas;
   vector<int> listOrden;
  // pthread_barrier_t *_barrier;
  // pthread_mutex_t *_lock;
   int k;
   float umbral;
   float umbralK;
   int p;
   datosQuerie()
   {

   }
   datosQuerie(int p,int pr, unsigned int id,int _k)
   {
     tid  = p;
     nthreads = pr;
     idQ = id;  
     k=_k;
   }
   
   datosQuerie(int pr, unsigned int id,int _k)
   {
     nthreads = pr;
     idQ = id;  
     k=_k;
   }
   
   void asignar()
   {
	   
   }
   
    ~datosQuerie()
   {
 //   delete consulta;
   // vBanderas.clear();
   //    delete[] _barrier;
   }
 
 
};

typedef struct 
{
    int nthreads;
    int called;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

} barrier_t;
int querierForJumpFree;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier;
pthread_barrier_t barrier2[THREADS];
pthread_barrierattr_t attr;
pthread_barrierattr_t attrT[THREADS];
pthread_cond_t llegaron = PTHREAD_COND_INITIALIZER;
pthread_rwlock_t rw_rdlock;

pthread_mutex_t mutex;
pthread_cond_t cond;
//pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t  condition_var   = PTHREAD_COND_INITIALIZER;
pthread_mutex_t count_mutex;
pthread_cond_t condition_var;

static pthread_mutex_t mutexes[THREADS];
static pthread_cond_t condiciones[THREADS];
static pthread_barrierattr_t barreras[THREADS];

//	pthread_mutex_t lock[THREADS];// = PTHREAD_MUTEX_INITIALIZER;
//    pthread_barrier_t barrier[THREADS];
//    pthread_cond_t llegaron[THREADS];// = PTHREAD_COND_INITIALIZER;
struct timespec tiempo1_GlobalQLote, tiempo2_GlobalQLote, latenciaQ;    

void dimDocsQuerie(DatosQuery* consulta)
{
set<unsigned int> iddocs_traza;
int n_terms=consulta->n_terminos;
unsigned long long id_term;
map<unsigned long long, PostingStream*>::iterator it;
PostingStream *stream;
//pre-procesamiento querie
for(int i=0; i<n_terms; i++){

	//tomar datos de la consulta
    id_term=consulta->terminos[i];
    //valor=consulta->valores[i];
	it=listaStruct[0].indiceThread.find(id_term);
	if(it!=listaStruct[0].indiceThread.end()){
		//termino encontrado

		stream=it->second;
		for(int h=0;h<stream->n;h++)
		{
			iddocs_traza.insert(stream->id_doc_l[h]);

		}
		//thersholdLogConsulta<<" "<<stream->n;
		DOCS_SUMA = DOCS_SUMA + stream->n;
		//stream->~PostingStream();
	}//if... termino encontrado
}//for... cada termino de la consulta


DOCS=iddocs_traza.size();



}

void dimDocsQuerieSize(DatosQuery* consulta)
{
set<unsigned int> iddocs_traza;
int n_terms=consulta->n_terminos;
unsigned long long id_term;
map<unsigned long long, PostingStream*>::iterator it;
PostingStream *stream;
//pre-procesamiento querie
for(int i=0; i<n_terms; i++){

	//tomar datos de la consulta
    id_term=consulta->terminos[i];
    //valor=consulta->valores[i];
	it=listaStruct[0].indiceThread.find(id_term);
	if(it!=listaStruct[0].indiceThread.end()){
		//termino encontrado

		stream=it->second;
		thersholdLogConsulta<<" "<<stream->n;

	}//if... termino encontrado
}//for... cada termino de la consulta

}


 void wandHeapLocalesDinamico(
                                   DatosQuery* consulta, //Datos consulta
                                   int consultaId,
                                   int tid,  // P++ -> proc % threads 
                                   int topK, // 
                                   int nThreads,
                                   float umbral,
                                   int nSeleccionadosPaso,
                                   CBT*& heapPaso,//elementos almacenados en proceso anterior
                                   float &thresholdPaso,//nuevo threshold
                                   int &nSelPaso//nuevo conteo elementos en heap 
                                 //  unsigned int &umbralK_data//,
                                 ,int &_docSuma,
                                 int paso,
                                 int &_nActualizaciones,
                                 int &_computoScore,
                                 int &_nextTmp,
                                 struct timespec &_timePre,
                                 int master,
                                 int tidOrigen
                                   )
  {

   //heapPaso->mostrarID_Prioridades();   
  // heapPaso->mostrarID_docs();  

      //cout<<"heapPaso->top: "<< heapPaso->top() <<" paso "<<paso<<endl;
      unsigned int id_doc;
      unsigned long long id_term;
      //float valor;
      float threshold = umbral;// UmbralesEstimados[consultaId];
      int nSeleccionadosTopK=nSeleccionadosPaso; //deberia ser igual al topk
      float min_thr=0.0;
      vector<QueryFeatureInfoT*> *query_terms=new vector<QueryFeatureInfoT*>();//optimizar eliminacion punteros
      vector<PostingStream*> *streams=new vector<PostingStream*>();
      QueryFeatureInfoT *info;
      PostingStream *stream;
     struct timespec tmpTiempo;
      map<unsigned long long, PostingStream*>::iterator it;
		//llevo el conteo de los terms de consulta encontrados en el indice
		int n_terms=consulta->n_terminos;
		int n_terms_encontrados=0;


float tfQ,score;
float alpha;
int docsSuma=0;
int nActualizaciones=0;
int _next = 0;
_computoScore=0;
 
 
 
              
 
			for(int i=0; i<n_terms; i++){
			//tomar datos de la consulta
            id_term=consulta->terminos[i];
            tfQ=(float)consulta->valores[i];
         //   cout<<"NUM TERMS:"<<n_terms<<"---tid --"<<tid<<"-------"<<endl;
            if(tipoMetodo==2)
            {
				it=listaStruct[tid].indiceThreadIntervalos.find(id_term);
				if(it!=listaStruct[tid].indiceThreadIntervalos.end()){
					//termino encontrado
					n_terms_encontrados++;
					stream=it->second;
					docsSuma = docsSuma + stream->n;
					// IDF*(2->BM25 o 3->TF/|D| )
					if(tipoRank!=1)
					alpha=tfQ*stream->idf_term;
					else
					alpha=tfQ;
					
					info=new QueryFeatureInfoT(id_term, alpha, stream->ub_inicial);
					info->ub=alpha*stream->ub_inicial;
					//sumUB=+info->ub;
				//	cout<<"info->ub: "<<info->ub<<" idf "<<stream->idf_term<<endl; 
		//	cout<<"["<< id_term<<"]info->ub: "<<info->ub<<" dim list"<< stream->n <<" Tid: "<<tid<<" alpha "<<alpha<<endl; 
					query_terms->push_back(info);
					streams->push_back(stream);

				}//if... termino encontrado
		    }
		  //  _DOCS_SUMA=docsSuma;
            _docSuma=docsSuma;
            //	cout<< "--------yyyyyyyy------TID:"<< tid  <<endl;
	}
	
	
	                      //   if(tidOrigen==master)
  	                      //   {   
								 clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tmpTiempo);
								 _timePre = tmpTiempo;
							// }
	
	
	
	
	//cout<< "--------XXXX------TID:"<< tid  <<endl;
		    if(n_terms_encontrados>0){
			Wand *wand=NULL;
            ScorerT *scorer=NULL;//exit(1);	
        //    cout<< "--------99999544444444444499999------TID:"<< tid  <<endl;
			if(tipoMetodo==1)	
			scorer=new ScorerT(streams, query_terms,0); //0--> baseLine	
		//	 cout<< "--------999995444------------4444499999------TID:"<< tid  <<endl;
			if(tipoMetodo==2)
			scorer=new ScorerT(streams, query_terms,1); //0--> bmw
		//	cout<< "--------9999999999999999------TID:"<< tid  <<endl;
			wand=new Wand(n_terms_encontrados, scorer, streams);
        	wand->init();
			wand->tipoWAND_AND_OR=busquedaAND;
			
		//	cout<< "----------------------TID:"<< tid  <<endl;
			//Buscar doc candidato
			while((id_doc=wand->next(threshold,tipoMetodo,tipoRank)) != END_OF_STREAM)
            {//cout <<"+";
				_next++;
                                score=wand->currentPayload;
                                //agregar al heap
                                if(nSeleccionadosTopK<topK)
                                {
									heapPaso->insert(score,id_doc); 
									nSeleccionadosTopK++;
									nActualizaciones++; 
								}
								else{
                                        min_thr = heapPaso->top();
										if(min_thr<score){
                                        heapPaso->insert(score,id_doc);
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
			_computoScore = wand->evaluacionesCompletas;
			wand->~Wand();
       //       cout <<"*"<<endl;
         }
      //   cout<< "--------XXXX------TID:"<< tid  <<endl;
        _nActualizaciones = nActualizaciones;
       //  cout <<"FUERA WAND* T:"<<threshold<<" S:"<<nSeleccionadosTopK<<endl;
         thresholdPaso = threshold;
         nSelPaso = nSeleccionadosTopK;
         _nextTmp=_next;
       //  cout <<"Despues FUERA WAND*"<<endl;
         
         //Fin llamada WAND % threads
  }

//tmp->consulta,i,ki,tmp->umbral,tipoRank,threadMaster,tidLocal,nT
//ejecucion Wand-querie
void WandMultihebraDinamica(DatosQuery* consulta,
              int consultaId,
              int topK,
              float umbral,
              int tipoRank,/*, pthread_barrier_t *barrierIn, pthread_mutex_t *lockIn*/
              unsigned long int &_HEAP_SIZE, unsigned long int &_DOCS_PROCESADOS,unsigned long int &_HEAP_UPDATES,unsigned long int &_DOCS_SUMA, unsigned long int &_NEXT,float &_UMBRAL,
              int maestro,
              int _pid,
              unsigned long int &tiempoPreQWD,
              int _nunThreads,
              vector<int> listAsig,
              CBT*& heapM
              )
{
	  
	 // struct timespec> _timeW1,_timeW2;
	  struct timespec _timePre1,_timePre2;
      DOCS = 0;
      int nSeleccionadosTopK=0; //deberia ser igual al topk
      int _nActualizaciones=0;
      struct timespec _tmp; 
      int docsSuma = 0;
      float threshold = umbral;
      int computoScore = 0,procScore;
      int _next;
	  //if(nThreads<nPaticionIn)
      //{   //for recursivo 
          int factor = MAXARCH/_nunThreads;//potencia de dos y numero de grupos
          struct timespec _latencia;
          int tidDinamic;
          float thresholTmp;
          int nSelTmp;
        //  mostrarCosulta(consulta);
         
              
              	//Tiempo fin preprocesamiento pesos querie
		//if(_pid==maestro)
		//{//cout <<" fin preprocesamiento time Tid: "<<_pid<<endl;
		//clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_tmp);//1
		//_time.push_back(_tmp);
		//clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_tmp);//2
		// _time.push_back(_tmp);
		//}
		
		//los threads pueden leer primeramente las particiones de listas con mayor densidad. Cabe recordar que los documentos estan repartidos por id_doc%MaxThreads y cada lista generada es 
		//leida por los threads de forma descendente por dendsidad de lista.
		
		//otra hipotesis, que si las listas mantienen estructuras ordenadas, se puede modelar y predecir de mejor forma el costo temporal.
		//El costo espacial tiene relacion del numero de documentos y la distribucion de los pesos de cada lista. Estos componentes se pueden interpretar como unidad de area y masa, donde la 
		//densidad de las listas es la integral de pesos, donde la aproximacion discreta es la sumatoria de todos los puntajes de las listas.
		//
	//	cout<<"-------_pid"<<_pid<<"------------ tidDinamic: "<<_nunThreads<<endl;
		// cout<<"Factor:  "<<factor<<endl;
          int listInicial = _pid%_nunThreads; 
          vector<int> _temp;
          for(int i=0;i<MAXARCH;i++)
          {
			  if(listAsig[i]==_pid)
			  _temp.push_back(i);
			  
		  }  
		  
          for(int i=0;i<factor;i++)
          {          
			//    if(_pid==maestro)
			//	{ // tiempo incial de procesamiento
		  		  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_timePre1);//0
		  	 //   }
			  //Para recorrido serial y numero estatico de threads de 1 a N.....
              //tidDinamic = _pid + _nunThreads*i;
              //Para multithread dinamico
              tidDinamic = _temp[i];
              
              
              
              //cout<<"tidDinamic: "<<tidDinamic<<endl;
              //cout<<"_nunThreads: "<<_nunThreads<<endl;
              wandHeapLocalesDinamico(
                                   consulta, //Datos consulta
                                   consultaId,
                                   tidDinamic,
                                   topK, // 
                                   _nunThreads,
                                   threshold,
                                   nSeleccionadosTopK,
                                   heapM,
                                   thresholTmp,
                                   nSelTmp,
                                   docsSuma,
                                   i,
                                   _nActualizaciones,
                                   procScore,
                                   _next,
									_timePre2, //time 0 es el tiempo de procesamiento
                                   maestro,
                                   _pid
                                   );
                //if(_pid==maestro)
				//{
		  		  //aca ir sumando el tiempo de preprocesamiento
		  		  diff_time(&_timePre2,&_timePre1,&_latencia);
				  tiempoPreQWD+=_latencia.tv_sec*1000000000+_latencia.tv_nsec;  		  		  		  
		  	   // }

          
          _UMBRAL=thresholTmp;
          nSeleccionadosTopK = nSelTmp;
          threshold = thresholTmp;  
         computoScore += procScore;
          
         _HEAP_UPDATES += _nActualizaciones;
          _DOCS_SUMA+=docsSuma;
          _NEXT+=_next;
		  _DOCS_PROCESADOS = computoScore;
          }//fin for recursivo   
             
     
    

    _HEAP_SIZE = nSeleccionadosTopK;

   
}
//CONSTRUIR PROGRAMA CORTO QUE SOLO CALCULE EL TIEMPO DE CADA TIOK PARA CADA TERTMINO DE LAS CONSULAST...






    //simulacion de tiempo deprocesamiento
	void funcionDormir()
	{    
		  
		  sleep(procesosTraza);
	}
	
	
  void *HeapLocalesTraza(void *arg)
  {
  datosQuerie *tmp;
  tmp = (datosQuerie*)arg;
  int i =  tmp->idQ; 
  int nT = tmp->nthreads;
  int tidLocal =tmp->tid;
  int ki =tmp->k;
 // float x;
  int tmpFile = tmp->p;
  int threadMaster = tmp->corePadre;
  int idQuerie = tmp->consulta->id_query;
  

  funcionDormir();
  
  
  
  
  
  
  
  
  
  
	unsigned long int tiempoWand_procQ1;
	unsigned long int tiempoWand_procQ2;
	unsigned long int tiempoWand_procQ3;
	unsigned long int tiempoWand_procQ4;
 // tmp->consulta->mostrar();
  			  unsigned long int _DOCS_PROCESADOS=0;
			  unsigned long int _HEAP_UPDATES=0;
			  //unsigned long int DOCS=0;
			  unsigned long int _DOCS_SUMA=0;
			  unsigned long int _NEXT=0;
			  unsigned long int _HEAP_SIZE=0;
			  float _UMBRAL=0.0;
			  struct timespec _latencia;
			  struct timespec _tiempo1_ProcQ,_tiempo2_ProcQ,_tiempo1_PreQ,_tiempo1_Wand,_tiempo1_MergeSort,_tiempo2_PreQ,_tiempo2_Wand,_tiempo2_MergeSort;	
  			  //struct timespec _times[8];
  			  vector<struct timespec> _times;
  			  //	 cout <<" INICIO LLAMADA WAND Tid: "<<tidLocal<<" Master: "<< threadMaster <<endl;
  			  //	cout <<" -------------- Q "<<i<<endl;
  			  if(tidLocal==threadMaster)		  
		      {clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_tiempo1_ProcQ);} //
		  
			  //WandQueryBaseLine(tmp->consulta,i,ki,tmp->umbral,tipoRank,_HEAP_SIZE,_DOCS_PROCESADOS,_HEAP_UPDATES,_DOCS_SUMA,_NEXT,_UMBRAL,threadMaster,tidLocal,_times,nT);
			//  WandMultihebraDinamica(tmp->consulta,i,ki,tmp->umbral,tipoRank,_HEAP_SIZE,_DOCS_PROCESADOS,_HEAP_UPDATES,_DOCS_SUMA,_NEXT,_UMBRAL,threadMaster,tidLocal,_times,nT);		
			 //			  	cout <<" ----ffffffffffffffff---------- Q "<<i<<endl;
			  if(tidLocal==threadMaster) 
			  {	
			   
		     	_tiempo1_PreQ =_times[0];
			  	_tiempo2_PreQ=_times[1];
			  	_tiempo1_Wand=_times[2];
			  	_tiempo2_Wand=_times[3];
			  	_tiempo1_MergeSort=_times[4];
			  	_tiempo2_MergeSort=_times[5];
			  	
			  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_tiempo2_ProcQ);
					
		if(termOrQueries==0)
		logWand[tmpFile]<<idQuerie<<" "<<_HEAP_SIZE<<" "<<_DOCS_SUMA<<" "<<_DOCS_PROCESADOS<<" "<<_NEXT<<" "<<_HEAP_UPDATES<<" "<<_UMBRAL;
		
		
		
		if(termOrQueries==1)
		logWand[tmpFile]<<tmp->consulta->terminos[0]<<" "<<_HEAP_SIZE<<" "<<_DOCS_SUMA<<" "<<_DOCS_PROCESADOS<<" "<<_NEXT<<" "<<_HEAP_UPDATES<<" "<<_UMBRAL;
		
	
					  

					  diff_time(&_tiempo2_ProcQ,&_tiempo1_ProcQ,&_latencia);
					  tiempoWand_procQ4=_latencia.tv_sec*1000000000+_latencia.tv_nsec;
					  logWand[tmpFile]<<" "<<tiempoWand_procQ4;        //Procesamiento Q

					  diff_time(&_tiempo2_PreQ,&_tiempo1_PreQ,&_latencia);            //Pre-procesamiento WAND
					  tiempoWand_procQ3=_latencia.tv_sec*1000000000+_latencia.tv_nsec;
					  logWand[tmpFile]<<" "<<tiempoWand_procQ3;

					  diff_time(&_tiempo2_Wand,&_tiempo1_Wand,&_latencia);
					  tiempoWand_procQ1=_latencia.tv_sec*1000000000+_latencia.tv_nsec;
					  logWand[tmpFile]<<" "<<tiempoWand_procQ1;      		   //Procesamiento WAND

					  diff_time(&_tiempo2_MergeSort,&_tiempo1_MergeSort,&_latencia);
					  tiempoWand_procQ2=_latencia.tv_sec*1000000000+_latencia.tv_nsec;
					  logWand[tmpFile]<<" "<<tiempoWand_procQ2;      		   //Procesamiento Merge
					  logWand[tmpFile]<<endl;
					   
					  
			}
  
//  cout <<"idTid: "<<tidLocal<<" querie: "<<i<<" numThreads: "<<nT<<" K: "<<ki<<endl;
  
  //if(nT>1)
// pthread_barrier_wait(&barrier);
  
 // delete tmp;
// cout <<"Paso--> "<<ki<<" Tid--> "<<tidLocal<<" q "<<i<<endl;
 pthread_exit(NULL);	
}





void abrirFileLogWand(int metodo,int tRank,int limIni,int limFin,int kDocs)
{
 char nbre[256];
 int potencia=kDocs;
	

//for(int j=0;j<4;j++)
//{ 	
	if(tRank==1)
	{
			if(metodo==1)
			sprintf(nbre,"/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_%d_%d_WandClueTrec09_K%d_TF_WAND_%d_Multithread_%dT_DinamicT%d.txt",limIni,limFin,potencia,SizeBloque,numThreadsWand,MAXARCH);
			if(metodo==2)
			sprintf(nbre,"/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_%d_%d_WandClueTrec09_K%d_TF_BMW_%d_Multithread_%dT_DinamicT%d.txt",limIni,limFin,potencia,SizeBloque,numThreadsWand,MAXARCH);
		
	}
	if(tRank==2)
	{
			if(metodo==1)
			{
			if(termOrQueries==0)	
			sprintf(nbre,"/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_%d_%d_WandClueTrec09_K%d_BM25_WAND_%d_Multithread_%dT_DinamicT%d.txt",limIni,limFin,potencia,SizeBloque,numThreadsWand,MAXARCH);
		    if(termOrQueries==1)	
			sprintf(nbre,"/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_%d_%d_WandClueTrec09_K%d_BM25_WAND_%d_Multithread_%dT_DinamicT%dTERMS.txt",limIni,limFin,potencia,SizeBloque,numThreadsWand,MAXARCH);
		
		
		
		    }
			if(metodo==2)
            {
			if(termOrQueries==0)	
			sprintf(nbre,"/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costoQueries%d_%d_Trec09_K%d_BM25BMW_%dMT_%dT_DT%d.txt",limIni,limFin,potencia,SizeBloque,numThreadsWand,MAXARCH);
		    if(termOrQueries==1)	
			sprintf(nbre,"/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/TERMS%d_%d_Trec09_K%d_BM25BMW_%dMT_%dT_DT%d.txt",limIni,limFin,potencia,SizeBloque,numThreadsWand,MAXARCH);
		    
		    }
	}
	if(tRank==3)
	{       if(metodo==1)
			sprintf(nbre,"/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_%d_%d_WandClueTrec09_K%d_TFIDF_WAND_%d_Multithread_%dT_DinamicT%d.txt",limIni,limFin,potencia,SizeBloque,numThreadsWand,MAXARCH);
			if(metodo==2)
			sprintf(nbre,"/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_%d_%d_WandClueTrec09_K%d_TFIDF_BMW_%d_Multithread_%dT_DinamicT%d.txt",limIni,limFin,potencia,SizeBloque,numThreadsWand,MAXARCH);
	}
	
	
	logWand[0].open(nbre);
//}





}

void cerrarFileLogWand()
{
logWand[0].close();
//logWand[1].close();
//logWand[2].close();
//logWand[3].close();	
}

void leerIndiceCharGrupos(const char *archivo, int opcionW, int tidThreadPadre,int numThreadsWand)
{
 //printf("Cargando archivo..\n");
 ifstream listaInvertida(archivo,ifstream::in);
    list< pair<unsigned int, unsigned int> > *lista[numThreadsWand];
    
     //   list< pair<unsigned int, unsigned int> > *lista2[numThreadsWand];
   // list< pair<unsigned int, unsigned int> > *lista;
   // list< pair<unsigned int, unsigned int> > *lista[n;
    string line;
	char *h;
	unsigned int posicion;
    int idDato,conteoDatos,posicionLista=0;
    

    char tmp;
	char buff[256];
	int i=0,numero,bandera=0;
    unsigned int idTerm=0;
    unsigned int idDoc=0,pesoDoc=0;//,numDocs=0;
    
    unsigned int conteoLista=0;
    unsigned int dimLista;
    //bLocal[i]=0;
           int nLinea=0,numDocs;

   // int UB_High;//, UB_Low;
    int contadorBloque;
    vector <float>  pesosDocumentos;
    
    vector <unsigned int> fronterasIdIniBlock;
    vector <unsigned int> fronterasIdFinBlock;
    vector <float> ubParticionesLocales;
    //vector <unsigned int> :: const_iterator UbTmp;
    float UbTmp,UB=0.0;
    unsigned int docFin;
    float w_term,idf_tmp;
    int numDocsGlobal = 0;
    for(int t=0;t< numThreadsWand;t++)
   lista[t]=new list< pair<unsigned int, unsigned int> >();
    
while(getline(listaInvertida,line))
{
nLinea++;
//cout<<" Lista: " <<nLinea<<endl;
   h = strdup(line.c_str());
   posicion=0;
   idDato=0;conteoDatos=0;posicionLista=0;
   numDocsGlobal=0;
   
   
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
		        UB=(float)numero;

		        }

		        if(idDato==2)
		        {
		        numDocs=numero;

		        }
            //datos 1 y 2 no se consideran ya que se van a calcular nuevamente UB y #docs, solo en version multithreading
		    if(idDato>2)
		    {
					if((conteoDatos%2)!=0)//frec
					{
						pesoDoc=numero;
						
						
						
						int posMod = idDoc%numThreadsWand;
						lista[posMod]->push_back(pair<unsigned int, unsigned int>(idDoc, pesoDoc)); //original
					
				//	lista2[posMod]->push_back(pair<unsigned int, unsigned int>(idDoc, pesoDoc)); //original
                        numDocsGlobal++;
                                              
                                                				//TF
			
						
                        //pesosDocumentos.push_back(w_term);
                        
                        
						//posicionLista++;
					}
					else//doc
					{
						
						idDoc=numero;
				//	pesos[posicionConsulta]=valor;
                                     


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
// cout<<i<<" idTerm"<<idTerm<<" sb"<<SizeBloque<<endl;


for(int t=0;t<numThreadsWand;t++)
{
	
	
	
	
	
	int sL = lista[t]->size();
	if(sL>0)
	{
    int cont=0;
	posicionLista=0;
	contadorBloque=0;
	float maxUb=0.0;
	
    //it=lista[t]->begin();
    float maxGlobal = 0.0;
    
     for (list< pair<unsigned int, unsigned int> >::iterator it=lista[t]->begin(); it!=lista[t]->end(); ++it){
	 //           for(int k=0; k<sL; k++){
                      unsigned int idDoc = it->first;
                      unsigned int pesoDoc = it->second;
 
                  
                        if(opcionW==1) 
						{
							 //TF
							 w_term = (float)pesoDoc;
					    }
						//BM25
						if(opcionW==2) 
						{
							 //IDF.BM25
						//	 w_term = BM25(pesoDoc,idDoc);
							  w_term = BM25_Particion(pesoDoc,idDoc,t);
						}
						//recuperar length doc
						//TF/sizeDoc
						if(opcionW==3)
						{
					//	if(sizeDoc(idDoc)==0)
					//	cout<<"Cero:"<<idDoc<<" Frec:"<<pesoDoc<<endl;
							w_term	= (float)(pesoDoc)/sizeDoc(idDoc);
						}
       
            
                       if(w_term>maxGlobal)
							 maxGlobal=w_term;
                       // maxGlobal
                        
                        if(cont==0)//primer idDoc
                        {
							
							//ORIGINAL
                            fronterasIdIniBlock.push_back(idDoc);
                            maxUb = w_term;
                            docFin = idDoc;
                            //		cout<<".. ini "<<idDoc;
                        }
                        
                        if(((cont%SizeBloque)==0)&&( cont>0))
						{	//cout<<".. docFin "<<docFin;
                            fronterasIdFinBlock.push_back(docFin);//idDoc primer doc bloque				                                                                                                        
							fronterasIdIniBlock.push_back(idDoc);//idDoc primer doc bloque
							//UbTmp = maxUb;
							//if(UbTmp==0.0)
							//cout<<".. ccc "<<UbTmp;
							//cout<<".. ini "<<idDoc;
								//int posModTraza = idDoc%numThreadsWand;
								//cout<<".. posModTraza "<<posModTraza<<endl;
								
							contadorBloque++;
							//pesosDocumentos.erase(pesosDocumentos.begin(),pesosDocumentos.end());
							//pesosDocumentos.resize(0);
							ubParticionesLocales.push_back(maxUb);
							
							maxUb=w_term;
						}
					    else if( cont>0)
                        {
							docFin = idDoc;
							 if(w_term>maxUb)
							 maxUb=w_term;
                        }
                          
                     
                        
                        if(sL==1)//ej: doc 56640585 con 8 threads
                        {
							docFin = idDoc;
							ubParticionesLocales.push_back(maxUb);
						}
						
						
						 cont++;
						 
						 idDoc = 0;
						 pesoDoc=0;
					}                                  
 	
   	
   	
   	
                            //bloque ultimo
if((cont-contadorBloque*SizeBloque)!=0)
{

								//UbTmp = *( std:: max_element(pesosDocumentos.begin(),pesosDocumentos.end()));
								//contadorBloque++;
							//maxUb=UbTmp;
                                     //   pesosDocumentos.erase(pesosDocumentos.begin(),pesosDocumentos.end());
                                    //    pesosDocumentos.resize(0);
                                    
                                    
                                        //ORIGINAL
										ubParticionesLocales.push_back(maxUb);
}
//ORIGINAL
fronterasIdFinBlock.push_back(docFin);
	
	
	 //pthread_mutex_lock( &lock );    
	//listaStruct[0].ubLocales[idTerm] = UB;
	
	//UB = *( std:: max_element(ubParticionesLocales.begin(),ubParticionesLocales.end()));
	//en la clase postingstream colocar punteros...

//ORIGINAL

	listaStruct[t].indiceThreadIntervalos[idTerm] = new PostingStream(lista[t],ubParticionesLocales,fronterasIdIniBlock,fronterasIdFinBlock,maxGlobal,numDocsGlobal,t);


	//pthread_mutex_unlock( &lock );
      cout<<" Term: "<<idTerm<<"  numDocs:"<<numDocs<<" numDocsGlobal:"<<numDocsGlobal<<" UB: "<<UB<<" MAX: "<<maxGlobal<< " UB_Bloque:"<<maxUb<<" size bloques: "<<ubParticionesLocales.size()<<""<<endl;
	  fronterasIdFinBlock.erase(fronterasIdFinBlock.begin(),fronterasIdFinBlock.end());
	  fronterasIdFinBlock.resize(0);
	  ubParticionesLocales.erase(ubParticionesLocales.begin(),ubParticionesLocales.end());
	  ubParticionesLocales.resize(0);
	  fronterasIdIniBlock.erase(fronterasIdIniBlock.begin(),fronterasIdIniBlock.end());
	  fronterasIdIniBlock.resize(0);
  
	}
}





for(int t=0;t<numThreadsWand;t++)
{


lista[t]->clear();
}
//delete* lista;

//mostrarVector(ubParticiones);
//mostrarVector(fronterasIdFronteras);
          }
          //delete lista;
          
		listaInvertida.close();
    //            exit(0);
}

vector<int> threadsLibresTids()
{
	vector<int> libres;
   for (int i=0;i<MAXARCH;i++)
	{
		if(activos[i]==0)
		libres.push_back(i);
		
	}	
	return libres;
	
}

int queriesActivas[8];

void asignaBanderaThreads(vector<int> entrada,int q)
{
	int s = entrada.size();
	int temp;
	//pthread_mutex_lock(&lock);
	pthread_rwlock_wrlock(&rw_rdlock);
	 for (int i=0;i<s;i++)
	{
		//if(activos[i]==0)
		//libres.push_back(i);
		temp = entrada[i];
		activos[temp] = 1;
		queriesActivas[temp]=q;
	}
	pthread_rwlock_unlock(&rw_rdlock);
	//pthread_mutex_unlock(&lock);
	
}

void  guardaThreads(int num,vector<int> &entrada,int q)
{
	//vector<int> v;
	int s = entrada.size();
	//cout << "S: "<<s<<endl;
	//entrada.clear();
	int temp;
	int cont = 0;
	 for (int i=0;i<MAXARCH;i++)
	{   pthread_mutex_lock(&lock);
		//pthread_rwlock_wrlock(&rw_rdlock);
		if(activos[i]==0 && cont<num)
		{   
			
	       
			activos[i] = 1;
			queriesActivas[i]=q;
			entrada.push_back(i);
		//	cout<<" :--->======================== "<< entrada[cont]<<endl;
			cont++;
	    }	
		//libres.push_back(i);
		//temp = entrada[i];
		//pthread_rwlock_unlock(&rw_rdlock);
		
		pthread_mutex_unlock(&lock);
		
	}
	
	
//	cout << "S2: "<<entrada.size()<<endl;
	//return v;
}

void liberarThread(int t)
{
//	pthread_mutex_lock(&lock);
	pthread_rwlock_wrlock(&rw_rdlock);
	activos[t] = 0;
	queriesActivas[t]=-1;
	pthread_rwlock_unlock(&rw_rdlock);
//	pthread_mutex_unlock(&lock);
}



void liberarBanderaThreads(vector<int> entrada)
{
	int s = entrada.size();
	int temp;
	//cout<<" SSSSS "<<s<<endl;
	pthread_rwlock_wrlock(&rw_rdlock);
	 for (int i=0;i<s;i++)
	{   
		//if(activos[i]==0)
		//libres.push_back(i);
		//pthread_mutex_lock(&lock);
		temp = entrada[i];
	//	cout<<"Libernado Q["<<q<<"] antes................... "<<temp<<" "<<activos[temp] <<endl;
		
		//if(temp<10)
		activos[temp] = 0;
		queriesActivas[temp]=-1;	
	//	cout<<"Libernado Q["<<q<<"] despues................. "<<temp<<" "<<activos[temp] <<endl;
		//pthread_mutex_unlock(&lock);
	}
pthread_rwlock_unlock(&rw_rdlock);
	
	
}
void mostrarListOrden(vector<int> entrada,int t,int q)
{
	int s = entrada.size();
	pthread_rwlock_rdlock(&rw_rdlock);
	
	// pthread_mutex_lock(&lock);
	 cout<<"ListOrdenT[ "<< t <<" ] Q["<<q<<"]";
	 for (int i=0;i<s;i++)
	{   
		  cout<<"-"<< entrada[i];
		 
     }
     cout<<endl;
     pthread_rwlock_unlock(&rw_rdlock);
    // pthread_mutex_unlock(&lock);
}

void mostrarBanderaThread()
{
	 //pthread_mutex_lock(&lock);
	 pthread_rwlock_rdlock(&rw_rdlock);
	
	 for (int i=0;i<MAXARCH;i++)
	 {
		 cout<<"Banderas threads T["<< i <<"]: "<< activos[i] <<" - "<<"Q["<<queriesActivas[i]<<"]"<<endl;
		 
     }
     pthread_rwlock_unlock(&rw_rdlock);
     //pthread_mutex_unlock(&lock);
}

void resetActivos()
{
	pthread_rwlock_wrlock(&rw_rdlock);
	
	 for (int i=0;i<MAXARCH;i++)
	 {
		 activos[i] = 0;
		 queriesActivas[i]=-1;	
	 }
	 pthread_rwlock_unlock(&rw_rdlock);
}

int conteoLibres()
{
	int conteo = 0;
//	pthread_mutex_lock(&lock);
	pthread_rwlock_rdlock(&rw_rdlock);
	for (int i=0;i<MAXARCH;i++)
	{
		
		if(activos[i]==0)
		conteo++;
		
	}		
	pthread_rwlock_unlock(&rw_rdlock);
	//pthread_mutex_unlock(&lock);
	return conteo;
	
}
void esperar()
{
	  for(;;)
	  {
      // Lock mutex and then wait for signal to relase mutex
      pthread_mutex_lock( &lock );

      // Wait while functionCount2() operates on count
      // mutex unlocked if condition varialbe in functionCount2() signaled.
      pthread_cond_wait( &condition_var, &count_mutex );
     // count++;
     // printf("Counter value functionCount1: %d\n",count);
      pthread_mutex_unlock( &lock );
      break;
      //if(count >= COUNT_DONE) 
      //return(NULL);

    }


	}



void resetActivo(int padre)
{
	pthread_rwlock_wrlock(&rw_rdlock);
	
	 
		 sumaPasoThread[padre]=0;

	 pthread_rwlock_unlock(&rw_rdlock);
}

int sumaActivos(int padre)
{   sumaPasoThread[padre]++;// = sumaPasoThread[padre] + 1;
    return sumaPasoThread[padre];
}


int pasadaQuerieInicio(int idQuerie)
{
consultasBanderaInicial[idQuerie]--;	
return consultasBanderaInicial[idQuerie];
}
int pasadaActualInicioQuerie(int idQuerie)
{	
return consultasBanderaInicial[idQuerie];
}



int pasadaActualQuerie(int idQuerie)
{	
return consultasBandera[idQuerie];
}


int pasadaQuerie(int idQuerie)
{
consultasBandera[idQuerie]--;	
return consultasBandera[idQuerie];
}

int obtenerActividad(int padre)
{
	return sumaPasoThread[padre];
}

void construirMerge(CBT*& heapGlobal)
{
	 
   
	
}
void construirMerge(CBT* heapGlobalGroup, int topK, int master,float &umbralSal)
{				 CBT* heapTraza = new CBT(topK);
				 float min_thr;
				 int nSeleccionadosTopK = 0;
				 //int actualizaciones = 0;
				 int nActualizaciones = 0;
				 float score;
				 unsigned int id_doc;
				//pthread_mutex_lock(&lock);
				 float _umbral = 0.0;
			 //  heapGlobal ,
			// cout<<"size entrada: "<< heapGlobalGroup->dimensionHeap()<<" "<<endl;
				for(int i=0;i<heapGlobalGroup->dimensionHeap();i++)
			    {
					score=heapGlobalGroup->Prioridades[i];
					id_doc=heapGlobalGroup->ID_docs[i];
					if(score > _umbral) 
					{
						
										if(nSeleccionadosTopK<topK)
										{
										//	cout<<"Insertando heap con:"<< score<<endl;
											heapTraza->insert(score,id_doc); 
											nSeleccionadosTopK++;
											nActualizaciones++; 
										}
										else{
												min_thr = heapTraza->top();
												if(min_thr<score){
												heapTraza->insert(score,id_doc);
												
										//		cout<<"Actualizando heap con:"<< score<<endl;
												nActualizaciones++;
													 }
											 }
											 
									   if(nSeleccionadosTopK<topK){
						_umbral=0.0;
						}
						else{
							
						_umbral = heapTraza->top();
						}
			    }
				
				
			    }
			    
			   umbralSal =  _umbral;
			    
			   // pthread_mutex_unlock(&lock);
			    
}


void agregarHeap(vector < pair<float, unsigned int> > & heapGlobalLocal,CBT*& heapLocal)
{
	//Creacion topk global
          int contador=1;
           //----------------------------
          //pthread_mutex_lock( &lock );
          pthread_rwlock_wrlock(&rw_rdlock);
          int dim = heapLocal->dimensionHeap();
                         if(dim>0)
                          while(contador<=dim)
                          {
                              heapGlobalLocal.push_back(pair<float, unsigned int>(heapLocal->Prioridades[contador],heapLocal->ID_docs[contador] ));
                              contador++;
                          }
          
          pthread_rwlock_unlock(&rw_rdlock);
          //pthread_mutex_unlock( &lock );    
             
         // pthread_barrier_wait(&barrier);
    
    
}

	int SetConcurrency(int concurrencyLevel) {
	
	    return pthread_setconcurrency(concurrencyLevel);

	}
	
	//----------------------------------------------------------------------------
	//
	// Description: Get the concurrency level
	//
	// Use static public
	//
	int GetConcurrency() {
	
	
	    return pthread_getconcurrency();
	
    
	}



void *schedulerThread(void *arg)
{	  struct timespec _tmp; 
	  clock_gettime(CLOCK_MONOTONIC, &_tmp);




  datosQuerie *tmp;
  tmp = (datosQuerie*)arg;
  int i =  tmp->idQ; 
  int nT = tmp->nthreads;
  int tidLocal =tmp->tid;
  int ki =tmp->k;
 // float x;
  int tmpFile = tmp->p;
  int threadMaster = tmp->corePadre;
  int idQuerie = tmp->consulta->id_query;
  
 // SetConcurrency(0);
  //cout<<" Concurrencia [" << tidLocal  << "]: " << GetConcurrency();	
  AsignaCPUThread(tidLocal);
 //int numT = 2;
 //vector<int> x = tmp->vBanderas ; 
                  		  
 //int _corePadre2 = tmp->corePadre;
 //int nT = tmp->nthreads;
 int tmp2;
 //int origen;
 //int libres = conteoLibres();
 
 //las sincronizaciones de las queries se miden entre estos bloques
 //aca se implementa la construccion del topk final realizando el merge e insert/update en el vector resultado heap final.
 
 //recordar futura idea de reordenamiento de las listas particionadas, y a partir de ellas, estudiar un posible patron de densidad de cada lista
 //y utilizar esta informacion para medir su distribucion de densidad y DFT	                  	
	// exit(1);
	
	
long int timepoLatenciaQuerie = 0;		
unsigned long int timepoLatenciaMerge = 0;		
unsigned long int _docs_suma = 0;	
unsigned long int _docs_procesados = 0;
unsigned long int _heapUpdate =0;
unsigned long int _heapSize = 0;
unsigned long int _next = 0;
float _umbral=0.0;
float _umbralGlobal = 0.0;	
CBT *heapLocal = new CBT(ki);
//CBT *heapM = new CBT(ki);
vector < pair<float, unsigned int> > heapGlobal;  	                  		 
  	unsigned long int tiempoWand_procQ1;
	unsigned long int tiempoWand_procQ2;
	unsigned long int tiempoWand_procQ3;
	unsigned long int tiempoWand_procQ4,_mergeLatencia;
	long int latenciaWAND,efectiveWAND,efectiveMerge;
	unsigned long int _tiempoPreQWD = 0;

	int paso =0;
	int pasoTreadcont;
 // tmp->consulta->mostrar();
    int tidWorking=-1;
    int banderaLocal = 0;
			  struct timespec _latencia;
			  struct timespec _tiempo1_ProcQ,_tiempo2_ProcQ,_tiempo1_PreQ,_tiempo1_Wand,_tiempo1_MergeSort,_tiempo2_PreQ,_tiempo2_Wand,_tiempo2_MergeSort;	
  			  //struct timespec _times[8];
  			  vector<struct timespec> _times;
		 struct timespec _tiempo1_MergeSortLatencia;
		 struct timespec _tiempo2_MergeSortLatencia;
		  struct timespec _tiempoBROKER;
int d;
		 	              //    pthread_mutex_lock(&lock);
		 	                 pthread_rwlock_wrlock(&rw_rdlock);
		 	                  d = pasadaActualInicioQuerie(idQuerie);
		 	               //   pthread_rwlock_wrlock(&rw_rdlock);
  	                  	//	  origen =   banderas[threadMaster];
  	                  		//  banderas[threadMaster]= banderas[threadMaster]+1;
  	                  		//  cout<<"ki: "<<ki<<" Libres: "<<libres<<" banderas[threadMaster]: "<<origen<<" banderas[threadMaster]+1: "<<banderas[threadMaster]<<" nT: "<<nT<<" threadMaster: "<<threadMaster<<" tidLocal: "<<tidLocal<<" Querie: ["<<idQuerie<<"]"<<endl;
  	                  		 // tmp2=banderas[threadMaster];
  	                  		  
  	                  		 if(d == nT)
  	                         {  
								//   cout<<"TOMANDO TIEMPO"<<ki<<" Libres: "<<libres<<" banderas[threadMaster]: "<<origen<<" banderas[threadMaster]+1: "<<banderas[threadMaster]<<" nT: "<<nT<<" threadMaster: "<<threadMaster<<" tidLocal: "<<tidLocal<<" Querie: ["<<idQuerie<<"]"<<endl;
  	                  	
								 
								 // pthread_mutex_lock(&lock);
								  
								//  
								  _timesLatencia[threadMaster]=0;
								 _timesQ[threadMaster] = _tmp;
								 //pthread_mutex_unlock(&lock);
								// 
				//				  		clock_gettime(CLOCK_MONOTONIC, &_tiempo1_MergeSort);  			
				//			 diff_time(&_tiempo1_MergeSort,&_tmp,&_latencia);
				//	  tiempoWand_procQ4=_latencia.tv_sec*1000000000+_latencia.tv_nsec;
				//	  cout<<"["<< idQuerie <<"] "<<tiempoWand_procQ4;        //Procesamiento Q
							 }
  	                         
							 //logWand[2]<<idQuerie<<endl;
				              pasadaQuerieInicio(idQuerie);
				              
				              
				              
  	                  		 // pthread_mutex_unlock(&lock);
		 pthread_rwlock_unlock(&rw_rdlock);
		                      // pthread_rwlock_unlock(&rw_rdlock);
		 
		 
							//tiempo procesamiento query y la medicion la controla el maestro
					

		 
		 
		  //    mostrarListOrden(tmp->listOrden,tidLocal,idQuerie);

			   clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_tiempo1_Wand);
		      WandMultihebraDinamica(tmp->consulta,i,ki,tmp->umbral,tipoRank,_heapSize,_docs_procesados,_heapUpdate,_docs_suma,_next,_umbral,threadMaster,tidLocal,_tiempoPreQWD,nT,tmp->listOrden,heapLocal);		
     		  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_tiempo2_Wand);
			  
	if(tidLocal==0)
	{		  

			}  
  	                  		  //    
  	                  		 //  exit(1);
  	                  		 //esto sirve para saber que termino la querie, cada thread antes de terminar 
  	                  		 
  	                  		 //aca corregir tiempo incial merege
  	                  		 
  	                  		/* pthread_mutex_lock(&lock);
  	                  		 if(_timesLatencia[threadMaster]<=0)
  	                  		 {
								 clock_gettime(CLOCK_MONOTONIC, &_tmp);
								  
								//   pthread_rwlock_wrlock(&rw_rdlock);
								// _timesMERGE[threadMaster] = _tmp;	
								 _timesMERGELatencia[threadMaster] = _tmp;
								//	 pthread_rwlock_unlock(&rw_rdlock);				 
								  _timesLatencia[threadMaster]=1;
								 
							 }
							 pthread_mutex_unlock(&lock);
							 */
							 
							
		     		    
							 //pthread_mutex_lock(&lock);
							// pasoTreadcont=sumaActivos(threadMaster);
					
		
					clock_gettime(CLOCK_MONOTONIC, &_tiempo1_MergeSort);      
					pthread_rwlock_wrlock(&rw_rdlock);
				   if( d == nT )
				   {   
					   //clock_gettime(CLOCK_MONOTONIC, &_tmp);
					   _timesMERGELatencia[threadMaster] = _tiempo1_MergeSort;	
								  //clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_tiempo1_MergeSort);
								 
				   }
				   
				   
				   
					
					d =  pasadaQuerie(idQuerie);
					pthread_rwlock_unlock(&rw_rdlock);
				
					
					
					
					//ACA MERGE
					
		//			construirMerge(heapLocal,ki,threadMaster,_umbralGlobal);
					
					//Fin merge
					clock_gettime(CLOCK_MONOTONIC, &_tiempo2_MergeSort);
					
 
					
 	                  		 if(d == 0)
							 {
								 
								 paso = 1;
								  tidWorking = tidLocal;
								 

										//clock_gettime(CLOCK_MONOTONIC, &_tiempo2_MergeSort);
									    clock_gettime(CLOCK_MONOTONIC, &_tiempo2_ProcQ);//time procces
									    //clock_gettime(CLOCK_MONOTONIC, &_tiempo2_MergeSortLatencia);
	pthread_rwlock_rdlock(&rw_rdlock);
							_tiempo1_MergeSortLatencia	= _timesMERGELatencia[threadMaster];	
							_tiempo1_ProcQ=_timesQ[threadMaster];//cuando parte el primero que llego
							_tiempoBROKER=_timesIniQBroker[threadMaster] ;
	pthread_rwlock_unlock(&rw_rdlock);						
							
							
							_tiempo2_MergeSortLatencia=_tiempo2_ProcQ;
							
						
							
		pthread_rwlock_wrlock(&rw_rdlock);					

	/*						
	    //pthread_mutex_lock(&lock);
		if(termOrQueries==0)
		logWand[tmpFile]<<idQuerie<<" "<<_heapSize<<" "<<_docs_suma<<" "<<_docs_procesados<<" "<<_next<<" "<<_heapUpdate<<" "<<_umbral;
		
		
		if(termOrQueries==1)
		logWand[tmpFile]<<tmp->consulta->terminos[0]<<" "<<_heapSize<<" "<<_docs_suma<<" "<<_docs_procesados<<" "<<_next<<" "<<_heapUpdate<<" "<<_umbral;
		
		
	
	
					  diff_time(&_tiempo2_ProcQ,&_tiempo1_ProcQ,&_latencia);
					  tiempoWand_procQ4=_latencia.tv_sec*1000000000+_latencia.tv_nsec;
					  logWand[tmpFile]<<" "<<tiempoWand_procQ4;        //Procesamiento Q

					  //diff_time(&_tiempo2_PreQ,&_tiempo1_PreQ,&_latencia);            //Pre-procesamiento WAND
					  //tiempoWand_procQ3=_latencia.tv_sec*1000000000+_latencia.tv_nsec;
					  logWand[tmpFile]<<" "<<_tiempoPreQWD;

					  diff_time(&_tiempo2_Wand,&_tiempo1_Wand,&_latencia);
					  tiempoWand_procQ1=_latencia.tv_sec*1000000000+_latencia.tv_nsec;				
				
					  logWand[tmpFile]<<" "<<tiempoWand_procQ1;      		   //Procesamiento WAND

					  diff_time(&_tiempo2_MergeSort,&_tiempo1_MergeSort,&_latencia);
					  tiempoWand_procQ2=_latencia.tv_sec*1000000000+_latencia.tv_nsec;
					
					
					
					  logWand[tmpFile]<<" "<<tiempoWand_procQ2;      		   //Procesamiento Merge
					  
					  
					  
					   	
					   diff_time(&_tiempo2_MergeSortLatencia,&_tiempo1_MergeSortLatencia,&_latencia);
					  _mergeLatencia=_latencia.tv_sec*1000000000+_latencia.tv_nsec;
					  
					//  _mergeLatencia = _mergeLatencia - tiempoWand_procQ2;
					  
					  logWand[tmpFile]<<" "<<_mergeLatencia; 
					  
					  
					//  efectiveWAND = (long int)tiempoWand_procQ1 - (long int)_tiempoPreQWD -;
					  //latenciaWAND =(long int) tiempoWand_procQ4;
					 // timepoLatenciaMerge = (long int)_mergeLatencia - (long int)tiempoWand_procQ2;//Proc Q - Tiempo total entre el primer y utlimo thread al realizar merge
					 // timepoLatenciaQuerie =(long int)latenciaWAND + (long int)timepoLatenciaMerge;
					 // cout<<"-------------"<<_tiempoPreQWD<<endl;
					  
					//  logWand[tmpFile]<<" "<<latenciaWAND<<" "<<timepoLatenciaMerge <<" "<< timepoLatenciaQuerie;
					  
					//   logWand[tmpFile]<<" "<<efectiveWAND;
					 	
				
					  diff_time(&_tiempo2_ProcQ,&_tiempoBROKER,&_latencia);
					  tiempoWand_procQ1=_latencia.tv_sec*1000000000+_latencia.tv_nsec;
					  
					  logWand[tmpFile]<<" "<<tiempoWand_procQ1; 
					  logWand[tmpFile]<<" "<<nT<<endl;    
					  pthread_rwlock_unlock(&rw_rdlock);
 */	
					  			}
					  		//	pthread_mutex_unlock(&lock);
					  		//	pthread_rwlock_wrlock(&rw_rdlock);
		     		   
		     		   
		    	    
		     		    
		     		    
					  			if(d == 0)
					            {
									//pthread_rwlock_wrlock(&rw_rdlock);
								//	liberarBanderaThreads(tmp->vBanderas); //-----------------actual
									// banderas[threadMaster]=0;
									//pthread_rwlock_unlock(&rw_rdlock);
									if(tidLocal == threadMaster)
									{
										liberarThread(tidLocal);
									}
									else
									{
									liberarThread(tidLocal);	
									liberarThread(threadMaster);
									}
								}
								else if(tidLocal !=threadMaster)
								{
									liberarThread(tidLocal);
								}

	  
							delete heapLocal;	 


	pthread_exit(NULL);	
}



void distribucionListas(vector<int> & _v, vector<int> _Lin,int _nThreads)
{
	int s = _Lin.size();
	int factor = MAXARCH/s;
	int c=0;
	for(int j=0;j<s;j++)
	{
		for(int i=0;i<factor;i++)
		{ 
		//	cout<<" "<<_Lin[j]<<endl;
			_v.push_back(_Lin[j]);	
		}
	}
	
}
map<int,float> predictor;
void prediccion()
{
 std::ifstream ifs ("./t.txt", std::ifstream::in);

    string linea,dato;
    int q;
    int contador = 0;
    float tiempoQ;
    
	while(getline(ifs,linea)){	
		stringstream st(linea);
		while(st.good()){	
			st>>dato;			
			if(contador==0)
			{	q=atoi(dato.c_str());
			//	cout <<" "<<q;
			}
			else{
					
				tiempoQ=atof(dato.c_str());
			//	cout <<" "<<tiempoQ<<endl;
				predictor[q]=tiempoQ;
			}	
			
			contador++;		
		}
		contador = 0;
	}
	
}

int asignaThreadsSheduler(int q)
{
	
	if(predictor[q] < 300000000.0)
	return 1;
	if(predictor[q] >= 300000000 && predictor[q] <= 600000000.0 )
	return 2;
	if(predictor[q] > 600000000.0)
	return 4;
	
	
	return 1;
}
int asignaThreadsShedulerFloat(int q)
{
	
	if(predictor[q] < 0.2)
	return 1;
	if(predictor[q] >= 0.2&& predictor[q] <= 0.4 )
	return 2;
	if(predictor[q] > 0.4)
	return 4;
	
	
	return 1;
}
int main (int argc, char *argv[]) {

AsignaCPUThread(7);
prediccion();
//exit(1);

nombreArchivoConsultas = argv[1];//file queries
numeroConsultasProceso = atoi(argv[2]);//N de consultas a procesar
querierForJumpFree = atoi(argv[3]);//file topK
opcionTraza = atoi(argv[4]);// 0-->#interesecciones, 1-->topk in, 2-->topk automatico 10,100,10000,100000
tipoMetodo = atoi(argv[5]);//1-->baseline, 2-->BMW
tipoRank = atoi(argv[6]);//1-->TF,2-->BM25,3-->TF-IDF
SizeBloque = atoi( argv[7] );//tamanio bloques
particionIni = atoi( argv[8] );//ini queries
particionFin = atoi( argv[9] );//fin queries
procId =  atoi(argv[10]); // procesador 
numThreadsWand = atoi(argv[11]); //num threads
MAXARCH = atoi(argv[12]);;//1-8
termOrQueries = atoi(argv[13]);//1-8
procesosTraza =  atoi(argv[14]);
int kDocs =  atoi(argv[15]);
char  *nombreArchivoConsultas2 = argv[16];
threadsLibres = MAXARCH;
 ofstream logQ;

 logQ.open(nombreArchivoConsultas2);

map<int, unsigned long int > tiemposPartidaQuerie;


	int PAPI_events[] = {
		PAPI_TOT_CYC,
		PAPI_L2_DCM,
		PAPI_L2_DCA,
		PAPI_L3_TCM };
		long long counters[4];


PAPI_library_init(PAPI_VER_CURRENT);


//AsignaCPUThread(p);
//busquedaAND = atoi(argv[7]);
if((tipoRank!=1)&&((tipoRank==2)||(tipoRank==3)))
//cargaDocs();// 2.3 Gbytes Ram - Verificar ---------------------------------------------------

//cargaDocsParticiones();

int ddd =10;


pthread_mutex_init(&lock, NULL);
                      
//cargaDocsMultithread(MAXARCH);
printf("...Cargando Consultas.  xxx ..\n");
//leerConsultasCharJump(nombreArchivoConsultas,&querys,numeroConsultasProceso,querierForJumpFree);
leerConsultasCharJumpBlocksQueries(nombreArchivoConsultas,&querys,numeroConsultasProceso,querierForJumpFree,particionIni,particionFin);
printf("...Fin carga Consultas...\n");
//mostrarTermsLotes();
abrirFileLogWand(tipoMetodo,tipoRank,particionIni,particionFin,kDocs);
//pFileTopk = fopen("./trazaTopKBM25","w");
resetActivos();
vector<int> asigThreads;
vector<int> pruebaV;
int a = 1;
//consultasBandera[0]=a;
vector<unsigned long int> tiempoLoteQueries;
for(int i=0;i<numeroConsultasProceso;i++)
{    
	a = a*2;
	if(a==8)
	a=1;
	
a=numThreadsWand;
	
	
	
		//a = asignaThreadsSheduler(i);
	
	
	//cout<<a<<" ";
	asigThreads.push_back(a);
	consultasBandera[i]=a;
	consultasBanderaInicial[i]=a;
}
//exit(1);
int ini,fin;
float threshold = 0.0; //default 0.0

for(int i=0;i<MAXARCH;i++)
{
	activos[i] = 0;
    banderas[i] = 0;
    sumaPasoThread[i] = 0;
}
int idQuery;
esperando = false;
int _tidPadre = 0;
	pthread_attr_t attr1;
	datosQuerie dataQ[MAXARCH]; 
    vector<int> _vBanderas;
    vector<int> _vListas;
	pthread_mutex_init(&lock,NULL);
    
    pthread_t mythread[8]; 
                   pthread_attr_t tattr4;
                   
				   int ret;
                   ret = pthread_attr_init(&tattr4);
					/* set the thread detach state */
				   ret = pthread_attr_setdetachstate(&tattr4,PTHREAD_CREATE_DETACHED);
				   pthread_mutex_init(&lock, NULL);
				   
				   pthread_rwlock_init(&rw_rdlock, NULL);
                   //checkResults("pthread_rwlock_init()\n", rc);
	for(int m=0;m<contSetQueries;m++)
	{   printf("Lote ---------> %d.\n",m);

	//carga de conjunto de listas
    //    listaStruct[0].cargarConjuntoListas(m,tipoMetodo,tipoRank); //--------------------------------------------
		
	//el llenado es fijo para MAX num threads
	//ORIGINAL	---------------------
	listaStruct[0].cargarConjuntoListasBMWMultithread(m,tipoMetodo,tipoRank,_tidPadre,MAXARCH);	
		
		//call wHeapLocalesand conjunto queries
		//listaStruct[t].tid=t;
	//listaStruct[t].nthreads=numThreads;
	//listaStruct[0].consultas = querys;datsch

	//listaStruct[0].umbralInicial=0;
	printf("--------- INICIO Llamada WAND Lote ---------> %d.\n",m);
	cout<<"--ini----"<<punterosIniciales[m]<<endl;
	cout<<"---fin---"<<punterosFinales[m]<<endl;
	
    ini = punterosIniciales[m];
    fin = punterosFinales[m];
    
    //si llegaron todas las anteriores o bien si se usa una barrera.
    //esto es util, en caso contrario borra informacion que requeirren los demas threads
    // resetActivos();
    

				  // pthread_cond_init(&condition_var, 0);
				   //pthread_cond_init(&llegaron, NULL);
				//   pthread_barrier_init(&barrier, &attr, MAXARCH);
    contQueries = 0;
	//callWand(0.0,punterosIniciales[m],punterosFinales[m]);
	int tmp = 0;// log10(kDocs) - 1;
	clock_gettime(CLOCK_MONOTONIC, &tiempo1_GlobalQLote);

	
	
		int I = PAPI_start_counters( PAPI_events, 4 );
	
	for( int i=ini;i<=fin;i++)  
	//for( int i=ini;i<=fin;i++)  
	{
		contQueries++;
  //    cout<<".. Q["<<querys[i]->id_query<<"] .. i["<<i<<"]"<<endl;
      DOCS_PROCESADOS=0;
      HEAP_UPDATES=0;
      DOCS=0;
      DOCS_SUMA=0;
      NEXT=0;
      UMBRAL=(float)threshold;
    
    
   // cout<<".. Q["<<querys[i]->id_query<<"] .. i["<<i<<"]"<<endl;
      
    
    
      //Numero docs Interseccion
      if(opcionTraza==0)
      {
		  thersholdLogConsulta<<i;
    	  dimDocsQuerie(querys[i]);
    	  thersholdLogConsulta<<" "<<DOCS_SUMA<<" "<<DOCS<<" "<<querys[i]->n_terminos;
    	  dimDocsQuerieSize(querys[i]);
      }
      else if(opcionTraza>0)
      {
			  // Wand 10,100,1000,10000
			  if(opcionTraza==2)
			  {    
				   
				   struct timespec _latenciaQ0;
				   struct timespec _latenciaQ0T;
			       struct timespec _tiempo1_ProcQ0,_tiempo2_ProcQ0;
			       struct timespec _tiempo1_ProcQ0T,_tiempo2_ProcQ0T;
			        struct timespec time0,time1;
				   unsigned long int tiempoWand_procQ0,tiempoWand_procQ0T,timeQ;
				  clock_gettime(CLOCK_MONOTONIC, &time0);
				  idQuery=querys[i]->id_query; 
				  diff_time(&time0,&tiempo1_GlobalQLote,&latenciaQ);
                  timeQ=latenciaQ.tv_sec*1000000000+latenciaQ.tv_nsec;
    
				  tiemposPartidaQuerie[idQuery]=timeQ;
				   //ACA CAMBIO SOLO POR TRAZA DE CONTROL DE PASO DE THREADS
				//  for(int p=10;p<=10000; p=p*10)
				//  {
				 	//  kDocs=p;
				 //// }  
                   
                  
                  
                   
            //       clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_tiempo1_ProcQ0T);
                   //aca el predictor y asignar numthreads....
                   int coreAsign =  asigThreads[i];
               //    cout<<".. Q["<<querys[i]->id_query<<"] .. i["<<i<<"]"<<" coreAsign: "<<coreAsign<<endl;
               //    mostrarBanderaThread();
                   //cout<<coreAsign;
                   int _corePadre;
                

//pthread_attr_setdetachstate(&tattr4, PTHREAD_CREATE_JOINABLE);
               //    datosQuerie *dataQ = new datosQuerie(coreAsign,querys[i]->id_query,kDocs);
              // cout<<".. Qaaa["<<querys[i]->id_query<<"] .. i["<<i<<"]"<<endl;
               
                      int libres = conteoLibres(); 
                    //  cout<<"libres: "<<libres<<endl;
                 			           
				           bool target=NULL;
				         //  while(!target)
				         //  {
				       //   cout<<"--------Consulta ---: "<<i<<endl;
				          int contTime = 0;
				         // mostrarBanderaThread();
									  while(libres<coreAsign &&(i<=fin) )
									  {
											//if(i==fin)
											//break;
											//if()
											 
										
										  //  if(contQueries%100 == 0)
										  //  sleep(5);
											
											sleep(0.001);
											libres=conteoLibres();
											contTime++;
											if(contTime%100000==0)
											{ 
											//	 cout<<"---Consulta ---: "<<i<<endl;
												 // cout<<"Disminuyendo  ... "<<conteoLibres()<<" "<< contTime <<endl;
											//	if(coreAsign!=1)
											//	coreAsign = coreAsign/2;
											//		cout<<"Otras ... "<<conteoLibres()<<" "<< contTime <<endl;
											//		cout<<".. Q["<<querys[i]->id_query<<endl;
												
											//	break;
											}
											
											//break;CLOCK_MONOTONIC
											//cout<<"aaaaaPasada..."<<	endl;
										  
										}
				       //  mostrarBanderaThread();
				       //  idQuery=querys[i]->id_query;         
				      //   cout << i <<" idQuery" << idQuery << " coreAsign: "<<coreAsign<<endl;
								   if(libres>=coreAsign)
								   {
									
									//		  
								//	cout <<i<< " idQuery" << idQuery << " coreAsign: "<<coreAsign<<endl;
									guardaThreads(coreAsign,_vBanderas,querys[i]->id_query);
									//mostrarBanderaThread();
									
									//
									int v; 
									//pthread_mutex_lock(&lock);
									pthread_rwlock_wrlock(&rw_rdlock);
									distribucionListas(_vListas,_vBanderas,coreAsign);
									pthread_rwlock_unlock(&rw_rdlock);
									//pthread_mutex_unlock(&lock);	
									//revisar uso de pthread_rwlock_rdlock para lecturas de listas invertidas compartidas...
				//			unsigned long int timeQQQ;
							
	//						struct timespec tiempoQueries001,tiempoQueries002;
    //clock_gettime(CLOCK_MONOTONIC, &tiempoQueries001);
    

									//exit(1);
									for(int t=0;t<coreAsign;t++)
									{   v = _vBanderas[t];
										
										// CBT_WandDinamic[v] = new CBT(kDocs);
										
										if(t==0)
										{ // pthread_rwlock_wrlock(&rw_rdlock);
											_timesLatencia[v]=-1;
											sumaPasoThread[v]=0;
										//	pthread_rwlock_unlock(&rw_rdlock);
											_corePadre=v;
										//cout<<" _corePadre "<<_corePadre<<endl;
										}
									//	cout<<"------------------------ v "<< v<<" i"<<i<<endl;
										dataQ[v].consulta=querys[i];
								     	dataQ[v].umbral=threshold;
										dataQ[v].idQ = i;
										dataQ[v].k = kDocs;
										dataQ[v].tid=v;
										dataQ[v].nthreads=coreAsign;
										dataQ[v].corePadre=_corePadre;
										dataQ[v].vBanderas=_vBanderas;
										dataQ[v].p=tmp;
										dataQ[v].listOrden=_vListas;
										
										 if(t==0)
										 clock_gettime(CLOCK_MONOTONIC, &_timesIniQBroker[_corePadre]);
										 
										 //  	PAPI_read_counters( counters, 3 );
 
										 
										//mostrarListOrden(_vListas,v,i);
										int rc =  pthread_create(&mythread[v], &tattr4,schedulerThread,(void *)&dataQ[v]);
										   if (rc)
											{
											  // if (s != 0)
											  handle_error_en(rc, "pthread_create");	
											  printf("ERROR; return code from pthread_create() is %d\n", rc);
											  abort();//exit(-1);
											}
											
										
									   }
									   
									   
	
										
	//clock_gettime(CLOCK_MONOTONIC, &tiempoQueries002);								       
   // diff_time(&tiempoQueries002,&tiempoQueries001,&latenciaQ);
    //timeQQQ=latenciaQ.tv_sec*1000000000+latenciaQ.tv_nsec;
	// cout << " ["<< i <<"] " << timeQQQ << endl;								   
									   
		
										_vBanderas.erase(_vBanderas.begin(),_vBanderas.end());
										_vBanderas.resize(0);  
										_vListas.erase(_vListas.begin(),_vListas.end());
										_vListas.resize(0); 
										
									
								}
	
	
//	tmp++;
//}
			  } 
			  particionIni++;
			 //  cout << "particionIni..." << particionIni << endl;
	  }
      else
      {
		  cout << "Opcion no reconocida..." << opcionTraza << endl;
		  exit(0);
		  
	  }
        
	}//fin for

   int deathing=conteoLibres();
   while(deathing<MAXARCH) 
   {
	   sleep(0.001);
	   deathing=conteoLibres();
	 //  cout<<deathing<<endl;
	   
	   
   }
   PAPI_read_counters( counters, 3 );
   printf("%lld L2 cache misses (%.3lf%% misses) in %lld cycles  L3:%lld\n", 
		counters[1],
		(double)counters[1] / (double)counters[2],
		counters[0],counters[2] );	
    unsigned long int tiempoQueries;
    clock_gettime(CLOCK_MONOTONIC, &tiempo2_GlobalQLote);
    diff_time(&tiempo2_GlobalQLote,&tiempo1_GlobalQLote,&latenciaQ);
    tiempoQueries=latenciaQ.tv_sec*1000000000+latenciaQ.tv_nsec;
    
    tiempoLoteQueries.push_back(tiempoQueries);
//mostrarBanderaThread();

 // pthread_mutex_destroy(&lock);
 //pthread_cond_destroy(&llegaron);
 //pthread_barrier_destroy(&barrier);
	
	printf("---------- FIN Llamada WAND Lote ---------> %d.\n",m);
	//pthread_mutex_destroy(&lock);
	
	//------------------------------------------------------------------------------------------------------------------
		 if(tipoMetodo==1)
		 listaStruct[0].liberarRAM_P();
		 
		 
		 if(tipoMetodo==2)
		 {
			
			for(int t=0;t<MAXARCH;t++)
			{
			    listaStruct[t].liberarRAM_BlocksTodo();
				//listaStruct[t].liberarRAM_Blocks();
			}
	     }
		 
		 
	}

//procesingQueries(tipoMetodo,tipoRank);



 
 for (std::map<int ,unsigned long int>::iterator it=tiemposPartidaQuerie.begin(); it!=tiemposPartidaQuerie.end(); ++it)
 //   std::cout << it->first << " => " << it->second << '\n';
 logQ<< it->first << " " << it->second<<endl;
 
 /*
 for(int i=0;i<tiempoLoteQueries.size();i++)
 {
	 logQ<<"DimLote: "<< querierForJumpFree << "Tiempo Nanos: "<< tiempoLoteQueries[i]<<endl;
	 
 }
 */
logQ.close();
//mostrarTermsLotes();
cerrarFileLogWand();
//fflush(pFileTopk);
//fclose(pFileTopk);
    pthread_exit( NULL );
    return (EXIT_SUCCESS);
}

//REVISAR PORQUE SE DEMORA TANTO CON BM25, OBSERVAR SI EXISTE ALGUN CALCULO U/O PARAMETRO QUE NO MINIMIZA EN NUMERO DE SCORE REALIZADOS
void *schedulerThreadRes(void *arg)
{
	

	datosQuerie *tmp;
  tmp = (datosQuerie*)arg;
  int i =  tmp->idQ; 
  int nT = tmp->nthreads;
  int tidLocal =tmp->tid;
  int ki =tmp->k;
 // float x;
  int tmpFile = tmp->p;
  int threadMaster = tmp->corePadre;
  int idQuerie = tmp->consulta->id_query;
 	
  
 //int numT = 2;
 vector<int> x = tmp->vBanderas ; 
                  		  
 int _corePadre2 = tmp->corePadre;
 //int nT = tmp->nthreads;
 int tmp2;
 int origen;
 int libres = conteoLibres();
 
 //las sincronizaciones de las queries se miden entre estos bloques
 //aca se implementa la construccion del topk final realizando el merge e insert/update en el vector resultado heap final.
 
 //recordar futura idea de reordenamiento de las listas particionadas, y a partir de ellas, estudiar un posible patron de densidad de cada lista
 //y utilizar esta informacion para medir su distribucion de densidad y DFT	                  	
	// exit(1);
	
	
long int timepoLatenciaQuerie = 0;		
unsigned long int timepoLatenciaMerge = 0;		
unsigned long int _docs_suma = 0;	
unsigned long int _docs_procesados = 0;
unsigned long int _heapUpdate =0;
unsigned long int _heapSize = 0;
unsigned long int _next = 0;
float _umbral=0.0;
float _umbralGlobal = 0.0;	
CBT *heapLocal = new CBT(ki);
//CBT *heapM = new CBT(ki);
vector < pair<float, unsigned int> > heapGlobal;  	                  		 
  	unsigned long int tiempoWand_procQ1;
	unsigned long int tiempoWand_procQ2;
	unsigned long int tiempoWand_procQ3;
	unsigned long int tiempoWand_procQ4,_mergeLatencia;
	unsigned long int _tiempoPreQWD = 0;
	struct timespec _tmp; 
	int paso =0;
	int pasoTreadcont;
 // tmp->consulta->mostrar();
    int tidWorking=-1;
			  struct timespec _latencia;
			  struct timespec _tiempo1_ProcQ,_tiempo2_ProcQ,_tiempo1_PreQ,_tiempo1_Wand,_tiempo1_MergeSort,_tiempo2_PreQ,_tiempo2_Wand,_tiempo2_MergeSort;	
  			  //struct timespec _times[8];
  			  vector<struct timespec> _times;
		 
		 	                  pthread_mutex_lock(&lock);
		 	                  
		 	                  
		 	               //   pthread_rwlock_wrlock(&rw_rdlock);
  	                  		  origen =   banderas[threadMaster];
  	                  		  banderas[threadMaster]= banderas[threadMaster]+1;
  	                  		  cout<<"ki: "<<ki<<" Libres: "<<libres<<" banderas[threadMaster]: "<<origen<<" banderas[threadMaster]+1: "<<banderas[threadMaster]<<" nT: "<<nT<<" threadMaster: "<<threadMaster<<" tidLocal: "<<tidLocal<<" Querie: ["<<idQuerie<<"]"<<endl;
  	                  		  tmp2=banderas[threadMaster];
  	                  		  
  	                  		 if(tmp2==1)
  	                         {  
							//	   cout<<"TOMANDO TIEMPO"<<ki<<" Libres: "<<libres<<" banderas[threadMaster]: "<<origen<<" banderas[threadMaster]+1: "<<banderas[threadMaster]<<" nT: "<<nT<<" threadMaster: "<<threadMaster<<" tidLocal: "<<tidLocal<<" Querie: ["<<idQuerie<<"]"<<endl;
  	                  	
								  clock_gettime(CLOCK_MONOTONIC, &_tmp);
								 // pthread_mutex_lock(&lock);
								  
								//  pthread_rwlock_wrlock(&rw_rdlock);
								  _timesLatencia[threadMaster]=0;
								 _timesQ[threadMaster] = _tmp;
								 //pthread_mutex_unlock(&lock);
								// pthread_rwlock_unlock(&rw_rdlock);
								  
							 }
  	                  		  
  	                  		  pthread_mutex_unlock(&lock);
		 
		                      // pthread_rwlock_unlock(&rw_rdlock);
		 
		 
							//tiempo procesamiento query y la medicion la controla el maestro
							
							
		     		    
							
		 
		 
		  //    mostrarListOrden(tmp->listOrden,tidLocal,idQuerie);

			   clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_tiempo1_Wand);
		      WandMultihebraDinamica(tmp->consulta,i,ki,tmp->umbral,tipoRank,_heapSize,_docs_procesados,_heapUpdate,_docs_suma,_next,_umbral,threadMaster,tidLocal,_tiempoPreQWD,nT,tmp->listOrden,heapLocal);		
     		  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_tiempo2_Wand);
			  
  
  	                  		
  	                  		   
  	                  		  //    
  	                  		 //  exit(1);
  	                  		 //esto sirve para saber que termino la querie, cada thread antes de terminar 
  	                  		 
  	                  		 //aca corregir tiempo incial merege
  	                  		 
  	                  		 pthread_mutex_lock(&lock);
  	                  		 if(_timesLatencia[threadMaster]<=0)
  	                  		 {
								 clock_gettime(CLOCK_MONOTONIC, &_tmp);
								  
								//   pthread_rwlock_wrlock(&rw_rdlock);
								// _timesMERGE[threadMaster] = _tmp;	
								 _timesMERGELatencia[threadMaster] = _tmp;
								//	 pthread_rwlock_unlock(&rw_rdlock);				 
								  _timesLatencia[threadMaster]=1;
								 
							 }
							 pasoTreadcont=sumaActivos(threadMaster);
							  if(pasoTreadcont == nT)
  	                
  	                 		  {   cout<<"----------M: "<<threadMaster<<" -------pasoTreadcont: "<< pasoTreadcont<<endl;
								  cout<<"nTxxxxxxxxxxxxxxxxxxxxxxx "<< nT <<endl;
								  paso = 1;
								  tidWorking = tidLocal;
								  cout<<"tidWorking "<< tidWorking <<endl;
								//  pthread_exit(NULL);	
							  }
							  
							 
							 
							 pthread_mutex_unlock(&lock);
							 
							
							 
							 
							
							 
							 
  	                  		  
  	                  		 
  	                  		// if(tmp2==1)
  	                        // {   
							// }
						  //  cout<<"nT "<< nT <<endl;
						  //    cout<<"---------tmp2---------------"<< tmp2 <<endl;
						//     mostrarBanderaThread();
  	                  		 //este trabajo lo puede realizar cualquier y lo realiza el ultimo que llego, este se encarga de construir el topk Final
  	                  		 
  	                  		 pthread_mutex_lock(&lock);
  	                  		 int actividad = obtenerActividad(threadMaster);
							  pthread_mutex_unlock(&lock);
							  
  	                  		 if(actividad==nT && tidWorking > 0)
  	                  		 {
								// sleep(0.05);
								 cout<<"tidWorking "<< tidWorking <<endl;
								 cout<<"tid  "<< tidLocal <<endl;
								 cout<<"tid  master"<< threadMaster <<endl;
								 //Solo el tiempo del utlimo thread - es decir practicamente el sort
								 clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_tiempo1_MergeSort);
								 //este thread almacena los topk finales
								 //en el master cuando se manda a resolver la siguiente querie, se envia la respuesta de la ultima querie, 
								 //donde existe una lista de vectores top-k y se extrae y responde segun prioridad.
								  
					  		  //aca merge final y liberacion de emoria
								  
								                		 		         //Tiempo inicial Merge Sort
										//clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_tmp);//4
										//_times.push_back(_timesMERGE[threadMaster]);
					/*			       
								       int si = heapGlobal.size();
						               sort (heapGlobal.begin(), heapGlobal.end());
										//EscribirVectorRespuesta(HEAP,topK,consultaId);

											if(si>=ki)
											{
												_umbralGlobal=heapGlobal[si - (ki-1) ].first;
											}
											else if(si>0)
											{
												_umbralGlobal= heapGlobal[0].first;
											}
											else
											{
												_umbralGlobal= 0.0;
											}
									
										heapGlobal.erase(heapGlobal.begin(), heapGlobal.end());
																	
																	
																
																	           
										clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_tiempo2_MergeSort);//cuando termina el master de construir topK - time thread
										clock_gettime(CLOCK_MONOTONIC, &_tiempo2_ProcQ);//time procces
							
							
						//	pthread_rwlock_wrlock(&rw_rdlock);
						//	contQueries++;	
						//	pthread_rwlock_unlock(&rw_rdlock);		
									
						    			
							//if(contQueries==querierForJumpFree)
							//clock_gettime(MONOTONIC, &tiempo2_GlobalQLote);		
							
							struct timespec _tiempo1_MergeSortLatencia;
						
							struct timespec _tiempo2_MergeSortLatencia=_tiempo2_ProcQ;
							
							pthread_mutex_lock(&lock);
							//pthread_rwlock_rdlock(&rw_rdlock);
							_tiempo1_MergeSortLatencia	= _timesMERGELatencia[threadMaster];	
							_tiempo1_ProcQ=_timesQ[threadMaster];//cuando parte el primero que llego
							//pthread_rwlock_unlock(&rw_rdlock);
							_timesLatencia[threadMaster]=-1;
							pthread_mutex_unlock(&lock);
							
			*/				//_tiempo1_MergeSort=_tiempo1_MergeSort;
																	

		//	
	  //  cout<<"size times:"<<_times.size();
	  //  cout<<"tmpfile"<<tmpFile<<"---";
	//	cout<<idQuerie<<" "<<_heapSize<<" "<<_docs_suma<<" "<<_docs_procesados<<" "<<_next<<" "<<_heapUpdate<<" "<<_umbral;
	/*
	
	 //    pthread_mutex_lock(&lock);
		if(termOrQueries==0)
		logWand[tmpFile]<<idQuerie<<" "<<_heapSize<<" "<<_docs_suma<<" "<<_docs_procesados<<" "<<_next<<" "<<_heapUpdate<<" "<<_umbral;
		
		
		if(termOrQueries==1)
		logWand[tmpFile]<<tmp->consulta->terminos[0]<<" "<<_heapSize<<" "<<_docs_suma<<" "<<_docs_procesados<<" "<<_next<<" "<<_heapUpdate<<" "<<_umbral;
		
		
	
	
					  diff_time(&_tiempo2_ProcQ,&_tiempo1_ProcQ,&_latencia);
					  tiempoWand_procQ4=_latencia.tv_sec*1000000000+_latencia.tv_nsec;
					  logWand[tmpFile]<<" "<<tiempoWand_procQ4;        //Procesamiento Q

					  //diff_time(&_tiempo2_PreQ,&_tiempo1_PreQ,&_latencia);            //Pre-procesamiento WAND
					  //tiempoWand_procQ3=_latencia.tv_sec*1000000000+_latencia.tv_nsec;
					  logWand[tmpFile]<<" "<<_tiempoPreQWD;

					  diff_time(&_tiempo2_Wand,&_tiempo1_Wand,&_latencia);
					  tiempoWand_procQ1=_latencia.tv_sec*1000000000+_latencia.tv_nsec;
					  logWand[tmpFile]<<" "<<tiempoWand_procQ1;      		   //Procesamiento WAND

					  diff_time(&_tiempo2_MergeSort,&_tiempo1_MergeSort,&_latencia);
					  tiempoWand_procQ2=_latencia.tv_sec*1000000000+_latencia.tv_nsec;
					  logWand[tmpFile]<<" "<<tiempoWand_procQ2;      		   //Procesamiento Merge
					  
					  
					   diff_time(&_tiempo2_MergeSortLatencia,&_tiempo1_MergeSortLatencia,&_latencia);
					  _mergeLatencia=_latencia.tv_sec*1000000000+_latencia.tv_nsec;
					  logWand[tmpFile]<<" "<<_mergeLatencia; 
					  
					  timepoLatenciaMerge = tiempoWand_procQ4 - _mergeLatencia;
					  timepoLatenciaQuerie =(long int)( (long int)tiempoWand_procQ4 -((long int)tiempoWand_procQ2 + (long int)tiempoWand_procQ1 + (long int)_tiempoPreQWD));
					 // cout<<"-------------"<<_tiempoPreQWD<<endl;
					  
					  logWand[tmpFile]<<" "<<timepoLatenciaMerge <<" "<< timepoLatenciaQuerie <<" "<<nT;
					  
					  logWand[tmpFile]<<endl;
					  */
//	pthread_mutex_unlock(&lock);
	
	
	//cout<<" "<<tiempoWand_procQ4<<" "<<_tiempoPreQWD<<" "<<tiempoWand_procQ1<<" "<<tiempoWand_procQ2<<" "<<_mergeLatencia<<" "<<nT;
								
								//   delete heapLocal;
								
								  liberarThread(threadMaster);
								//  liberarThread(threadMaster);
								//  pthread_rwlock_wrlock(&rw_rdlock);
								  pthread_mutex_lock(&lock);								   
								  banderas[threadMaster]=0;
								//  pthread_rwlock_unlock(&rw_rdlock);
								  
								  pthread_mutex_unlock(&lock);
														
								//		 cout<<"---------tmp2-5--------------"<< tmp2 <<endl;
								//		mostrarBanderaThread();
							}
							
							
							
							// cout<<"---------tmp2-666--------------"<< tmp2 <<endl;
							//			mostrarBanderaThread();
							
							//el buffer de memoria usada para sincronizacion del master se libera al terminar el proceso, el tid local despues de agregar sus resultados al merge continuan y se libera
							else
							{
								if(tidWorking!=threadMaster)
								{
									 cout<<"tid  liberar TID: "<< tidLocal <<endl;
									liberarThread(tidLocal);
									//if(paso!=1)
									//{
									//	delete heapLocal;
									//	heapGlobal.erase(heapGlobal.begin(), heapGlobal.end());
									//}
								}
							
						    }		
										cout<<"-------******----"<< tmp2 <<endl;
										mostrarBanderaThread();
							
							
							
							

	pthread_exit(NULL);	
}

//ejecucion Wand-querie
void WandQueryBaseLine(DatosQuery* consulta,
              int consultaId,
              int topK,
              float umbral,
              int tipoRank/*, pthread_barrier_t *barrierIn, pthread_mutex_t *lockIn*/,
              unsigned long int &_HEAP_SIZE, unsigned long int &_DOCS_PROCESADOS,unsigned long int &_HEAP_UPDATES,unsigned long int &_DOCS_SUMA, unsigned long int &_NEXT,float &_UMBRAL,
              int maestro,
              int _pid,
              vector<struct timespec> &_time,
              int _nunThreads
              )
{
   // struct timespec &_tiempo1_PreQ,struct timespec &_tiempo2_PreQ,struct timespec &_tiempo1_Wand,struct timespec &_tiempo2_Wand,struct timespec &_tiempo1_MergeSort,struct timespec &_tiempo2_MergeSort
      unsigned int id_doc;
      vector<QueryFeatureInfoT*> *query_terms=new vector<QueryFeatureInfoT*>();
      vector<PostingStream*> *streams=new vector<PostingStream*>();
      QueryFeatureInfoT *info;
      PostingStream *stream;
      unsigned long long id_term;
      float tfQ,score;//frecuencia termino consulta en consulta, si fuese el peso se debe cambiar el calculo de upper bounds
      Wand *wand=NULL;
      ScorerT *scorer=NULL;
      //float threshold_tmp = umbral;
      float threshold = umbral;
      float min_thr=0.0;
      CBT *heapM = new CBT(topK);
      map<unsigned long long, PostingStream*>::iterator it;
	  //conteo de los terms de consulta encontrados en el indice
	  int n_terms=consulta->n_terminos;
	  int n_terms_encontrados=0;
       map<unsigned long long, int> nDocumentos;// = new map<int,unsigned int>();
    //  typedef pair <int, unsigned long long> Int_Par_term;
    //  typedef pair <unsigned long long, int> Int_Par_docs;
    //  typedef pair <int,int> sizeListas;
    //  typedef pair <int,float> timeQ;
    //  typedef pair <int, sizeListas> tid_sizeListas;
      float alpha;
      int nActualizaciones=0;
      int docsSuma = 0;
     // contPasoThread[consultaId]+=1;
      struct timespec _tmp;     
      // cout <<"INICIO PROCESAMIENTO PID: "<<_pid<<endl;
      // Tiempo ini preprocesamiento querie
              if(_pid==maestro)
              {
				  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_tmp/*_tiempo1_PreQ*/);//0
				  _time.push_back(_tmp);
              }
        //BM25
        //float sumUB = 0.0;
               //pre-procesamiento querie 
		for(int i=0; i<n_terms; i++){
			//tomar datos de la consulta
            id_term=consulta->terminos[i];
            tfQ=(float)consulta->valores[i];
            
            if(tipoMetodo==1)
            {
				it=listaStruct[_pid].indiceThread.find(id_term);
				if(it!=listaStruct[_pid].indiceThread.end()){
					//termino encontrado
					n_terms_encontrados++;
					stream=it->second;
					docsSuma = docsSuma + stream->n;
					// IDF*(2->BM25 o 3->TF/|D| )
					if(tipoRank!=1)
					alpha=tfQ*stream->idf_term;
					else
					alpha=tfQ;
					
					info=new QueryFeatureInfoT(id_term, alpha, stream->ub_inicial);
					info->ub=alpha*stream->ub_inicial;
					//sumUB=+info->ub;
					//cout<<"["<< id_term<<"]info->ub: "<<info->ub<<endl; 
					query_terms->push_back(info);
					streams->push_back(stream);

				}//if... termino encontrado
		    }
		    if(tipoMetodo==2)
            {
				it=listaStruct[_pid].indiceThreadIntervalos.find(id_term);
				if(it!=listaStruct[_pid].indiceThreadIntervalos.end()){
					//termino encontrado
					n_terms_encontrados++;
					stream=it->second;
					docsSuma = docsSuma + stream->n;
					// IDF*(2->BM25 o 3->TF/|D| )
					if(tipoRank!=1)
					alpha=tfQ*stream->idf_term;
					else
					alpha=tfQ;
					
					info=new QueryFeatureInfoT(id_term, alpha, stream->ub_inicial);
					info->ub=alpha*stream->ub_inicial;
					//sumUB=+info->ub;
			//		cout<<"info->ub: "<<info->ub<<endl; 
			//cout<<"["<< id_term<<"]info->ub: "<<info->ub<<" dim list"<<stream->n<<" Tid: "<<_pid<<endl; 
					query_terms->push_back(info);
					streams->push_back(stream);

				}//if... termino encontrado
		    }
		    _DOCS_SUMA=docsSuma;

	}//for... cada termino de la consulta

		//Tiempo fin preprocesamiento pesos querie
		if(_pid==maestro)
		{//cout <<" fin preprocesamiento time Tid: "<<_pid<<endl;
		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_tmp/*&_tiempo2_PreQ*/);//1
		 _time.push_back(_tmp);
		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_tmp/*&_tiempo1_Wand*/);//2
		 _time.push_back(_tmp);
		}
            int nSeleccionadosTopK=0;
            if(n_terms_encontrados>0){
				
			if(tipoMetodo==1)	
			scorer=new ScorerT(streams, query_terms,0); //0--> baseLine	
			if(tipoMetodo==2)
			scorer=new ScorerT(streams, query_terms,1); //0--> bmw
			
			wand=new Wand(n_terms_encontrados, scorer, streams);
        	wand->init();
			wand->tipoWAND_AND_OR=busquedaAND;
			//Buscar doc candidato
				while((id_doc=wand->next(threshold,tipoMetodo,tipoRank)) != END_OF_STREAM)
				{
					//Score candidato seleccionado
					score=wand->currentPayload;
					//_NEXT++;
					//cout<<score<<" "<<threshold<<endl;
					//agregar al heap
					if(nSeleccionadosTopK<topK)
					{
						heapM->insert(score,id_doc); //Propuesta M. Marin        
						nSeleccionadosTopK++;
					}
					else{
						min_thr = heapM->top();
						if(min_thr<score){
						heapM->insert(score,id_doc);
						//cout<<id_doc<<" "<<valor<<endl;
						nActualizaciones++;
						}
					}
					//Mantiene threshold inicial
					if(nSeleccionadosTopK<topK){
					threshold=umbral;
					}
					else
					{
					//nuevo threshold	
					threshold = heapM->top();
					}
					//cout<<threshold<<endl;
     }		
	// if(_pid==maestro)
    // _DOCS_PROCESADOS = wand->evaluacionesCompletas;
     
	 wand->~Wand(); 
     }
            //Tiempo fin WAND
            if(_pid==maestro)
            {
				clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_tmp/*&_tiempo2_Wand*/);//3
				_time.push_back(_tmp);
                //Tiempo inicial Merge Sort
                clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_tmp/*&_tiempo1_MergeSort*/);//4
                _time.push_back(_tmp);
		  }
              
     //Build and Sort topK
     vector < pair<unsigned int, unsigned int> > docsTopKLocal;
     int contador=1;
     int dim = heapM->dimensionHeap();
     
     
     
 /*
		 pthread_mutex_lock( &lock );

		 //lock
		 while(contador<=dim)
		 {
		   HEAP.push_back(pair<float, unsigned int>(heapM->Prioridades[contador],heapM->ID_docs[contador] ));
		   contador++;
		 }
		 int si = HEAP.size();
		 pthread_mutex_unlock( &lock );

		 pthread_barrier_wait(&barrier);
		//sort topK
		if(_pid==maestro)
		{
		sort (HEAP.begin(), HEAP.end());
		//EscribirVectorRespuesta(HEAP,topK,consultaId);

			if(si>=topK)
			{
			_UMBRAL=HEAP[si - (topK-1) ].first;
			}
			else if(si>0)
			{
				_UMBRAL= HEAP[0].first;
			}
			else
			{
				_UMBRAL= 0.0;
			}
	
		HEAP.erase(HEAP.begin(), HEAP.end());
	  	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_tmp);//5
		_time.push_back(_tmp);
		
		}
		*/
		
    delete heapM;
  /*  if(_pid==maestro)
    {
    _HEAP_SIZE = nSeleccionadosTopK;
    // else
    _HEAP_UPDATES = nActualizaciones;
	}
   */
}
//CONSTRUIR PROGRAMA CORTO QUE SOLO CALCULE EL TIEMPO DE CADA TIOK PARA CADA TERTMINO DE LAS CONSULAST...
	
	
  void *HeapLocales(void *arg)
  {
  datosQuerie *tmp;
  tmp = (datosQuerie*)arg;
  int i =  tmp->idQ; 
  int nT = tmp->nthreads;
  int tidLocal =tmp->tid;
  int ki =tmp->k;
  float x;
  int tmpFile = tmp->p;
  int threadMaster = tmp->corePadre;
  int idQuerie = tmp->consulta->id_query;
	unsigned long int tiempoWand_procQ1;
	unsigned long int tiempoWand_procQ2;
	unsigned long int tiempoWand_procQ3;
	unsigned long int tiempoWand_procQ4;
 // tmp->consulta->mostrar();
  			  unsigned long int _DOCS_PROCESADOS=0;
			  unsigned long int _HEAP_UPDATES=0;
			  //unsigned long int DOCS=0;
			  unsigned long int _DOCS_SUMA=0;
			  unsigned long int _NEXT=0;
			  unsigned long int _HEAP_SIZE=0;
			  float _UMBRAL=0.0;
			  struct timespec _latencia;
			  struct timespec _tiempo1_ProcQ,_tiempo2_ProcQ,_tiempo1_PreQ,_tiempo1_Wand,_tiempo1_MergeSort,_tiempo2_PreQ,_tiempo2_Wand,_tiempo2_MergeSort;	
  			  //struct timespec _times[8];
  			  vector<struct timespec> _times;
  			  //	 cout <<" INICIO LLAMADA WAND Tid: "<<tidLocal<<" Master: "<< threadMaster <<endl;
  			  //	cout <<" -------------- Q "<<i<<endl;
  			  if(tidLocal==threadMaster)		  
		      {clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_tiempo1_ProcQ);} //
		  
			  //WandQueryBaseLine(tmp->consulta,i,ki,tmp->umbral,tipoRank,_HEAP_SIZE,_DOCS_PROCESADOS,_HEAP_UPDATES,_DOCS_SUMA,_NEXT,_UMBRAL,threadMaster,tidLocal,_times,nT);
		//	  WandMultihebraDinamica(tmp->consulta,i,ki,tmp->umbral,tipoRank,_HEAP_SIZE,_DOCS_PROCESADOS,_HEAP_UPDATES,_DOCS_SUMA,_NEXT,_UMBRAL,threadMaster,tidLocal,_times,nT);		
			 //			  	cout <<" ----ffffffffffffffff---------- Q "<<i<<endl;
			  if(tidLocal==threadMaster) 
			  {	
			   
		      	_tiempo1_PreQ =_times[0];
			  	_tiempo2_PreQ=_times[1];
			  	_tiempo1_Wand=_times[2];
			  	_tiempo2_Wand=_times[3];
			  	_tiempo1_MergeSort=_times[4];
			  	_tiempo2_MergeSort=_times[5];
			  	
			  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_tiempo2_ProcQ);
					
		if(termOrQueries==0)
		logWand[tmpFile]<<idQuerie<<" "<<_HEAP_SIZE<<" "<<_DOCS_SUMA<<" "<<_DOCS_PROCESADOS<<" "<<_NEXT<<" "<<_HEAP_UPDATES<<" "<<_UMBRAL;
		
		
		
		if(termOrQueries==1)
		logWand[tmpFile]<<tmp->consulta->terminos[0]<<" "<<_HEAP_SIZE<<" "<<_DOCS_SUMA<<" "<<_DOCS_PROCESADOS<<" "<<_NEXT<<" "<<_HEAP_UPDATES<<" "<<_UMBRAL;
		
	
					  

					  diff_time(&_tiempo2_ProcQ,&_tiempo1_ProcQ,&_latencia);
					  tiempoWand_procQ4=_latencia.tv_sec*1000000000+_latencia.tv_nsec;
					  logWand[tmpFile]<<" "<<tiempoWand_procQ4;        //Procesamiento Q

					  diff_time(&_tiempo2_PreQ,&_tiempo1_PreQ,&_latencia);            //Pre-procesamiento WAND
					  tiempoWand_procQ3=_latencia.tv_sec*1000000000+_latencia.tv_nsec;
					  logWand[tmpFile]<<" "<<tiempoWand_procQ3;

					  diff_time(&_tiempo2_Wand,&_tiempo1_Wand,&_latencia);
					  tiempoWand_procQ1=_latencia.tv_sec*1000000000+_latencia.tv_nsec;
					  logWand[tmpFile]<<" "<<tiempoWand_procQ1;      		   //Procesamiento WAND

					  diff_time(&_tiempo2_MergeSort,&_tiempo1_MergeSort,&_latencia);
					  tiempoWand_procQ2=_latencia.tv_sec*1000000000+_latencia.tv_nsec;
					  logWand[tmpFile]<<" "<<tiempoWand_procQ2;      		   //Procesamiento Merge
					  logWand[tmpFile]<<endl;
					   
					  
			}
  
//  cout <<"idTid: "<<tidLocal<<" querie: "<<i<<" numThreads: "<<nT<<" K: "<<ki<<endl;
  
  //if(nT>1)
 pthread_barrier_wait(&barrier);
  
 // delete tmp;
// cout <<"Paso--> "<<ki<<" Tid--> "<<tidLocal<<" q "<<i<<endl;
 pthread_exit(NULL);	
}
