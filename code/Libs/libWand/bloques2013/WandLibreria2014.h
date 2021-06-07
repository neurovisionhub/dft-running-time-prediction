/* 
 * File:   WandIndexBlockMax.h
 * Author: oscar
 *
 * Created on 26 de septiembre de 2013, 9:27
 */
  
#if !defined(_WAND_H)
#define _WAND_H

#include <stdio.h>
#include <algorithm>
#include <unistd.h>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <cstring>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <istream>
#include <ostream>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <math.h>

#define END_OF_STREAM 999999999
using namespace std;
float avgL = 525.0;
float nDocsCollection = 50220423.0;
map<int,int> dimDocs; // En un entorno paralelo cada procesador tiene su propio conjunto de docs
map<int,int> dimDocsParticion[8]; 
map<int, map<int,int> > dimDocsThreads;

int sizeDocThreads(int id_doc,int threadC)
{
	//	cout<<"id:"<<id_doc<< endl;
	
	
	
	map<int,int> ::iterator p = dimDocsThreads[threadC].find(id_doc);
	if(p != dimDocs.end())
	return p->second;
	//if(dimDocs[id_doc]>0)
	//return dimDocs[id_doc];
	else return 1.0;
	
	
}

int sizeDoc(int id_doc)
{
	//	cout<<"id:"<<id_doc<< endl;
	int valor;
	
	
	map<int,int> ::iterator p = dimDocs.find(id_doc);
	if(p != dimDocs.end())
	return p->second;
	//if(dimDocs[id_doc]>0)
	//return dimDocs[id_doc];
	else return 1.0;
	
	
}

int sizeDocParticion(int id_doc,int particion)
{
	//	cout<<"id:"<<id_doc<< endl;
	int valor;
	
	
	map<int,int> ::iterator p = dimDocsParticion[particion].find(id_doc);
	if(p != dimDocs.end())
	return p->second;
	//if(dimDocs[id_doc]>0)
	//return dimDocs[id_doc];
	else return 1.0;
	
	
}
	
	
void avgLAsign(float in)
{avgL=in;}
void nDocsCollectionAsign(float in)
{nDocsCollection=in;}

float idf(int nDocsTerm)
{
	
	float idf = (0.5 + nDocsCollection - (float)(nDocsTerm) )/ (0.5 + (float)(nDocsTerm) );
	
	float x = log2(idf);
	if (x<0.0)
	x=0.01;
	
	return x;
}

float BM25(unsigned int tf, unsigned int idDoc/*,float fTerm*/)
{
	
	float _tf = (float)(tf);
	float sDoc = (float)(sizeDoc(idDoc));
	//float sDoc = (float)(sizeDoc(idDoc))
	//sizeDocParticion(int id_doc,int particion)
{
	//cout<<sDoc<<" id:"<<idDoc<< endl;
	//float avgL = MEDIA_COLLECTION;
	//float b=0.75,k1=1.2,k3=1000.0;
	//float BM= (k1+1.0)*_tf/( k1*( (1.0 - b) + b*(sDoc/avgL)) + _tf);
	//float BM = 2.2*_tf/( 1.2*( 0.25 + 0.75*(sDoc/avgL)) + _tf);
	float BM = _tf*2.2/(0.3 + 0.9*(sDoc/avgL) + _tf);
	return BM;
	
	
}
}

float BM25_Particion(unsigned int tf, unsigned int idDoc/*,float fTerm*/,int particion)
{
	
	float _tf = (float)(tf);
	float sDoc = (float)(sizeDocParticion(idDoc,particion));
	//float sDoc = (float)(sizeDoc(idDoc))
	//sizeDocParticion(int id_doc,int particion)
{
	//cout<<sDoc<<" id:"<<idDoc<< endl;
	//float avgL = MEDIA_COLLECTION;
	//float b=0.75,k1=1.2,k3=1000.0;
	//float BM= (k1+1.0)*_tf/( k1*( (1.0 - b) + b*(sDoc/avgL)) + _tf);
	//float BM = 2.2*_tf/( 1.2*( 0.25 + 0.75*(sDoc/avgL)) + _tf);
	float BM = _tf*2.2/(0.3 + 0.9*(sDoc/avgL) + _tf);
	return BM;
	
	
}
}
float BM25_R(int tf, int idDoc/*,float fTerm*/)
{
	float sDoc = (float) sizeDoc(idDoc);
	//float avgL = MEDIA_COLLECTION;
	float b=0.75,k1=1.2,k3=1000.0;
	//float idf_tmp = idf(nDocsTerm);
	float BM= (k1+1.0)*(float)(tf)/( k1*( (1.0-b) + b*(sDoc/avgL)) + (float)(tf));
	//float x0 = (float)(k3+1.0)*fTerm/(k3+fTerm);
	//float BM = BM_tmp*x0;
    //cout<<"idf: "<<idf<<" nDocsTerm: "<<nDocsTerm<<"idf: "<<sDoc<<"idf: "<<tf <<"fTerm "<<fTerm<<" BM"<<BM <<" x0"<< x0 <<" x1 "<<x1<<endl;
	return BM;
}

//no guarda el id_term para un map<id_term, PostingStream*>
//class PostingStream
class PostingStream{
public:
	
	//lista normal o bajo UB de (id_doc, payload)
	int n;
	unsigned int *id_doc_l;
    unsigned int *payload_l;
    //posicion lista y/o Grupo Low
	int pos;
    //ub del termino o ub del termino Grupo Low 
	float ub_inicial;
    //Fancy list
    int nH;
    bool *bandera;
	//lista alta con UB alto
	//unsigned int *id_doc_l_GH;
	//unsigned int *payload_l_GH;
    //posicion lista Grupo High
	int pos_GH;
    float ub_inicial_GH;	
    //Bloques
    //vector ub_bloques        
 	float *ub_bloques;
	//vector idDoc_bloques
	unsigned int *idDoc_bloques; 
    //vector idDoc_bloques
	unsigned int *idDoc_bloques_Fin; 
	//numero de bloques 
    int nBloques;
	//bloque actual
    int bloqueActual;
    
    float idf_term;
    
    int particionList;
	PostingStream(){
                //
		n=0;
		nH=0;
		id_doc_l=NULL;
		payload_l=NULL;
		pos=0;
		ub_inicial=0.0;
     	//id_doc_l_GH=NULL;
		//payload_l_GH=NULL;
		pos_GH=0;
		ub_inicial_GH=0.0;
	}
	PostingStream(int _n, unsigned int *_id_doc_l, unsigned int *_payload_l){
		n=_n;
		id_doc_l=new unsigned int[n];
		payload_l=new unsigned int[n];
		for(int i=0; i<n; i++){
			id_doc_l[i]=_id_doc_l[i];
			payload_l[i]=_payload_l[i];
		}
		pos=0;
		ub_inicial=0.0;
		idf_term=idf(n);
	}
    PostingStream(list< pair<unsigned int, unsigned int> > *lista,vector<float> UbBloques,vector<unsigned int> idDocFronteras,vector<unsigned int> idDocFronterasFin,float _ub_inicial){
                //cout<<"PostingStream ("<<lista->size()<<" elementos) \n";
                n=lista->size();
                id_doc_l=new unsigned int[n];
                payload_l=new unsigned int[n];
                list< pair<unsigned int, unsigned int> >::iterator it;
                it=lista->begin();
                for(int i=0; i<n; i++){
                        id_doc_l[i]=it->first;
                        payload_l[i]=it->second;
                        it++;
                }
                n=UbBloques.size(); 
                nBloques=UbBloques.size();
                ub_bloques = new float[n];
        		for(int i=0;i<n;i++)
				{
                ub_bloques[i]=UbBloques[i]; 
				}
                n=idDocFronteras.size();
                idDoc_bloques = new unsigned int[n];
                idDoc_bloques_Fin = new unsigned int[n];
                for(int i=0;i<n;i++)
                {
					idDoc_bloques[i]=idDocFronteras[i];
					idDoc_bloques_Fin[i]=idDocFronterasFin[i];
				}  
                
		 pos=0;
         ub_inicial=_ub_inicial;
         bloqueActual=0;
         n=lista->size();       
         idf_term=idf(n);
        }
        PostingStream(list< pair<unsigned int, unsigned int> > *lista,vector<float> UbBloques,vector<unsigned int> idDocFronteras,vector<unsigned int> idDocFronterasFin,float _ub_inicial,int docsGlobal){
                //cout<<"PostingStream ("<<lista->size()<<" elementos) \n";
                n=lista->size();
                id_doc_l=new unsigned int[n];
                payload_l=new unsigned int[n];
                list< pair<unsigned int, unsigned int> >::iterator it;
                it=lista->begin();
                for(int i=0; i<n; i++){
                        id_doc_l[i]=it->first;
                        payload_l[i]=it->second;
                        it++;
                }
                n=UbBloques.size(); 
                nBloques=UbBloques.size();
                ub_bloques = new float[n];
        		for(int i=0;i<n;i++)
				{
                ub_bloques[i]=UbBloques[i]; 
				}
                n=idDocFronteras.size();
                idDoc_bloques = new unsigned int[n];
                idDoc_bloques_Fin = new unsigned int[n];
                for(int i=0;i<n;i++)
                {
					idDoc_bloques[i]=idDocFronteras[i];
					idDoc_bloques_Fin[i]=idDocFronterasFin[i];
				}  
                
		 pos=0;
         ub_inicial=_ub_inicial;
         bloqueActual=0;
         n=lista->size();       
         idf_term=idf(docsGlobal);
        }
        
        
               PostingStream(list< pair<unsigned int, unsigned int> > *lista,vector<float> UbBloques,vector<unsigned int> idDocFronteras,vector<unsigned int> idDocFronterasFin,float _ub_inicial,int docsGlobal,int _partition){
                //cout<<"PostingStream ("<<lista->size()<<" elementos) \n";
                n=lista->size();
                id_doc_l=new unsigned int[n];
                payload_l=new unsigned int[n];
                list< pair<unsigned int, unsigned int> >::iterator it;
                it=lista->begin();
                for(int i=0; i<n; i++){
                        id_doc_l[i]=it->first;
                        payload_l[i]=it->second;
                        it++;
                }
                n=UbBloques.size(); 
                nBloques=UbBloques.size();
                ub_bloques = new float[n];
        		for(int i=0;i<n;i++)
				{
                ub_bloques[i]=UbBloques[i]; 
				}
                n=idDocFronteras.size();
                idDoc_bloques = new unsigned int[n];
                idDoc_bloques_Fin = new unsigned int[n];
                for(int i=0;i<n;i++)
                {
					idDoc_bloques[i]=idDocFronteras[i];
					idDoc_bloques_Fin[i]=idDocFronterasFin[i];
				}  
                
		 pos=0;
         ub_inicial=_ub_inicial;
         bloqueActual=0;
         n=lista->size();       
         idf_term=idf(docsGlobal);
         particionList = _partition;
        }
	PostingStream(list< pair<unsigned int, unsigned int> > *lista){
		//cout<<"PostingStream ("<<lista->size()<<" elementos) \n";
		n=lista->size();
		id_doc_l=new unsigned int[n];
		payload_l=new unsigned int[n];
		list< pair<unsigned int, unsigned int> >::iterator it;
		it=lista->begin();
		for(int i=0; i<n; i++){
			id_doc_l[i]=it->first;
			payload_l[i]=it->second;
			it++;
		}
		pos=0;
		ub_inicial=0.0;
		idf_term=idf(n);
		//cout<<"PostingStream ok\n";
	}
	PostingStream(list< pair<unsigned int, unsigned int> > *lista, float _ub_inicial){
		//cout<<"PostingStream ("<<lista->size()<<" elementos) \n";
		//grupo=0->GH, grupo 1->GL
		n=lista->size();
		id_doc_l=new unsigned int[n];
		payload_l=new unsigned int[n];
		list< pair<unsigned int, unsigned int> >::iterator it;
		it=lista->begin();
		for(int i=0; i<n; i++){
			id_doc_l[i]=it->first;
			payload_l[i]=it->second;
			it++;
		}
		pos=0;
		ub_inicial=_ub_inicial;
		ub_bloques = NULL;
		idDoc_bloques =NULL;
		idDoc_bloques_Fin=NULL;
		idf_term=idf(n);
		//cout<<"PostingStream ok\n";
	}
	/*PostingStream(list< pair<unsigned int, unsigned int> > *lista, unsigned int _ub_inicial, int grupo){
		//cout<<"PostingStream ("<<lista->size()<<" elementos) \n";
		//grupo=0->GH, grupo 1->GL
		n=lista->size();
		id_doc_l=new unsigned int[n];
		payload_l=new unsigned int[n];
                if(grupo==1)
                bandera = new bool[n];
		list< pair<unsigned int, unsigned int> >::iterator it;
		it=lista->begin();

                if(grupo==0){
		for(int i=0; i<n; i++){
			id_doc_l[i]=it->first;
			payload_l[i]=it->second;
			it++;
                }}
                else //solo llena banderas en la parte baja
                {
                for(int i=0; i<n; i++){
			id_doc_l[i]=it->first;
			payload_l[i]=it->second;
                        bandera[i]=0;
			it++;
                }


                }
		pos=0;
		ub_inicial=_ub_inicial;
		//cout<<"PostingStream ok\n";
	}
	*/
	~PostingStream(){
		if(id_doc_l!=NULL){
			delete [] id_doc_l;
			id_doc_l=NULL;
		}
		if(payload_l!=NULL){
			delete [] payload_l;
			payload_l=NULL;
		}
		
               	if(ub_bloques!=NULL){
                        delete [] ub_bloques;
			ub_bloques=NULL;
		}
		if(idDoc_bloques!=NULL){
			delete [] idDoc_bloques;
			idDoc_bloques=NULL;
		}
		if(idDoc_bloques_Fin!=NULL){
			delete [] idDoc_bloques_Fin;
			idDoc_bloques_Fin=NULL;
		}
		
		
		
		
          /*      if(id_doc_l_GH!=NULL){
                        delete [] id_doc_l_GH;
			id_doc_l_GH=NULL;
		}
		if(payload_l_GH!=NULL){
			delete [] payload_l_GH;
			payload_l_GH=NULL;
		}
*/


		pos=0;
	}
	
	void reset(){
		pos=0;
		bloqueActual=0;
	}
	
	//retorna el payload del doc actual
	unsigned int payload(){
		return payload_l[pos];
	}
	
	//retorna el id_doc del doc actual
	unsigned int docId(){
		return id_doc_l[pos];
	}
//retorna el id_doc del doc actual
//	unsigned int docId_1(){
//		return id_doc_l[pos-1];
//	}

    unsigned int payload_pos(int indice){return payload_l[indice];}

    unsigned int docId_pos(int indice){return id_doc_l[indice];}
	//retorna el numero de docs
	int count(){return n;}
	
 	int bloque_actual(){return bloqueActual;}

    int numeroBloques(){return nBloques;}

    void nextBloque(){bloqueActual++;}

    void prevBloque(){bloqueActual--;}

    unsigned int docId_bloque_ini(int indice){return idDoc_bloques[indice];}

    unsigned int docId_bloque_end(int indice){return idDoc_bloques_Fin[indice];}
  
    void mostrarBloques(unsigned long long t)
    {cout<<"Term: "<<t<<" nBloques: "<<nBloques<<endl;
     for(int i=0;i<nBloques;i++)
     {cout<<" "<<ub_bloques[i];}    
    }
        
	/* unsigned int next(unsigned int desiredDocId,int &posBloque)
	{
		 while(pos<(n-1) && id_doc_l[pos]<desiredDocId){pos++;} 
         if(pos>=(n-1))
		{
           if(bloqueActual!=nBloques-1)
           {while(id_doc_l[pos-1]>=idDoc_bloques[bloqueActual+1]){bloqueActual++;}

           }
			posBloque=bloqueActual;
			return END_OF_STREAM;
        }else
		{
			if(bloqueActual!=nBloques-1)
            {                       
             while(id_doc_l[pos]>=idDoc_bloques[bloqueActual+1]){bloqueActual++;}
			}
         posBloque=bloqueActual;
         return id_doc_l[pos];
        }
    }

*/

	//mueve el cursor hasta desiredDocId o uno despues si no esta
	//retorna el id_doc resultante, o END_OF_STREAM si llega al final
	unsigned int next(unsigned int desiredDocId){
	//printf("NEXT DOC- ");
		while(pos<(n-1) && id_doc_l[pos]<desiredDocId)
			pos++;
	
		if(pos>=(n-1)){
			return END_OF_STREAM;
		}
		else{
			return id_doc_l[pos];
		}
	}

    unsigned int back(){pos--;return id_doc_l[pos];}
    
	//mueve el cursor una posicion
	//retorna el id_doc resultante, o END_OF_STREAM si llega al final
	unsigned int next(){
       // printf("NEXT (%d,%d)[%u,%u]-",bloqueActual,nBloques-1,id_doc_l[pos],idDoc_bloques[bloqueActual+1]);
		//notar que llega hasta (n-1), pues se pide el proximo, que no existe
		//si hay 0 o 1 doc, no hay next, deben haber al menos 2 (el actual, y el proximo)
		if(pos>=(n-1)){

			return END_OF_STREAM;
		}
		else{
			pos++;
			return id_doc_l[pos];
		}
	}


};

//class QueryFeatureInfoT
class QueryFeatureInfoT{
public:
	unsigned long long id_term;
	float weight;
	float ub_ini;
	float ub;
    float *ub_bloques_intervalo; //ub locales

	QueryFeatureInfoT(){
		id_term=0;
		weight=0.0;
		ub_ini=0.0;
		ub=0.0;
	}
	QueryFeatureInfoT(unsigned long long _id_term, float _weight, float _ub_ini){
		//cout<<"QueryFeatureInfoT ("<<_id_term<<", "<<_weight<<", "<< _ub_ini<<")\n";
		id_term=_id_term;
		weight=_weight;
		ub_ini=_ub_ini;
		//ub=_ub_ini;
		//cout<<"QueryFeatureInfoT ok\n";
	}
        QueryFeatureInfoT(unsigned long long _id_term,float _weight){
	    id_term=_id_term;
        weight=_weight;
	}



	~QueryFeatureInfoT(){

        if(ub_bloques_intervalo!=NULL){
		delete [] ub_bloques_intervalo;
		ub_bloques_intervalo=NULL;
        }
	}
	
};

//class ScorerT
class ScorerT{
public:
	
	    vector<PostingStream*> *streams;
        vector<PostingStream*> *streamsLows;
	    vector<QueryFeatureInfoT*> *query_terms;
        vector<QueryFeatureInfoT*> *query_terms_low;
        vector<vector<float> > query_terms_bloques_pesos;

        int *banderaQ;
        int *posBanderaQ;
		float sum_ub;
		int count;
        int dimQ;
        int *posTmp;
        int sumaBloquesComprimidos;
        
        
	ScorerT(){
        streams=NULL;
		query_terms=NULL;
		sum_ub=0.0;
		count=0;
	}

        //ubBloqueIn = 0 --> baseline, ubBloqueIn = 1 --> Wand Index Block Max
        ScorerT(vector<PostingStream*> *_streams, vector<QueryFeatureInfoT*> *_query_terms,int ubBloqueIn){
                streams=_streams;
                //streamsLows=_streamsLows;
                query_terms=_query_terms;
            
                
                if(ubBloqueIn == 1)
                {
					int dimL=streams->size();
					dimQ=dimL;
					query_terms_bloques_pesos.resize(dimL);
					int nLocal;
					sumaBloquesComprimidos=0;
				   // query_terms_bloques_pesos = new vector< unsigned int >(dimL);
					for(int i=0;i<dimL;i++)
					{
					  nLocal=(*streams)[i]->nBloques;
					  query_terms_bloques_pesos[i].resize(nLocal);
					  //(*streams)[i]->mostrarBloques((*query_terms)[i]->id_term);
					  for(int j=0;j<nLocal;j++)
					  {
					   query_terms_bloques_pesos[i][j]=(*streams)[i]->ub_bloques[j]*(*query_terms)[i]->weight; //ACA CAMBIO 2014, ADAPTAR PARA QUE ESTO SE REALICE MAS OPTIMIZADO...
					   //printf(" [%d][%d]=(UB:%u x PQ:%u)=%u\n",i,j,(*streams)[i]->ub_bloques[j],(*query_terms)[i]->weight ,query_terms_bloques_pesos[i][j]);
					   sumaBloquesComprimidos++;
					  } 

					}
			    }
				
				
                sum_ub=0.0;
                count=0;

        }

		ScorerT(vector<PostingStream*> *_streams, vector<QueryFeatureInfoT*> *_query_terms,vector<PostingStream*> *_streamsLows,vector<QueryFeatureInfoT*> *_query_terms_low){
		streams=_streams;
        streamsLows=_streamsLows;
		query_terms=_query_terms;
        query_terms_low=_query_terms_low;
		sum_ub=0;
		count=0;
                dimQ=query_terms->size();
                banderaQ = new int[dimQ];
                posBanderaQ = new int[dimQ];
                posTmp = new int[dimQ];
                for(int i=0;i<dimQ;i++)
                {
                banderaQ[i]=1;
                posBanderaQ[i]=0;
                posTmp[i]=0;
                }


	}

	~ScorerT(){
		streams=NULL;
		query_terms=NULL;
		sum_ub=0.0;
		count=0;
     
    }
	


        /*
        void scoreCurrentDocIntervalosBM25(const unsigned int docId, const int *streamIdx, const int startIdx, const int endIdx, float &result){
                count++;
                float score = 0.0;
                int tf;
                int dimTerm;
                for (int i = startIdx; i <= endIdx; ++i) {
                tf = (*streams)[streamIdx[i]]->payload();
			    dimTerm=(*streams)[streamIdx[i]]->n;
			    score +=BM25(tf,docId) * (*query_terms)[streamIdx[i]]->weight;

               }
                result=score;
                return;
        }
        */
        //time~=TimeTF*10;
        //IDF.BM25,  IDF=(*query_terms)[streamIdx[i]]->weight;
        void scoreCurrentDocBM25(const unsigned int docId, const int *streamIdx, const int startIdx, const int endIdx, float &result){
		count++;
		float score = 0.0;
		int tf;
        //int dimTerm;
		for (int i = startIdx; i <= endIdx; ++i) {
			tf = (float)((*streams)[streamIdx[i]]->payload());	
			//dimTerm=(*streams)[streamIdx[i]]->n;
			score += BM25(tf,docId) * (*query_terms)[streamIdx[i]]->weight;//->weight = alpha = ftQ*IDF(term)
			//cout<<"-----> Score(docId: "<<docId<<"): "<<score<<" (*query_terms)[streamIdx[i]]->weight "<<(*query_terms)[streamIdx[i]]->weight<<endl;
		}
		result=score;
		return;
		}


        void scoreCurrentDocBM25Partition(const unsigned int docId, const int *streamIdx, const int startIdx, const int endIdx, float &result){
		count++;
		float score = 0.0;
		int tf;
        //int dimTerm;
		for (int i = startIdx; i <= endIdx; ++i) {
			tf = (float)((*streams)[streamIdx[i]]->payload());	
			//dimTerm=(*streams)[streamIdx[i]]->n;
			score += BM25_Particion(tf,docId,(*streams)[streamIdx[i]]->particionList) * (*query_terms)[streamIdx[i]]->weight;//->weight = alpha = ftQ*IDF(term)
			//cout<<"-----> Score(docId: "<<docId<<"): "<<score<<" (*query_terms)[streamIdx[i]]->weight "<<(*query_terms)[streamIdx[i]]->weight<<endl;
		}
		result=score;
		return;
		}

        //TF.IDF, IDF=(*query_terms)[streamIdx[i]]->weight;
		void scoreCurrentDocTFIDF(const unsigned int docId, const int *streamIdx, const int startIdx, const int endIdx, float &result){
		count++;
		float score = 0.0;
		int tf;
		for (int i = startIdx; i <= endIdx; ++i) {
		tf = (float)((*streams)[streamIdx[i]]->payload());	
			score += (tf/ (float) sizeDoc(docId))* (*query_terms)[streamIdx[i]]->weight;//->weight = alpha = ftQ*IDF(term)
		}
		result=score;
		return;
		}

		//TF timeTF=x
	   void scoreCurrentDocTF(const unsigned int docId, const int *streamIdx, const int startIdx, const int endIdx, float &result){
		count++;
		float score = 0.0;
		int tf;
		for (int i = startIdx; i <= endIdx; ++i) {
		tf = (float)((*streams)[streamIdx[i]]->payload());
		score += tf	* (*query_terms)[streamIdx[i]]->weight;
		}
		result=score;
		return;
	}

     
	inline void resetForPivotFeatureSelection(){
		sum_ub = 0; 
	}
	
	    inline float accumulateUpperBound(const int index){
		sum_ub += (*query_terms)[index]->ub;
		return sum_ub; 
	}

        inline float accumulateUpperBoundVariable(const int index){//nuevos
        sum_ub += (*query_terms)[index]->ub;
        return sum_ub;

        }

        //ACA ACUMULAR UPPER BOUND CON UB VARIABLE
        inline float accumulateUpperBoundBloques(const int index,int bloque){//nuevos  
       sum_ub += query_terms_bloques_pesos[index][bloque];//ACA LOS NUEVOS ALGORITMOS
       return sum_ub;

        }

	// Resta un upperBounds (en la practica previamente estimado)
       inline float DesAccumulateUpperBoundBloques(const int index,int bloque){//nuevos  
       sum_ub -= query_terms_bloques_pesos[index][bloque];//ACA LOS NUEVOS ALGORITMOS
       return sum_ub;

        }

       inline float ub_query_terms(const int index,int bloque){return query_terms_bloques_pesos[index][bloque];} 


	
};

class Wand{
public:
  //std::ofstream ficheroTrazaWand;//
        //nuevas
	unsigned int limiteIdDocMenor;
    unsigned int limiteIdDocMayor;
    
    long unsigned int evaluacionesCompletas;	
	//se pasan como int& a los metodos
	unsigned int currentDocId;
	float currentPayload;
//int currentPayload;	
	unsigned int *streamDocIds;
    float *streamUB;
	
	// two arrays of indices into the _streams vector. This is used for merge.
	// inplace_merge is not as efficient as the copy merge.
	int *streamIndices;
	int *streamIndicesAux;
	
	// maximum number of streams that WAND can operate on
	int maxNumStreams;
	
	// number of streams present in the query
	int numStreams;
	
	//
	int numTermQ;
	// scorer
	ScorerT *scorer; 
	
	//PostingStreamPtr
	vector<PostingStream*> *streams;
    int tipoWAND_AND_OR;
        //traza
       // unsigned docTraza;
        //
        int docFrontera;
	    int *bloqueNow;
        int *bloqueSize;
        int *bloqueReview;
        int bloquesDescomprimidos;
        int bloquesNoUsados;
        int bloquesEvaluados;
        int contadorTMP;
        int numeroListasReal;
	// comparator class that compares two streams. The variables a and b are
	// logically indices into the streams vector.
	class StreamComparator
				: public std::binary_function<int, int, bool> {
	private:
		const unsigned int *streamDocIds;

	public:
		StreamComparator(){
			streamDocIds=NULL;
		}
		StreamComparator(const unsigned int *_streamDocIds){
			streamDocIds=_streamDocIds;
		}
		inline bool operator()(const int a, const int b){
			//cout<<"comparando "<<a<<" ("<<streamDocIds[a]<<") con "<<b<<" ("<<streamDocIds[b]<<")\n";
	        if (streamDocIds[a] < streamDocIds[b]){
				//cout<<"true\n";
				return true;
			}
			else{
				//cout<<"false\n";
				return false;
			}
		}

	};

        class StreamComparatorUB
				: public std::binary_function<int, int, bool> {
	private:

                const float *streamUB;
	public:
		StreamComparatorUB(){
                        streamUB=NULL;
		}
		StreamComparatorUB(const float *_streamUB){
			streamUB=_streamUB;
		}
		inline bool operator()(const int a, const int b){
			//cout<<"comparando "<<a<<" ("<<streamDocIds[a]<<") con "<<b<<" ("<<streamDocIds[b]<<")\n";
	        if (streamUB[a] < streamUB[b]){
				//cout<<"true\n";
				return true;
			}
			else{
				//cout<<"false\n";
				return false;
			}
		}

	};

	// comparator that compares two streams
	StreamComparator streamComparator;
    StreamComparatorUB UBComparator;
	
	Wand(){
		maxNumStreams=0;
		numStreams=0;
		scorer=NULL;
		streams=NULL;
		streamDocIds=NULL;
		streamIndices=NULL;
		streamIndicesAux=NULL;
		bloqueNow=NULL;
		streamComparator=StreamComparator(streamDocIds);
		UBComparator=StreamComparatorUB(streamUB);
		currentDocId=END_OF_STREAM;
		currentPayload=-1.0;
	}
	
	Wand(int _maxNumStreams, ScorerT *_scorer, vector<PostingStream*> *_streams){
		//cout<<"Wand ("<<_maxNumStreams<<" max, "<<_streams->size()<<" streams)\n";
		maxNumStreams=_maxNumStreams;
		numStreams=0;
		
		scorer=_scorer;
		streams=_streams;
		
		streamDocIds=new unsigned int[maxNumStreams];
		streamIndices=new int[maxNumStreams];
		streamIndicesAux=new int[maxNumStreams];
        streamUB=new float[maxNumStreams];
		
		streamComparator=StreamComparator(streamDocIds);
        UBComparator=StreamComparatorUB(streamUB);

                //UBComparator=StreamComparatorUB(streamUB);
		currentDocId=END_OF_STREAM;
		currentPayload=-1.0;
		//cout<<"Wand ok\n";
	}
	
~Wand(){
        if(scorer!=NULL){delete scorer;}
        if(streams!=NULL){delete streams;}

		if(streamDocIds!=NULL){
			delete [] streamDocIds;
			streamDocIds=NULL;
		}
		
		if(streamIndices!=NULL){
			delete [] streamIndices;
			streamIndices=NULL;
		}
		
		if(streamIndicesAux!=NULL){
			delete [] streamIndicesAux;
			streamIndicesAux=NULL;
		}
	
                if(bloqueNow!=NULL){
			delete [] bloqueNow;
			bloqueNow=NULL;

		}
                if(bloqueReview!=NULL){
			delete [] bloqueReview;
			bloqueReview=NULL;

		}
                 if(streamUB!=NULL){
			delete [] streamUB;
			streamUB=NULL;

		}

                   if(bloqueSize!=NULL){
			delete [] bloqueSize;
			bloqueSize=NULL;

		}

		currentDocId=END_OF_STREAM;
		currentPayload=-1.0;
	
	}


	void init(){
		numStreams = streams->size();
		evaluacionesCompletas=0;
                bloquesDescomprimidos=0;
                bloquesNoUsados=0;
                bloquesEvaluados=numStreams;
                contadorTMP=0;
                numeroListasReal=numStreams;
                bloqueNow = new int[numStreams];
                bloqueReview = new int[numStreams];
                bloqueSize = new int[numStreams];
                streamUB = new float[numStreams];
             
                numTermQ=numStreams;
		if (numStreams == 0) {
			currentDocId = END_OF_STREAM;
			currentPayload = -1.0;
		}
		
		//los indices parten en su posicion natural (0.. n-1)
		for (int i=0; i<numStreams; ++i) {
			streamIndices[i] = i;
                        bloqueNow[i]=0;//(*streams)[i]->idDoc_bloques[1];
                        bloqueSize[i]=(*streams)[i]->nBloques;
                        streamUB[i]=0.0;
		}
		
		//reseteo los streams antes de empezar
		for (int i=0; i<numStreams; ++i) {
			(*streams)[i]->reset();
			streamDocIds[i] = (*streams)[i]->docId();
		}

		sort(streamIndices, streamIndices+numStreams, streamComparator);
	
	}

        //tipoWand: 1-> ,2-> ,3-> 
        int next(float threshold,int tipoWand,int tipoRank){
		
		
		if(tipoWand==1)
		return next_wand_baseline(threshold,tipoRank);//baseline
		
		if(tipoWand==2)
		return next_wand_ibm(threshold,tipoRank);//index block max
		
		return 0;
		}
		
		
	int next_wand_baseline(float threshold,int tipoRank){
		
		//cout<<"Wand::next - inicio (threshold: "<<threshold<<")\n";
		
		// We do not check whether the stream is already at the end 
		// here based on the assumption that application won't call 
		// next() for streams that are already at the end, or atleast
		// won't do this frequently. 
		
		int pivotIdx=-1;
		unsigned int pivotDocId = END_OF_STREAM;
		
		while (true) {
	 		
	 		if(numStreams==0){
				currentDocId = END_OF_STREAM;
				currentPayload = -1.0;
				return END_OF_STREAM;
	 		}
	 		
			if (!findPivotFeatureIdxBaseLine(threshold, pivotIdx)) {
				currentDocId = END_OF_STREAM;
				currentPayload = -1.0;
				return END_OF_STREAM;
			}
			pivotDocId = streamDocIds[streamIndices[pivotIdx]];
					
			if (streamDocIds[streamIndices[0]] == streamDocIds[streamIndices[pivotIdx]]) {
				// Found candidate. All cursors before (*streams)[pivotIdx] point to
				// the same doc and this doc is the candidate for full evaluation.
				currentDocId = pivotDocId;
				// Advance pivotIdx sufficiently so that all instances of pivotDocId are included
				while (pivotIdx < numStreams-1 && streamDocIds[streamIndices[pivotIdx+1]] == pivotDocId) {
					++pivotIdx;
				}
				// scorer calculates the exact score for the docs [0, pivotIdx] 
						      //TF
							   if(tipoRank==1)
                               {scorer->scoreCurrentDocTF(currentDocId, streamIndices, 0, pivotIdx, currentPayload);}
                               //BM25
                               if(tipoRank==2)
                               {scorer->scoreCurrentDocBM25(currentDocId, streamIndices, 0, pivotIdx, currentPayload);}
                                //TF-IDF
                               if(tipoRank==3)
                               {scorer->scoreCurrentDocTFIDF(currentDocId, streamIndices, 0, pivotIdx, currentPayload);}
                             
                               
                               
                evaluacionesCompletas++;
				// advance all cursors preceding the pivot cursor and the pivot
				// cursor past the pivot docid
				moveStreamsAndSort(pivotIdx+1);
				
				// If the fully-evaluated score is higher then the threshold, then
				// currentDocId is a matching doc
				if (currentPayload > threshold) {
					//cout<<"Wand::next - candidato encontrado (doc "<<currentDocId<<" por score "<<currentPayload<<")\n";
					return currentDocId;
				} //otherwise do nothing - let the while-loop continue
				else{
					//cout<<"Continuando busqueda\n";
				}
				
			}
			else { // not all cursors upto the pivot are aligned at the same doc yet
				// decreases pivotIdx to the first stream pointing at the pivotDocId
				while (pivotIdx && streamDocIds[streamIndices[pivotIdx-1]] == pivotDocId) {
					--pivotIdx;
				}
				moveStreamsToDocAndSort(pivotIdx, pivotDocId);
			}
			
		}  /* while (true) */
	}

    int next_wand_ibm(float threshold,int tipoRank){
		
		int pivotIdx=-1;
		unsigned int pivotDocId = END_OF_STREAM;

		while (true) {
	 		
	 		if(numStreams==0){
				currentDocId = END_OF_STREAM;
				currentPayload = -1.0;
                                if(bloquesDescomprimidos<numeroListasReal) //cuando listas no tienen k documentos
                                bloquesDescomprimidos=bloquesDescomprimidos+numeroListasReal;
                                //bloquesDescomprimidos=bloquesDescomprimidos+numeroListasReal;

                      //          printf("******** FIN LISTAS - Descompresión real: [%d] Descompresion simulada por score: [%d]\n",bloquesDescomprimidos,contadorTMP);
				return END_OF_STREAM;
	 		}                       
	 		//aca cambios
			if (!findPivotFeatureIdxBlocks(threshold, pivotIdx)) {
				//cout<<"Wand::next - ! findPivotFeatureIdx (EOS)\n";
           
				currentDocId = END_OF_STREAM;
				currentPayload = -1;
                if(bloquesDescomprimidos<numeroListasReal) //cuando listas no tienen k documentos
                bloquesDescomprimidos=bloquesDescomprimidos+numeroListasReal;
                                //bloquesDescomprimidos=bloquesDescomprimidos;
                        //        printf("******** Descompresión real: [%d] Descompresion simulada por score: [%d]\n",bloquesDescomprimidos,contadorTMP);
             	return END_OF_STREAM; 
			}
               // continue; 
		       bool flag = false;
               unsigned int d;
			   pivotDocId = streamDocIds[streamIndices[pivotIdx]];
                        
                        for (int i=0; i <= pivotIdx; i++) { NextShallow(pivotDocId,i);}
                        flag = CheckBlockMax(threshold,pivotIdx,pivotDocId);
                        if(flag==true)
                        {
							if (streamDocIds[streamIndices[0]] == streamDocIds[streamIndices[pivotIdx]])
							{
								currentDocId = pivotDocId;

								   //TF
								   if(tipoRank==1){scorer->scoreCurrentDocTF(currentDocId, streamIndices, 0, pivotIdx, currentPayload);}
								   //BM25
								   if(tipoRank==2){
									//scorer->scoreCurrentDocBM25(currentDocId, streamIndices, 0, pivotIdx, currentPayload);
									scorer->scoreCurrentDocBM25Partition(currentDocId, streamIndices, 0, pivotIdx, currentPayload);   
									   
									   }
								   //TF-IDF
								   if(tipoRank==3){scorer->scoreCurrentDocTFIDF(currentDocId, streamIndices, 0, pivotIdx, currentPayload);}	
								   
									evaluacionesCompletas++;
									
										if(bloquesEvaluados!=bloquesDescomprimidos)
										{
										contadorTMP++;
										bloquesEvaluados=bloquesDescomprimidos;
										}
										moveStreamsAndSort(pivotIdx+1);
										if (currentPayload > threshold) {return currentDocId;} else{	}
										
							}
							else 
							{ // not all cursors upto the pivot are aligned at the same doc yet
										// decreases pivotIdx to the first stream pointing at the pivotDocId
								while (pivotIdx && streamDocIds[streamIndices[pivotIdx-1]] == pivotDocId) 
								{--pivotIdx;}
								moveStreamsAndSort(pivotIdx);
							//moveStreamsToDocAndSort(pivotIdx, pivotDocId);
							}
                        }        
                        else
                        {
							int posMin,posMax;    
							d = GetNewCandidate(pivotIdx,posMin,posMax,pivotDocId);
							if(d<=pivotDocId){moveStreamsAndSort(pivotIdx+1);}
							if(d>pivotDocId){moveStreamsToDocAndSort(pivotIdx+1, d);}
                        }
     			
		}  /* while (true) */
	}
        void NextShallow(unsigned int doc,int index)
        { 
         
             int cont = 0;
            while(doc > (*streams)[streamIndices[index]]->docId_bloque_end(bloqueNow[streamIndices[index]]) && bloqueNow[streamIndices[index]]!=( (*streams)[streamIndices[index]]->numeroBloques()) )
            {
            (*streams)[streamIndices[index]]->nextBloque();//avanzar el bloque del stream
            bloqueNow[streamIndices[index]]=(*streams)[streamIndices[index]]->bloque_actual();
            cont ++;
   
            }
        }
        
        bool CheckBlockMax(const float threshold,int pivot,unsigned int d)
        {
            float maxposs = 0.0;
       
                
            for(int idx=0;idx <= pivot; idx++)
            {
               
            maxposs += scorer->ub_query_terms(streamIndices[idx],bloqueNow[streamIndices[idx]]);//ACA LOS NUEVOS ALGORITMOS
            }
            if(maxposs>threshold){return true;}
            else return false;
        }
        
        unsigned int GetNewCandidate(int frontier,int &posMin,int &posMax,unsigned int docin)
        {
            unsigned int D = (*streams)[streamIndices[0]]->docId_bloque_end(bloqueNow[streamIndices[0]]);
            float Dmax = scorer->ub_query_terms(streamIndices[0],bloqueNow[streamIndices[0]]);
            posMin = 0;
            posMax = 0;
            for(int i=0;i<=frontier;i++)
            {
                if((*streams)[streamIndices[i]]->docId_bloque_end(bloqueNow[streamIndices[i]]) <= D)
                { 
                    
                    D = (*streams)[streamIndices[i]]->docId_bloque_end(bloqueNow[streamIndices[i]]); 
                    posMin = i;
                }
                if( scorer->ub_query_terms(streamIndices[i],bloqueNow[streamIndices[i]])>Dmax)
                { Dmax =  scorer->ub_query_terms(streamIndices[i],bloqueNow[streamIndices[i]]); 
                  posMax = i;
                  
                }
            }      
            
            for(int i = frontier + 1; i<numStreams;i++)
            {
            if(streamDocIds[streamIndices[i]] < D && streamDocIds[streamIndices[i]] >= docin) // CAMBIO 2014 && streamDocIds[streamIndices[i]] >= docin
            { 
                D = streamDocIds[streamIndices[i]]; 
                posMin = i;
                // cout<<" if 2- "<<D<<endl;
            }

            }
        return D;
        
        }        
        
        bool findPivotFeatureIdxBlocks(const float threshold, int &pivotIdx){
		//cout<<"Wand::findPivotFeatureIdx - inicio (threshold "<<threshold<<")\n";
		// The upper bound accumulation is done by the scorer. Different types of scorers might do upper bound accumulation differently.
		scorer->resetForPivotFeatureSelection();
		int idx;
               
		float accumUB = 0.0;
        unsigned int P;
           
        for (idx=0; accumUB <= threshold && idx < numStreams; ++idx) {
			accumUB = scorer->accumulateUpperBound(streamIndices[idx]);
		}
		if( accumUB >= threshold ) {
			pivotIdx = idx - 1;
                       
                        P = streamDocIds[streamIndices[pivotIdx]];
                        while (pivotIdx < numStreams-1 && streamDocIds[streamIndices[pivotIdx+1]] == P) {
					++pivotIdx;
				}        
                        return true;
		} 
             	return false; 
	}
	
		bool findPivotFeatureIdxBaseLine(const float threshold, int &pivotIdx){
		//cout<<"Wand::findPivotFeatureIdx - inicio (threshold "<<threshold<<")\n";
		// The upper bound accumulation is done by the scorer. Different types of scorers might do upper bound accumulation differently.
		scorer->resetForPivotFeatureSelection();
		int idx;
		float accumUB = 0.0;
		//2014 para funcion AND
		if(tipoWAND_AND_OR==1)//
		{
			if(numStreams<numTermQ)//
			return false;//
			
			for (idx=0; idx < numTermQ; ++idx) {
				accumUB = scorer->accumulateUpperBound(streamIndices[idx]);
			}
			if( accumUB >= threshold ) {
				pivotIdx = idx - 1;
				//cout<<"Wand::findPivotFeatureIdx - encontrado (posicion "<<pivotIdx<<", accumUB "<<accumUB<<")\n";
				return true;
			}
			
		}
		else
		{
		
			for (idx=0; accumUB <= threshold && idx < numStreams; ++idx) {
				accumUB = scorer->accumulateUpperBound(streamIndices[idx]);
			}
			if( accumUB >= threshold ) {
				pivotIdx = idx - 1;
				//cout<<"Wand::findPivotFeatureIdx - encontrado (posicion "<<pivotIdx<<", accumUB "<<accumUB<<")\n";
				return true;
			}
	    } 
		//cout<<"Wand::findPivotFeatureIdx - No encontrado (accumUB "<<accumUB<<")\n";
		return false; 
	}
        
         bool bloqueRevisado(int idx)
        {
            if(bloqueNow[streamIndices[idx]]==bloqueReview[streamIndices[idx]])
            return true;
            else
            return false;
        }

	
void moveStreamsAndSort(const int numStreamsToMove)
{ 
		for (int i=0; i<numStreamsToMove; ++i) {
                streamDocIds[streamIndices[i]] = (*streams)[streamIndices[i]]->next();
		}
		sortMerge(numStreamsToMove);	
}


void sortMergeUB(const int numStreamsToMove)
{
for (int i=0; i<numStreamsToMove; ++i) {
    
streamUB[streamIndices[i]]=scorer->ub_query_terms(streamIndices[i],bloqueNow[streamIndices[i]]);
//streamIndicesAux[i] = streamIndices[i];//scorer->ub_query_terms(i,bloqueNow[streamIndices[i]]);

}
sort(streamIndices, streamIndices+numStreamsToMove,UBComparator);


}


/*
void moveStreamsToDoc(const int numStreamsToMove, const int desiredDocId){
int bloque,bloqueTMP;
for(int i=0; i<numStreamsToMove; ++i) {
//			streamDocIds[streamIndices[i]] = (*streams)[streamIndices[i]]->next(desiredDocId);
//Inicio traza
bloqueTMP=bloqueNow[streamIndices[i]];
streamDocIds[streamIndices[i]] = (*streams)[streamIndices[i]]->next(desiredDocId,bloque); //original
//streamDocIds[streamIndices[i]] = (*streams)[streamIndices[i]]->next(bloque); //solo temporal para no saltar bloques que contengan documentos candidatos
bloqueNow[streamIndices[i]]=bloque;


if(bloqueTMP!=bloque)
{
    bloquesDescomprimidos++;
}
		}

	}
	
	*/
void sortMerge(const int numStreamsToMove)
{
		for (int i=0; i<numStreamsToMove; ++i) {
			streamIndicesAux[i] = streamIndices[i];
		}
		sort(streamIndicesAux, streamIndicesAux+numStreamsToMove, streamComparator);

		int j=numStreamsToMove, k=0, i=0;
		while (i < numStreamsToMove && j < numStreams) {
			if (streamComparator(streamIndicesAux[i], streamIndices[j])) {
				streamIndices[k++] = streamIndicesAux[i++];

			}
			else {
				streamIndices[k++] = streamIndices[j++];
			}
		}

		if (j == numStreams) {
			while (i < numStreamsToMove) {
				streamIndices[k++] = streamIndicesAux[i++];
			}
		}

		while (numStreams && streamDocIds[streamIndices[numStreams-1]] == END_OF_STREAM) {
			--numStreams;
		}
	}

void moveStreamsToDocAndSort(const int numStreamsToMove, const int desiredDocId){
for(int i=0; i<numStreamsToMove; ++i) {
streamDocIds[streamIndices[i]] = (*streams)[streamIndices[i]]->next(desiredDocId);
		}
		sortMerge(numStreamsToMove);
	}

};

#endif

