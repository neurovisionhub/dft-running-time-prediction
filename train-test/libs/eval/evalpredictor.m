 %% Evaluation prediction %%
function [matrixCompleta] = evalpredictor(tipoVCOption,strAlgoritmo,strScore,strDataWeb,hLayers,metodo,all,sw)
matrixCompleta = [];
%metodo
%pause
%% 0-> solo llamada actual; this call %%

    if all == 0
        [medidasErrorValidacion] = evaluacionPredictor(tipoVCOption,strAlgoritmo,strScore,strDataWeb,hLayers,metodo,sw);
        matrixCompleta = [matrixCompleta;medidasErrorValidacion];
    end
%pause
%% 1-> + todas las tecnicas; all calls %%
    if all == 1
        metodo = 'rlineal'
        [medidasErrorValidacion] = evaluacionPredictor(tipoVCOption,strAlgoritmo,strScore,strDataWeb,hLayers,metodo,sw);
        matrixCompleta = [matrixCompleta;medidasErrorValidacion];

        metodo = 'mRegresion'
        [medidasErrorValidacion] = evaluacionPredictor(tipoVCOption,strAlgoritmo,strScore,strDataWeb,hLayers,metodo,sw);
        matrixCompleta = [matrixCompleta;medidasErrorValidacion];

        metodo = 'elm'
        [medidasErrorValidacion] = evaluacionPredictor(tipoVCOption,strAlgoritmo,strScore,strDataWeb,hLayers,metodo,sw);
        matrixCompleta = [matrixCompleta;medidasErrorValidacion];

        metodo = 'bp'
        [medidasErrorValidacion] = evaluacionPredictor(tipoVCOption,strAlgoritmo,strScore,strDataWeb,hLayers,metodo,sw);
        matrixCompleta = [matrixCompleta;medidasErrorValidacion];

        metodo = 'rforest'
        [medidasErrorValidacion] = evaluacionPredictor(tipoVCOption,strAlgoritmo,strScore,strDataWeb,hLayers,metodo,sw);
        matrixCompleta = [matrixCompleta;medidasErrorValidacion];

        metodo = 'svm'
        [medidasErrorValidacion] = evaluacionPredictor(tipoVCOption,strAlgoritmo,strScore,strDataWeb,hLayers,metodo,sw);
        matrixCompleta = [matrixCompleta;medidasErrorValidacion];
    end
    
   % pause
end
