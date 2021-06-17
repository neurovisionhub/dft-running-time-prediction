%%

%%
function [mEV] = evaluacionPredictor(tipoVCOption,strAlgoritmo,strScore,strDataWeb,hLayersIn,metodoIn,sw)
mRMSETrain = [];
mRMSEVal = [];
mPearsonVal = [];
mEV = [];
%examples
%tipoVCOption = 10; % CF10 ->10=10, CF5->20=5, CF3->33=3, CF2->50=2 
%strAlgoritmo = 'BMW'; % BMW/WAND
%strScore = 'BM25'; % BM25/TFIDF
%strDataWeb = 'CW'; % CW/Gov2

    switch tipoVCOption
        case 10
        fileVC = 10;
        case 5
        fileVC = 20;
        case 3
        fileVC = 33;
        case 2
        fileVC = 50; 
    end

    formatEpec = './data/CV/%d/%s%s/%s/';
    strPath = sprintf(formatEpec,fileVC,strAlgoritmo,strScore,strDataWeb);

    for i=1:tipoVCOption

        %Data train   
        if strcmp(strScore,'TFIDF')
        formatEpec = 'patron6%sI7TFIDF%sNorm_cv%d_%d_%d_Train.arff';   
        else
        formatEpec = 'patron6%sI7%sNorm_cv%d_%d_%d_Train.arff';    
        end

    strFileTrain = sprintf(formatEpec,strDataWeb,strAlgoritmo,fileVC,tipoVCOption,i);
    strPath2 = '%s%s';
    strPathFileTrain = sprintf(strPath2,strPath,strFileTrain);
    patron6InTrain = importfile(strPathFileTrain);
    xInTrain = patron6InTrain(:,1:6);
    xTargetTrain = patron6InTrain(:,7);
    TrainingData_File = [xTargetTrain,xInTrain];

        %Data Validate
        if strcmp(strScore,'TFIDF')
        formatEpec = 'patron6%sI7TFIDF%sNorm_cv%d_%d_%d_Validate.arff';   
        else
        formatEpec = 'patron6%sI7%sNorm_cv%d_%d_%d_Validate.arff';    
        end

    strFileValidate = sprintf(formatEpec,strDataWeb,strAlgoritmo,fileVC,tipoVCOption,i);
    strPathFileValidate = sprintf(strPath2,strPath,strFileValidate);
    patron6InValidate = importfile(strPathFileValidate);
    xInValidate = patron6InValidate(:,1:6);
    xTargetValidate = patron6InValidate(:,7);
    TestingData_File =  [xTargetValidate,xInValidate];
    %% Atention
    %% default:0 (traditional cross-fold), inverse swap-cross-folder sw=1
            if sw == 1
            tmp=TrainingData_File;
            TrainingData_File = TestingData_File;
            TestingData_File =  tmp;
            end
    %% 

    ActivationFunction = 'sig';
    %ActivationFunction = 'radbas';

        %% ELM
        if strcmp(metodoIn,'elm')
        sigma2=0.1;
        C=0.01;
        NumberofHiddenNeurons = hLayersIn;    
        [TrainingAccuracy, TestingAccuracy,r] = ELM(TrainingData_File, TestingData_File, 0, NumberofHiddenNeurons, ActivationFunction,3);
        mRMSETrain = [mRMSETrain,TrainingAccuracy];
        mRMSEVal = [mRMSEVal,TestingAccuracy];
        mPearsonVal = [mPearsonVal,r];
        end

        %% BP    
        if strcmp(metodoIn,'bp')
        %disp('Bp');    
        %sigma2=0.1;
        %C=0.01;
        NumberofHiddenNeurons = hLayersIn;
        %ActivationFunction = 'sig';
        [TrainingAccuracy, TestingAccuracy,r] = bp(TrainingData_File, TestingData_File, NumberofHiddenNeurons, ActivationFunction);
        mRMSETrain = [mRMSETrain,TrainingAccuracy];
        mRMSEVal = [mRMSEVal,TestingAccuracy];
        mPearsonVal = [mPearsonVal,r];
        end  

        %% RForest    
        if strcmp(metodoIn,'rforest')
        disp('Random Forest');    
        leaf=10;
        ntrees=500;
        [TrainingAccuracy, TestingAccuracy,r] = rforest(TrainingData_File, TestingData_File, leaf, ntrees);
        mRMSETrain = [mRMSETrain,TrainingAccuracy];
        mRMSEVal = [mRMSEVal,TestingAccuracy];
        mPearsonVal = [mPearsonVal,r];
        end  

        %% mRegresion - regression multilinear
        if strcmp(metodoIn,'mRegresion')
        disp('R. mRegresion');   
        [TrainingAccuracy, TestingAccuracy,r] = mRegresion(TrainingData_File, TestingData_File);
        mRMSETrain = [mRMSETrain,TrainingAccuracy];
        mRMSEVal = [mRMSEVal,TestingAccuracy];
        mPearsonVal = [mPearsonVal,r]; 
        end

        %% SVM
        if strcmp(metodoIn,'svm')
        disp('SVM');    
        [TrainingAccuracy, TestingAccuracy,r] = svm(TrainingData_File, TestingData_File);
        mRMSETrain = [mRMSETrain,TrainingAccuracy];
        mRMSEVal = [mRMSEVal,TestingAccuracy];
        mPearsonVal = [mPearsonVal,r];
        end

        %% RNolineal
        if strcmp(metodoIn,'rnolineal')
        disp('R. no Lineal');    
        end

        %% RLineal 
        if strcmp(metodoIn,'rlineal')
        disp('R. Lineal'); 
       [TrainingAccuracy, TestingAccuracy,r] = rlineal(TrainingData_File, TestingData_File);
        mRMSETrain = [mRMSETrain,TrainingAccuracy];
        mRMSEVal = [mRMSEVal,TestingAccuracy];
        mPearsonVal = [mPearsonVal,r];
        end

    end

valMean = mean(mRMSEVal);
vari = var(mRMSEVal);
desv = sqrt(vari);
coefVar = desv./valMean;
rMean = mean(mPearsonVal);
mEV = [mEV,valMean,coefVar,rMean;];
%validationPromedio
%coefVariacion
%rPromedio
end
 