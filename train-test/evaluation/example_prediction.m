function [test_trace,str,str_a] = example_prediction(data_web_in,p_test,NHNeurons)
test_trace=[];
%%  ----------- Test ----------------------- %%
%% baseline / DFT runtime bp prediction K=1000 %%
%% 0: WAND/ClueWeb/BM25, 1: WAND/Gov2/BM25
%% 2: WAND/ClueWeb/TfIDF: 3: WAND/Gov2/TfIDF %%
%% 4: BM-WAND/ClueWeb/BM25, 5: BM-WAND/Gov2/BM25
%% 6: BM-WAND/ClueWeb/TfIDF: 7: BM-WAND/Gov2/TfIDF %%
data_web = data_web_in;
str = '';
switch data_web
      case 0
        str ='W.CW.BM25'; 
      case 1
        str ='W.Gov2.BM25'; 
      case 2
        str ='W.CW.TfIDF'; 
      case 3
        str ='W.Gov2.TfIDF'; 
      case 4
        str ='BMW.CW.BM25'; 
      case 5
        str ='BMW.Gov2.BM25'; 
      case 6
        str ='BMW.CW.TfIDF'; 
      case 7
        str ='BMW.Gov2.TfIDF';   
      
      otherwise
        disp('no menu')
end

%% Partition train-test data
% Data for Test (ie. 40% => 60% train + 40% test)
%percentTest = 0.4;
NumberofHiddenNeurons = NHNeurons;
percentTest = p_test;

%% Baseline
[~,TestingAccuracy,rOut] = simple_BP_test_baseline(data_web,percentTest,NumberofHiddenNeurons);
[test_trace] = [test_trace;rOut,TestingAccuracy];
%% DFT-prediction
[~,TestingAccuracy,rOut] = simple_BP_test_dft_prediction(data_web,percentTest,NumberofHiddenNeurons);
[test_trace] = [test_trace;rOut,TestingAccuracy];
str_a = ["PCr","RMSE"];
str = [string(str),string(str)];
%%  --------------------------------------------- %%
end