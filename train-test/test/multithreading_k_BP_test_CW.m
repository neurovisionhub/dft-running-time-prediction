function [TrainAcc, TestAcc,rOut] = multithreading_k_BP_test_CW(tech,k,LS,optionsTreads,percentTest,NumberofHiddenNeurons)
global baselineKCW
global timesLS
%% Example load data-Web ClueWeb baseline descriptors 
%% K=10,100,1000,10000 - build from code XXX (buscar fuente) version 2017.01 
%% Solo ClueWeb BM25
if k==10    
    data = baselineKCW.descriptoresQuerieTrecCW09topk10(:,2:end);
    if LS == 0
        times = timesLS.timeLocalMultithreadQuerie10(:,2:5); % [1,2,4,8] Threads
    end
    if LS == 1
        times = timesLS.timeSharedMultithreadQuerie10(:,2:5); % [1,2,4,8] Threads
    end
end

if k==100
data = baselineKCW.descriptoresQuerieTrecCW09topk100(:,2:end);
    if LS == 0
        times = timesLS.timeLocalMultithreadQuerie100(:,2:5); % [1,2,4,8] Threads
    end
    if LS == 1
        times = timesLS.timeSharedMultithreadQuerie100(:,2:5); % [1,2,4,8] Threads
    end

end

if k==1000
data = baselineKCW.descriptoresQuerieTrecCW09topk1000(:,2:end);
    if LS == 0
        times = timesLS.timeLocalMultithreadQuerie1000(:,2:5); % [1,2,4,8] Threads
    end
    if LS == 1
        times = timesLS.timeSharedMultithreadQuerie1000(:,2:5); % [1,2,4,8] Threads
    end

end
if k==10000
data = baselineKCW.descriptoresQuerieTrecCW09topk10000(:,2:end);
    if LS == 0
        times = timesLS.timeLocalMultithreadQuerie10000(:,2:5); % [1,2,4,8] Threads
    end
    if LS == 1
        times = timesLS.timeSharedMultithreadQuerie10000(:,2:5); % [1,2,4,8] Threads
    end

end

if tech == 1
    data = importfile('./data/timeNorm/NdataBMWBM25CW.arff');
    data = data(:,1:end-1); % adaptative format
end

%% Partition train-test data
% Data for Test (ie. 40% => 60% train + 40% test)
%percentTest = 0.4;
%%  [1] Threads on this test
maxs = max(times(:,optionsTreads));
Te = [times(:,optionsTreads)./maxs,data(:,1:end)]; 
Te = sortrows(Te,1); %Sort for time data 
rng('default') % For reproducibility
cv = cvpartition(size(Te,1),'HoldOut',percentTest);
idx = cv.test;
% Separate to training and test data
train_data = Te(~idx,:);
test_data = Te(idx,:);

ActivationFunction = 'sig';
%train_data & test_data - column 1 is target time
[TrainAcc, TestAcc, rOut] = bp(train_data, test_data, NumberofHiddenNeurons, ActivationFunction);

