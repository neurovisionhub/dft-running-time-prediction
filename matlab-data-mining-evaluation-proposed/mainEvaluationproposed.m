clear;
%% 5.2. Learning Methods 

%scrips para analisis de eficacia de red neuronal
addpath('./BP/')
addpath('./ELM/')
addpath('./mRegresion')
addpath('./RForest/')
addpath('./RLineal/')
addpath('./SVM/')
addpath('./data/')
load('../features-baseline/baseline-features.mat')
load('../descriptores42QuerieTrecCW09.mat')





%Experimentos multiples clasificadores
matrixCompletaTMP = [];

%Opciones de evaluación
tipoVCOption = 2 % CF10 ->10=10, CF5->20=5, CF3->33=3, CF2->50=2 
strAlgoritmo = 'BMW'; % BMW/WAND
strDataWeb = 'Gov2'; % CW/Gov2
hLayers = 1;
%metodo -> BP,ELM,RForest,RNolineal,RLineal,SVM... ej: metodo = 'elm';
strScore = 'TFIDF'; % BM25/TFIDF

%[matrixCompletaTMP0] = mainBase(tipoVCOption,strAlgoritmo,strScore,strDataWeb,hLayers);

matrixCompletaRN = [];

hLayers = 1;
[matrixCompletaTMP] = mainBase(tipoVCOption,strAlgoritmo,strScore,strDataWeb,hLayers);
matrixCompletaRN = [matrixCompletaRN,matrixCompletaTMP];

hLayers = 5;
[matrixCompletaTMP] = mainBase(tipoVCOption,strAlgoritmo,strScore,strDataWeb,hLayers);
matrixCompletaRN = [matrixCompletaRN,matrixCompletaTMP];

hLayers = 10;
[matrixCompletaTMP] = mainBase(tipoVCOption,strAlgoritmo,strScore,strDataWeb,hLayers);
matrixCompletaRN = [matrixCompletaRN,matrixCompletaTMP];

hLayers = 25;
[matrixCompletaTMP] = mainBase(tipoVCOption,strAlgoritmo,strScore,strDataWeb,hLayers);
matrixCompletaRN = [matrixCompletaRN,matrixCompletaTMP];

hLayers = 50;
[matrixCompletaTMP] = mainBase(tipoVCOption,strAlgoritmo,strScore,strDataWeb,hLayers);
matrixCompletaRN = [matrixCompletaRN,matrixCompletaTMP];

