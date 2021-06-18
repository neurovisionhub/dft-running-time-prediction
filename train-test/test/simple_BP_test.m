addpath('./BP/')
addpath('./data/')
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
NumberofHiddenNeurons = 5;
ActivationFunction = 'sig';
%train_data & test_data - column 1 is target time
[TrainingAccuracy, TestingAccuracy,rOut] = bp(train_data, test_data, NumberofHiddenNeurons, ActivationFunction)