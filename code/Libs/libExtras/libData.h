#ifndef LIBDATA_H_
#define LIBDATA_H_

#include "./estructfloat.h"
vector < pair<float, unsigned int> > HEAP;//heap que almacena topK
map<int,int> termsGlobalFrec;
map<int,int> conteoTermsGlobalFrec;
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
int contSetQueries;
//vector<DatosQuery*> querys;	

vector<DatosQuery*> querys;			//consultas
map<int,vector<DatosQuery*> > vQ;
void leerIndiceCharGrupos(const char *archivo, int opcionW, int tidThreadPadre,int numThreadsWand);
void densidadesLista(const char *archivo, int opcionW, int tidThreadPadre,int numThreadsWand);
class ListaInvertida
{
  private:

 
   public:
   int tid;
   int nthreads;
   map<unsigned long long, PostingStream* > indiceThread;
   map<unsigned long long, PostingStream* > indiceThreadIntervalos;//lista completa
   
   vector < DatosQuery* > consultas;
   //int prueba;
   int umbralInicial;
   map<unsigned long long,float> ubLocales;

   ListaInvertida(){}
   ListaInvertida(int p,int pr)
   {
     tid  = p;
     nthreads = pr;
     
   }
   unsigned int ub_local(PostingStream *entrada){return 1;}
   
   void liberarRAM()
   {   
	   for (std::map<unsigned long long, PostingStream*>::iterator it=indiceThread.begin(); it!=indiceThread.end(); ++it)
       it->second->~PostingStream();
     //  for (std::map<unsigned long long, PostingStream*>::iterator it=indiceThreadIntervalos.begin(); it!=indiceThreadIntervalos.end(); ++it)
	 //  it->second->~PostingStream();
	   cout<<"------------------ANTES------------->>> "<<indiceThread.size()<<endl;
	   indiceThread.clear();
	 //  indiceThreadIntervalos.clear();
	   ubLocales.clear();
	   cout<<"------------------DESPUES------------->>> "<<indiceThread.size()<<endl;
   }

   void liberarRAM_P()
   {     
	 //  for (std::map<unsigned long long, PostingStream*>::iterator it=indiceThread.begin(); it!=indiceThread.end(); ++it)
     //  it->second->~PostingStream();
       
       for (std::map<unsigned long long, PostingStream*>::iterator it=indiceThread.begin(); it!=indiceThread.end(); ++it)
	   {
		   termsGlobalFrec[it->first] = termsGlobalFrec[it->first]-1;
	   
		   if(termsGlobalFrec[it->first]<1) 
		   {
			  it->second->~PostingStream();
			  termsGlobalFrec[it->first] = 0;
			  conteoTermsGlobalFrec[it->first]=0;
		   }
	  }
	  
   }
   
   void liberarRAM_Blocks()
   {     
	 //  for (std::map<unsigned long long, PostingStream*>::iterator it=indiceThread.begin(); it!=indiceThread.end(); ++it)
     //  it->second->~PostingStream();
       
       for (std::map<unsigned long long, PostingStream*>::iterator it=indiceThreadIntervalos.begin(); it!=indiceThreadIntervalos.end(); ++it)
	   {
		   termsGlobalFrec[it->first] = termsGlobalFrec[it->first]-1;
	   
		   if(termsGlobalFrec[it->first]<1) 
		   {
			  it->second->~PostingStream();
			  
			  termsGlobalFrec[it->first] = 0;
			  conteoTermsGlobalFrec[it->first]=0;
		   }
	  }
	  
   }
   
      void liberarRAM_BlocksTodo()
   {     
	 //  for (std::map<unsigned long long, PostingStream*>::iterator it=indiceThread.begin(); it!=indiceThread.end(); ++it)
     //  it->second->~PostingStream();
       
       for (std::map<unsigned long long, PostingStream*>::iterator it=indiceThreadIntervalos.begin(); it!=indiceThreadIntervalos.end(); ++it)
	   {
		  // termsGlobalFrec[it->first] = termsGlobalFrec[it->first]-1;
	   
		  // if(termsGlobalFrec[it->first]<1) 
		  // {
			  it->second->~PostingStream();
			  
			//  termsGlobalFrec[it->first] = 0;
			//  conteoTermsGlobalFrec[it->first]=0;
		  // }
	  }
	  
   }
   
//cargar file indice
void cargaIndice(const char *archivo,int opcionW/*,ListaC *listaStruct*/)
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
    int idDoc=0,pesoDoc=0,numDocs=0;   
    unsigned int conteoLista=0;
    unsigned int dimLista;  
    int nLinea=0;
    float ub_term_temp = 0.0,UB=0.0;
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
		        float w_term,idf_tmp;
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
		        cout<<"numDocs:"<<numDocs<<endl;
		        }
		        
            //En caso de cambiar estructura de listas invertidas, modificar condiciones anteriores...
		    if(idDato>2)
		    {
				    
					if(conteoDatos%2!=0)//frec
					{pesoDoc=numero;
					lista->push_back(pair<unsigned int, unsigned int>(idDoc, pesoDoc)); //original
			
						//TF
						if(opcionW==1) 
						{
							
							 
							 //TF
							 w_term = (float)pesoDoc;
							 // cout<<w_term<<endl;
							 if(ub_term_temp<w_term)
							 ub_term_temp = w_term;
							 //cout<<ub_term_temp<<endl;
						     UB = w_term;
						}
						//BM25
						if(opcionW==2) 
						{
							 //IDF.BM25
							 w_term = BM25(pesoDoc,idDoc);
							 // cout<<w_term<<endl;
							 if(ub_term_temp<w_term)
							 ub_term_temp = w_term;
							 //cout<<ub_term_temp<<endl;
							 
							 UB = w_term;
						
						}
						//recuperar length doc
						//TF/sizeDoc
						if(opcionW==3)
						{
					//	if(sizeDoc(idDoc)==0)
					//	cout<<"Cero:"<<idDoc<<" Frec:"<<pesoDoc<<endl;
						w_term	= (float)(pesoDoc)/sizeDoc(idDoc);
					    if(ub_term_temp<w_term)
					    ub_term_temp = w_term;
						UB = w_term;
						}
						
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
								
                                //listaStruct[tid].
                                indiceThread[idTerm] = new PostingStream(lista, ub_term_temp);
                                //listaStruct[tid].
                                ubLocales[idTerm] = UB;
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



void cargaIndiceBloques(const char *archivo,int opcionW)
{
 //printf("Cargando archivo..\n");
 ifstream listaInvertida(archivo,ifstream::in);
  //  list< pair<unsigned int, unsigned int> > *lista[nThreads];
    list< pair<unsigned int, unsigned int> > *lista;
   // list< pair<unsigned int, unsigned int> > *lista[n;
    string line;
	char *h;
	unsigned int posicion;
    int idDato,conteoDatos,posicionLista=0;
    char tmp;
	char buff[256];
	int i=0,numero,bandera=0;
    unsigned long long idTerm=0;
    int idDoc=0,pesoDoc=0;//,numDocs=0;
    unsigned int conteoLista=0;
    unsigned int dimLista;
    //bLocal[i]=0;
           int nLinea=0,numDocs;

   // int UB_High;//, UB_Low;
    int contadorBloque;
    vector <float> pesosDocumentos;
    vector <unsigned int> fronterasIdIniBlock;
    vector <unsigned int> fronterasIdFinBlock;
    vector <float> ubParticiones;
    //vector <unsigned int> :: const_iterator UbTmp;
    float UbTmp,UB=0.0;
    unsigned int docFin;
while(getline(listaInvertida,line))
{
nLinea++;
//cout<<" Lista: " <<nLinea<<endl;
   h = strdup(line.c_str());
   posicion=0;
   idDato=0;conteoDatos=0;posicionLista=0;
   lista=new list< pair<unsigned int, unsigned int> >();
   contadorBloque=0;

   dimLista=strlen(h);
   while(posicion<dimLista)
		{
		        i=0;
 float w_term,idf_tmp;
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
		        if(idDato==2)
		        {
		        numDocs=numero;
		        cout<<"numDocs:"<<numDocs<<endl;
		        }
            //datos 1 y 2 no se consideran ya que se van a calcular nuevamente UB y #docs, solo en version multithreading
		    if(idDato>2)
		    {
					if(conteoDatos%2!=0)//frec
					{
						pesoDoc=numero;
						lista->push_back(pair<unsigned int, unsigned int>(idDoc, pesoDoc)); //original

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

						//TF
						if(opcionW==1) 
						{
							 //TF
							 w_term = (float)pesoDoc;
					    }
						//BM25
						if(opcionW==2) 
						{
							 //IDF.BM25
							 w_term = BM25(pesoDoc,idDoc);
						}
						//recuperar length doc
						//TF/sizeDoc
						if(opcionW==3)
						{
					//	if(sizeDoc(idDoc)==0)
					//	cout<<"Cero:"<<idDoc<<" Frec:"<<pesoDoc<<endl;
							w_term	= (float)(pesoDoc)/sizeDoc(idDoc);
						}
						
                        pesosDocumentos.push_back(w_term);
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

if(lista->size()>0)
{

// pthread_mutex_lock( &lock );
//listaStruct[0].ubLocales[idTerm] = UB;

//en la clase postingstream colocar punteros...
UB = *( std:: max_element(ubParticiones.begin(),ubParticiones.end()));
cout<<"Term:"<<idTerm<<" ub:"<<UB<<endl;
indiceThreadIntervalos[idTerm] = new PostingStream(lista,ubParticiones,fronterasIdIniBlock,fronterasIdFinBlock,UB);
//pthread_mutex_unlock( &lock );


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
cout<<" Lista: " <<nLinea<<" size: "<<lista->size()<<endl;
lista->clear();

          }

		listaInvertida.close();
    //            exit(0);

}
 
void cargarConjuntoListasBMWUBS(int i,int metodoDAAT,int opcionW,int tidPadre, int numThreadsWand)
{   char nbre[256];
	vector<int> tmp = termsblocksqueries[i];
	int t;
	for(int j=0;j<tmp.size();j++)
	{   cout<<" .. "<<j;
		//CLUEWEB09	
		sprintf(nbre,"/mnt/DataWeb/indices/ClueWebIndex/completaWC09/invertido/invertidoLogTrec09/partitions/%d.dat",tmp[j]);
		//leerIndiceChar(nbre);
		t = tmp[j];
		if(conteoTermsGlobalFrec.count(t)==0)
		{
		conteoTermsGlobalFrec[t]=1;
		if(metodoDAAT==1)//DAAT 
		cargaIndice(nbre,opcionW);
	    if(metodoDAAT==2)//DAAT Blocks
		{cout<<" --> "<<tmp.size()<<" --> frec: "<<termsGlobalFrec[tmp[j]] <<endl;
	
		//leerIndiceCharGrupos(nbre,opcionW,tidPadre,numThreadsWand);
		densidadesLista(nbre,opcionW,tidPadre,numThreadsWand);
		
		}
		
		
		
		
		
		//
		
		
		
		//if(j%5==0)
		cout<<" --> "<<tmp.size()<<" --> frec: "<<termsGlobalFrec[tmp[j]] <<endl;
	    }
	    else
	    {//termsGlobalFrec
			if(conteoTermsGlobalFrec[t]<1)
			{
			 if(metodoDAAT==1)//DAAT 
			 cargaIndice(nbre,opcionW);
			 if(metodoDAAT==2)//DAAT Blocks
			 {cout<<" --> "<<tmp.size()<<" --> frec: "<<termsGlobalFrec[tmp[j]] <<endl;
		     //cargaIndiceBloques(nbre,opcionW);
		     densidadesLista(nbre,opcionW,tidPadre,numThreadsWand);
			 }
		     //if(j%5==0)
		     cout<<" --> "<<tmp.size()<<" --> frec: "<<termsGlobalFrec[tmp[j]] <<endl;
	         }
	         else
	         cout<<"EN CACHE --> "<<tmp.size()<<" --> frec: "<<termsGlobalFrec[tmp[j]] <<endl;
		}
	    
	}
	
} 
 


void cargarConjuntoListasBMWMultithread(int i,int metodoDAAT,int opcionW,int tidPadre, int numThreadsWand)
{   char nbre[256];
	vector<int> tmp = termsblocksqueries[i];
	int t;
	for(int j=0;j<tmp.size();j++)
	{   cout<<" .. "<<j;
		//CLUEWEB09	
		sprintf(nbre,"/mnt/DataWeb/indices/ClueWebIndex/completaWC09/invertido/invertidoLogTrec09/partitions/%d.dat",tmp[j]);
		//leerIndiceChar(nbre);
		t = tmp[j];
	//	if(conteoTermsGlobalFrec.count(t)==0)
	//	{
		conteoTermsGlobalFrec[t]=1;
		if(metodoDAAT==1)//DAAT 
		cargaIndice(nbre,opcionW);
		
		
		if(metodoDAAT==2)//DAAT Blocks
		{cout<<" --___________> "<<tmp.size()<<" --> frec: "<<termsGlobalFrec[tmp[j]] <<endl;
	
		leerIndiceCharGrupos(nbre,opcionW,tidPadre,numThreadsWand);
		//densidadesLista(nbre,opcionW,tidPadre,numThreadsWand);
		
		}

		/*
		
		
		
		
		//
		
		
		
		//if(j%5==0)
		cout<<" --> "<<tmp.size()<<" --> frec: "<<termsGlobalFrec[tmp[j]] <<endl;
	    }
	    else
	    {//termsGlobalFrec
			if(conteoTermsGlobalFrec[t]<1)
			{
			 if(metodoDAAT==1)//DAAT 
			 cargaIndice(nbre,opcionW,tidPadre);
			 if(metodoDAAT==2)//DAAT Blocks
			 {cout<<" XXXXXXXXXXXXXXXXXXXX--> "<<tmp.size()<<" --> frec: "<<termsGlobalFrec[tmp[j]] <<endl;
		     //cargaIndiceBloques(nbre,opcionW);
		     leerIndiceCharGrupos(nbre,opcionW,tidPadre,numThreadsWand);
			 }
		     //if(j%5==0)
		     cout<<" XXXXXXXXXXXXXXXXX--> "<<tmp.size()<<" --> frec: "<<termsGlobalFrec[tmp[j]] <<endl;
	         }
	         else
	         cout<<"XXXXXXXXXXXXXXXXXXXXXXEN CACHE --> "<<tmp.size()<<" --> frec: "<<termsGlobalFrec[tmp[j]] <<endl;
		}
		
		*/
	    
	}
	
} 
 
void cargarConjuntoListas(int i,int metodoDAAT,int opcionW)
{   char nbre[256];
	vector<int> tmp = termsblocksqueries[i];
	int t;
	for(int j=0;j<tmp.size();j++)
	{   cout<<" .. "<<j;
		//CLUEWEB09	
		sprintf(nbre,"/mnt/DataWeb/indices/ClueWebIndex/completaWC09/invertido/invertidoLogTrec09/partitions/%d.dat",tmp[j]);
		//leerIndiceChar(nbre);
		t = tmp[j];
		if(conteoTermsGlobalFrec.count(t)==0)
		{
		conteoTermsGlobalFrec[t]=1;
		if(metodoDAAT==1)//DAAT 
		cargaIndice(nbre,opcionW);
		if(metodoDAAT==2)//DAAT Blocks
		{cout<<" --> "<<tmp.size()<<" --> frec: "<<termsGlobalFrec[tmp[j]] <<endl;
		cargaIndiceBloques(nbre,opcionW);
		}
		
		//leerIndiceCharGrupos(nbre,,int tidThread, int opcionW)
		
		
		
		//if(j%5==0)
		cout<<" --> "<<tmp.size()<<" --> frec: "<<termsGlobalFrec[tmp[j]] <<endl;
	    }
	    else
	    {//termsGlobalFrec
			if(conteoTermsGlobalFrec[t]<1)
			{
			 if(metodoDAAT==1)//DAAT 
			 cargaIndice(nbre,opcionW);
			 if(metodoDAAT==2)//DAAT Blocks
			 {cout<<" --> "<<tmp.size()<<" --> frec: "<<termsGlobalFrec[tmp[j]] <<endl;
		     cargaIndiceBloques(nbre,opcionW);
			 }
		     //if(j%5==0)
		     cout<<" --> "<<tmp.size()<<" --> frec: "<<termsGlobalFrec[tmp[j]] <<endl;
	         }
	         else
	         cout<<"EN CACHE --> "<<tmp.size()<<" --> frec: "<<termsGlobalFrec[tmp[j]] <<endl;
		}
	    
	}
	
} 
};
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
		float *pesos=NULL;
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
			    pesos=new float[n_terms];	
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
		    pesos[posicionConsulta]=(float)valor;
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

void EscribirVectorRespuesta(vector< pair<float,unsigned int > > respuesta, int dim,int idQ)
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
void cargaDocsMultithread(int numTreadsPadre)
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
	   //  if(k==100001)
	   //  exit(1);
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
				//	cout<<"item: "<<item<<" ";
				//cout<<"idDoc: "<<item.replace(item.size()-1,1, "");
				
				 string::size_type pos = item.find(":", 0);
				 if (pos < string::npos)
                 item.replace(pos, string(":").length(), "");
                 
					//id = atoi(item.replace(item.size()-1,1, "").c_str());
					id = atoi(item.c_str());
					if(k%100000==0)
					cout<<"idDoc: "<<" "<<id;
				}
			    if(c==1)
				{
				//tokens.push_back( item ); 
				//IdTerms.insert( atoi( item.c_str()) );
				if(k%100000==0)
				cout << " "<<item<<endl;
				
				x = atoi(item.c_str());
				
				int modulo  = x%numTreadsPadre;
				dimDocsThreads[modulo][id] = x;
			//	dimDocs[id] = x;
				//  cout<< C <<" + "<<x<<" = ";
			    C = C + x;
			    //cout<< C <<endl;
			    
			    if(dimDocsThreads[modulo][id]==0)
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
void cargaDocsParticiones()
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
	   //  if(k==100001)
	   //  exit(1);
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
				//	cout<<"item: "<<item<<" ";
				//cout<<"idDoc: "<<item.replace(item.size()-1,1, "");
				
				 string::size_type pos = item.find(":", 0);
				 if (pos < string::npos)
                 item.replace(pos, string(":").length(), "");
                 
					//id = atoi(item.replace(item.size()-1,1, "").c_str());
					id = atoi(item.c_str());
					if(k%100000==0)
					cout<<"idDoc: "<<" "<<id;
				}
			    if(c==1)
				{
				//tokens.push_back( item ); 
				//IdTerms.insert( atoi( item.c_str()) );
				if(k%100000==0)
				cout << " "<<item<<endl;
				
				x = atoi(item.c_str());
		//		dimDocs[id] = x;
				
				dimDocsParticion[x%8][id] = x;
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
	   //  if(k==100001)
	   //  exit(1);
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
				//	cout<<"item: "<<item<<" ";
				//cout<<"idDoc: "<<item.replace(item.size()-1,1, "");
				
				 string::size_type pos = item.find(":", 0);
				 if (pos < string::npos)
                 item.replace(pos, string(":").length(), "");
                 
					//id = atoi(item.replace(item.size()-1,1, "").c_str());
					id = atoi(item.c_str());
					if(k%100000==0)
					cout<<"idDoc: "<<" "<<id;
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
    float *pesos=NULL;
    int modulo,conteoDatos;	
    unsigned int dimString;
    int inicial = 0;
    int finalQ=0;
    int contP = 0;
    int ini,fin;
   while(getline(consultas,line))
   {
	   
   i=0;
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
			    pesos=new float[n_terms];	
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

void leerConsultasCharJumpBlocksQueries(const char *archivo,vector < DatosQuery* > *querys,int nQ,int salto,int ini,int fin)
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
    float *pesos=NULL;
    int modulo,conteoDatos;	
    unsigned int dimString;
    int inicial = 0;
    int finalQ = 0;
    int contP = 0;
    
   while(getline(consultas,line))
   {
	   if((contP<ini)||(contP>fin) )
	   {
		   //cout<<"No agregada: "<<contP<<endl; 
		   contP++;     
	   continue;
       }
       else
       {
		   contP++;
		//cout<<"agregada: "<<contP<<endl;   
	   }
	   
	   
      
   i=0;
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
			    pesos=new float[n_terms];	
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
	id_query=contP-1;
	querys->push_back(new DatosQuery(id_query, n_terms, terms, pesos));
	querysT.push_back(new DatosQuery(id_query, n_terms, terms, pesos));
	vectorTemporalQueriesID.push_back(id_query);
	finalQ = idConsulta;
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

void mostrarTermsLotes()
{    for(int i=0;i<contSetQueries;i++)
	{
	 vector<int> tmp = termsblocksqueries[i];
	 for (int j=0;j<tmp.size();j++)
     {cout << " "<<tmp[j];}
     cout<<endl;
    }
	
} 

#endif
