//./gl.o 
// /mnt/DataWeb/procesamiento/predictor42/descriptores/descriptoresGlasgow
// /mnt/DataWeb/indices/ClueWebIndex/completaWC09/invertido/indiceInvertido.txt 


#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include "/mnt/DataWeb/procesamiento/wand_setup/Wand.h"
#include "/mnt/DataWeb/procesamiento/wand_setup/estruc.h"
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
using namespace std;
char *nombreArchivoConsultas;	
vector<DatosQuery*> querys;	
map<int,vector<double> > descriptoresLista;
map<int,vector<double> > descriptoresQuerie;
//vector<double> w_t;
double sumaValores();
int sizeDesciptor;
void agregadosQuerie(DatosQuery* consultaIn);
double sumaValores(vector<double> w_t);
double mediaAritmetica(vector<double> w_t);
void Tokenize(const string& str,
                    vector<string>& tokens,
                    const string& delimiters = " ")
{
  string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  string::size_type pos     = str.find_first_of(delimiters, lastPos);
  while (string::npos != pos || string::npos != lastPos)
  {
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    lastPos = str.find_first_not_of(delimiters, pos);
    pos = str.find_first_of(delimiters, lastPos);
  }
}
void llamadaAgregados(vector < DatosQuery* > querys)
{
unsigned int contador=0;

 //printf("\tMostrando Consulta:\n");

    while(contador<querys.size())
    {
       // printf("Consulta[%d]:[%d]: ",contador,querys[contador]->n_terminos);
        agregadosQuerie(querys[contador]);
     //   printf("\n");
        contador++;
    }
}

void mostrarMatrizTraspuesta(vector< vector< double> > entrada)
{
	    for(int i=0;i<sizeDesciptor;i++)
	    {
			for(int j=0;j<entrada.size();j++)
			{
				cout<<" "<<entrada[j][i];
			}
			cout<<endl;
	    }
}

void mostrarMatriz(vector< vector< double> > entrada)
{
	    for(int i=0;i<entrada.size();i++)
	    {
			for(int j=0;j<entrada[i].size();j++)
			{
				cout<<" "<<entrada[i][j];
			}
			cout<<endl;
	    }
}

vector<double> suma(vector< vector< double> > entrada)
{
vector<double> tmp;
double tmpSuma = 0;
for(int i=0;i<sizeDesciptor;i++)
tmp.push_back(0);
//cout<<"sumando: "<<endl;
	    for(int i=0;i<entrada.size();i++)
	    {
			for(int j=0;j<entrada[i].size();j++)
			{
				tmp[j]+=entrada[i][j];
				
			//	cout<<" "<<tmp[j];
			}
		//	cout<<endl;
		}

return tmp;	
}
double varianzaW(vector<double> w_t)
{
	double Z = (double)w_t.size();
	double suma = 0.0;
	double tmp,tmp2;
	double media = mediaAritmetica(w_t);
	for(int i=0;i<w_t.size();i++)
	{
		
		tmp = (double)w_t[i]-media;
	    tmp2 = pow(tmp,2);
	    suma = suma + tmp2;
	    
	}
	return suma/Z;
}

double sumaValores(vector<double> w_t)
{
	
	double suma = 0;
	for(int i=0;i<w_t.size();i++)
	suma+=w_t[i];
	return suma;
	
}

double mediaAritmetica(vector<double> w_t)
{
return	(double)(sumaValores(w_t)/(double)w_t.size());
}

vector<double> var(vector< vector< double> > entrada)
{
//cout<<"Varianza: "<<endl;
vector<double> tmp;
vector<double> valores;
double varTmp;

        for(int i=0;i<sizeDesciptor;i++)
	    {
			for(int j=0;j<entrada.size();j++)
			{
			//	cout<<" "<<entrada[j][i];
				tmp.push_back(entrada[j][i]);
			}
			//cout<<endl;
			varTmp = varianzaW(tmp);
		//	cout<<" "<< varTmp;
			valores.push_back(varTmp);
			tmp.clear();
	    }

//cout<<endl;

return valores;	
}

vector<double> maxi(vector< vector< double> > entrada)
{
vector<double> tmp ;
double tmpSuma = 0;
for(int i=0;i<sizeDesciptor;i++)
tmp.push_back(0);
//cout<<"\n maximos: "<<endl;
	    for(int i=0;i<entrada.size();i++)
	    {
			for(int j=0;j<entrada[i].size();j++)
			{
				if(tmp[j]<entrada[i][j])
				tmp[j]=entrada[i][j];
				
			//	cout<<" "<<tmp[j];
			}
			//cout<<endl;
		}

return tmp;	
}


vector<double> fusionInfo(vector< double> a,vector< double> b,vector< double> c)
{
	vector<double> v;
	for(int j=0;j<a.size();j++)
	v.push_back(a[j]);
	for(int j=0;j<b.size();j++)
	v.push_back(b[j]);
	for(int j=0;j<c.size();j++)
	v.push_back(c[j]);
	
	return v;
}
void agregadosQuerie(DatosQuery* consultaIn)
{
 //printf("\nConsulta\n:");
 vector< vector< double> > agregados;
 vector<double> tmp;
       for(int j=0;j<consultaIn->n_terminos;j++)
       {
			int id = consultaIn->terminos[j];
			agregados.push_back(descriptoresLista[id]);
          //  printf(" [%d][%llu] -",consultaIn->valores[j],);
            
           // tmp.clear();
           
       }
       int idQ = consultaIn->id_query;
       descriptoresQuerie[idQ]=fusionInfo(maxi(agregados),var(agregados),suma(agregados));
       
       
      // suma(agregados);
      // maxi(agregados);
      // var(agregados);
     //  cout<<endl;
    //   mostrarMatriz(agregados);
     //  mostrarMatrizTraspuesta(agregados);
       for(int j=0;j<consultaIn->n_terminos;j++)
       {
			int id = consultaIn->terminos[j];
			agregados[j].clear();
          //  printf(" [%d][%llu] -",consultaIn->valores[j],);
            
           // tmp.clear();
       }       

		agregados.clear();
      //  printf("\n");

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

void mostrarMapQ()
{
	
	   
      for( map<int, vector<double> >::iterator ii=descriptoresQuerie.begin(); ii!=descriptoresQuerie.end(); ++ii)
   {  cout << (*ii).first;
	   int tam = (*ii).second.size();
       for(int i=0;i<tam;i++)
       {
		 
		   cout <<" "<< (*ii).second[i];
	   }
       //
       cout<< endl;

   }
}





void mostrarMap()
{
	
	   
      for( map<int, vector<double> >::iterator ii=descriptoresLista.begin(); ii!=descriptoresLista.end(); ++ii)
   {  cout << (*ii).first << " : [";
	   int tam = (*ii).second.size();
       for(int i=0;i<tam;i++)
       {
		   cout <<" "<< (*ii).second[i];
	   }
       //
       cout<<"]"<< endl;

   }
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


int main( int argc, char** argv )
{
   char *nombreArchivoEntradaTerms = argv[1]; //terms file
   //char *nombreArchivoEntradaLista = argv[1]; //lista in
  // char *nombreArchivoSalidaLista = argv[3];  //lista out
   int entrada = atoi(argv[2]);
   nombreArchivoConsultas = argv[3];//file queries
   int nQueries = atoi(argv[4]);
   int TOPK=atoi(argv[5]);
   //char nbre[256];
   string linea;
   int id_doc, proc, tira;

   ofstream out;
   //sprintf(nbre,"./IDDOCS.dat");
   ifstream terms(nombreArchivoEntradaTerms); //terms file
  // ifstream listaIn(nombreArchivoEntradaLista); //lista in
 //  out.open(nombreArchivoSalidaLista); // lista out
   
   vector<double> tokens;
   int idt,nd;
  // set<int>IdTerms;
   int cont=0;
   int j=0;
   unsigned int k=0;
   
   // la lista comienza en la linea 2, pero se debe comparar el id linea + 2 si se parte contando de 0
   int doc,lenght;
  // unsigned int totalLenght = 0;
  // double avgL = 11.0;
  // int idTerm,Ft;
  // double ub;
 //  double MAXTF;
 //  double N = 50220423.0;
   //ifstream todo(nombreArchivoEntrada);//Docs de entrada
   //vector<string> tokens;
   //carga de terms a seleccionar
   int idTerm;//1
   double mediaArit;//2
   double mediaGeo;//3
   double mediaArmo;//4
   double ub;//5
   double MAXTF;//6
   double Varianza;//7
   double N_post;//8
   double maximos;//9
   double maximoMayorMedia;//10
   double N_Post_ub;//11
   double N_Post_5Porc_ub;//12
   double N_Post_5porc_Umbral_10;//13
   double N_Post_5porc_Umbral_100;//14
   double N_Post_5porc_Umbral_1000;//15
   double N_Post_5porc_Umbral_10000;//16
   double update_k_10;//17
   double update_k_100;//18
   double update_k_1000;//19
   double update_k_10000;//20
   double IDF;//21
   double maximo_querie;//agregado
   double varianzaQuerie;//agregado
   double suma_querie;// agregado
   
   leerConsultasChar(nombreArchivoConsultas,&querys,nQueries);
  
   while( getline(terms,linea))
   {
	   
	   	 int c = 0;
         stringstream st(linea);
         string item;
	   //  if(k%1000==0)
	   //  cout<<k<<endl;
	     
	      if(k==entrada)
	      break;
	     
	     while(st.good())
         {
            if ( !st.good() ) 
            continue; 
            st >> item;

				if(c==0)
				{
					idTerm = atoi(item.c_str());
					
				}
				else
				{
					double tmp =  (double) (atof(item.c_str()));
					
					if(TOPK==10)
					{
						if(c==13||c==14||c==15||c==17||c==18||c==19)
						{
						c++;
						continue;	
						
						}
					
					}
					if(TOPK==100)
					{
						if(c==12||c==14||c==15||c==16||c==18||c==19)
						{
						c++;
						continue;	
						
						}
						
					}
					if(TOPK==1000)
					{
						if(c==12||c==13||c==15||c==16||c==17||c==19)
						{
						c++;
						continue;	
						
						}
						
					}
					if(TOPK==10000)
					{
						if(c==13||c==14||c==12||c==17||c==18||c==16)
						{
						c++;
						continue;	
						
						}
						
					}
					
					
				 tokens.push_back( tmp);
				}

				c++;

         }
         descriptoresLista[idTerm]=tokens;
         sizeDesciptor=tokens.size();
         tokens.clear();

	   k++;
    
   }
   terms.close();


 llamadaAgregados(querys);
 mostrarMapQ();
  // k--;
  // cout<<"Total Lenght: "<<totalLenght<<endl;
  // cout<<"Media de lenght colecccion: "<<totalLenght/k<<endl; 
  
 //  out.close();
 //mostrarMap();
   return 0;
}
