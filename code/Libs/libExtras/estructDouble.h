#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

typedef struct 
{
    int nthreads;
    int called;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

} ov_barrier_t;


extern ov_barrier_t **ov_barrera;

int ov_barrier_init(ov_barrier_t ***barrier, int nthreads);
int ov_barrier_destroy(int tid, ov_barrier_t *barrier);
int ov_barrier_wait(int my_tid, int tid, ov_barrier_t **ov_barrier);


int ov_barrier_init(ov_barrier_t ***barrier, int nthreads)
{
    *barrier = new ov_barrier_t*[nthreads];
    
    for(int i=0;i<nthreads;i++)
    {
      (*barrier)[i] = new ov_barrier_t();
      (*barrier)[i]->nthreads = nthreads;
      (*barrier)[i]->called   = 0;
      pthread_mutex_init(&(*barrier)[i]->mutex,NULL);
      pthread_cond_init(&(*barrier)[i]->cond,NULL);
    }
    return 0;
}


int ov_barrier_wait(int my_tid, int tid, ov_barrier_t **ov_barrier)
{
    ov_barrier_t *barrier = ov_barrier[tid];
    
    pthread_mutex_lock(&barrier->mutex);
    
    barrier->called++;
    
    if ( my_tid==tid )
    {
       if (barrier->called < barrier->nthreads)     
         pthread_cond_wait(&barrier->cond,&barrier->mutex);
       else
         barrier->called = 0;
    }
    else
    if (barrier->called == barrier->nthreads) 
    {
       barrier->called = 0;
       pthread_cond_signal(&barrier->cond);
    } 

    pthread_mutex_unlock(&barrier->mutex);
    
    return 0;
}


int ov_barrier_destroy(ov_barrier_t **barrier, int nthreads)
{
    for(int i=0;i<nthreads;i++)
    {
      pthread_mutex_destroy(&barrier[i]->mutex);
      pthread_cond_destroy(&barrier[i]->cond);
    }
    return 0;
}

class DatosQuery{
public:
	int id_query;
	int n_terminos;
	unsigned long long *terminos;
	float *valores;
	DatosQuery(){
		id_query=0;
		n_terminos=0;
		terminos=NULL;
		valores=NULL;
	}
	DatosQuery(int _id_query, int _n_terminos, unsigned long long *_terminos, float *_valores){
		
		map<unsigned long long,float> idTerms;
		
		for(int i=0; i<_n_terminos; i++){
			
			if(idTerms.count(_terminos[i])<1)
			idTerms[_terminos[i]] = 1.0;
			else
			idTerms[_terminos[i]]+= 1.0;

		}
		
		
		
		
		id_query=_id_query;
		n_terminos=idTerms.size();
		//cout<<"---------------:"<<n_terminos<<endl;
		terminos=new unsigned long long[n_terminos];
		valores=new float[n_terminos];
		int i = 0;
		for (std::map<unsigned long long,float>::iterator it=idTerms.begin(); it!=idTerms.end(); ++it)
   {     terminos[i]=it->first;
	     valores[i]=it->second;
	i++;   
	
   }
		/*
		n_terminos=_n_terminos;
		terminos=new unsigned long long[n_terminos];
		valores=new float[n_terminos];
		for(int i=0; i<n_terminos; i++){
			
			
			terminos[i]=_terminos[i];
			valores[i]=_valores[i];
		}*/
	}
		
    void mostrar()
    {
		cout<<"Q["<<id_query<<"]";
		for(int i=0; i<n_terminos; i++){
			cout<<" "<<terminos[i] <<" "<<valores[i];
		}
		cout<<endl;
	} 		
	
	~DatosQuery(){
		if(terminos!=NULL){
			delete [] terminos;
			terminos=NULL;
		}
		if(valores!=NULL){
			delete [] valores;
			valores=NULL;
		}
	}
};
/*
class Nodo{
public:
	unsigned int id_doc;
	unsigned int valor;
	Nodo(){
		id_doc=0;
		valor=0;
	}
	Nodo(unsigned int _id_doc, unsigned int _valor){
		id_doc=id_doc;
		valor=_valor;
	}
	//dejare el ordenamiento natural por doc_id (aqui no hay frecuencias)
	//tambien para el comparador de nodos
	bool operator> (class Nodo x) {return id_doc>x.id_doc;}
};
*/
/*
struct comp: binary_function<Nodo*,Nodo*,bool>{
bool operator() (Nodo* x,Nodo* y) 
{return x->id_doc > y->id_doc;}
};
*/

#endif


