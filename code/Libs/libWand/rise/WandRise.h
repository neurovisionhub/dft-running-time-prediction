/* 
 * File:   WandRise.h
 * Author: oscar
 *
 */


#if !defined(_WANDRISE_H)
#define _WANDRISE_H
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

#define END_OF_STREAM 999999999

using namespace std;

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
	unsigned int ub_inicial;
        
        //Fancy list
        int nH;
        bool *bandera;
	//lista alta con UB alto
	unsigned int *id_doc_l_GH;
	unsigned int *payload_l_GH;
        //posicion lista Grupo High
	int pos_GH;
        unsigned int ub_inicial_GH;	
 
        //Bloques
        //vector ub_bloques        
 	unsigned int *ub_bloques;
	//vector idDoc_bloques
	unsigned int *idDoc_bloques; 
	//numero de bloques 
        int nBloques;
	//bloque actual
        int bloqueActual;
	
    //
    
//	unsigned int 
	
	PostingStream(){
                //Base line
		n=0;
		id_doc_l=NULL;
		payload_l=NULL;
		pos=0;
		ub_inicial=0;
                //Fancy
                nH=0;
		id_doc_l_GH=NULL;
		payload_l_GH=NULL;
		pos_GH=0;
		ub_inicial_GH=0;
	}
	//Base line
	PostingStream(int _n, unsigned int *_id_doc_l, unsigned int *_payload_l){
		n=_n;
		id_doc_l=new unsigned int[n];
		payload_l=new unsigned int[n];
		for(int i=0; i<n; i++){
			id_doc_l[i]=_id_doc_l[i];
			payload_l[i]=_payload_l[i];
		}
		pos=0;
		ub_inicial=0;
	}
        //Bloques
        PostingStream(list< pair<unsigned int, unsigned int> > *lista,vector<unsigned int> UbBloques,vector<unsigned int> idDocFronteras,unsigned int _ub_inicial){
                //cout<<"PostingStream ("<<lista->size()<<" elementos) \n";
                n=lista->size();
                id_doc_l=new unsigned int[n];
                payload_l=new unsigned int[n];
                list< pair<unsigned int, unsigned int> >::iterator it;
                it=lista->begin();
                for(int i=0; i<n; i++){
                        id_doc_l[i]=it->first;
                        payload_l[i]=it->second;
//printf("[%u,%u]",id_doc_l[i],payload_l[i]);

                        it++;
                }
 //printf("lista_size= %d\n",n);
                n=UbBloques.size(); 
                nBloques=UbBloques.size();
                ub_bloques = new unsigned int[n];
               // ub_bloques = UbBloques; buscar forma de carga directa
   //             printf("ubbloques_size= %d\n",n);
		for(int i=0;i<n;i++)
                {
                ub_bloques[i]=UbBloques[i]; 
	//	printf("UB_BLO[%u]",ub_bloques[i]);
               
 		}

                n=idDocFronteras.size();
     //             printf("doc_fronteras_size= %d\n",n);
                idDoc_bloques = new unsigned int[n];
                for(int i=0;i<n;i++)
                {
		idDoc_bloques[i]=idDocFronteras[i];
		
		

		}  
                
		pos=0;
                ub_inicial=_ub_inicial;
         bloqueActual=0;
         n=lista->size();//esto faltaba y estube mas horas que la @#$%        
        //cout<<"PostingStream ok\n";
        }


	//Base line
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
		ub_inicial=0;
		//cout<<"PostingStream ok\n";
	}
        //Base line
	PostingStream(list< pair<unsigned int, unsigned int> > *lista, unsigned int _ub_inicial){
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
		//cout<<"PostingStream ok\n";
	}
        
 	~PostingStream(){
                //base
		if(id_doc_l!=NULL){
			delete [] id_doc_l;
			id_doc_l=NULL;
		}
		if(payload_l!=NULL){
			delete [] payload_l;
			payload_l=NULL;
		}
                //bloques
               	if(ub_bloques!=NULL){
                        delete [] ub_bloques;
			ub_bloques=NULL;
		}
		if(idDoc_bloques!=NULL){
			delete [] idDoc_bloques;
			idDoc_bloques=NULL;
		}
                
                //fancy
                if(id_doc_l_GH!=NULL){
                        delete [] id_doc_l_GH;
			id_doc_l_GH=NULL;
		}
		if(payload_l_GH!=NULL){
			delete [] payload_l_GH;
			payload_l_GH=NULL;
		}

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

        unsigned int payload_pos(int indice)
        {
            return payload_l[indice];
        }

       unsigned int docId_pos(int indice)
        {
            return id_doc_l[indice];
        }
	//retorna el numero de docs
	int count(){
		return n;
	}
	
 	int bloque_actual()
	{
		return bloqueActual;
	}

        int numeroBloques()
        {
        return nBloques;
        }

        void nextBloque()
        {

            bloqueActual++;
        }

        void prevBloque()
        {
            bloqueActual--;

        }

        unsigned int docId_bloque_ini(int indice)
        {
		return idDoc_bloques[indice];
	}

        unsigned int docId_bloque_end(int indice)
        {
                return idDoc_bloques[indice+1];
        }

        // Next bloque
        unsigned int next(unsigned int desiredDocId,int &posBloque)
        {
            while(pos<(n-1) && id_doc_l[pos]<desiredDocId)
           {
                pos++;
           }
                       if(pos>=(n-1))
                       {
                               if(bloqueActual!=nBloques-1)
                               {
                                   while(id_doc_l[pos-1]>=idDoc_bloques[bloqueActual+1])
                                   {
                                           bloqueActual++;
                                   }
                               }

                       posBloque=bloqueActual;
                       return END_OF_STREAM;
                       }
                       else
                       {      	
                           if(bloqueActual!=nBloques-1)
                           {
                                   while(id_doc_l[pos]>=idDoc_bloques[bloqueActual+1])
                                   {
                                           bloqueActual++;
                                   }
                           }
                           posBloque=bloqueActual;
                           return id_doc_l[pos];
                       }
       }
        //Base line & Bloques
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

        unsigned int back()
        {
            pos--;
            return id_doc_l[pos];

        }
        
        //Base line & Bloques
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

                if(bloqueActual!=nBloques-1)
                { 
                if(id_doc_l[pos]<idDoc_bloques[bloqueActual+1])
                {


                }
                else
                {
//   			printf("NEXT (%d,%d)[%u,%u]-",bloqueActual,nBloques-1,id_doc_l[pos],idDoc_bloques[bloqueActual+1]);
  //                      printf("Cambio de bloque [%d]->[%d]",bloqueActual,bloqueActual+1);
                        bloqueActual++;


                }}



			return id_doc_l[pos];
		}
	}

       //Bloques
        //Aca posible arreglo de evitar retroceso de bloques... verificar
        unsigned int next(int &posBloque){
           // printf(" NEXTB ");
       // printf("NEXT (%d,%d)[%u,%u]-",bloqueActual,nBloques-1,id_doc_l[pos],idDoc_bloques[bloqueActual+1]);
                //notar que llega hasta (n-1), pues se pide el proximo, que no existe
                //si hay 0 o 1 doc, no hay next, deben haber al menos 2 (el actual, y el proximo)
                if(pos>=(n-1))
                {
               /*     if(bloqueActual!=nBloques-1)
                {
                	if(id_doc_l[pos-1]>=idDoc_bloques[bloqueActual+1])
                	{
                    // printf("NEXT (%d,%d)[%u,%u]-",bloqueActual,nBloques-1,id_doc_l[pos],idDoc_bloques[bloqueActual+1]);
                   //     printf("Cambio de bloque [%d]->[%d]",bloqueActual,bloqueActual+1);
                        bloqueActual++;

                	}
		}*/
                    posBloque=bloqueActual;
                        //printf("****** pos[%d] n[%d] ******",pos,n);
                        return END_OF_STREAM;
                }
                else{ pos++;
                if(bloqueActual!=nBloques-1)
                {
                	if(id_doc_l[pos]>=idDoc_bloques[bloqueActual+1])
                	{
                    // printf("NEXT (%d,%d)[%u,%u]-",bloqueActual,nBloques-1,id_doc_l[pos],idDoc_bloques[bloqueActual+1]);
                   //     printf("Cambio de bloque [%d]->[%d]",bloqueActual,bloqueActual+1);
                        bloqueActual++;

                	}
		}

              //      printf(" N[nBloques:%d,%d][ %u ] ",nBloques,bloqueActual,id_doc_l[pos]);
//printf(" [nBloques:%d] ",nBloques);

                // printf(" Next [nBloques:%d,%d][ %d:%d,%u ] ",nBloques,bloqueActual,pos,n-1,id_doc_l[pos]);
                        posBloque=bloqueActual;
                //        printf(" // ");
                        return id_doc_l[pos];
                }
        }	
};

//class QueryFeatureInfoT
class QueryFeatureInfoT{
public:
        //Base line
	unsigned long long id_term;
	unsigned int weight;
	unsigned int ub_ini;
	unsigned int ub;
        //Bloques
        unsigned int *ub_bloques_intervalo; //ub locales

	QueryFeatureInfoT(){
		id_term=0;
		weight=0;
		ub_ini=0;
		ub=0;
	}
	QueryFeatureInfoT(unsigned long long _id_term, unsigned int _weight, unsigned int _ub_ini){
		//cout<<"QueryFeatureInfoT ("<<_id_term<<", "<<_weight<<", "<< _ub<<")\n";
		id_term=_id_term;
		weight=_weight;
		ub_ini=_ub_ini;
		ub=0;
		//cout<<"QueryFeatureInfoT ok\n";
	}
        QueryFeatureInfoT(unsigned long long _id_term,unsigned int _weight){
	id_term=_id_term;
        weight=_weight;
	}



	~QueryFeatureInfoT(){
            //Bloques
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
        vector<QueryFeatureInfoT*> *query_terms;
        //
        vector<PostingStream*> *streamsLows;
	vector<QueryFeatureInfoT*> *query_terms_low;
        
        //Bloques
        vector<vector<unsigned int> > query_terms_bloques_pesos;

        int *banderaQ;
        int *posBanderaQ;
	unsigned int sum_ub;
	int count;
        int dimQ;
        int *posTmp;
        int sumaBloquesComprimidos;
   
        /*
	ScorerT(){
                streams=NULL;
		query_terms=NULL;
		sum_ub=0;
		count=0;
	}
        */
        
        //Base line & Bloques
        ScorerT(vector<PostingStream*> *_streams, vector<QueryFeatureInfoT*> *_query_terms){
                streams=_streams;
                query_terms=_query_terms;
                int dimL=streams->size();
                dimQ=dimL;
                query_terms_bloques_pesos.resize(dimL);
                int nLocal;
                sumaBloquesComprimidos=0;
               
                for(int i=0;i<dimL;i++)
                {
                  nLocal=(*streams)[i]->nBloques;
                  query_terms_bloques_pesos[i].resize(nLocal);
               
                   for(int j=0;j<nLocal;j++)
                   {
                   query_terms_bloques_pesos[i][j]=(*streams)[i]->ub_bloques[j]*(*query_terms)[i]->weight; 
                   sumaBloquesComprimidos++;
    		   } 
		}

                sum_ub=0;
                count=0;
        }

	//Fancy list
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
	
	//los punteros son de llamador
	~ScorerT(){
		streams=NULL;
		query_terms=NULL;
		sum_ub=0;
		count=0;
                
        // ******* Borrar punteros *******

      
    }

        //Bloques
        void scoreCurrentDocIntervalos(const unsigned int docId, const int *streamIdx, const int startIdx, const int endIdx, unsigned int &result){
                count++;
                unsigned int score = 0;
                for (int i = startIdx; i <= endIdx; ++i) {
                        score += (*streams)[streamIdx[i]]->payload()
                                        * (*query_terms)[streamIdx[i]]->weight;


              //     if(docId == 3604480)
               //   printf("Doc [%u][%d]-[%d] Term[%llu] high score = %d = ( %d * %d ) **** ",docId,i,streamIdx[i],(*query_terms)[streamIdx[i]]->id_term,score,(*streams)[streamIdx[i]]->payload(),(*query_terms)[streamIdx[i]]->weight);

                }
                result=score;
                return;
        }

        //Base line
	void scoreCurrentDoc(const unsigned int docId, const int *streamIdx, const int startIdx, const int endIdx, unsigned int &result){
		count++;
		unsigned int score = 0;
		for (int i = startIdx; i <= endIdx; ++i) {
			score += (*streams)[streamIdx[i]]->payload()
					* (*query_terms)[streamIdx[i]]->weight;
		}
		result=score;
		return;
	}

        //Fancy-list
        void scoreCurrentDocHigh(const unsigned int docId, const int *streamIdx, const int startIdx, const int endIdx, unsigned int &result){
		count++;
		unsigned int score = 0;
            //    unsigned int sumaScore = 0;
                int suma = 0;
                unsigned int idDocTmp = 0;

               //  printf("Doc [%u] ScoreIni= [%d]\n **** ",docId,score);
		for (int i = startIdx; i <= endIdx; ++i) {


			score += (*streams)[streamIdx[i]]->payload()
					* (*query_terms)[streamIdx[i]]->weight;
                        
                        posBanderaQ[streamIdx[i]]=1;
                        suma++;
         	}
                
                if(suma<dimQ)
                {
                    for(int i=0;i<dimQ;i++)
                    {
                        if(posBanderaQ[streamIdx[i]]==0)
                        {
                            idDocTmp = next_tmp(docId,streamIdx[i]);
                            if(idDocTmp==docId)
                            {
                                score += (*streamsLows)[streamIdx[i]]->payload_pos(posTmp[streamIdx[i]])*(*query_terms)[streamIdx[i]]->weight;
                                (*streamsLows)[streamIdx[i]]->payload_l[posTmp[streamIdx[i]] ] =0; //en esa lista se anula el valor del peso                    
                            }
                        }
                    }
                }
                   for(int i=0;i<dimQ;i++)
                   {
                        posBanderaQ[streamIdx[i]]=0;
                   }
                
                result=score;
		return;
	}

	//Fancy-list
	unsigned int next_tmp(unsigned int desiredDocId,int termino){
            int n =  (*streamsLows)[termino]->count();
		while(posTmp[termino]<(n-1) && (*streamsLows)[termino]->docId_pos(posTmp[termino])<desiredDocId)
			posTmp[termino]++;

		if(posTmp[termino]>=(n-1)){
			return END_OF_STREAM;
		}
		else{
			return (*streamsLows)[termino]->docId_pos(posTmp[termino]);
		}
	}
        
        //All
	inline void resetForPivotFeatureSelection(){
		sum_ub = 0; 
	}
        
	//Bloques
	inline unsigned int accumulateUpperBound(const int index){
		sum_ub += (*query_terms)[index]->ub;
		return sum_ub; 
	}
        
        //Bloques
        inline unsigned int accumulateUpperBoundVariable(const int index){//nuevos
        sum_ub += (*query_terms)[index]->ub;
        return sum_ub;
        }
        
        //Bloques
        //ACA ACUMULAR UPPER BOUND CON UB VARIABLE
        inline unsigned int accumulateUpperBoundBloques(const int index,int bloque){//nuevos  
       sum_ub += query_terms_bloques_pesos[index][bloque];//ACA LOS NUEVOS ALGORITMOS
       return sum_ub;

        }

        //Bloques
	// Resta un upperBounds (en la practica previamente estimado)
        inline unsigned int DesAccumulateUpperBoundBloques(const int index,int bloque){//nuevos  
       sum_ub -= query_terms_bloques_pesos[index][bloque];//ACA LOS NUEVOS ALGORITMOS
       return sum_ub;

        }
       
        //Bloques
        inline unsigned int ub_query_terms(const int index,int bloque)
        {
                return query_terms_bloques_pesos[index][bloque];
        } 


	
};

//Bloques
class Wand{
public:
  std::ofstream ficheroTrazaWand;//
        //nuevas
	unsigned int limiteIdDocMenor;
        unsigned int limiteIdDocMayor;
  
        long unsigned int evaluacionesCompletas;	
	//se pasan como int& a los metodos
	unsigned int currentDocId;
	unsigned int currentPayload;
//int currentPayload;	
	unsigned int *streamDocIds;
        int *streamUB;
	
	// two arrays of indices into the _streams vector. This is used for merge.
	// inplace_merge is not as efficient as the copy merge.
	int *streamIndices;
	int *streamIndicesAux;
	
	// maximum number of streams that WAND can operate on
	int maxNumStreams;
	
	// number of streams present in the query
	int numStreams;
	
	// scorer
	ScorerT *scorer; 
	
	//PostingStreamPtr se cambiara por PostingStream (sin puntero) por ahora
	vector<PostingStream*> *streams;

        //traza
        unsigned docTraza;
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

                const int *streamUB;
	public:
		StreamComparatorUB(){
                        streamUB=NULL;
		}
		StreamComparatorUB(const int *_streamUB){
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
	
        //Verificar
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
	//	StreamComparatorUB=StreamComparator(streamDocIds);
		currentDocId=END_OF_STREAM;
		currentPayload=-1;
	}
	
        //Bloques
	Wand(int _maxNumStreams, ScorerT *_scorer, vector<PostingStream*> *_streams){
		//cout<<"Wand ("<<_maxNumStreams<<" max, "<<_streams->size()<<" streams)\n";
		maxNumStreams=_maxNumStreams;
		numStreams=0;
		
		scorer=_scorer;
		streams=_streams;
		
		streamDocIds=new unsigned int[maxNumStreams];
		streamIndices=new int[maxNumStreams];
		streamIndicesAux=new int[maxNumStreams];
                streamUB=new int[maxNumStreams];
		
		streamComparator=StreamComparator(streamDocIds);
                UBComparator=StreamComparatorUB(streamUB);

                //UBComparator=StreamComparatorUB(streamUB);
		currentDocId=END_OF_STREAM;
		currentPayload=-1;
		//cout<<"Wand ok\n";
	}
        //Bloques	
        ~Wand(){
     
                if(scorer!=NULL){
                  //      scorer->~ScorerT();
			delete scorer;
  //                      scorer = NULL;
		}
 
               if(streams!=NULL){
                  //      scorer->~ScorerT();
			delete streams;
//                        streams = NULL;
		}

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
		currentPayload=-1;
		//cout<<"~Wand - fin\n";
	}

        //Bloques
	void init(){
		//cout<<"Wand::init - inicio\n";
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
                streamUB = new int[numStreams];




                //       ficheroTrazaWand.open ("traza_2f.dat");
              //  ficheroTrazaWand << "Inicio Traza";
       
		if (numStreams == 0) {
			currentDocId = END_OF_STREAM;
			currentPayload = -1;
		}
		
		//los indices parten en su posicion natural (0.. n-1)
		for (int i=0; i<numStreams; ++i) {
			streamIndices[i] = i;
                        bloqueNow[i]=0;//(*streams)[i]->idDoc_bloques[1];
                        bloqueSize[i]=(*streams)[i]->nBloques;
                        streamUB[i]=0;
		}
		
		//reseteo los streams antes de empezar
		for (int i=0; i<numStreams; ++i) {
			(*streams)[i]->reset();
			streamDocIds[i] = (*streams)[i]->docId();
		}

		sort(streamIndices, streamIndices+numStreams, streamComparator);
		
	//	cout<<"Wand::init - fin\n";
		
	}

        //opcion 0->parte alta y seguimiento en ambos iteradores, No activa
        //opcion 1->parte baja y seguimiento normal pero con banderas, No activa
        
        //Bloques & No ( fancy o Base line)
	int next(unsigned int threshold,int opcion){
		
		int pivotIdx=-1;
		unsigned int pivotDocId = END_OF_STREAM;
                int UB;
		while (true) 
                {
	 		
	 		if(numStreams==0){
				//cout<<"Wand::next - No quedan streams\n";
				currentDocId = END_OF_STREAM;
				currentPayload = -1;
                                if(bloquesDescomprimidos<numeroListasReal) //cuando listas no tienen k documentos
                                bloquesDescomprimidos=bloquesDescomprimidos+numeroListasReal;
                                //bloquesDescomprimidos=bloquesDescomprimidos+numeroListasReal;

                      //          printf("******** FIN LISTAS - Descompresión real: [%d] Descompresion simulada por score: [%d]\n",bloquesDescomprimidos,contadorTMP);
				return END_OF_STREAM;
	 		}
	 		//aca cambios
			if (!findPivotFeatureIdx(threshold, pivotIdx,UB)) {
				//cout<<"Wand::next - ! findPivotFeatureIdx (EOS)\n";
				currentDocId = END_OF_STREAM;
				currentPayload = -1;
                                if(bloquesDescomprimidos<numeroListasReal) //cuando listas no tienen k documentos
                                bloquesDescomprimidos=bloquesDescomprimidos+numeroListasReal;
                                return END_OF_STREAM; //aca modificar porque siempre que el umbral es muy alto, no seleccionara el siguiente doc con ub mas pequeño y asume q termino lista
			}
			
			//cout<<"Wand::next - pivotIdx: "<<pivotIdx<<"\n";
			pivotDocId = streamDocIds[streamIndices[pivotIdx]];
	
			if (streamDocIds[streamIndices[0]] == streamDocIds[streamIndices[pivotIdx]])
			{
                        	currentDocId = pivotDocId;
				// Advance pivotIdx sufficiently so that all instances of pivotDocId are included
				while (pivotIdx < numStreams-1 && streamDocIds[streamIndices[pivotIdx+1]] == pivotDocId) 
                                {
					++pivotIdx;
				}
                                
                                //Fancy-list
                                if(opcion==0)
                                scorer->scoreCurrentDocHigh(currentDocId, streamIndices, 0, pivotIdx, currentPayload);
                                
                                //Base-Line
                                if(opcion==1)
                                scorer->scoreCurrentDoc(currentDocId, streamIndices, 0, pivotIdx, currentPayload);
                                   
                                //Bloques    
								if(opcion==2)
                                scorer->scoreCurrentDocIntervalos(currentDocId, streamIndices, 0, pivotIdx, currentPayload);

                                evaluacionesCompletas++;

                                if(bloquesEvaluados!=bloquesDescomprimidos)
                                {
                                    contadorTMP++;
                                    bloquesEvaluados=bloquesDescomprimidos;
                                }

				moveStreamsAndSort(pivotIdx+1);
		
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

        //Bloques
        bool findSubBlockCandidate(const unsigned threshold,int idx,unsigned int idDocBorder,unsigned int accumUB_Actual,unsigned int &accumUB)
        {
      
        int contador=0;
        unsigned int sumaTmp = accumUB_Actual;
        accumUB=accumUB_Actual;
        //unsigned int sumaRes=0;

        int streams_idx_1 = streamIndices[idx-1];
    //    int streams_idx   = streamIndices[idx];
        int bloqueA =  bloqueNow[streamIndices[idx-1]];
        int bloqueB = bloqueA;
        int nBloquesB = (*streams)[streams_idx_1]->nBloques-1;
        
          if(sumaTmp>=threshold) 
          {
                accumUB=sumaTmp;
                return true;
    
          }

        while(( idDocBorder >= (*streams)[streams_idx_1]->docId_bloque_end(bloqueB) )&&(bloqueB<nBloquesB))
        {
            if(contador==0)
            sumaTmp-=scorer->ub_query_terms(streams_idx_1,bloqueNow[streams_idx_1]);

            (*streams)[streams_idx_1]->nextBloque();
            bloqueNow[streams_idx_1]=(*streams)[streams_idx_1]->bloque_actual();
            bloqueB = bloqueNow[streams_idx_1];
            sumaTmp+=scorer->ub_query_terms(streams_idx_1,bloqueNow[streams_idx_1]);

            if(sumaTmp>=threshold) 
            {
            accumUB=sumaTmp;
            //bloquesDescomprimidos++;
            streamDocIds[streams_idx_1] = (*streams)[streams_idx_1]->next( (*streams)[streams_idx_1]->docId_bloque_ini(bloqueB) );
            //bloqueReview[streamIndices[idx-1]]=bloqueNow[streamIndices[idx-1]];

            return true;
            //sumaTmp=sumaTmp-scorer->ub_query_terms(streamIndices[idx-1],bloqueNow[ streamIndices[idx-1]];
            }
            else if(bloqueB<nBloquesB-1)
            {
            sumaTmp-=scorer->ub_query_terms(streams_idx_1,bloqueNow[streams_idx_1]);
           // sumaRes=sumaTmp;
            }
        contador++;       
        }
            if(contador>0)
            {
                   accumUB=sumaTmp;
                   return false;
            }

        return false;
        }

        //Bloques
        //arreglar esta funcion es vital para el conteo
        bool bloqueRevisado(int idx)
        {
            if(bloqueNow[streamIndices[idx]]==bloqueReview[streamIndices[idx]])
            return true;
            else
            return false;
        }
        //Bloques
	bool findPivotFeatureIdx(const unsigned int threshold, int &pivotIdx,int &UB){
		//cout<<"Wand::findPivotFeatureIdx - inicio (threshold "<<threshold<<")\n";
		// The upper bound accumulation is done by the scorer. Different types of scorers might do upper bound accumulation differently.
		scorer->resetForPivotFeatureSelection();
                bool response=false;
		int idx,bloque,streams_idx_1,streams_idx;
                unsigned int docTmp=1;
                int bandera_local=0;//en 1 si ya no quedan documentos que evaluar
		unsigned int accumUB = 0;
                unsigned int accumUB_bloque=0;
//if(currentDocId==8508928)
//printf(" P5 docTmp:(curId:%u,curP:%d) ",currentDocId,currentPayload);
		for (idx=0; accumUB <= threshold && idx < numStreams; ++idx) 
                { 
                    streams_idx_1 = streamIndices[idx-1];
                    streams_idx   = streamIndices[idx];
                    bloque = bloqueNow[streamIndices[idx]];
                    docTmp = streamDocIds[streamIndices[idx]];
                    bandera_local=0;

                    if(numStreams>1)
                    {    if(idx>0)
                        {
                            if(findSubBlockCandidate(threshold,idx,docTmp,accumUB,accumUB_bloque)) //accumUB dinamico dependiente del bloque activado
                            {
                                accumUB = accumUB_bloque;
                                pivotIdx = idx - 1;
                                UB=accumUB;
                                return true;
                            }
                            else
                            {
                                accumUB = accumUB_bloque + scorer->ub_query_terms(streamIndices[idx],bloque);
                            }
                        }
                        else
                        {
                            //accumUB = scorer->accumulateUpperBoundBloques(streams_idx,bloque);
                         accumUB = scorer->accumulateUpperBoundBloques(streamIndices[idx],bloque);
                      //   printf(" /B[%d,%d][%u][%d](%d)/ ",bloque,idx,streamDocIds[streamIndices[idx]],accumUB,threshold);
                        }
                    }
                    else
                    {
                       accumUB = scorer->accumulateUpperBoundBloques(streamIndices[idx],bloque);
                    }
                }//fin for

                UB=accumUB;
		if( accumUB >= threshold )
                {
                    pivotIdx = idx - 1;
                    return true;
		}

                if(numStreams>0)//aca se debe cambiar la condicion ya q el ultimo bloque es recorrido completo
                {
                    if(numStreams>1)
                    {
                        //aca buscar el siguiente candidato.......                 
                        sortMergeUB(numStreams); //ordenar streams por UB
                        if(bloqueNow[streamIndices[0]] <= (*streams)[streamIndices[0]]->nBloques - 1 )
                        { (*streams)[streamIndices[0]]->nextBloque();//avanzar el primer bloque del stream

                        bloqueNow[streamIndices[0]]=(*streams)[streamIndices[0]]->bloque_actual();
                        streamDocIds[streamIndices[0]] = (*streams)[streamIndices[0]]->next((*streams)[streamIndices[0]]->docId_bloque_ini( bloqueNow[streamIndices[0]])); //cambiar este next por un salto


                       // streamDocIds[streamIndices[0]] = (*streams)[streamIndices[0]]->back(); //aca una prueba


                        //retroceder un doc para streamIndice[0]
                        sortMerge(numStreams);
                        }
                    //    exit(1);
                //       moveStreamsToDoc(numStreams,docTmp); //este solo original....
                        
               //         moveStreamsToDoc(numStreams,docTmp);  //los mueve todos, deberian avanzar por bloques...

 //cout<<" S>1 ";

                    
                    }
                    else
                    {                           
                        if(bloqueNow[streamIndices[idx - 1]] >= (*streams)[streamIndices[idx - 1]]->nBloques - 1 )
                        { return false; }

                        (*streams)[streamIndices[idx - 1]]->nextBloque();
                        bloqueNow[streamIndices[idx - 1]]=(*streams)[streamIndices[idx - 1]]->bloque_actual();
                        streamDocIds[streamIndices[idx - 1]] = (*streams)[streamIndices[idx - 1]]->next((*streams)[streamIndices[0]]->docId_bloque_ini( bloqueNow[streamIndices[idx - 1]])); //cambiar este next por un salto
                        

                        if(streamDocIds[streamIndices[idx - 1]]==END_OF_STREAM)
                        {return false;}

               //          cout<<" S==1 ";
                    }



           
           //   cout<<"\n";



              moveStreamsAndSort(numStreams);//todos avanzan 1 doc
              response=findPivotFeatureIdx(threshold,pivotIdx,UB);
              

              //return findPivotFeatureIdx(threshold,pivotIdx,UB);

		}

            else
            {
                    UB=accumUB;
                    return false;
            }
//cout<<"Wand-FALSE B[["<<bloqueNow[streamIndices[idx - 1]]<<"]::findPivotFeatureIdx - inicio (threshold "<<threshold<<")\n";
//return true;
return response;
	}


        //Bloques
        void moveStreamsAndSort_Unico(const int numStreamsToMove,const int ST)
        {
            int bloque,bloqueTMP;
            //cout<<"Wand::moveStreamsAndSort - inicio (numStreamsToMove "<<numStreamsToMove<<")\n";
            for (int i=0; i<numStreamsToMove; ++i) 
            {
                bloqueTMP=bloqueNow[streamIndices[i]];
                streamDocIds[streamIndices[i]] = (*streams)[streamIndices[i]]->next(bloque);
                bloqueNow[streamIndices[i]]=bloque;

                if(bloqueTMP!=bloque)
                {
                    bloquesDescomprimidos++;
                }
            }

            sortMerge(ST);
        }

        //Bloques
        //aca modificar ? o puede no ser necesario...	
        void moveStreamsAndSort(const int numStreamsToMove)
        { 
                int bloque,bloqueTMP;
                //cout<<"Wand::moveStreamsAndSort - inicio (numStreamsToMove "<<numStreamsToMove<<")\n";
                for (int i=0; i<numStreamsToMove; ++i) 
                {
                    //Inicio traza
                    bloqueTMP=bloqueNow[streamIndices[i]];
                    streamDocIds[streamIndices[i]] = (*streams)[streamIndices[i]]->next(bloque);
                    //streamDocIds[streamIndices[i]] = (*streams)[streamIndices[i]]->next();
                    bloqueNow[streamIndices[i]]=bloque;
                    //printf(" \nbloqueTMP[%d] ,",bloqueTMP);
                    //printf(" BLOQUE_T:[%d][%d][%d] ",bloque,bloqueNow[streamIndices[i]],streamIndices[i]);
                    if(bloqueTMP!=bloque)
                    {
                        bloquesDescomprimidos++;
                    //printf(" \nmoveStreamsAndSort bloqueTMP[%d] ,",bloqueTMP);
                    //printf(" BLOQUE_T:[b:%d][bnow:%d][st:%d] \n",bloque,bloqueNow[streamIndices[i]],streamIndices[i]);
                    }
		}
		sortMerge(numStreamsToMove);
		//cout<<"Wand::moveStreamsAndSort - fin\n";
	}

        //Bloques
        void sortMergeUB(const int numStreamsToMove)
        {
            for (int i=0; i<numStreamsToMove; ++i) 
            {
                    streamUB[streamIndices[i]]=scorer->ub_query_terms(streamIndices[i],bloqueNow[streamIndices[i]]);
            //streamIndicesAux[i] = streamIndices[i];//scorer->ub_query_terms(i,bloqueNow[streamIndices[i]]);
            }
            
            sort(streamIndices, streamIndices+numStreamsToMove,UBComparator);
        }
        //Bloques & base line
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

        //Bloques
        /*
        void moveStreamsToDoc(const int numStreamsToMove, const int desiredDocId)
        {
        int bloque,bloqueTMP;
            for(int i=0; i<numStreamsToMove; ++i) 
            {
            //			streamDocIds[streamIndices[i]] = (*streams)[streamIndices[i]]->next(desiredDocId);
            //Inicio traza
            bloqueTMP=bloqueNow[streamIndices[i]];
            streamDocIds[streamIndices[i]] = (*streams)[streamIndices[i]]->next(desiredDocId,bloque); //original
            //streamDocIds[streamIndices[i]] = (*streams)[streamIndices[i]]->next(bloque); //solo temporal para no saltar bloques que contengan documentos candidatos
            bloqueNow[streamIndices[i]]=bloque;


            if(bloqueTMP!=bloque)
            {
                bloquesDescomprimidos++;
            //printf(" B_T[%d],",bloque);
            //printf(" BLOQUE_T:[%d][%d][%d]",bloque,bloqueNow[streamIndices[i]],streamIndices[i]);
            }
            //Fin traza

            }

        }
        */

        //Bloques
        void moveStreamsToDocAndSort(const int numStreamsToMove, const int desiredDocId)
        {
            int bloque,bloqueTMP;		
            for(int i=0; i<numStreamsToMove; ++i) 
            {
                //streamDocIds[streamIndices[i]] = (*streams)[streamIndices[i]]->next(desiredDocId);
                //Inicio traza
                bloqueTMP=bloqueNow[streamIndices[i]];
                //streamDocIds[streamIndices[i]] = (*streams)[streamIndices[i]]->next(desiredDocId);
                streamDocIds[streamIndices[i]] = (*streams)[streamIndices[i]]->next(desiredDocId,bloque); //original ACA FALLAAAAAAAAAAAAAA
                //streamDocIds[streamIndices[i]] = (*streams)[streamIndices[i]]->next(bloque); //solo temporal para no saltar bloques que contengan documentos candidatos
                bloqueNow[streamIndices[i]]=bloque;
                //printf(" MV_B[%d][%d],",streamIndices[i],bloque);
                //printf(" \nbloqueTMP[%d] ,",bloqueTMP);
                //printf(" BLOQUE_T:[%d][%d][%d] ",bloque,bloqueNow[streamIndices[i]],streamIndices[i]);
                if(bloqueTMP!=bloque)
                {
                    bloquesDescomprimidos++;
                //printf(" \nmoveStreamsToDocAndSort bloqueTMP[%d] ,",bloqueTMP);
                //printf(" BLOQUE_T:[b:%d][bnow:%d][st:%d] \n ",bloque,bloqueNow[streamIndices[i]],streamIndices[i]);
                }
      
            }
            sortMerge(numStreamsToMove);
	}
	
};

#endif

