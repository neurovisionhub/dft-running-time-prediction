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
#define END_OF_STREAM 999999999

using namespace std;

//no guarda el id_term para un map<id_term, PostingStream*>
//class PostingStream
class PostingStream{
public:
	
	//lista de (id_doc, payload)
	int n;
	unsigned int *id_doc_l;
	unsigned int *payload_l;
	
	//posicion
	int pos;
	
	//ub del termino
	unsigned int ub_inicial;
	
	PostingStream(){
		n=0;
		id_doc_l=NULL;
		payload_l=NULL;
		pos=0;
		ub_inicial=0;
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
		ub_inicial=0;
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
		ub_inicial=0;
		//cout<<"PostingStream ok\n";
	}
	
	PostingStream(list< pair<unsigned int, unsigned int> > *lista, unsigned int _ub_inicial){
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
		ub_inicial=_ub_inicial;
		//cout<<"PostingStream ok\n";
	}
	
	~PostingStream(){
		if(id_doc_l!=NULL){
			delete [] id_doc_l;
			id_doc_l=NULL;
		}
		if(payload_l!=NULL){
			delete [] payload_l;
			payload_l=NULL;
		}
		pos=0;
	}
	
	void reset(){
		pos=0;
	}
	
	//retorna el payload del doc actual
	unsigned int payload(){
		return payload_l[pos];
	}
	
	//retorna el id_doc del doc actual
	unsigned int docId(){
		return id_doc_l[pos];
	}
	
	//retorna el numero de docs
	int count(){
		return n;
	}
	
	//mueve el cursor hasta desiredDocId o uno despues si no esta
	//retorna el id_doc resultante, o END_OF_STREAM si llega al final
	unsigned int next(unsigned int desiredDocId){
	
		while(pos<(n-1) && id_doc_l[pos]<desiredDocId)
			pos++;
	
		if(pos>=(n-1)){
			return END_OF_STREAM;
		}
		else{
			return id_doc_l[pos];
		}
	}
	
	//mueve el cursor una posicion
	//retorna el id_doc resultante, o END_OF_STREAM si llega al final
	unsigned int next(){
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
	unsigned int weight;
	unsigned int ub_ini;
	unsigned int ub;
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
	~QueryFeatureInfoT(){
		
	}
	
};

//class ScorerT
class ScorerT{
public:
	
	vector<PostingStream*> *streams;
	
	vector<QueryFeatureInfoT*> *query_terms; 
	
	unsigned int sum_ub;
	int count;
	
	ScorerT(){
		streams=NULL;
		query_terms=NULL;
		sum_ub=0;
		count=0;
	}
	
	ScorerT(vector<PostingStream*> *_streams, vector<QueryFeatureInfoT*> *_query_terms){
		streams=_streams;
		query_terms=_query_terms;
		sum_ub=0;
		count=0;
	}
	
	//los punteros son de llamador
	~ScorerT(){
		streams=NULL;
		query_terms=NULL;
		sum_ub=0;
		count=0;
	}
	
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
	
	inline void resetForPivotFeatureSelection(){
		sum_ub = 0; 
	}
	
	inline unsigned int accumulateUpperBound(const int index){
		sum_ub += (*query_terms)[index]->ub;
		return sum_ub; 
	}
	
};

class Wand{
public:
	
	//se pasan como int& a los metodos
	unsigned int currentDocId;
	unsigned int currentPayload;
	
	unsigned int *streamDocIds;
	
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
	 long unsigned int evaluacionesCompletas;
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
	
	// comparator that compares two streams
	StreamComparator streamComparator;
	
	Wand(){
		maxNumStreams=0;
		numStreams=0;
		
		scorer=NULL;
		streams=NULL;
		
		streamDocIds=NULL;
		streamIndices=NULL;
		streamIndicesAux=NULL;
		
		streamComparator=StreamComparator(streamDocIds);
		
		currentDocId=END_OF_STREAM;
		currentPayload=-1;
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
		
		streamComparator=StreamComparator(streamDocIds);
		
		currentDocId=END_OF_STREAM;
		currentPayload=-1;
		//cout<<"Wand ok\n";
	}
	
	~Wand(){
		//cout<<"~Wand - inicio\n";
		
		scorer=NULL;
		streams=NULL;
		
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
		
		currentDocId=END_OF_STREAM;
		currentPayload=-1;
		//cout<<"~Wand - fin\n";
	}
	
	void init(){
		//cout<<"Wand::init - inicio\n";
		numStreams = streams->size();
		evaluacionesCompletas=0;
		if (numStreams == 0) {
			currentDocId = END_OF_STREAM;
			currentPayload = -1;
		}
		
		//los indices parten en su posicion natural (0.. n-1)
		for (int i=0; i<numStreams; ++i) {
			streamIndices[i] = i;
		}
		
		//reseteo los streams antes de empezar
		for (int i=0; i<numStreams; ++i) {
			(*streams)[i]->reset();
			streamDocIds[i] = (*streams)[i]->docId();
		}

		sort(streamIndices, streamIndices+numStreams, streamComparator);
		
		//cout<<"Wand::init - fin\n";
		
	}
	
	int next(unsigned int threshold){
		
		//cout<<"Wand::next - inicio (threshold: "<<threshold<<")\n";
		
		// We do not check whether the stream is already at the end 
		// here based on the assumption that application won't call 
		// next() for streams that are already at the end, or atleast
		// won't do this frequently. 
		
		int pivotIdx=-1;
		unsigned int pivotDocId = END_OF_STREAM;
		
		while (true) {
	 		
	 		if(numStreams==0){
				//cout<<"Wand::next - No quedan streams\n";
				currentDocId = END_OF_STREAM;
				currentPayload = -1;
				return END_OF_STREAM;
	 		}
	 		
			if (!findPivotFeatureIdx(threshold, pivotIdx)) {
				//cout<<"Wand::next - ! findPivotFeatureIdx (EOS)\n";
				currentDocId = END_OF_STREAM;
				currentPayload = -1;
				return END_OF_STREAM;
			}
			
			//cout<<"Wand::next - pivotIdx: "<<pivotIdx<<"\n";
			
			pivotDocId = streamDocIds[streamIndices[pivotIdx]];
			
			//cout<<"Wand::next - pivotDocId: "<<pivotDocId<<"\n";
			
			if (streamDocIds[streamIndices[0]] == streamDocIds[streamIndices[pivotIdx]]) {
				//cout<<"Wand::next - if 1... candidato encontrado, evaluando\n";
				
				// Found candidate. All cursors before (*streams)[pivotIdx] point to
				// the same doc and this doc is the candidate for full evaluation.
				currentDocId = pivotDocId;
				
				// Advance pivotIdx sufficiently so that all instances of pivotDocId are included
				while (pivotIdx < numStreams-1 && streamDocIds[streamIndices[pivotIdx+1]] == pivotDocId) {
					++pivotIdx;
				}
				
				// scorer calculates the exact score for the docs [0, pivotIdx] 
				scorer->scoreCurrentDoc(currentDocId, streamIndices, 0, pivotIdx, currentPayload);
                                evaluacionesCompletas++;
				//cout<< "EC: "<< evaluacionesCompletas<<endl;
				//cout<<"Wand::next - currentPayload: "<<currentPayload<<"\n";
				
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
				//cout<<"Wand::next - else (no todos alineados)\n";
				while (pivotIdx && streamDocIds[streamIndices[pivotIdx-1]] == pivotDocId) {
					--pivotIdx;
				}
				
				moveStreamsToDocAndSort(pivotIdx, pivotDocId);
			}
			
		}  /* while (true) */
	}
	bool findPivotFeatureIdx(const unsigned int threshold, int &pivotIdx){
		//cout<<"Wand::findPivotFeatureIdx - inicio (threshold "<<threshold<<")\n";
		// The upper bound accumulation is done by the scorer. Different types of scorers might do upper bound accumulation differently.
		scorer->resetForPivotFeatureSelection();
		int idx;
		unsigned int accumUB = 0;
		for (idx=0; accumUB <= threshold && idx < numStreams; ++idx) {
			accumUB = scorer->accumulateUpperBound(streamIndices[idx]);
		}
		if( accumUB >= threshold ) {
			pivotIdx = idx - 1;
			//cout<<"Wand::findPivotFeatureIdx - encontrado (posicion "<<pivotIdx<<", accumUB "<<accumUB<<")\n";
			return true;
		} 
		//cout<<"Wand::findPivotFeatureIdx - No encontrado (accumUB "<<accumUB<<")\n";
		return false; 
	}
	
	void moveStreamsAndSort(const int numStreamsToMove){ 
		//cout<<"Wand::moveStreamsAndSort - inicio (numStreamsToMove "<<numStreamsToMove<<")\n";
		for (int i=0; i<numStreamsToMove; ++i) {
			streamDocIds[streamIndices[i]] = (*streams)[streamIndices[i]]->next();
		}
		sortMerge(numStreamsToMove);
		//cout<<"Wand::moveStreamsAndSort - fin\n";
	}
	
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
/*
class TiempoHistorial
{
  private:


   public:
   int tid;
   int nthreads;
   map<int,list< pair<double, unsigned int> > > tiemposPid;

   TiempoHistorial(int nT)
   {
   nthreads=nT;
     // list< pair<double, unsigned int> > *lista[nT];
   //   tiemposPid =  new map<int,list< pair<double, unsigned int> >[nT] >();


   }

   ~TiempoHistorial()
   {}


};
*/
#endif

