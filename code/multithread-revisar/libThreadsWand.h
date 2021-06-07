#ifndef LIBTHREADS_H_
#define LIBTHREADS_H_
//pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;;
//pthread_barrier_t barrierT;
//map<int,pthread_barrier_t> barrierT;
//pthread_barrierattr_t attr;
//pthread_cond_t llegaron = PTHREAD_COND_INITIALIZER;
//void *HeapLocales(void *arg);


class datosQuerie
{
   public:
   int tid;
   int nthreads;
   int idQ;
   //vector < DatosQuery* > consultas;
   DatosQuery* consulta;
  // pthread_barrier_t *_barrier;
  // pthread_mutex_t *_lock;
   int k;
   double umbral;
   double umbralK;
   datosQuerie()
   {

   }
   datosQuerie(int p,int pr, unsigned int id,int k)
   {
     tid  = p;
     nthreads = pr;
     idQ = id;  
   }
   
   
    ~datosQuerie()
   {
   //  delete consulta;
   //    delete[] _barrier;
   }
 
 
};





class ThreadPosix{
public:
	int id_query;
	int numThreads;
	//int K;
	
	datosQuerie *dataQ;//[THREADS];
	//pthread_mutex_t lock;// = PTHREAD_MUTEX_INITIALIZER;;
    //pthread_barrier_t barrier;
   // pthread_cond_t llegaron;// = PTHREAD_COND_INITIALIZER;
    
    pthread_barrierattr_t attrBarrier;
    pthread_attr_t attrThread;
    pthread_t *mythread;//[numThreads];
    int metodoHeap;
	
	
	ThreadPosix(int _id_query,int _numThreads, int _k, DatosQuery* _q,double _umbral,pthread_mutex_t _lock,pthread_barrier_t _barrier,pthread_cond_t _llegaron){
		id_query=_id_query;
		numThreads=_numThreads;
		mythread = new pthread_t[numThreads];
		pthread_mutex_init(&_lock, NULL);
	    pthread_cond_init(&_llegaron, NULL);
        pthread_attr_init(&attrThread);
        pthread_attr_setdetachstate(&attrThread, PTHREAD_CREATE_JOINABLE);
       
        //if(numThreads>1)
        pthread_barrier_init(&_barrier, &attrBarrier, _numThreads);
        
        dataQ = new datosQuerie[_numThreads];
                
        for(int t=0;t<_numThreads;t++)
        {
			dataQ[t].consulta=_q;
			dataQ[t].umbral=_umbral;
			dataQ[t].idQ = _id_query;
			dataQ[t].k = _k;
			dataQ[t].tid=t;
			dataQ[t].nthreads=numThreads;	
		//	dataQ[t]._barrier=&_barrier;
		//	dataQ[t]._lock=&_lock;
		}
        //K = _k;
	}	

	void initWandThreads()
	{
	int rc;
	//Numero de threads a utilizar 
	for(int t=0; t<numThreads; t++)
	{
		//if(metodoHeap==0)
			rc =  pthread_create(&mythread[t], &attrThread,HeapLocales,(void *)&dataQ[t]); //se puede aplicar ub locales REVISAR ESTA LLAMADA PARA NO TENER QUE PASAR LA LISTA COMPLETA, OPCION CREAR OBJETO SOLO DE PARAMETROS

	//	rc =  pthread_create(&mythread[t], &attrThread,HeapLocales,(void *)&dataQ[t]); //se puede aplicar ub locales REVISAR ESTA LLAMADA PARA NO TENER QUE PASAR LA LISTA COMPLETA, OPCION CREAR OBJETO SOLO DE PARAMETROS
	//	if(metodoHeap==1)
	//	rc =  pthread_create(&mythread[t], &attrThread,HeapCentralizado,(void *)&dataQ[t]); //se puede aplicar ub locales REVISAR ESTA LLAMADA PARA NO TENER QUE PASAR LA LISTA COMPLETA, OPCION CREAR OBJETO SOLO DE PARAMETROS
		if (rc)
		{
		  printf("ERROR; return code from pthread_create() is %d\n", rc);
		  abort();//exit(-1);
		}

	}
		
	}
	
	void barrera(pthread_barrier_t _barrier)
	{
		pthread_barrier_wait(&_barrier);
		
	}
	void barreraDestroy(pthread_barrier_t _barrier)
	{
		pthread_barrier_destroy(&_barrier);
	}
	
	
	int exitThread()
	{
		pthread_exit(NULL);	
	}



	
	void ThreadFinished(pthread_mutex_t _lock,pthread_barrier_t _barrier,pthread_cond_t _llegaron){
	void *status;
	int rc;
 
  pthread_attr_destroy(&attrThread);
  //if(numThreads>1)
  
  for(int t = 0; t < numThreads; t++ )
  {
	 //dataQ[t].~datosQuerie();
	  cout<<"nt "<< numThreads<<"Join: "<<t<<" Q: "<<id_query<<endl;
     //pthread_join( mythread[t], &status );
     rc = pthread_join(mythread[t], &status);
      if (rc){
         cout << "Error:unable to join-----------------------------------------------------------------," << rc << endl;
         exit(-1);
         }
  cout<<"Join sal: "<<t<<" Q: "<<id_query<<endl;
  }
  
 pthread_mutex_destroy(&_lock);
 pthread_cond_destroy(&_llegaron);
 //
 //if(numThreads>1)
 barreraDestroy(_barrier);
	
	
}
	
		
	
	
~ThreadPosix(){}

	
	
};



#endif
