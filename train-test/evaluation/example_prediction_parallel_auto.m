function [test_trace] = example_prediction_parallel_auto(p_test,vector_NHNeurons)
test_trace=[];
%% --------- Test 4 ---------%%
%% --------- automatic testing - Multithreading prediction ------- %%
%% LS = 0: LBM-WAND, LS = 1: SBM-WAND %%
LS = [0,1];%% Multithreading options %%
%% 1:1 thread, 2:2 threads,3:4 threads, 4:8 threads %%
optionsTreads = [1,2,3,4]; 
%% Experiment for k={10,100,1000,10000}%%
k=[10,100,1000,10000];
%% tech -> 0:baseline; 1: dft-prediction %%
tech = [0,1]; 
%% bp: NumberofHiddenNeurons = n %%
%NHNeurons = [1,5,10,25,50];
NHNeurons = vector_NHNeurons;
percentTest = p_test;
sizeN = size(NHNeurons,2);
sizeTech = size(tech,2);
sizeK = size(k,2);
sizeO = size(optionsTreads,2);
sizeLS = size(LS,2);

baselineTrace = [];
dftTrace = [];
%times for 1,2,4 o 8 threads of Strategy parallel multithreading        
for o=1:sizeO
%Strategy parallel multithreading    
for l=1:sizeLS 
%Values k={10,100,1000,10000}    
for m=1:sizeK
    % Prediction tech
    for j=1:sizeTech    
        %HLayers
        forNeuron = [];
        for i=1:sizeN
        NumberofHiddenNeurons=NHNeurons(1,i);
        [~, TestAcc,rOut] = multithreading_k_BP_test_CW(tech(1,j),k(1,m),LS(1,l),optionsTreads(1,o),percentTest,NumberofHiddenNeurons);
        resultsMultithreading = [rOut,TestAcc];
        [test_trace] = [test_trace;resultsMultithreading];
        forNeuron = [forNeuron,resultsMultithreading]; 
        end      
            if tech(1,j)==0
               baselineTrace = [baselineTrace;percentTest,tech(1,j),optionsTreads(1,o),LS(1,l),k(1,m),forNeuron]; 
            end
            if tech(1,j)==1
               dftTrace = [dftTrace;percentTest,tech(1,j),optionsTreads(1,o),LS(1,l),k(1,m),forNeuron]; 
            end
            % re-save for backup - service break
            %save results-exp-2.mat baselineTrace dftTrace
            save ./evaluation/results/results-exp-n.mat baselineTrace dftTrace
    end


%%  ------------------------------------------ %%
end
end
end
%save results-exp-2.mat baselineTrace dftTrace
save ./evaluation/results/results-exp-n.mat baselineTrace dftTrace
test_trace = './evaluation/results/results-exp-n.mat';
end
