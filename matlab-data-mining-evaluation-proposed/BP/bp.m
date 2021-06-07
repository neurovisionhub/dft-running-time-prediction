function [TrainingAccuracy, TestingAccuracy,rOut] = bp(train_data, test_data, NumberofHiddenNeurons, ActivationFunction)

    x = train_data(:,2:7)';
    t = train_data(:,1)';
    
%     switch lower(ActivationFunction)
%     case {'sig','sigmoid'}
%         %%%%%%%% Sigmoid 
%         H_test = 1 ./ (1 + exp(-tempH_test));
%     case {'sin','sine'}
%         %%%%%%%% Sine
%         H_test = sin(tempH_test);        
%     case {'hardlim'}
%         %%%%%%%% Hard Limit
%         H_test = hardlim(tempH_test);        
%     case {'tribas'}
%         %%%%%%%% Triangular basis function
%         H_test = tribas(tempH_test);        
%     case {'radbas'}
%         %%%%%%%% Radial basis function
%         H_test = radbas(tempH_test);        
%         %%%%%%%% More activation functions can be added here        
%     end
    
    net = feedforwardnet(NumberofHiddenNeurons);
    net = configure(net,x,t);
    net = train(net,x,t);

    
    
    x = test_data(:,2:7)';
    yT = test_data(:,1)';
    y2 = net(x);
    TestingAccuracy = sqrt(mse(y2 - yT));
    [r,p] = corrcoef(y2',yT');
    rOut = r(1,2);
    
    x = train_data(:,2:7)';
    yT = train_data(:,1)';
    y2 = net(x);
    TrainingAccuracy = sqrt(mse(y2 - yT));

    
end