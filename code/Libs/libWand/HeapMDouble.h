#if !defined(_CBT_H)
#define _CBT_H
#include <stdio.h>
class CBT
{
private:
   int N; // tamanho actual del CBT

public:
    float *Prioridades;
    unsigned int    *ID_docs;
    int    *cbt; //posiciones
 
 
    int K; // tamanho final del CBT
  CBT( int k )
  {
     N = 0;
     K = k;
 
     cbt         = new int[2*K];
     Prioridades = new float[K+1];
     ID_docs     = new unsigned int[K+1]; //id
 
   //  Prioridades[0]= 1000000000; // infinito

     for(int i=0;i<=K;i++)
     {
      Prioridades[i]= 1000000000.0;

     }

     int contador=1;
     for(int n=0; n<=2*K-1; n++)
     {
        if(n>=K)
        {
            cbt[n]= contador;
            contador++;
        }
        else
        {
        cbt[n]= 0;
        }
     }
  }

  ~CBT( )
  {
  delete[]cbt;
  delete[]Prioridades;
  delete[]ID_docs;
  }


  float top()
  {
     return Prioridades[cbt[1]]; //saca el minimo
  }



  void insert( float prio, unsigned int id_doc)
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
         printf(" Prioridades[%d]:%f \n",n,Prioridades[n]);
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
   void mostrarCBT()
  {
       printf("\n CBT \n");
  for(int n=0; n<=2*K-1; n++)
     {
         printf(" cbt[%d]:%f ",n,cbt[n]);
     }


  }
   int dimensionHeap()
   {
       return N;
   }
   
   int dimensionHeapUsada()
   {
       return N;
   }
private:

   void updateCBT( int n )
   { // recorre hasta la raiz
      int a, b;
      for( int f=n/2; f>0; f=f/2 )
      {
         a = cbt[f*2];
         b = cbt[f*2+1]; 
         if( Prioridades[a]<Prioridades[b] )
         {
             cbt[f] = a;
         }
         else
         {
             cbt[f] = b;
         }
    
      
      
  //    printf("\n Prioridades[a]:%u,Prioridades[b]:%u, (i:%d)(f: %d)(cbt[f]:%d): (a,b): (%u,%u) \n",Prioridades[a],Prioridades[b],n,f,cbt[f],a,b);

     }
      

   } // end updateCBT()



};
#endif
