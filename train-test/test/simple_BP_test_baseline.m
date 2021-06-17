function [TrainingAccuracy, TestingAccuracy,rOut] = simple_BP_test_baseline(strategy,percentTest,NumberofHiddenNeurons)
global multibaseline;
% NumberofHiddenNeurons = 5;
% %% Table 3 %%
% %% 0: WAND/ClueWeb/BM25, 1: WAND/Gov2/BM25
% %% 2: WAND/ClueWeb/TfIDF: 3: WAND/Gov2/TfIDF %%
% %% 4: BM-WAND/ClueWeb/BM25, 5: BM-WAND/Gov2/BM25
% %% 6: BM-WAND/ClueWeb/TfIDF: 7: BM-WAND/Gov2/TfIDF %%
% strategy = 3;
% %% Partition train-test data
% % Data for Test (ie. 40% => 60% train + 40% test)
% % percentTest = 0.4;
%% load data-Web baseline descriptors %%
     
    if strategy == 0 % WAND/ClueWeb/BM25
        data = multibaseline.data42DescriptoresPDP2015QUERIESClueWebTrec09BM25(:,2:end);  
        times = importfile('./data/paramsNorm/dataWANDBM25CW.arff'); % [1] Threads
        times = times(:,end)./(max(times(:,end))); % adaptative format
    end
    
    if strategy == 1% WAND/Gov2/BM25
        data = multibaseline.data42DescriptoresPDP2015QUERIESGov2BM25(:,2:end);
        times = importfile('./data/paramsNorm/dataWANDBM25Gov2.arff');
        times = times(:,end)./(max(times(:,end))); % adaptative format
    end
    
    if strategy == 2% WAND/ClueWeb/TfIDF
       data = multibaseline.data42DescriptoresPDP2015QUERIESClueWebTrec09TFIDF(:,2:end);
       times = importfile('./data/paramsNorm/dataWANDTFIDFCW.arff');
       times = times(:,end)./(max(times(:,end))); % adaptative format
    end
    
    if strategy == 3% WAND/Gov2/TfIDF
       data = multibaseline.data42DescriptoresPDP2015QUERIESGov2TFIDF(:,2:end); 
       times = importfile('./data/paramsNorm/dataWANDTFIDFGov2.arff');
       times = times(:,end)./(max(times(:,end))); % adaptative format
    end
    
    if strategy == 4% BM-WAND/ClueWeb/BM25
        data = multibaseline.data42DescriptoresPDP2015QUERIESClueWebTrec09BM25(:,2:end);  
        times = importfile('./data/paramsNorm/dataBMWBM25CW.arff'); % [1] Threads
        times = times(:,end)./(max(times(:,end))); % adaptative format
    end
    
    if strategy == 5%  BM-WAND/Gov2/BM25
        data = multibaseline.data42DescriptoresPDP2015QUERIESGov2BM25(:,2:end);
        times = importfile('./data/paramsNorm/dataBMWBM25Gov2.arff');
        times = times(:,end)./(max(times(:,end))); % adaptative format 
      
    end
    
    if strategy == 6% BM-WAND/ClueWeb/TfIDF
        data = multibaseline.data42DescriptoresPDP2015QUERIESClueWebTrec09TFIDF(:,2:end);
        times = importfile('./data/paramsNorm/dataBMWTFIDFCW.arff');
       times = times(:,end)./(max(times(:,end))); % adaptative format
    end
    
    if strategy == 7% BM-WAND/Gov2/TfIDF
       data = multibaseline.data42DescriptoresPDP2015QUERIESGov2TFIDF(:,2:end); 
       times = importfile('./data/paramsNorm/dataBMWTFIDFGov2.arff');
       times = times(:,end)./(max(times(:,end))); % adaptative format
    end  


%%  [1] Threads on this test
%maxs = max(times(:,optionsTreads));
Te = [times(:,1),data(:,1:end)];
Te = sortrows(Te,1); %Sort for time data 
rng('default') % For reproducibility
cv = cvpartition(size(Te,1),'HoldOut',percentTest);
idx = cv.test;
% Separate to training and test data
train_data = Te(~idx,:);
test_data = Te(idx,:);
ActivationFunction = 'sig';
%train_data & test_data - column 1 is target time
[TrainingAccuracy, TestingAccuracy,rOut] = bp(train_data, test_data, NumberofHiddenNeurons, ActivationFunction);

