function [matrixCompleta] = mainBase(tipoVCOption,strAlgoritmo,strScore,strDataWeb,hLayers)
matrixCompleta = [];

% metodo = 'rlineal'
% [medidasErrorValidacion] = evaluacionPredictor(tipoVCOption,strAlgoritmo,strScore,strDataWeb,hLayers,metodo)
% matrixCompleta = [matrixCompleta;medidasErrorValidacion];
% 
% metodo = 'mRegresion'
% [medidasErrorValidacion] = evaluacionPredictor(tipoVCOption,strAlgoritmo,strScore,strDataWeb,hLayers,metodo)
% matrixCompleta = [matrixCompleta;medidasErrorValidacion];
% 
% metodo = 'elm'
% [medidasErrorValidacion] = evaluacionPredictor(tipoVCOption,strAlgoritmo,strScore,strDataWeb,hLayers,metodo)
% matrixCompleta = [matrixCompleta;medidasErrorValidacion];

metodo = 'bp'
[medidasErrorValidacion] = evaluacionPredictor(tipoVCOption,strAlgoritmo,strScore,strDataWeb,hLayers,metodo)
matrixCompleta = [matrixCompleta;medidasErrorValidacion];

% metodo = 'rforest'
% [medidasErrorValidacion] = evaluacionPredictor(tipoVCOption,strAlgoritmo,strScore,strDataWeb,hLayers,metodo)
% matrixCompleta = [matrixCompleta;medidasErrorValidacion];
% 
% 
% metodo = 'svm'
% [medidasErrorValidacion] = evaluacionPredictor(tipoVCOption,strAlgoritmo,strScore,strDataWeb,hLayers,metodo)
% matrixCompleta = [matrixCompleta;medidasErrorValidacion];

end
