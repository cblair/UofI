%**** General formats

axis ('normal');

set(gca,'LineWidth',2.0);
H1=get(gca,'Children');
set(H1,'LineWidth',2.0);
set(gca,'FontSize',10,'FontName','Arial', 'FontWeight','bold');

set(gca,'YLim',[-.5,2])
set(gca,'YTick',[0 1 2]')
set(gca,'YTickLabels',[ ]');

set(gca,'XLim',[0,100])
set(gca,'XTick',[0 50 100]')
set(gca,'XTickLabels',[ ]');

grid off;


%**** Add zero reference line

         x=[0;150];
         y=[0;0];
         H=plot(x,y);
 	 set(H,'LineWidth',0.5);        
