clear
addpath(genpath('./'))
configdefault;
cd train-test\


% config examples
% Simple test Baseline vs DFT-Based 
% Selecction data-web
[data_simple,labelsD,labelsA]= main('simple');
data = [labelsD;labelsA;data_simple];
data = [["","", "Baseline","DFT-Based"]',data];
T = table(data)

% runtime-parallel prediction
%[data_parallel,~]= main('parallel');

%% all test runtime-parallel prediction 
%% 10 to 15 min (i9-10C/128 Gb)
%[data_parallel_all,~]= main('all');

%% Cross-fold build Table 2.
%% 1min (2 folds) to 5min (10 folds) (i9-10C/128 Gb)
%[data_CV,~]= main('cross-fold');

%% Generate images of accuracy distribution 
% main('visual-map');

%% Multi-test Baseline vs DFT-Based / experiments block (hidden neurons)
%% BM-WAND ClueWeb 
%[data_multi]= main('block');

cd ..\