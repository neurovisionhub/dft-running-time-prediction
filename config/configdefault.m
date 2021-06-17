addpath(genpath('./dft-running-time-predicction'))
%addpath('./extras')
addpath('./train-test')
% Directory of data tests
dir_dataOut = './results'
global baselineKCW
global timesLS
global multibaseline

%% default %%
%NumberofHiddenNeurons = 5;
%percentTest = 0.4;
%test_trace = [];

test_trace=[];
%%  MB-WAND local/shared multithreading times %%
timesLS = load('./data-web/data-times/times-local-shared-bmw.mat');
%% Baseline: 14 descriptors -> 42 querie features [{ClueWeb&Gov2}{TfIDF&BM25}] : Ref: PDP 2015%%
multibaseline = load('./data-web/features-baseline/baseline-features.mat');
%% Baseline: 14 descriptors -> 42 querie features [{ClueWeb}{BM25} k=10,100,1000,10000]%% This Work 
baselineKCW = load('./data-web/features-baseline/baseline-features-k.mat');
%% This work: 5 descriptors -> 6 querie features %%
%timesLS = load('../../data-times/    times-multithreading-bmw.mat');
%% Queries ClueWeb & Gov2 from set Trec09%%
termsQueriesLogs = load('./data-web/queries-gov2-clueWeb/queriesCW_Gov2_Trec09.mat');