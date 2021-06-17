addpath('./BP/')
addpath('./data/')
%data = importfile('./data/timeNorm/NdataBMWBM25CW.arff')
data = baselineKCW.descriptoresQuerieTrecCW09topk1000;


%% Partition train-test data
% Data for Test (ie. 40% => 60% train + 40% test)
percentTest = 0.4;
Te = [data(:,end),data(:,1:end-1)];
Te = sortrows(Te,1); %Sort for time data 
rng('default') % For reproducibility
cv = cvpartition(size(Te,1),'HoldOut',percentTest);
idx = cv.test;
% Separate to training and test data
train_data = Te(~idx,:);
test_data = Te(idx,:);

%% POs 1: time; Pos > 1: inputs  
%X=Te(:,2:end); %inputs
%T=Te(:,1); %Target

%train_data= data
%test_data = data 

%load('../features-baseline/baseline-features.mat')
%load('../descriptores42QuerieTrecCW09.mat')
NumberofHiddenNeurons = 5;
ActivationFunction = 'sig';
%train_data & test_data - column 1 is target time
[TrainingAccuracy, TestingAccuracy,rOut] = bp(train_data, test_data, NumberofHiddenNeurons, ActivationFunction)