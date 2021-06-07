function [medidasErrorValidacion] = evaluacionPredictor(tipoVCOption,strAlgoritmo,strScore,strDataWeb,hLayersIn,metodoIn)
matrixRMSETrain = [];
matrixRMSEValidate = [];
matrixCPearsonValidate = [];
medidasErrorValidacion = [];
%Ejemplos de entradas
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
ActivationFunction = 'sig';
%ActivationFunction = 'radbas';

%Aca evaluacion con patron
    %ELM
    if strcmp(metodoIn,'elm')
    sigma2=0.1;
    C=0.01;
    NumberofHiddenNeurons = hLayersIn;
    
    [TrainingAccuracy, TestingAccuracy,r] = ELM(TrainingData_File, TestingData_File, 0, NumberofHiddenNeurons, ActivationFunction,3);
    matrixRMSETrain = [matrixRMSETrain,TrainingAccuracy];
    matrixRMSEValidate = [matrixRMSEValidate,TestingAccuracy];
    matrixCPearsonValidate = [matrixCPearsonValidate,r];
    end
    
    
    %BP
    
    if strcmp(metodoIn,'bp')
    %disp('Bp');    
    %sigma2=0.1;
    %C=0.01;
    NumberofHiddenNeurons = hLayersIn;
    %ActivationFunction = 'sig';


    [TrainingAccuracy, TestingAccuracy,r] = bp(TrainingData_File, TestingData_File, NumberofHiddenNeurons, ActivationFunction);
    matrixRMSETrain = [matrixRMSETrain,TrainingAccuracy];
    matrixRMSEValidate = [matrixRMSEValidate,TestingAccuracy];
    matrixCPearsonValidate = [matrixCPearsonValidate,r];
    end
    

    
    %RForest.
    
    if strcmp(metodoIn,'rforest')
    disp('Random Forest');    
    leaf=10;
    ntrees=500;
    [TrainingAccuracy, TestingAccuracy,r] = rforest(TrainingData_File, TestingData_File, leaf, ntrees);
    matrixRMSETrain = [matrixRMSETrain,TrainingAccuracy];
    matrixRMSEValidate = [matrixRMSEValidate,TestingAccuracy];
    matrixCPearsonValidate = [matrixCPearsonValidate,r];
    %sigma2=0.1;
    %C=0.01;
    %NumberofHiddenNeurons = hLayersIn;
    %ActivationFunction = 'sig';
    % [TrainingTime, TestingTime, TrainingAccuracy, TestingAccuracy,r] = ELM(TrainingData_File, TestingData_File, 0, NumberofHiddenNeurons, ActivationFunction,3);
    %matrixRMSETrain = [matrixRMSETrain,TrainingAccuracy];
    %matrixRMSEValidate = [matrixRMSEValidate,TestingAccuracy];
    %matrixCPearsonValidate = [matrixCPearsonValidate,r];
    end
    
    
    %mRegresion - regression multilinear
    
    if strcmp(metodoIn,'mRegresion')
    disp('R. mRegresion');   
    [TrainingAccuracy, TestingAccuracy,r] = mRegresion(TrainingData_File, TestingData_File);
    matrixRMSETrain = [matrixRMSETrain,TrainingAccuracy];
    matrixRMSEValidate = [matrixRMSEValidate,TestingAccuracy];
    matrixCPearsonValidate = [matrixCPearsonValidate,r];
    
    %sigma2=0.1;
    %C=0.01;
    %NumberofHiddenNeurons = hLayersIn;
    %ActivationFunction = 'sig';
    % [TrainingTime, TestingTime, TrainingAccuracy, TestingAccuracy,r] = ELM(TrainingData_File, TestingData_File, 0, NumberofHiddenNeurons, ActivationFunction,3);
    %matrixRMSETrain = [matrixRMSETrain,TrainingAccuracy];
    %matrixRMSEValidate = [matrixRMSEValidate,TestingAccuracy];
    %matrixCPearsonValidate = [matrixCPearsonValidate,r];
    end
    
        %SVM
    if strcmp(metodoIn,'svm')
    disp('SVM');    
    [TrainingAccuracy, TestingAccuracy,r] = svm(TrainingData_File, TestingData_File);
    matrixRMSETrain = [matrixRMSETrain,TrainingAccuracy];
    matrixRMSEValidate = [matrixRMSEValidate,TestingAccuracy];
    matrixCPearsonValidate = [matrixCPearsonValidate,r];
    
    end
    
    %RNolineal
    if strcmp(metodoIn,'rnolineal')
    disp('R. no Lineal');    
    %sigma2=0.1;
    %C=0.01;
    %NumberofHiddenNeurons = hLayersIn;
    %ActivationFunction = 'sig';
    % [TrainingTime, TestingTime, TrainingAccuracy, TestingAccuracy,r] = ELM(TrainingData_File, TestingData_File, 0, NumberofHiddenNeurons, ActivationFunction,3);
    %matrixRMSETrain = [matrixRMSETrain,TrainingAccuracy];
    %matrixRMSEValidate = [matrixRMSEValidate,TestingAccuracy];
    %matrixCPearsonValidate = [matrixCPearsonValidate,r];
    end
    
    
    %RLineal
    
    if strcmp(metodoIn,'rlineal')
    disp('R. Lineal'); 
   [TrainingAccuracy, TestingAccuracy,r] = rlineal(TrainingData_File, TestingData_File);
    matrixRMSETrain = [matrixRMSETrain,TrainingAccuracy];
    matrixRMSEValidate = [matrixRMSEValidate,TestingAccuracy];
    matrixCPearsonValidate = [matrixCPearsonValidate,r];
    
    
    %sigma2=0.1;
    %C=0.01;
    %NumberofHiddenNeurons = hLayersIn;
    %ActivationFunction = 'sig';
    % [TrainingTime, TestingTime, TrainingAccuracy, TestingAccuracy,r] = ELM(TrainingData_File, TestingData_File, 0, NumberofHiddenNeurons, ActivationFunction,3);
    %matrixRMSETrain = [matrixRMSETrain,TrainingAccuracy];
    %matrixRMSEValidate = [matrixRMSEValidate,TestingAccuracy];
    %matrixCPearsonValidate = [matrixCPearsonValidate,r];
    end


end
    
%comparacion = [matrixRMSETrain;matrixRMSEValidate]';
validacionPromedio = mean(matrixRMSEValidate);
varianza = var(matrixRMSEValidate);
desviacion = sqrt(varianza);
coefVariacion = desviacion./validacionPromedio;
rPromedio = mean(matrixCPearsonValidate);

medidasErrorValidacion = [medidasErrorValidacion,validacionPromedio,coefVariacion,rPromedio;];

validacionPromedio
coefVariacion
rPromedio
end
 