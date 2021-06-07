/*
 Programa que obtiene topK utilizando el metodo WAND-Baseline
 
 Compilacion: 
 sh compilar.sh
 
 Ejecution WAND:
 [nombrePrograma K fileQueries fileIndice #Queries fileTopK] 
 
 ej: ./programa 10 ./consultas/ConsultasWand.dat lista_UB_256_Nodo_0.dat 100000 topk.dat
 
 */
 
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
//Libreria WAND-Baseline
#include "./Libs/libWand/base/WandBM25.h"
#include "estruc.h"
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
// docs vacios 119757
map<int,int> termsGlobalFrec;
map<int,int> conteoTermsGlobalFrec;
double UMBRAL_BM25;
int contSetQueries;
int K;
using namespace std;
vector < pair<double, unsigned int> > HEAP;//heap que almacena topK

unsigned int numeroConsultasProceso;//numero de consultas a procesar
char *nombreArchivoTopk;			//nombre file topK
char *nombreArchivoConsultas;		//nombre file queries
char *nombreArchivoLista;			//nombre file indice invertido
char *nombreArchivoLog;			//nombre file indice invertido
FILE * pFileTopk;					//file topK
map<int,vector<int> > termsblocksqueries;
map<int,vector<int> > idQueriesBlock;
vector<unsigned int> punterosIniciales;
vector<unsigned int> punterosFinales;
//map<int,int> dimDocs;
int nSeleccionadosG;
int contadorBandera;
unsigned int thresholdGlobal;
map < int, vector < pair<unsigned int, unsigned int> > > HEAP_LOCALES;
vector < unsigned int> HEAP_UBS;
//map<int,int> dimDocsTmp;

void AsignaCPUThread(int cpuId);

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;;
pthread_barrier_t barrier;
pthread_barrierattr_t attr;
pthread_cond_t llegaron = PTHREAD_COND_INITIALIZER;



class ListaC
{
  private:

 
   public:
   int tid;
   int nthreads;
   map<unsigned long long, PostingStream* > indiceThread;
   vector < DatosQuery* > consultas;
   int prueba;
   int umbralInicial;
   map<unsigned long long,unsigned int> ubLocales;

   ListaC()
   {

   }
   ListaC(int p,int pr)
   {
     tid  = p;
     nthreads = pr;
   }
   unsigned int ub_local(PostingStream *entrada)
   {
       return 1;
   }
   
   void liberarRAM()
   {   
	                for (std::map<unsigned long long, PostingStream*>::iterator it=indiceThread.begin(); it!=indiceThread.end(); ++it)
                    it->second->~PostingStream();
                    
       
                    
	   cout<<"------------------ANTES------------->>> "<<indiceThread.size()<<endl;
	   indiceThread.clear();
	   ubLocales.clear();
	   cout<<"------------------DESPUES------------->>> "<<indiceThread.size()<<endl;
	   
	   
   }
   
         void liberarRAM_P()
   {   
	   
	   
	   
	   //for (std::map<unsigned long long, PostingStream*>::iterator it=indiceThread.begin(); it!=indiceThread.end(); ++it)
       //it->second->~PostingStream();
       
       
       
       for (std::map<unsigned long long, PostingStream*>::iterator it=indiceThread.begin(); it!=indiceThread.end(); ++it)
	{   termsGlobalFrec[it->first] = termsGlobalFrec[it->first]-1;
	   
	   if(termsGlobalFrec[it->first]<1) 
	   {
		  it->second->~PostingStream();
		  termsGlobalFrec[it->first] = 0;
		  conteoTermsGlobalFrec[it->first]=0;
	   }
   }

	   //cout<<"------------------ANTES------------->>> "<<indiceThread.size()<<endl;
	   //indiceThread.clear();
	   
	   //ubLocales.clear();
	   //cout<<"------------------DESPUES------------->>> "<<indiceThread.size()<<endl;


   }
   
};

ListaC listaStruct[1];				//lista de terminos
vector<DatosQuery*> querys;			//consultas
map<int,vector<DatosQuery*> > vQ;   //observar
//escribir file topK
void EscribirVectorRespuesta(vector< pair<double,unsigned int > > respuesta, int dim,int idQ)
{
  int cont=respuesta.size();
  //if(idQ%1000==0)  
  printf("****Escribiendo consulta %d en archivo... size %d\n",idQ,cont);
  fprintf(pFileTopk,"%d",idQ);
  if(dim>cont)
    dim=cont;
    for(int i=0;i<dim;i++)
    {
	fprintf(pFileTopk," %u %f",respuesta[cont - 1 - i].second,respuesta[cont - 1 - i].first);
 }
 fprintf(pFileTopk,"\n");

}



void cargaDocs()
{
	

   string linea;
   int id_doc, proc, tira;
   unsigned long int C = 0;
   ifstream terms("/mnt/DataWeb/indices/ClueWebIndex/completaWC09/lexico/docId.txt"); //terms file
   int idt,nd;
   set<int>IdTerms;
   int cont=0;
   int j=0;
   unsigned int k=0;
   
   // la lista comienza en la linea 2, pero se debe comparar el id linea + 2 si se parte contando de 0
   int doc,lenght;
   unsigned int totalLenght = 0;
   int contVacios = 0;
   //ifstream todo(nombreArchivoEntrada);//Docs de entrada
   //vector<string> tokens;
   //carga de terms a seleccionar
   while( getline(terms,linea))
   {
	   
	   	 int c = 0;
	   	 int id = 0;
         stringstream st(linea);
         string item;
	     if(k%100000==0)
	     cout<<k<<endl;
	     int x;
	 
	     
	     while(st.good())
         {
            if ( !st.good() ) 
            continue; 
            st >> item;
            if(k!=0)
            {
				if(c==0)
				{
					
				//	cout<<"idDoc: "<<item.replace(item.size()-1,1, "");
					//if(k%100000==0)
				    
				    id = atoi(item.replace(item.size()-1,1, "").c_str());
				    if(k%100000==0)
					cout<<"idDoc: "<<item.replace(item.size()-1,1, "")<<" "<<id<<endl;
					
				}
			    if(c==1)
				{
				//tokens.push_back( item ); 
				//IdTerms.insert( atoi( item.c_str()) );
				if(k%100000==0)
				cout << " "<<item<<endl;
				
				x = atoi(item.c_str());
				dimDocs[id] = x;
				//  cout<< C <<" + "<<x<<" = ";
			    C = C + x;
			    //cout<< C <<endl;
			    
			    if(dimDocs[id]==0)
			    contVacios++;
				}
				c++;
		    }

         }
	   k++;
		    
   }
   cout<<"K: "<< k << " C : "<<C<< "media: "<<C/k<<endl;	
   cout<<"VACIOS-------------------------------> "<<contVacios<<endl;	
   //exit(1);
}

  ofstream fileListBM25;

//cargar file indice
void leerIndiceCharBM25(const char *archivo/*,ListaC *listaStruct*/)
{
//	printf("Cargando archivo de indice invertido..\n");       
	ifstream listaInvertida(archivo,ifstream::in);
    list< pair<unsigned int, unsigned int> > *lista;
    string line;
	char *h;
	unsigned int posicion;        
    int idDato,conteoDatos,posicionLista=0;  
    char tmp;
	char buff[256];
	int i=0,numero,bandera=0;
    unsigned long long idTerm;
    int idDoc=0,UB=0,pesoDoc=0,numDocs=0;   
    unsigned int conteoLista=0;
    unsigned int dimLista;  
    int nLinea=0;
    double ub_term_temp = 0;
    char nbre2[256];
   while(getline(listaInvertida,line))
   {
   nLinea++;
   h = strdup(line.c_str());
   posicion=0;
   idDato=0;conteoDatos=0;posicionLista=0;
   lista=new list< pair<unsigned int, unsigned int> >();
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

			//Esta asignacion es ad-hoc a la estructura de la lista a utilizar...
		    if(i!=0)
		    {
            numero= atoi(buff);
		        if(idDato==0)
				{   
				idTerm=numero;
				/*	if(opcionTraza==2)
					{
						sprintf(nbre2,"/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/%d.BM25.dat",idTerm);
		
						
						fileListBM25.open(nbre2);
						fileListBM25<<
					}*/
				}
		        if(idDato==1)
		        {
		        UB=numero;
		        }
		        //no se utiliza, pero puede ser necesario para trazas de magnitud de listas
		        if(idDato==2)
		        {
		        numDocs=numero;
		        
		        }
		        
            //En caso de cambiar estructura de listas invertidas, modificar condiciones anteriores...
		    if(idDato>2)
		    {
				    
					if(conteoDatos%2!=0)//frec
					{pesoDoc=numero;
					lista->push_back(pair<unsigned int, unsigned int>(idDoc, pesoDoc)); //original
					
					//BM25
					if(opcionTraza==2)
					{
						 double b25 = BM25(numDocs,pesoDoc,idDoc,1.0);
						// cout<<b25<<endl;
						 if(ub_term_temp<b25)
					     ub_term_temp = b25;
					     //cout<<ub_term_temp<<endl;
					
					}
					//recuperar length doc
					
					//TF-IDF
					if(opcionTraza==3)
					{}
          			posicionLista++;
		    		}
					else//doc
					{idDoc=numero;
								
					}
		    	    conteoDatos++;
		    }
		
			idDato++;
			}//fin if i!=0
			
	}
	                        if(lista->size()>0)
                            {
								
								cout<<" --> UPPER_BOUNDS: "<<ub_term_temp<<endl;
								
                                listaStruct[0].indiceThread[idTerm] = new PostingStream(lista, ub_term_temp);
                                listaStruct[0].ubLocales[idTerm] = UB;
                             //   fileListBM25.close();
                            }
lista->clear();
delete lista;
//if(conteoLista%100==0)
//printf("Cargando termino %d\n",idTerm);
conteoLista++;   
   	free(h); 
	h = NULL;
	posicionLista++;        
   
          }      
		listaInvertida.close();


}


//cargar file indice
void leerIndiceChar(const char *archivo/*,ListaC *listaStruct*/)
{
//	printf("Cargando archivo de indice invertido..\n");       
	ifstream listaInvertida(archivo,ifstream::in);
    list< pair<unsigned int, unsigned int> > *lista;
    string line;
	char *h;
	unsigned int posicion;        
    int idDato,conteoDatos,posicionLista=0;  
    char tmp;
	char buff[256];
	int i=0,numero,bandera=0;
    unsigned long long idTerm;
    int idDoc=0,UB=0,pesoDoc=0;//,numDocs=0;   
    unsigned int conteoLista=0;
    unsigned int dimLista;  
    int nLinea=0;
   while(getline(listaInvertida,line))
   {
   nLinea++;
   h = strdup(line.c_str());
   posicion=0;
   idDato=0;conteoDatos=0;posicionLista=0;
   lista=new list< pair<unsigned int, unsigned int> >();
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

			//Esta asignacion es ad-hoc a la estructura de la lista a utilizar...
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
		        //no se utiliza, pero puede ser necesario para trazas de magnitud de listas
		        /*if(idDato==2)
		        {
		        numDocs=numero;
		        
		        }
		        */
            //En caso de cambiar estructura de listas invertidas, modificar condiciones anteriores...
		    if(idDato>2)
		    {
				    
					if(conteoDatos%2!=0)//frec
					{pesoDoc=numero;
					lista->push_back(pair<unsigned int, unsigned int>(idDoc, pesoDoc)); //original
          			posicionLista++;
		    		}
					else//doc
					{idDoc=numero;
								
					}
		    	    conteoDatos++;
		    }
		
			idDato++;
			}//fin if i!=0
			
	}
	                        if(lista->size()>0)
                            {
                                listaStruct[0].indiceThread[idTerm] = new PostingStream(lista, UB);
                                listaStruct[0].ubLocales[idTerm] = UB;
                              
                            }
lista->clear();
delete lista;
//if(conteoLista%100==0)
//printf("Cargando termino %d\n",idTerm);
conteoLista++;   
   	free(h); 
	h = NULL;
	posicionLista++;        
   
          }      
		listaInvertida.close();


}
//cargar file queries
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
		        terms=new unsigned long long[n_terms];
			    pesos=new unsigned int[n_terms];	
		        }
            
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
		    conteoDatos++;
		    }
		
			idDato++;
			}//fin if i!=0
			
	}
	if(i!=0)
	{
	querys->push_back(new DatosQuery(id_query, n_terms, terms, pesos));
	
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
}
//ejecucion Wand-querie
void WandQuery(DatosQuery* consulta,
              int consultaId,
              int topK,
              double umbral)
{
   
      unsigned int id_doc;
      vector<QueryFeatureInfoT*> *query_terms=new vector<QueryFeatureInfoT*>();
      vector<PostingStream*> *streams=new vector<PostingStream*>();
      QueryFeatureInfoT *info;
      PostingStream *stream;
      unsigned long long id_term;
      double valor;
      Wand *wand=NULL;
      ScorerT *scorer=NULL;
      //double threshold = umbral;
      double thresholdBM25 = (double)umbral;
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

      int nActualizaciones=0;
      // Tiempo ini preprocesamiento querie
              clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo1_PreQ);



        //BM25
 
               //pre-procesamiento querie 
		for(int i=0; i<n_terms; i++){
			//tomar datos de la consulta
            id_term=consulta->terminos[i];
            valor=(double)consulta->valores[i];
			it=listaStruct[0].indiceThread.find(id_term);
			if(it!=listaStruct[0].indiceThread.end()){
				//termino encontrado
				n_terms_encontrados++;
				stream=it->second;
				DOCS_SUMA = DOCS_SUMA + stream->n;
				info=new QueryFeatureInfoT(id_term, valor, stream->ub_inicial);
				info->ub=valor*stream->ub_inicial;
				query_terms->push_back(info);
				streams->push_back(stream);

          	}//if... termino encontrado
		}//for... cada termino de la consulta




		//Tiempo fin preprocesamiento pesos querie
		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo2_PreQ);

		//tiempo ini WAND
		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo1_Wand);

            int nSeleccionadosTopK=0;
            if(n_terms_encontrados>0){
			scorer=new ScorerT(streams, query_terms);
			wand=new Wand(n_terms_encontrados, scorer, streams);
        	wand->init();
            


				//Buscar doc candidato
				while((id_doc=wand->next(thresholdBM25)) != END_OF_STREAM)
				{
					//Score candidato seleccionado
					valor=wand->currentPayload;
					
					
					
					NEXT++;
					//cout<<valor<<" "<<threshold<<endl;
					//agregar al heap
					if(nSeleccionadosTopK<topK)
					{
						heapM->insert(valor,id_doc); //Propuesta M. Marin        
						nSeleccionadosTopK++;
					}
					else{
						min_thr = heapM->top();
						if(min_thr<valor){
						heapM->insert(valor,id_doc);
						//cout<<id_doc<<" "<<valor<<endl;
						nActualizaciones++;
						}
					}
					//Mantiene threshold inicial
					if(nSeleccionadosTopK<topK){
					thresholdBM25=(double)umbral;
					}
					else
					{
					//nuevo threshold	
					thresholdBM25 = heapM->top();
					}
					//cout<<thresholdBM25<<endl;
					

				}
	
     DOCS_PROCESADOS = wand->evaluacionesCompletas;
	 wand->~Wand(); 
     }
            //Tiempo fin WAND
              clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo2_Wand);
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
    UMBRAL_BM25=thresholdBM25;
}

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
    
      if(opcionTraza==1)
      {
		  thersholdLogConsulta<<i;
    	  dimDocsQuerie(listaStruct[0].consultas[i]);
    	  thersholdLogConsulta<<" "<<DOCS_SUMA<<" "<<DOCS<<" "<<listaStruct[0].consultas[i]->n_terminos;
    	  dimDocsQuerieSize(listaStruct[0].consultas[i]);
      }
      //Wand TF
      else if(opcionTraza==0)
      {

      
      clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo1_ProcQ); //
	  WandQuery(listaStruct[0].consultas[i],i,K,threshold);//Umbral inicial
	  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo2_ProcQ);


      thersholdLogConsulta<<i<<" "<<HEAP_SIZE<<" "<<DOCS_SUMA<<" "<<DOCS_PROCESADOS<<" "<<NEXT<<" "<<HEAP_UPDATES<<" "<<UMBRAL_BM25;

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
      // Wand BM25
      else if(opcionTraza==2)
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
			  WandQuery(listaStruct[0].consultas[i],i,p,threshold);//Umbral inicial
			  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tiempo2_ProcQ);

			  logWand[tmp]<<i<<" "<<HEAP_SIZE<<" "<<DOCS_SUMA<<" "<<DOCS_PROCESADOS<<" "<<NEXT<<" "<<HEAP_UPDATES<<" "<<UMBRAL_BM25;

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
      // Wand TF-IDF
      else if(opcionTraza==3)
      {
		 // thersholdLogConsulta<<endl;
	  }
      else
      {
		  cout << "Opcion no reconocida..." << opcionTraza << endl;
		  exit(0);
		  
	  }
    
	}


}


void leerConsultasCharJump(const char *archivo,vector < DatosQuery* > *querys,int nQ,int salto)
{   vector<int> vectorTemporal;
	vector<int> vectorTemporalQueriesID;
	set<int> termsLote;
	vector<DatosQuery*> querysT;	
	contSetQueries = 0;
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
    int inicial = 0;
   int finalQ;
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
		        terms=new unsigned long long[n_terms];
			    pesos=new unsigned int[n_terms];	
		        }
            
		    if(idDato>0)
		    {
		    modulo = conteoDatos%2;
		    if(modulo==0)
		    {id_term=numero;
		    terms[posicionConsulta]=id_term;
		    
		    
		    if(termsLote.count(id_term)==0)
		    {
				termsLote.insert(id_term);
				vectorTemporal.push_back(id_term);
			}
		    	if(termsGlobalFrec.count(id_term)==0)
				{
				termsGlobalFrec[id_term]=1;
				
				}
				else
				{
				termsGlobalFrec[id_term] = termsGlobalFrec[id_term]+1;
				}
		    
		    
		    }
		    else
		    {valor=numero;
		    pesos[posicionConsulta]=valor;
		    posicionConsulta++;
		    }
		    conteoDatos++;
		    }
		
			idDato++;
			}//fin if i!=0
			
	}
	if(i!=0)
	{
	querys->push_back(new DatosQuery(id_query, n_terms, terms, pesos));
	querysT.push_back(new DatosQuery(id_query, n_terms, terms, pesos));
	vectorTemporalQueriesID.push_back(id_query);
	finalQ = id_query;
    delete [] terms;
	delete [] pesos;
    }
   
   	free(h); 
	h = NULL;
	idConsulta++;
if(idConsulta==nQ)
{
	break;
}


if(idConsulta%salto==0 && idConsulta > 0)
{
	punterosIniciales.push_back(inicial);
	punterosFinales.push_back(finalQ);
	//cout<<"Salto queries: "<<contSetQueries<<endl;
	termsblocksqueries[contSetQueries] = vectorTemporal;
	idQueriesBlock[contSetQueries]=vectorTemporalQueriesID;
	vQ[contSetQueries]=querysT;
	//	cout<<"size1 queries: "<<querysT.size()<<endl;
	//cout<<"size2 queries: "<<vQ[contSetQueries].size()<<endl;
	contSetQueries++;
	termsLote.clear();
	vectorTemporal.clear();
	//vectorTemporalQueriesID.clear();
	querysT.clear();
   // cout<<"size3 queries: "<<vQ[contSetQueries].size()<<endl;
    inicial=idConsulta;
}

}
if(vectorTemporal.size()>0)
{		
	punterosIniciales.push_back(inicial);
	punterosFinales.push_back(finalQ);
	//cout<<"Salto final: "<<contSetQueries<<endl;
	termsblocksqueries[contSetQueries] = vectorTemporal;
	idQueriesBlock[contSetQueries]=vectorTemporalQueriesID;
	vQ[contSetQueries]=querysT;
	//	cout<<"size1 queries: "<<querysT.size()<<endl;
	//cout<<"size2 queries: "<<vQ[contSetQueries].size()<<endl;
	contSetQueries++;
	termsLote.clear();
	vectorTemporal.clear();
	//vectorTemporalQueriesID.clear();
	querysT.clear();
	//	cout<<"size3 queries: "<<vQ[contSetQueries].size()<<endl;
		inicial=idConsulta;
}

consultas.close();
}
//ejecucion Wand-querie
void leerIndiceCharJump(const char *archivo,ListaC *listaStruct)
{
	printf("Cargando archivo de indice invertido..\n");       
	ifstream listaInvertida(archivo,ifstream::in);
    list< pair<unsigned int, unsigned int> > *lista;
    string line;
	char *h;
	unsigned int posicion;        
    int idDato,conteoDatos,posicionLista=0;  
    char tmp;
	char buff[256];
	int i=0,numero,bandera=0;
    unsigned long long idTerm;
    int idDoc=0,UB=0,pesoDoc=0;//,numDocs=0;   
    unsigned int conteoLista=0;
    unsigned int dimLista;  
    int nLinea=0;
   while(getline(listaInvertida,line))
   {
   nLinea++;
   h = strdup(line.c_str());
   posicion=0;
   idDato=0;conteoDatos=0;posicionLista=0;
   lista=new list< pair<unsigned int, unsigned int> >();
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

			//Esta asignacion es ad-hoc a la estructura de la lista a utilizar...
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
		        //no se utiliza, pero puede ser necesario para trazas de magnitud de listas
		        /*if(idDato==2)
		        {
		        numDocs=numero;
		        
		        }
		        */
            //En caso de cambiar estructura de listas invertidas, modificar condiciones anteriores...
		    if(idDato>2)
		    {
				    
					if(conteoDatos%2!=0)//frec
					{pesoDoc=numero;
					lista->push_back(pair<unsigned int, unsigned int>(idDoc, pesoDoc)); //original
          			posicionLista++;
		    		}
					else//doc
					{idDoc=numero;
								
					}
		    	    conteoDatos++;
		    }
		
			idDato++;
			}//fin if i!=0
			
	}
	                        if(lista->size()>0)
                            {
                                listaStruct[0].indiceThread[idTerm] = new PostingStream(lista, UB);
                                listaStruct[0].ubLocales[idTerm] = UB;
                              
                            }
lista->clear();
if(conteoLista%100==0)
printf("Cargando termino %d\n",conteoLista);
conteoLista++;   
   	free(h); 
	h = NULL;
	posicionLista++;        
   
          }      
		listaInvertida.close();


} 
void mostrarTermsLotes()
{    for(int i=0;i<contSetQueries;i++)
	{
	 vector<int> tmp = termsblocksqueries[i];
	 for (int j=0;j<tmp.size();j++)
     {cout << " "<<tmp[j];}
     cout<<endl;
    }
	
} 

void cargarConjuntoListasR(int i)
{   char nbre[256];
	vector<int> tmp = termsblocksqueries[i];
	for(int j=0;j<tmp.size();j++)
	{   cout<<" .. "<<j;
		sprintf(nbre,"/mnt/DataWeb/indices/ClueWebIndex/completaWC09/invertido/invertidoLogTrec09/partitions/%d.dat",tmp[j]);
		
		if(opcionTraza==2)
		leerIndiceCharBM25(nbre);
		else
		leerIndiceChar(nbre);
		
		if(j%5==0)
		cout<<endl;
	}
	
}
 
void cargarConjuntoListas(int i)
{   char nbre[256];
	vector<int> tmp = termsblocksqueries[i];
	int t;
	for(int j=0;j<tmp.size();j++)
	{   cout<<" .. "<<j;
		sprintf(nbre,"/mnt/DataWeb/indices/ClueWebIndex/completaWC09/invertido/invertidoLogTrec09/partitions/%d.dat",tmp[j]);
		//leerIndiceChar(nbre);
		t = tmp[j];
		if(conteoTermsGlobalFrec.count(t)==0)
		{
		conteoTermsGlobalFrec[t]=1;
		if(opcionTraza==2)
		leerIndiceCharBM25(nbre);
		else
		leerIndiceChar(nbre);
		//if(j%5==0)
		cout<<" --> "<<tmp.size()<<" --> frec: "<<termsGlobalFrec[tmp[j]] <<endl;
	    }
	    else
	    {//termsGlobalFrec
			if(conteoTermsGlobalFrec[t]<1)
			{
			 if(opcionTraza==2)
			 leerIndiceCharBM25(nbre);
			 else
			 leerIndiceChar(nbre);
		     //if(j%5==0)
		     cout<<" --> "<<tmp.size()<<" --> frec: "<<termsGlobalFrec[tmp[j]] <<endl;
	         }
	         else
	         cout<<"EN CACHE --> "<<tmp.size()<<" --> frec: "<<termsGlobalFrec[tmp[j]] <<endl;
		}
	    
	}
	
} 
 
//inicio 
int main (int argc, char *argv[]) {
	AsignaCPUThread(2);
//int tmpK = atoi( argv[1]); //Size top k
nombreArchivoConsultas = argv[1];//file queries
//
numeroConsultasProceso = atoi(argv[2]);//N de consultas a procesar
int querierForJumpFree = atoi(argv[3]);//file topK
//nombreArchivoLog = argv[4];//file indice invertido
//unsigned int umbral = 0; //defecto para este experimento
//K=atoi(argv[5]);

opcionTraza = atoi(argv[4]);
cargaDocs();

//pFileTopk = fopen("./trazaTopKBM25","w");
logWand[0].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K10_BM25.txt");
logWand[1].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K100_BM25.txt");
logWand[2].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K1000_BM25.txt");
logWand[3].open ("/mnt/DataWeb/procesamiento/wand_setup/wand_experiments/src/trazas/logsTmp/costos_20000_S250_WandClueTrec09_K10000_BM25.txt");
printf("...Cargando Consultas...\n");
leerConsultasCharJump(nombreArchivoConsultas,&querys,numeroConsultasProceso,querierForJumpFree);
printf("...Fin carga Consultas...\n");
cout<<"---------- "<<contSetQueries<<endl;
//mostrarTermsLotes();
for(int m=0;m<contSetQueries;m++)
{   printf("Lote ---------> %d.\n",m);

	//carga de conjunto de listas
	cargarConjuntoListas(m);
	
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
     listaStruct[0].liberarRAM_P();
}
//int t=0;
//listaStruct[t].tid=t;
//listaStruct[t].nthreads=numThreads;
//listaStruct[t].consultas=querys;
//listaStruct[t].umbralInicial=umbral;

printf("Inicio carga indice invertido...\n");
//leerIndiceChar(nombreArchivoLista,listaStruct);
printf("Fin carga indice invertido...\n");

//llamada a ejecutar Wand con N consultas
//callWand(&listaStruct[0]);
//fflush(pFileTopk);
//fclose(pFileTopk);
logWand[0].close();
logWand[1].close();
logWand[2].close();
logWand[3].close();
exit(0);

}
