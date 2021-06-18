/*
    Simulation of computational cost of build features queries and predictions
*/
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include "math.h"
#include <string.h>
#include <math.h>
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

/*
    Data for simulation

*/
using namespace std;
vector<float> entradasRed;// dim vector descriptor I RAND simulacion
vector<float> entradasSalidaRed;//dim hidden layer or generation compute network and sigmoide function
vector< vector<float> > pesosOcultaRed;// I x O RAND simulation
vector<float> pesosSalidaRedOculta;// O x S RAND simulation
vector<float> salidasRed;//output of neural network
vector< vector<float> > descriptoresTerminos;

/* end data of simulation */

#define handle_error_en(en, msg) \
               do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

       #define handle_error(msg) \
               do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define THREADS 8 //MAX num thread architect


int K;
struct timespec tiempo1_GlobalQLote, tiempo2_GlobalQLote, latenciaQ;    

/* Fit trace time */
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

float mediaAritmetica(vector<float> w_t);
float sumaValores(vector<float> w_t);

/*  Variance of vector */
float varianzaW(vector<float> w_t)
{
	float Z = (float)w_t.size();
	float suma = 0.0;
	float tmp,tmp2;
	float media = mediaAritmetica(w_t);
	for(int i=0;i<w_t.size();i++)
	{		
		tmp = (float)w_t[i]-media;
	    tmp2 = pow(tmp,2);
	    suma = suma + tmp2;	    
	}
	return suma/Z;
}

/*  Sum of elements vector */
float sumaValores(vector<float> w_t)
{	
	float suma = 0;
	for(int i=0;i<w_t.size();i++)
	suma+=w_t[i];
	return suma;	
}

/*  Max value of vector */
float maximoValores(vector<float> w_t)
{	
	float max = w_t[0];
	for(int i=1;i<w_t.size();i++)
	if(w_t[i]>max)
	max = w_t[i];
	return max;	
}

/*  Mean value of vector */
float mediaAritmetica(vector<float> w_t)
{
	return	(float)(sumaValores(w_t)/(float)w_t.size());
}

/*  Generator of random number */
float llamadaRand()
{	
	int iSecret, iGuess;
	int numeros =  1000000000;
	float numeros2 = (float)numeros;
	unsigned long int  tiempoX;
	clock_gettime(CLOCK_MONOTONIC, &tiempo2_GlobalQLote);
	diff_time(&tiempo2_GlobalQLote,&tiempo1_GlobalQLote,&latenciaQ);
	tiempoX=latenciaQ.tv_sec*1000000000+latenciaQ.tv_nsec;
	/* initialize random seed: */
	srand (time(NULL));
	/* generate secret number between 1 and 10: */
	iSecret = rand() % tiempoX;
	return (float)(iSecret)/numeros2;
}

void mostrarMatriz(vector< vector<float> > in)
{
	int dim = in.size();
	//cout <<" dim: "<<dim<<endl;
	for(int k0=0; k0<dim; k0++)
	{
		int dim2 = in[k0].size();		
		for(int i=0;i<dim2;i++)
		cout <<" "<< in[k0][i];		
		cout<<endl;
	}
	
}

void mostrarVector(vector<float> in)
{
	int dim = in.size();
	cout <<" dim entrada: "<<dim<<endl;
	for(int k0=0; k0<dim; k0++)
	{	
		cout <<" "<< in[k0];			
	}
	cout<<endl;
}
	
/* Simulation de weights hidden layer of neural network */	
void generadorVectorPesoOculta(int entradas,int ocultas)
{
	int dim = pesosOcultaRed.size();
	for(int k0=0; k0<dim; k0++)
	pesosOcultaRed.clear();	
	for(int i=0;i<ocultas;i++)
	{
		vector<float> tmp(entradas);
		for(int k1=0;k1<entradas;k1++)

		tmp[k1]=llamadaRand();		
		pesosOcultaRed.push_back(tmp);
	}	

}

/* Simulation input DFT-Based */	
void generadorVectorEntrada5()
{
	entradasRed.clear();
	for(int i=0;i<5;i++)
	{
		entradasRed.push_back(llamadaRand());
	}	
}

/* Simulation input Baseline */	
void generadorVectorEntrada42()
{
	entradasRed.clear();
	for(int i=0;i<14;i++)
	{
		entradasRed.push_back(llamadaRand());
	}	
}

/* Simulation weights of output layer */	
void generadorVectorPesoSalida(int ocultas)
{
	pesosSalidaRedOculta.clear();
	for(int i=0;i<ocultas;i++)
	{
		pesosSalidaRedOculta.push_back(llamadaRand());
	}	
}

/* main */	

int main (int argc, char *argv[]) {
	
	int entradas=atoi(argv[1]);
	int intermedia=atoi(argv[2]);
	int salidas=atoi(argv[3]);
	int termsIN=atoi(argv[4]);
	int metodo=atoi(argv[5]); // 6, 42 -> trace Options
	double peso = 0.0;
	double number = 0.0;
	double suma = 0.0;
	double numberX = 0.0;
	double numberW = 0.0;
	int terms = 5;
	vector <float> vCaracteristico;	
	unsigned long int tiempo;
	double sumaDescriptores = 0.0;
	cout<<"Test: "<<metodo<<" features"<< endl;
	cout<<"inputs:"<<entradas<<endl;
	cout<<"neurons hidden layers: "<<intermedia<<endl;
	cout<<"outputs: "<<intermedia<<endl;
	cout<<"size querie term: "<<termsIN<<endl;
	generadorVectorPesoOculta(entradas,intermedia);
	//cout<<intermedia<<endl;  
	//cout<<entradas<<endl;      

        //DFT-Based terms descriptors simulation
		if(metodo==6)	
            for(int j =0;j<termsIN;j++)
            {
                    generadorVectorEntrada5();
                    descriptoresTerminos.push_back(entradasRed);
            }
        //Baseline terms descriptors simulation   
        if(metodo==42)	
            for(int j =0;j<termsIN;j++)
            {
                generadorVectorEntrada42();
                descriptoresTerminos.push_back(entradasRed);
            }

		generadorVectorPesoSalida(intermedia);
		clock_gettime(CLOCK_MONOTONIC, &tiempo1_GlobalQLote);

        //DFT-Based querie descriptors simulation	
	    if(metodo==6)
		{  	 
			for(int j=0;j<4;j++)
			{   sumaDescriptores = 0.0;
				for(int i=0;i<termsIN;i++)
				sumaDescriptores += descriptoresTerminos[i][j];				
				vCaracteristico.push_back(sumaDescriptores);				
			}
			float max1[2];
			max1[0] = 0.0;
			max1[1] = 0.0;			
			//     float max2 = 0.0;
			int contTMP =0;
			for(int j=3;j<5;j++)
			{    			
				for(int i=0;i<termsIN;i++)
				{
				if (descriptoresTerminos[i][j] > max1[contTMP])
				max1[contTMP]  = descriptoresTerminos[i][j];
			    }	    			    
				vCaracteristico.push_back(max1[contTMP]);
				contTMP++;
			}			
		}
		
		//Baseline querie descriptors simulation	
		if(metodo==42)
		{				
			for(int j=0;j<14;j++)
			{		
		    vector<float> tmpValores;
		    float max = 0.0;
		    float sum = 0.0;
		    	for(int i=0;i<termsIN;i++)
				{
				tmpValores.push_back(descriptoresTerminos[i][j]);				
				if(descriptoresTerminos[i][j]>max)
				max = descriptoresTerminos[i][j];				
				sum = sum + descriptoresTerminos[i][j];				
			    }
		    
			vCaracteristico.push_back(varianzaW(tmpValores));
			vCaracteristico.push_back(sum);
			vCaracteristico.push_back(max);
			}
		}
	
clock_gettime(CLOCK_MONOTONIC, &tiempo2_GlobalQLote);
diff_time(&tiempo2_GlobalQLote,&tiempo1_GlobalQLote,&latenciaQ);
tiempo=latenciaQ.tv_sec*1000000000+latenciaQ.tv_nsec;
cout<<"build vector querie"<<endl;
cout<<(double)tiempo<<" ns. (nanoseconds)"<<endl;
//cout<<(double)tiempo/1000000<<" ms. (miliseconds)"<<endl;
//cout<<(double)tiempo/1000000000<<" s. (seconds)"<<endl;
//cout<<entradas<<"entradas"<<endl;
//cout<<intermedia<<"intermedia"<<endl;

	double pesos[entradas][intermedia];		
	float matrizCapaOcultaRED[entradas][intermedia];
	float matrizSalidaRED[intermedia][1];
	float sumaOculta = 0.0;
	int contProductos = 0;
	int contSigmoide = 0;	
	
	clock_gettime(CLOCK_MONOTONIC, &tiempo1_GlobalQLote);
    // Simulation neural networks
	for(int i=0;i<intermedia;i++)
	{
		sumaOculta = 0.0;
		for(int j=0;j<entradas;j++)
		{sumaOculta = sumaOculta + vCaracteristico[j]*pesosOcultaRed[i][j];
			contProductos++;
	    }
		peso = 1.0/(1.0+exp(- sumaOculta));
		matrizSalidaRED[i][0] = peso;
		contSigmoide++;
	}
	
	    sumaOculta = 0.0;
		for(int j=0;j<intermedia;j++)
		{sumaOculta = sumaOculta + matrizSalidaRED[j][0]*pesosSalidaRedOculta[j];
			contProductos++;
		}
		
		peso = 1.0/(1.0+exp(- sumaOculta));
	    contSigmoide++;  
    // end simulation neural networks


clock_gettime(CLOCK_MONOTONIC, &tiempo2_GlobalQLote);
diff_time(&tiempo2_GlobalQLote,&tiempo1_GlobalQLote,&latenciaQ);
tiempo=latenciaQ.tv_sec*1000000000+latenciaQ.tv_nsec;
cout<<"runtime neural networks"<<endl;
cout<<(double)tiempo<<" ns. (nanoseconds)"<<endl;
//cout<<(double)tiempo/1000000<<" ms. (miliseconds)"<<endl;
//cout<<(double)tiempo/1000000000<<" s. (seconds)"<<endl;
//cout<<" Vector caracteristico query"<<endl;
//mostrarVector(vCaracteristico);

vCaracteristico.clear();
descriptoresTerminos.clear();

cout<<"Neural Networks Multiplications: "<<contProductos<<endl;
cout<<"evaluation Sigmoide function: "<<contSigmoide<<endl;



}
