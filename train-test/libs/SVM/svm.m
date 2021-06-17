function [TrainingAccuracy, TestingAccuracy,rOut] = svm(train_data, test_data)

%rng default  % For reproducibility
%Mdl = fitrsvm(Tbl,'Rings','KernelFunction','gaussian','KernelScale','auto','Standardize',true)
X = train_data(:,2:7);
Y = train_data(:,1);
%Mdl = fitrsvm(xInTrain,xTargetTrain)
%MdlLin = fitrsvm(X,Y,'Standardize',true,'KFold',5)
MdlLin = fitrsvm(X,Y,'KernelFunction','gaussian','KernelScale','auto','Standardize',true);
% MdlLin = fitrsvm(X,Y,'OptimizeHyperparameters','auto',...
%     'HyperparameterOptimizationOptions',struct('AcquisitionFunctionName',...
%     'expected-improvement-plus'))  %,'KernelFunction','polynomial'

%[MdlLin,FitInfo] = fitrlinear(X,Y)
%MdlLin=fitrsvm(X,Y,'KernelFunction','polynomial');
%MdlLin=fitlm(X,Y)
%compactMdl = compact(MdlLin)
%vars = whos('compactMdl','MdlLin')
%[vars(1).bytes,vars(2).bytes]
%MdlGau = fitrsvm(X,Y,'Standardize',true,'KFold',5,'KernelFunction','gaussian')

%MdlLin = fitrsvm(X,Y)
%MdlGau = fitrsvm(X,Y,'Standardize',true,'KFold',5,'KernelFunction','gaussian')
%x = test_data(:,2:7);
%y=test_data(:,1);
%mseLin = kfoldLoss(MdlLin)
%mseGau = kfoldLoss(MdlGau)
y2 = predict(MdlLin,test_data(:,2:7));
yT = test_data(:,1)';
TestingAccuracy = sqrt(mse(y2 - yT));
[r,p] = corrcoef(y2',yT');
    rOut = r(1,2);
    
y2 = predict(MdlLin,X);
yT = Y;
TrainingAccuracy = sqrt(mse(y2 - yT));
    
    
%mseLin = 1.1;
% TrainingAccuracy = mseLin;
%TestingAccuracy = mseLin;
%rOut = mseLin;
%MdlLin
end