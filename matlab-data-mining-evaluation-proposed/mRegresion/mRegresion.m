function [TrainingAccuracy, TestingAccuracy,rOut] = mRegresion(train_data, test_data)
X = train_data(:,2:7);
Y = train_data(:,1);

[beta,Sigma,E,CovB,logL] = mvregress(X,Y);
TrainingAccuracy = sqrt(mse(E));

%rOut = sqrt(mse(E));


In = test_data(:,2:7);
x=test_data(:,1);
salida = In*beta;
TestingAccuracy = sqrt(mse(salida - x));
cct=corrcoef(x,salida);
cct=cct(2,1);
rOut = cct;


% 
% y2 = predict(mdl,test_data(:,2:7));
% yT = test_data(:,1)';
% TestingAccuracy = sqrt(mse(y2 - yT));
% [r,p] = corrcoef(y2',yT');
%     rOut = r(1,2);
%     
% y2 = predict(mdl,X);
% yT = Y;
% TrainingAccuracy = sqrt(mse(y2 - yT));

end