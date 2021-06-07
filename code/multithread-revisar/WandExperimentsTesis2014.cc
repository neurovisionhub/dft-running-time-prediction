//Crear wand para todo tipo de experimentos
//Ordenar las librerias para distintos tipos de listas
//incorporar cualquier tipo de calgulo de modelo alpha*w(d,t), derivado de modelo tf-idf   tf=f/sizeDoc, tfBM25, otros...

//Disenar la otra version multithread separa pero con identicas llamads y estructuras...

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
//Libreria WAND-Index Block Max
#include "./Libs/libWand/bloques2013/WandLibreria2014.h"
#include <math.h>
//Libreria WAND-Baseline
//#include "Wand.h"
#include "./Libs/libExtras/estructDouble.h"
#include "math.h"
//Libreria que almacena el heap-topk
#include "./Libs/libWand/HeapMDouble.h"
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
using namespace std;
ListaInvertida listaStruct[1];				//lista de terminos
void procesingQueries(int opcionW);
//unsigned int UMBRAL,HEAP_UPDATES,DOCS_PROCESADOS;
int K;
int tipoMetodo; //1-->baseline, 2-->BMW
int tipoRank;   //1-->TF,2-->BM25,3-->TF-IDF
int busquedaAND=0;//suma UB
int busquedaOR;//min UN o bien threshold estatico en 0
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

//ejecucion Wand-querie
void WandQueryBaseLine(DatosQuery* consulta,
              int consultaId,
              int topK,
              double umbral,
              int tipoRank)
{
   
      unsigned int id_doc;
      vector<QueryFeatureInfoT*> *query_terms=new vector<QueryFeatureInfoT*>();
      vector<PostingStream*> *streams=new vector<PostingStream*>();
      QueryFeatureInfoT *info;
      PostingStream *stream;
      unsigned long long id_term;
      double tfQ,score;//frecuencia termino consulta en consulta, si fuese el peso se debe cambiar el calculo de upper bounds
      Wand *wand=NULL;
      ScorerT *scorer=NULL;
      //double threshold_tmp = umbral;
      double threshold = umbral;
      double min_thr=0.0;
      CBT *heapM = new CBT(topK);
      map<unsigned long long, PostingStream*>::iterator it;
	  //conteo de los terms de consulta encontrados en el indice
	  int n_terms=consulta->n_terminos;
	  int n_terms_encontrados=0;
       map<unsigned long long, int> nDocumentos;// = new map<int,unsigned int>();
      typedef pair <int, unsigned long long> Int_Par_term;
      typedef pair <unsigned long long, int> Int_Par_docs;
      typedef pair <int,int> sizeListas;
      typedef pair <int,double> timeQ;
      typedef pair <int, sizeListas> tid_sizeListas;
      double alpha;
      int nActualizaciones=0;
      // Tiempo ini preprocesamiento querie
              clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo1_PreQ);



        //BM25
        //double sumUB = 0.0;
               //pre-procesamiento querie 
		for(int i=0; i<n_terms; i++){
			//tomar datos de la consulta
            id_term=consulta->terminos[i];
            tfQ=(double)consulta->valores[i];
            
            if(tipoMetodo==1)
            {
				it=listaStruct[0].indiceThread.find(id_term);
				if(it!=listaStruct[0].indiceThread.end()){
					//termino encontrado
					n_terms_encontrados++;
					stream=it->second;
					DOCS_SUMA = DOCS_SUMA + stream->n;
					// IDF*(2->BM25 o 3->TF/|D| )
					if(tipoRank!=1)
					alpha=tfQ*stream->idf_term;
					else
					alpha=tfQ;
					
					info=new QueryFeatureInfoT(id_term, alpha, stream->ub_inicial);
					info->ub=alpha*stream->ub_inicial;
					//sumUB=+info->ub;
					//cout<<"info->ub: "<<info->ub<<endl; 
					query_terms->push_back(info);
					streams->push_back(stream);

				}//if... termino encontrado
		    }
		    if(tipoMetodo==2)
            {
				it=listaStruct[0].indiceThreadIntervalos.find(id_term);
				if(it!=listaStruct[0].indiceThreadIntervalos.end()){
					//termino encontrado
					n_terms_encontrados++;
					stream=it->second;
					DOCS_SUMA = DOCS_SUMA + stream->n;
					// IDF*(2->BM25 o 3->TF/|D| )
					if(tipoRank!=1)
					alpha=tfQ*stream->idf_term;
					else
					alpha=tfQ;
					
					info=new QueryFeatureInfoT(id_term, alpha, stream->ub_inicial);
					info->ub=alpha*stream->ub_inicial;
					//sumUB=+info->ub;
					//cout<<"info->ub: "<<info->ub<<endl; 
					query_terms->push_back(info);
					streams->push_back(stream);

				}//if... termino encontrado
		    }
		
          	
          	
          	
		}//for... cada termino de la consulta



       


		//Tiempo fin preprocesamiento pesos querie
		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo2_PreQ);

		//tiempo ini WAND
		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo1_Wand);

            int nSeleccionadosTopK=0;
            if(n_terms_encontrados>0){
			scorer=new ScorerT(streams, query_terms,0); //0--> baseLine
			wand=new Wand(n_terms_encontrados, scorer, streams);
        	wand->init();
			wand->tipoWAND_AND_OR=busquedaAND;
		


				//Buscar doc candidato
				while((id_doc=wand->next(threshold,tipoMetodo,tipoRank)) != END_OF_STREAM)
				{
					//Score candidato seleccionado
					score=wand->currentPayload;
					NEXT++;
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
	
     DOCS_PROCESADOS = wand->evaluacionesCompletas;
	 wand->~Wand(); 
     }
            //Tiempo fin WAND
              clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo2_Wand);
              
     UMBRAL=threshold;
             //Tiempo inicial Merge Sort
              clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo1_MergeSort);
     //Build and Sort topK
     vector < pair<unsigned int, unsigned int> > docsTopKLocal;
     int contador=1;
     int dim = heapM->dimensionHeap();
     if(dim>0)
     //Build - Pack topK
     while(contador<=dim)
     {
       HEAP.push_back(pair<double, unsigned int>(heapM->Prioridades[contador],heapM->ID_docs[contador] ));
       contador++;
     }
     //cout<<HEAP.size()<<endl;
	//sort topK
	sort (HEAP.begin(), HEAP.end());
	//EscribirVectorRespuesta(HEAP,topK,consultaId);
	HEAP.erase(HEAP.begin(), HEAP.end());
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo2_MergeSort);
    delete heapM;
    HEAP_SIZE = nSeleccionadosTopK;
    // else
    HEAP_UPDATES = nActualizaciones;
   
}
//CONSTRUIR PROGRAMA CORTO QUE SOLO CALCULE EL TIEMPO DE CADA TIOK PARA CADA TERTMINO DE LAS CONSULAST...

//llamadas wand-queries
void callWand(double threshold,int inicio,int fin)
{
	AsignaCPUThread(4);
	unsigned long int tiempoWand_procQ1;
	unsigned long int tiempoWand_procQ2;
	unsigned long int tiempoWand_procQ3;
	unsigned long int tiempoWand_procQ4;
	unsigned long int tiempoWand_procQ5;
//ListaC *tmp;
//tmp = (ListaC*)arg;
//unsigned int threshold=tmp->umbralInicial;
//unsigned int dimConsultas=listaStruct[0].consultas.size(); 
 // cout<<"-------------------------- dimConsultas : "<<fin<<endl;

    //Ciclo de llamadas a Wand-Querie
	for( unsigned int i=inicio;i<=fin;i++)  
	{
      cout<<".. Q["<<i<<"] .. "<<endl;
      DOCS_PROCESADOS=0;
      HEAP_UPDATES=0;
      DOCS=0;
      DOCS_SUMA=0;
      NEXT=0;
      UMBRAL=(double)threshold;
    
      //Numero docs Interseccion
      if(opcionTraza==0)
      {
		  thersholdLogConsulta<<i;
    	  dimDocsQuerie(listaStruct[0].consultas[i]);
    	  thersholdLogConsulta<<" "<<DOCS_SUMA<<" "<<DOCS<<" "<<listaStruct[0].consultas[i]->n_terminos;
    	  dimDocsQuerieSize(listaStruct[0].consultas[i]);
      }
      else if(opcionTraza>0)
      {
			  //Wand K
			  if(opcionTraza==1)
			  {			  
			  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo1_ProcQ); //
			  //INCORPORARA LAS VARIANTES +++
			  
			  if(tipoMetodo==1)
			  WandQueryBaseLine(listaStruct[0].consultas[i],i,K,threshold,tipoRank);//Umbral inicial
			 
			  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo2_ProcQ);
			  thersholdLogConsulta<<i<<" "<<HEAP_SIZE<<" "<<DOCS_SUMA<<" "<<DOCS_PROCESADOS<<" "<<NEXT<<" "<<HEAP_UPDATES<<" "<<UMBRAL;

			  diff_time(&tiempo2_ProcQ,&tiempo1_ProcQ,&latencia);
			  tiempoWand_procQ4=latencia.tv_sec*1000000000+latencia.tv_nsec;
			  thersholdLogConsulta<<" "<<tiempoWand_procQ4;        //Procesamiento Q

					diff_time(&tiempo2_PreQ,&tiempo1_PreQ,&latencia);            //Pre-procesamiento WAND
			  tiempoWand_procQ3=latencia.tv_sec*1000000000+latencia.tv_nsec;
			  thersholdLogConsulta<<" "<<tiempoWand_procQ3;

			  diff_time(&tiempo2_Wand,&tiempo1_Wand,&latencia);
			  tiempoWand_procQ1=latencia.tv_sec*1000000000+latencia.tv_nsec;
			  thersholdLogConsulta<<" "<<tiempoWand_procQ1;      		   //Procesamiento WAND

			  diff_time(&tiempo2_MergeSort,&tiempo1_MergeSort,&latencia);
			  tiempoWand_procQ2=latencia.tv_sec*1000000000+latencia.tv_nsec;
			  thersholdLogConsulta<<" "<<tiempoWand_procQ2;      		   //Procesamiento Merge
			  thersholdLogConsulta<<endl;


			  }
			  // Wand 10,100,1000,10000
			  if(opcionTraza==2)
			  {
				  int tmp = 0;
				  for(int p=10;p<=10000; p=p*10)
				  {  
						 DOCS_PROCESADOS=0;
			  HEAP_UPDATES=0;
			  DOCS=0;
			  DOCS_SUMA=0;
			  NEXT=0;
			  UMBRAL=threshold;
							  
					  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo1_ProcQ); //
					  //INCORPORARA LAS VARIANTES +++
					  if(tipoMetodo==1)
					  WandQueryBaseLine(listaStruct[0].consultas[i],i,p,threshold,tipoRank);
					  
					  
					  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo2_ProcQ);

					  logWand[tmp]<<i<<" "<<HEAP_SIZE<<" "<<DOCS_SUMA<<" "<<DOCS_PROCESADOS<<" "<<NEXT<<" "<<HEAP_UPDATES<<" "<<UMBRAL;

					  diff_time(&tiempo2_ProcQ,&tiempo1_ProcQ,&latencia);
					  tiempoWand_procQ4=latencia.tv_sec*1000000000+latencia.tv_nsec;
					  logWand[tmp]<<" "<<tiempoWand_procQ4;        //Procesamiento Q

					  diff_time(&tiempo2_PreQ,&tiempo1_PreQ,&latencia);            //Pre-procesamiento WAND
					  tiempoWand_procQ3=latencia.tv_sec*1000000000+latencia.tv_nsec;
					  logWand[tmp]<<" "<<tiempoWand_procQ3;

					  diff_time(&tiempo2_Wand,&tiempo1_Wand,&latencia);
					  tiempoWand_procQ1=latencia.tv_sec*1000000000+latencia.tv_nsec;
					  logWand[tmp]<<" "<<tiempoWand_procQ1;      		   //Procesamiento WAND

					  diff_time(&tiempo2_MergeSort,&tiempo1_MergeSort,&latencia);
					  tiempoWand_procQ2=latencia.tv_sec*1000000000+latencia.tv_nsec;
					  logWand[tmp]<<" "<<tiempoWand_procQ2;      		   //Procesamiento Merge
					  logWand[tmp]<<endl;
			 
					 tmp++;
		 
				  }  

			  }
	  }
      else
      {
		  cout << "Opcion no reconocida..." << opcionTraza << endl;
		  exit(0);
		  
	  }
    
	}//fin for


}


void abrirFileLogWand(int metodo,int tRank)
{
if(tRank==1)
{
	if(metodo==1)
	{
	logWand[0].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K10_TF_WAND.txt");
	logWand[1].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K100_TF_WAND.txt");
	logWand[2].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K1000_TF_WAND.txt");
	logWand[3].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K10000_TF_WAND.txt");	
	}
	if(metodo==2)
	{
	logWand[0].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K10_TF_BMW.txt");
	logWand[1].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K100_TF_BMW.txt");
	logWand[2].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K1000_TF_BMW.txt");
	logWand[3].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K10000_TF_BMW.txt");	
	}
}
if(tRank==2)
{
	if(metodo==1)
	{
	logWand[0].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K10_BM25_WAND.txt");
	logWand[1].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K100_BM25_WAND.txt");
	logWand[2].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K1000_BM25_WAND.txt");
	logWand[3].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K10000_BM25_WAND.txt");	
	}
	if(metodo==2)
	{
	logWand[0].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K10_BM25_BMW.txt");
	logWand[1].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K100_BM25_BMW.txt");
	logWand[2].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K1000_BM25_BMW.txt");
	logWand[3].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K10000_BM25_BMW.txt");	
	}
}
if(tRank==3)
{
	if(metodo==1)
	{
	logWand[0].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K10_TFIDF_WAND.txt");
	logWand[1].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K100_TFIDF_WAND.txt");
	logWand[2].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K1000_TFIDF_WAND.txt");
	logWand[3].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K10000_TFIDF_WAND.txt");	
	}
	if(metodo==2)
	{
	logWand[0].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K10_TFIDF_BMW.txt");
	logWand[1].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K100_TFIDF_BMW.txt");
	logWand[2].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K1000_TFIDF_BMW.txt");
	logWand[3].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K10000_TFIDF_BMW.txt");	
	}
}

}

void cerrarFileLogWand()
{
logWand[0].close();
logWand[1].close();
logWand[2].close();
logWand[3].close();	
}
//metodo: 1-->baseline, 2-->BMW, opcionW: 1-->TF,2-->BM25,3-->TF-IDF 
void procesingQueries(int metodo,int opcionRank)
{
	for(int m=0;m<contSetQueries;m++)
	{   printf("Lote ---------> %d.\n",m);

	//carga de conjunto de listas
	listaStruct[0].cargarConjuntoListas(m,metodo,opcionRank);
		
		//call wand conjunto queries
		//listaStruct[t].tid=t;
	//listaStruct[t].nthreads=numThreads;
	listaStruct[0].consultas= querys;
	//listaStruct[0].umbralInicial=0;
	printf("--------- INICIO Llamada WAND Lote ---------> %d.\n",m);
	cout<<"--ini----"<<punterosIniciales[m]<<endl;
	cout<<"---fin---"<<punterosFinales[m]<<endl;
	callWand(0.0,punterosIniciales[m],punterosFinales[m]);
	printf("---------- FIN Llamada WAND Lote ---------> %d.\n",m);
	   // for()
	   // {
			//punterosIniciales.push_back(inicial);
		//punterosFinales.push_back(finalQ);
	   // }
		//liberar memoria listas
		//listaStruct[0].liberarRAM();
		 if(metodo==1)
		 listaStruct[0].liberarRAM_P();
		 if(metodo==2)
		 listaStruct[0].liberarRAM_Blocks();
	}
	
}

int main (int argc, char *argv[]) {
AsignaCPUThread(2);
nombreArchivoConsultas = argv[1];//file queries
numeroConsultasProceso = atoi(argv[2]);//N de consultas a procesar
int querierForJumpFree = atoi(argv[3]);//file topK
opcionTraza = atoi(argv[4]);// 0-->#interesecciones, 1-->topk in, 2-->topk automatico 10,100,10000,100000
tipoMetodo = atoi(argv[5]);//1-->baseline, 2-->BMW
tipoRank = atoi(argv[6]);//1-->TF,2-->BM25,3-->TF-IDF
SizeBloque = atoi( argv[7] );
//busquedaAND = atoi(argv[7]);
if(tipoRank!=1)
cargaDocs();// 2.3 Gbytes Ram - Verificar

printf("...Cargando Consultas...\n");
leerConsultasCharJump(nombreArchivoConsultas,&querys,numeroConsultasProceso,querierForJumpFree);
printf("...Fin carga Consultas...\n");
//mostrarTermsLotes();
abrirFileLogWand(tipoMetodo,tipoRank);
//pFileTopk = fopen("./trazaTopKBM25","w");
procesingQueries(tipoMetodo,tipoRank);
cerrarFileLogWand();
//fflush(pFileTopk);
//fclose(pFileTopk);
exit(0);
}

//REVISAR PORQUE SE DEMORA TANTO CON BM25, OBSERVAR SI EXISTE ALGUN CALCULO U/O PARAMETRO QUE NO MINIMIZA EN NUMERO DE SCORE REALIZADOS
