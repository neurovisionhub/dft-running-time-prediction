# Train-Test
This directory contains routines for tests of ranking algorithm run-time predictors. 

## *main.m*

Function main.m run experiments (ie. config_id='cross-fold') and return results of accuracy (matrix 'data_test')

```sh 
function [data_test,~] = main(config_id)  
```

You can add new experiments or modify existing ones to add more functionalities or traces. For example, evaluate accuracy of all regression model, neural networs with five neuron in hidden layer, compute traditional cross-fold-5 and swap-cross-fold-5 using data-web ClueWeb and runtimes of BM-WAND/BM25. 


```sh 
    case 'cross-fold'
        disp('manual cross fold')
        % folds:  % CF10 ->10=10, CF5->20=5, CF3->33=3, CF2->50=2
        folds = 5;
        % sw=0 -> cross-fold, sw=1 -> swap-cross-fold, 
        sw = 1;
        NHNeurons=5; % neurons hidden layer
        %% unique method
        %[data_test] = test_files_weka_cross_fold(NHNeurons,'BMW','CW','BM25','rlineal',0,folds);
        
        %% All methods
        %% example build Table 2 [paper]
        sw = 0;
        data_test = test_files_weka_cross_fold(NHNeurons,'BMW','CW','BM25',' ',1,folds,sw);
        sw = 1;
        data_test = [data_test,test_files_weka_cross_fold(NHNeurons,'BMW','CW','BM25',' ',1,folds,sw)];
```

### Directories

 **Directory data**: Data of manual cross-fold, DFT-Based features of queries for multiples data-web (ClueWeb/Gov2) and score BM25/TFIDF. More information [dir-data]

 **Directory evaluation**: Routines of control and scheduler of experiments. More information [dir-evaluation] 

 **Directory test**: Scheduler for call Neural Networks routines & data mining testing. More information in [dir-test]

**Directory libs**: Machine Learning & routines for evaluation prediction. More information in [dir-libs]

**Directory images**: images of accuracy (see example [dir-images])


[dir-test]: https://github.com/neurovisionhub/dft-running-time-prediction/tree/master/train-test/test

[dir-libs]: https://github.com/neurovisionhub/dft-running-time-prediction/tree/master/train-test/libs


[dir-data]: https://github.com/neurovisionhub/dft-running-time-prediction/tree/master/train-test/data

[dir-evaluation]: https://github.com/neurovisionhub/dft-running-time-prediction/tree/master/train-test/evaluation

[dir-images]: https://github.com/neurovisionhub/dft-running-time-prediction/blob/master/train-test/images/DFT-Based-lbm-wand.png
