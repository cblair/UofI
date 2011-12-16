%**** General formats

axis ('normal');

set(gca,'LineWidth',2.0);
H1=get(gca,'Children');
set(H1,'LineWidth',2.0);
set(gca,'FontSize',10,'FontName','Arial', 'FontWeight','bold');

set(gca,'XLim',[0 165])
set(gca,'XTick',[0 50 100 150 165]')
set(gca,'XTickLabel',[ ]');

grid off;

%**** Add zero reference line

         x=[0;165];
         y=[0;0];
         H=line(x,y);
 	 set(H,'LineWidth',0.5,'LineStyle','-','Color','k');        
