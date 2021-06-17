%load('./results.mat')
%% results min {under/over}-fitting
load('./evaluation/results/results-exp-5.mat')
xTmp = baselineTrace;

%Pere
p1bTmp = sortrows(xTmp,4);
xLocal = p1bTmp(1:16,:)
xShared = p1bTmp(17:end,:)

%xLocalSort = sortrows(xLocal,5);

xLocalSort5NeuronsB = [xLocal(:,1:5),xLocal(:,8:9)]'
xSharedSort5NeuronsB = [xShared(:,1:5),xShared(:,8:9)]'
studioBaseline = [xLocalSort5NeuronsB;xSharedSort5NeuronsB]
xTmp = dftTrace;

%Pere
p1bTmpDFT = sortrows(xTmp,4);
xLocalDFT = p1bTmpDFT(1:16,:)
xSharedDFT = p1bTmpDFT(17:end,:)

%xLocalSort = sortrows(xLocal,5);

xLocalSort5NeuronsDFT = [xLocalDFT(:,1:5),xLocalDFT(:,8:9)]'
xSharedSort5NeuronsDFT = [xSharedDFT(:,1:5),xSharedDFT(:,8:9)]'

studioDFT = [xLocalSort5NeuronsDFT;xSharedSort5NeuronsDFT]

studio = [studioBaseline;studioDFT]
studioMapsLB=[];
studioMapsSB=[];
studioMapsLP=[];
studioMapsSP=[];
studioMapsLBe=[];
studioMapsSBe=[];
studioMapsLPe=[];
studioMapsSPe=[];

ini=1;
fin=4;
for i=1:4
%     studioMapsLB = [studioMapsLB;studio(6,ini+(i-1)*4:fin+(i-1)*4)];
%     studioMapsSB = [studioMapsSB;studio(13,ini+(i-1)*4:fin+(i-1)*4)];
%     studioMapsLP = [studioMapsLP;studio(20,ini+(i-1)*4:fin+(i-1)*4)];
%     studioMapsSP = [studioMapsSP;studio(27,ini+(i-1)*4:fin+(i-1)*4)];
%     
%     studioMapsLBe = [studioMapsLBe;studio(7,ini+(i-1)*4:fin+(i-1)*4)];
%     studioMapsSBe = [studioMapsSBe;studio(14,ini+(i-1)*4:fin+(i-1)*4)];
%     studioMapsLPe = [studioMapsLPe;studio(21,ini+(i-1)*4:fin+(i-1)*4)];
%     studioMapsSPe = [studioMapsSPe;studio(28,ini+(i-1)*4:fin+(i-1)*4)];
%     
    studioMapsLBe = [studioMapsLBe;studio(6,ini+(i-1)*4:fin+(i-1)*4)];
    studioMapsSBe = [studioMapsSBe;studio(13,ini+(i-1)*4:fin+(i-1)*4)];
    studioMapsLPe = [studioMapsLPe;studio(20,ini+(i-1)*4:fin+(i-1)*4)];
    studioMapsSPe = [studioMapsSPe;studio(27,ini+(i-1)*4:fin+(i-1)*4)];
    
end

% figure;heatmap(studioMapsLB);
% figure;heatmap(studioMapsSB);
% figure;heatmap(studioMapsLP);
% figure;heatmap(studioMapsSP);

% figure;
% mesh(studioMapsLB);
% hold on
% mesh(studioMapsLP);
% mesh(studioMapsSB);
% mesh(studioMapsSP);

% figure;
% %mesh(studioMapsLBe);
% hold on
% %mesh(studioMapsLPe);
% mesh(studioMapsSBe);
% 
% mesh(studioMapsSPe);


% contour(studioMapsSBe)
% contour(studioMapsSPe)

PrC(studioMapsSBe,'Baseline - SBM-WAND')
saveas(gcf,'baseline-sbm-wand.png')
PrC(studioMapsSPe,'DFT-Based  - SBM-WAND')
saveas(gcf,'DFT-Based-sbm-wand.png')


PrC(studioMapsLBe,'Baseline - LBM-WAND')
saveas(gcf,'baseline-lbm-wand.png')
PrC(studioMapsLPe,'DFT-Based - LBM-WAND')
saveas(gcf,'DFT-Based-lbm-wand.png')
% 
% 
% figure
% imagesc(studioMapsSBe)
% figure
% imagesc(studioMapsSPe)
% figure
% imagesc(studioMapsLBe)
% figure
% imagesc(studioMapsLPe)