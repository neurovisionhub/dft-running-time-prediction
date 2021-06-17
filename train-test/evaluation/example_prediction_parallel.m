function [test_trace] = example_prediction_parallel(ls,t,k,p,p_test,NHNeurons)
test_trace=[];
%% --------- Test 3 ---------%%
%% --------- Multithreading prediction ------- %%
%% LS = 0: LBM-WAND, LS = 1: SBM-WAND %%
LS = ls;%% Multithreading options %%
%% 1:1 thread, 2:2 threads,3:4 threads, 4:8 threads %%
optionsTreads = t; 
%% Experiment for k={10,100,1000,10000}%%
K=k;
%% P -> 0:baseline; 1: dft-prediction %%
P = p; 
%% bp: NumberofHiddenNeurons = n %%
NumberofHiddenNeurons = NHNeurons;
percentTest = p_test;

[~, TestAcc,rOut] = multithreading_k_BP_test_CW(P,K,LS,optionsTreads,percentTest,NumberofHiddenNeurons);
[test_trace] = [test_trace;rOut,TestAcc];
%%  ------------------------------------------ %%
end
