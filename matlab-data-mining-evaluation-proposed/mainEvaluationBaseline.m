addpath('./BP/')
addpath('./data/')
load('../features-baseline/baseline-features.mat')
load('../descriptores42QuerieTrecCW09.mat')
NumberofHiddenNeurons = 1;


[TrainingAccuracy, TestingAccuracy,rOut] = bp(train_data, test_data, NumberofHiddenNeurons, ActivationFunction)