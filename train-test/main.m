function [data_test] = main(config_id)
data_test=[];
%% general options
%% default
%% ls = {0:local,1:shared}
ls = 0;
%% t = {0,1,2,3} = {1 thread, 2 threads, 4 threads, 8 threads}
t = 1;
%% k = {10,100,1000,10000}  
k=1000;
%% P ={0:baseline, 1:DFT-Based}
P=1;
%% p_test ={ %test }
p_test=0.4;
%%  sequence of neurons hidden layer for multiple test
vector_NHNeurons = [1,5,10,25,50];
%% number of hidden neurons
NHNeurons = 5;


switch config_id
    case 'simple'
        disp('test runtime prediction')
        %%  ----------- Test 2 ----------------------- %%
        %% baseline / DFT runtime bp prediction / default K=1000 %%
        %% 0: WAND/ClueWeb/BM25, 1: WAND/Gov2/BM25
        %% 2: WAND/ClueWeb/TfIDF: 3: WAND/Gov2/TfIDF %%
        %% 4: BM-WAND/ClueWeb/BM25, 5: BM-WAND/Gov2/BM25
        %% 6: BM-WAND/ClueWeb/TfIDF: 7: BM-WAND/Gov2/TfIDF %%
        %% examples 
        data_web = 0;
        data_test=example_prediction(data_web,p_test,NHNeurons);
        data_web = 1;
        data_test=[data_test,example_prediction(data_web,p_test,NHNeurons)];
        data_web = 4;
        data_test=[data_test,example_prediction(data_web,p_test,NHNeurons)];
        data_web = 5;
        data_test=[data_test,example_prediction(data_web,p_test,NHNeurons)];
        
    case 'parallel'
        disp('test runtime parallel prediction')
        %% --------- Multithreading prediction ------- %%
        %% ls = 0: LBM-WAND, LS = 1: SBM-WAND %%
        ls = 1;%% Multithreading options %%
        %% 1:1 thread, 2:2 threads,3:4 threads, 4:8 threads %%
        t=4; 
        %% Experiment for k={10,100,1000,10000}%%
        %k=1000;
        %% P -> 0:baseline; 1: dft-prediction %%
        %% examples
        k=10;P = 0;       
        [data_test] = example_prediction_parallel(ls,t,k,P,p_test,NHNeurons)
        P = 1;       
        [data_test] = [data_test,example_prediction_parallel(ls,t,k,P,p_test,NHNeurons)];
        
        k=100;P = 0;       
        data_tmp = example_prediction_parallel(ls,t,k,P,p_test,NHNeurons);
        P = 1;       
        data_tmp = [data_tmp,example_prediction_parallel(ls,t,k,P,p_test,NHNeurons)];
        [data_test] = [data_test;data_tmp];
        
        k=1000;P = 0;       
        data_tmp = example_prediction_parallel(ls,t,k,P,p_test,NHNeurons);
        P = 1;       
        data_tmp = [data_tmp,example_prediction_parallel(ls,t,k,P,p_test,NHNeurons)];
        [data_test] = [data_test;data_tmp];
        
        k=10000;P = 0;       
        data_tmp = example_prediction_parallel(ls,t,k,P,p_test,NHNeurons);
        P = 1;       
        data_tmp = [data_tmp,example_prediction_parallel(ls,t,k,P,p_test,NHNeurons)];
        [data_test] = [data_test;data_tmp];
        
    case 'all'
        disp('all test runtime parallel prediction')
        data_test = example_prediction_parallel_auto(p_test,vector_NHNeurons);
    
    case 'block'
        disp('block test runtime parallel prediction')
        %% examples
        %% Baseline
        P=0;
        data_test = example_prediction_parallel_block(ls,t,k,P,p_test,vector_NHNeurons);
        %% DFT-Based
        P=1;
        data_test = [data_test,example_prediction_parallel_block(ls,t,k,P,p_test,vector_NHNeurons)];
    
    case 'cross-fold'
        disp('manual cross fold')
        % 'BMW'; % BMW/WAND
        % 'CW'; % CW/Gov2
        % 'BM25'; % BM25/TFIDF
        %metodo -> BP,ELM,RForest,RNolineal,RLineal,SVM... ej: metodo = 'elm';
        % folds:  % CF10 ->10=10, CF5->20=5, CF3->33=3, CF2->50=2
        folds = 5;
        % sw=0 -> cross-fold, sw=1 -> swap-cross-fold, 
        sw = 1;
        %% unique method
        %[data_test] = test_files_weka_cross_fold(NHNeurons,'BMW','CW','BM25','rlineal',0,folds);
        
        %% All methods
        
        %% example build Table 2 []
        sw = 0;
        data_test = test_files_weka_cross_fold(NHNeurons,'BMW','CW','BM25',' ',1,folds,sw);
        sw = 1;
        data_test = [data_test,test_files_weka_cross_fold(NHNeurons,'BMW','CW','BM25',' ',1,folds,sw)];
        
    case 'visual-map'
        disp('Visual Map')
        study
        
    otherwise
        disp('no menu')
end

%data_test = [];
end