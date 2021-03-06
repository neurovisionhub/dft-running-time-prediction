function createfigure_dist(zdata1)
%CREATEFIGURE(zdata1)
%  ZDATA1:  contour z

%  Auto-generated by MATLAB on 12-Jun-2021 17:55:55

% Create figure
figure1 = figure;
colormap(pink);

% Create axes
axes1 = axes('Parent',figure1);
hold(axes1,'on');

% Create contour
[c1,h1] = contour(zdata1,'TextStep',0.005,'LineWidth',5,'LevelStep',0.01,...
    'LabelSpacing',1000,...
    'Fill','on');
clabel(c1,h1);

box(axes1,'on');
axis(axes1,'tight');
hold(axes1,'off');
% Set the remaining axes properties
set(axes1,'BoxStyle','full','FontSize',16,'Layer','top','OuterPosition',...
    [0 0 1 1],'XTick',[1 2 3 4],'XTickLabel',{'10','100','1000','10000'},...
    'YTick',[1 2 3 4],'YTickLabel',{'1','2','4','8'});
% Create colorbar
colorbar(axes1,'Position',...
    [1.0 0.113636363636364 0.0125195618153365 0.815047021943573],...
    'Limits',[0.01 0.07]);

