function [TrainingAccuracy, TestingAccuracy,rOut] = simple_BP_test_dft_prediction(strategy,percentTest,NumberofHiddenNeurons)

    if strategy == 0 % WAND/ClueWeb/BM25
      %  data = multibaseline.data42DescriptoresPDP2015QUERIESClueWebTrec09BM25(:,2:end);  
        data = importfile('./data/paramsNorm/dataWANDBM25CW.arff'); % [1] Threads
      %  times = times(:,end)./(max(times(:,end))); % adaptative format
    end
    if strategy == 1% WAND/Gov2/BM25
    %    data = multibaseline.data42DescriptoresPDP2015QUERIESGov2BM25(:,2:end);
        data = importfile('./data/paramsNorm/dataWANDBM25Gov2.arff');
      %  times = times(:,end)./(max(times(:,end))); % adaptative format
    end
    if strategy == 2% WAND/ClueWeb/TfIDF
      % data = multibaseline.data42DescriptoresPDP2015QUERIESClueWebTrec09TFIDF(:,2:end);
       data = importfile('./data/paramsNorm/dataWANDTFIDFCW.arff');
    %   times = times(:,end)./(max(times(:,end))); % adaptative format
    end
    if strategy == 3% WAND/Gov2/TfIDF
      % data = multibaseline.data42DescriptoresPDP2015QUERIESGov2TFIDF(:,2:end); 
       data = importfile('./data/paramsNorm/dataWANDTFIDFGov2.arff');
      % times = times(:,end)./(max(times(:,end))); % adaptative format
    end  
    
     if strategy == 4% BM-WAND/ClueWeb/BM25
       % data = multibaseline.data42DescriptoresPDP2015QUERIESClueWebTrec09BM25(:,2:end);  
        data = importfile('./data/paramsNorm/dataBMWBM25CW.arff'); % [1] Threads
       % times = times(:,end)./(max(times(:,end))); % adaptative format
    end
    if strategy == 5%  BM-WAND/Gov2/BM25
      %  data = multibaseline.data42DescriptoresPDP2015QUERIESGov2BM25(:,2:end);
        data = importfile('./data/paramsNorm/dataBMWBM25Gov2.arff');
      %  times = times(:,end)./(max(times(:,end))); % adaptative format 
      
    end
    if strategy == 6% BM-WAND/ClueWeb/TfIDF
       % data = multibaseline.data42DescriptoresPDP2015QUERIESClueWebTrec09TFIDF(:,2:end);
        data = importfile('./data/paramsNorm/dataBMWTFIDFCW.arff');
      % times = times(:,end)./(max(times(:,end))); % adaptative format
    end
    if strategy == 7% BM-WAND/Gov2/TfIDF
     %  data = multibaseline.data42DescriptoresPDP2015QUERIESGov2TFIDF(:,2:end); 
       data = importfile('./data/paramsNorm/dataBMWTFIDFGov2.arff');
     %  times = times(:,end)./(max(times(:,end))); % adaptative format
    end  


%% Partition train-test data
% Data for Test (ie. 40% => 60% train + 40% test)
%percentTest = 0.4;
Te = [data(:,end)./(max(data(:,end))),data(:,1:end-1)];
Te = sortrows(Te,1); %Sort for time data 
rng('default') % For reproducibility
cv = cvpartition(size(Te,1),'HoldOut',percentTest);
idx = cv.test;
% Separate to training and test data
train_data = Te(~idx,:);
test_data = Te(idx,:);
%NumberofHiddenNeurons = 5;
ActivationFunction = 'sig';
%train_data & test_data - column 1 is target time
[TrainingAccuracy, TestingAccuracy,rOut] = bp(train_data, test_data, NumberofHiddenNeurons, ActivationFunction);
end
%data = baselineKCW.descriptoresQuerieTrecCW09topk1000;