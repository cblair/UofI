% This draws a figure of emg patterns and block-excitation timing

clear all;
close all;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
load W60emgave.txt; %experimental emg data
emg=W60emgave;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[row,col]=size(emg); %the first column is index, then ave and sd for sol,gas,bflf,ta,rf,vas,gas in this order
x_axis=emg(:,1); % gait cycle index
nmus=7; %7 emg data

nmusG=15;

     
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%normalize emg
for i=1:nmus
    max_amp(i)=max(emg(:,2*i));
    emg(:,2*i)=emg(:,2*i)/max_amp(i);
end

%%add vas2 and vas3 (for plotting purpose)
emg_add(:,1)=emg(:,2); %sol
emg_add(:,2)=emg(:,4); %gas
emg_add(:,3)=emg(:,6); %ham
emg_add(:,4)=emg(:,8); %ta
emg_add(:,5)=emg(:,10); %rf
emg_add(:,6)=emg(:,12); %vas
emg_add(:,7)=emg(:,12); %vas
emg_add(:,8)=emg(:,12); %vas
emg_add(:,9)=emg(:,14); %gmax



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% add simulation excitation patterns to the plots

load SAoptEMG.dat;
data=SAoptEMG;


% %% spline
% for j=1:101
%     x1(j)=data(j,29);
%     vas1(j)=data(j,30);
%     x2(j)=data(j,33);
%     vas2(j)=data(j,34);
%     x3(j)=data(j,37);
%     vas3(j)=data(j,38);
% end
%  xspace=linspace(0,100,101);
%  
%  vas1spline=spline(x1,vas1,xspace);
%  vas2spline=spline(x2,vas2,xspace);
%  vas3spline=spline(x3,vas3,xspace);
%  
%  for j=1:101
%      if vas1spline(j)<0
%          vas1spline(j)=0;
%      end
%      
%      if vas2spline(j)<0
%          vas2spline(j)=0;
%      end
%      
%      if vas2spline(j)<0
%          vas3spline(j)=0;
%      end
%  end
%  
%  %%take average
%  for j=1:101
%      avevas(j)=(vas1spline(j)+vas2spline(j)+vas3spline(j))/3.0;
%  end
 
 xaxis=1:101;
     

% rearrange data

exc=[data(:,1),data(:,2),...%solG
    data(:,5),data(:,6),...%gasG
    data(:,9),data(:,10),...%hamG
    data(:,21),data(:,22), ...%taG
    data(:,25),data(:,26),...rfG
    data(:,29),data(:,30),...vas1G
    data(:,81),data(:,82),...vas2G
    data(:,85),data(:,86),...vas3G
    data(:,33),data(:,34),...gmaxG
    data(:,13),data(:,14),...%bfshG
    data(:,17),data(:,18),...%psoasG
    data(:,37),data(:,38),...%gmedG
    data(:,41), data(:,42),... %perlngG
    data(:,49), data(:,50),... %flxdigG
    data(:,73), data(:,74)]; %extdigG

% Normalize muscle excitations

%normalize emg

for i=1:nmusG
    max_exc(i)=max(exc(:,2*i));
    norm_exc(:,2*i)=exc(:,2*i)/max_exc(i);
end

j=1;

figure(1);
set(gcf,'Position',[140 40 640 650]);
orient tall;

curve_name=str2mat('SOL','MGAS','BF','TA','RF','VAS1','VAS2','VAS3','GMAX','BFsh','PSOAS','GMED','PERLNG','FLXDIG','EXTDIG');

for nfig=1:nmusG  
    hold on;
    subplot(4,4,nfig),plot(exc(:,j),exc(:,j+1),'r');
    j=j+2; 
    hold on;
    %**** General formats
    axis ('normal');
    set(gca,'LineWidth',2.0);
    H1=get(gca,'Children');
    set(H1,'LineWidth',2.0);
    set(gca,'FontSize',12,'FontName','Arial', 'FontWeight','bold');
    set(gca,'XLim',[0 100])
    set(gca,'YLim',[0 1])
    set(gca,'XTick',[0 50 100]')
    set(gca,'XTickLabel',[ ]');
    grid off;
    box off;
    %****************************************************************
    title(curve_name(nfig,:));
    if (nfig==12 | nfig==13 | nfig==14 | nfig==15)
	   xlabel('% Gait Cycle');
       set(gca,'XTickLabel',[0 50 100]')
    end
    
    if(nfig==13)
        ylabel('% Max');
    end
 
end



%plot emg pattern
xplot=0:100;
n=1;
for i=1:nmus+2
     subplot(4,4,n)
     hold on;
     %plot(x_axis,emg(:,2*i),'g');
     plot(x_axis,emg_add(:,i),'g');
     n=n+1;
     set(gca,'LineWidth',2.0);
     H1=get(gca,'Children');
     set(H1,'LineWidth',2.0);

end


