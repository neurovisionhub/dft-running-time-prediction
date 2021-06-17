%% 5.2. Learning Methods 
function [test_trace] = test_files_weka_cross_fold(NHNeurons,strAlg,strData,strS,m,all_in,cv,sw)
%Options
tipoVCOption = cv;%2 % CF10 ->10=10, CF5->20=5, CF3->33=3, CF2->50=2
%imports files WEKA format
strAlgoritmo = strAlg; % 'BMW'; % BMW/WAND
strDataWeb = strData; %'CW'; % CW/Gov2
strScore = strS; %'BM25'; % BM25/TFIDF
%m -> BP,ELM,RForest,RNolineal,RLineal,SVM...
%% neural network m='bp' %%
metodo = m;% 'bp';
%h.layers for atention test%
hLayers = NHNeurons; 
% unique experiment or all methods %
all = all_in; % 0=no (default), 1=yes (all methods)
test_trace = evalpredictor(tipoVCOption,strAlgoritmo,strScore,strDataWeb,hLayers,metodo,all,sw);        
end



