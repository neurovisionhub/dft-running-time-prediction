function [test_trace] = example_prediction_parallel_block(ls,t,K,P,p_test,NHNeurons)
test_trace=[];
%if test_id == 5
%% --------- Test 3 Por bloques ---------%%
%% --------- Multithreading prediction ------- %%
%% LS = 0: LBM-WAND, LS = 1: SBM-WAND %%
LS = ls;%% Multithreading options %%
%% 1:1 thread, 2:2 threads,3:4 threads, 4:8 threads %%
Threads = t; 
%% Experiment for k={10,100,1000,10000}%%
k=K;
%% P -> 0:baseline; 1: DFT-based %%
% P = 0; 
%% bp: NumberofHiddenNeurons = n %%
% NHNeurons = [1,5,10,25,50];
sizeN = size(NHNeurons,2);                     
pTest = p_test;


for i=1:sizeN
NHN = NHNeurons(1,i);
[~, TestAcc,rOut] = multithreading_k_BP_test_CW(P,k,LS,Threads,pTest,NHN);
resultsMultithreading = [rOut,TestAcc];
test_trace = [test_trace;resultsMultithreading];
end
%%  ------------------------------------------ %%
end

