function PrC(zdata1,titulo)
%CREATEFIGURE(zdata1)
%  ZDATA1:  contour z

%  Auto-generated by MATLAB on 13-Jun-2021 12:47:29

% Create figure
figure1 = figure;
colormap(pink);

% Create axes
axes1 = axes('Parent',figure1,...
    'Position',[0.087467362924282 0.138364779874214 0.803283932412508 0.791485966394443]);
hold(axes1,'on');

% Create contour
[c1,h1] = contour(zdata1,'TextStep',0.005,'LineWidth',1,'LevelStep',0.01,...
    'LabelSpacing',200,...
    'Fill','on');
clabel(c1,h1,...
    [0.78 0.785 0.79 0.795 0.8 0.805 0.81 0.815 0.82 0.825 0.83 0.835 0.84 0.845 0.85 0.855 0.86 0.865 0.87 0.875 0.88 0.885 0.89 0.895 0.9 0.905 0.91 0.915 0.92 0.925 0.93 0.935 0.94 0.945 0.95 0.955 0.96]);

% Create ylabel
ylabel('Threads');

% Create xlabel
xlabel('k');

% Create title
title(titulo);

box(axes1,'on');
grid(axes1,'on');
axis(axes1,'tight');
hold(axes1,'off');
% Set the remaining axes properties
set(axes1,'BoxStyle','full','FontSize',14,'Layer','top','XTick',[1 2 3 4],...
    'XTickLabel',{'10','100','1000','10,000'},'YTick',[1 2 3 4],'YTickLabel',...
    {'1','2','4','8'});
% Create colorbar
colorbar(axes1,'Position',...
    [0.896121976238735 0.138364779874214 0.0266001662510391 0.791202422040117],...
    'Ticks',[0.7 0.75 0.8 0.85 0.9 0.95 1],...
    'TickLabels',{'0.7','0.75','0.8','0.85','0.9','0.95','1'},...
    'Limits',[0.7 1],...
    'FontWeight','bold',...
    'FontSize',12,...
    'FontName','Arial');

