clear all;
%close all;

    
%**** Generate array of muscle names

   name=['SOL  ';'GAS  ';'HAMS ';'BFsh ';'PSOAS';'TA   ';'RF   ';'VAS  ';'GMAX '];;  


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Plot the simulation data versus the experimental data

% load simulation data

%load 'C:\Documents and Settings\Rick Neptune\Desktop\UF_Synergy_Combined_Modules_ForceRF2-6\oct21\SAtrack.dat';
load SAtrack.dat;
    time=[1:1:165]';
    best=[time,SAtrack];
    [npts,ncol]=size(best);
    best(:,1)=165.0*best(:,1)/max(best(:,1));


%load experimental data

load Control_Mean_Data_All.txt;
    gait=Control_Mean_Data_All;
    [npts,ncol]=size(gait);


%       opt(i,2) = hipr
%       opt(i,3) = kneer
%       opt(i,4) = ankr
%       opt(i,5) = hipl
%       opt(i,6) = kneel
%       opt(i,7) = ankl
%       opt(i,8) = hiptx
%       opt(i,9) = hipty
%       opt(i,10) = grfzl
%       opt(i,11) = hip_trq
%       opt(i,12) = kne_trq
%       opt(i,13) = ank_trq
%       opt(i,14) = grfzr
%       opt(i,15) = hip_pwr
%       opt(i,16) = kne_pwr
%       opt(i,17) = ank_pwr
%       opt(i,18) = grfxl
%       opt(i,19) = ankle_vel
%       opt(i,20) = segme right leg
%       opt(i,21) = segme left leg
%       opt(i,22) = grfxr
%       opt(i,23) = trunk angle
%       opt(i,24) = external power
%       opt(i,25) = system ME
%       opt(i,26) = system PE
%       opt(i,27) = system KE


%         read(4,*)time,hipr,hiprsd,kneer,kneersd,ankr,ankrsd,
%     * 	8	hipl,hiplsd,kneel,kneelsd,ankl,anklsd,
%     *         14      hiptx,hiptxsd,hipty,hiptysd,grfz,grfzsd,
%     *         20      hiptrq,hiptrqsd,knetrq,knetrqsd,
%     *		24      anktrq,anktrqsd,grfzl,grfzlsd,
%     *         28      hippwr,hippwrsd,knepwr,knepwrsd,
%     *		32      ankpwr,ankpwrsd,grfx,grfxsd,
%     *		36      ankvel,ankvelsd

curve_name=str2mat(' Vertical GRF','Horizontal GRF','Hip Torque','Hip Power',...
		   'Knee Torque','Knee Power','Ankle Torque','Ankle Power');
curve_name2=str2mat('Trunk-TX','Trunk-TY','Rt Hip Angle','Lt Hip Angle',...
		    'Rt Knee Angle','Lt Knee Angle','Rt Ankle Angle','Lt Ankle Angle','Pelv Angle');

sim_data = [best(:,1),best(:,14),best(:,18),-best(:,11),best(:,15),best(:,12),-best(:,16), ...
	   -best(:,13),best(:,17)];

exp_data = [gait(:,1),gait(:,26),gait(:,27),gait(:,34),gait(:,35),-gait(:,20),-gait(:,21), ...
	    gait(:,28),gait(:,29),gait(:,22),gait(:,23),-gait(:,30),-gait(:,31),-gait(:,24),-gait(:,25), ...
	    gait(:,32),gait(:,33),-gait(:,30)];

%sim_data2 = [best(:,1),best(:,8),best(:,9),best(:,2),best(:,5),best(:,3),best(:,6),best(:,4),best(:,7)];
sim_data2 = [best(:,1),best(:,8),best(:,9),best(:,2),best(:,5),best(:,3),best(:,6),best(:,4),best(:,7),best(:,23)];

% exp_data2 = [gait(:,1),gait(:,14),gait(:,15),gait(:,16),gait(:,17),gait(:,2),gait(:,3),gait(:,8),gait(:,9), ...
% 	     gait(:,4),gait(:,5),gait(:,10),gait(:,11),gait(:,6),gait(:,7),gait(:,12),gait(:,13)];
exp_data2 = [gait(:,1),gait(:,14),gait(:,15),gait(:,16),gait(:,17),gait(:,2),gait(:,3),gait(:,8),gait(:,9), ...
	     gait(:,4),gait(:,5),gait(:,10),gait(:,11),gait(:,6),gait(:,7),gait(:,12),gait(:,13),gait(:,38),gait(:,39)];

[nexp,ncol]=size(exp_data);
[npts,ncol]=size(sim_data);

nsd=2;
k=2;
n=1;
nstep=3;

figure(1);
%set(gcf,'Position',[240 9 816 945]);
set(gcf,'Position',[140 40 640 650]);
orient tall;

for j=1:8
%  subplot(5,2,n,'align'),errorbar(exp_data(2:nstep:nexp,1),exp_data(2:nstep:nexp,k),nsd*exp_data(2:nstep:nexp,k+1),'g')
   hold on
   subplot(5,2,n,'align'),plot(sim_data(:,1),sim_data(:,j+1),'r--');
   figfrmt;
   box off;
   title(curve_name(j,:));


	if j == 1
           ylabel('% BW');
           
           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
           %subplot(5,2,n), errorbar(exp_data(2:nstep:nexp,1),gait(2:nstep:nexp,18),gait(2:nstep:nexp,19),'c')
           subplot(5,2,n,'align'), plot(best(:,10),'r--');
           figfrmt;
           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	end
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    if j==2
        %subplot(5,2,n), errorbar(exp_data(2:nstep:nexp,1),gait(2:nstep:nexp,42),gait(2:nstep:nexp,43),'c')
        subplot(5,2,n,'align'),plot(best(:,22),'r--');
        figfrmt;
    end
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	%if j == 7
    %   ylabel('Moment (N-m/Kg)');
	%   xlabel('% Gait Cycle');
	%   set(gca,'XTickLabels',[0 50 100 150]')
    %end
    
	if j == 8
       ylabel('Power (watts)');
	   xlabel('% Gait Cycle');
	   set(gca,'XTickLabel',[0 50 100 150]')
   end

   k=k+2;
   n=n+1;

end


% ** Add External Power 

%load andriac150_extpwr.dat;
%    extpwr=andriac150_extpwr;

   %nstart=18;
   %[npts,ncol]=size(extpwr);
   %truncated=[extpwr(1:npts-nstart,1),extpwr(nstart+1:npts,2:ncol)];
   %clear extpwr;
   %extpwr=truncated;

%   subplot(5,2,9,'align'),errorbar(gait(:,1),gait(:,40),gait(:,41),'g')
   hold on
   subplot(5,2,9,'align'),plot(best(:,1),best(:,24),'r--');
   figfrmt;
   box off;
   title('External Power');
   xlabel('% Gait Cycle');
   set(gca,'XTickLabel',[0 50 100 150]')
   set(gca,'YLim',[-1.0,.6])
   set(gca,'YTick',[-1.0 0 .6]');
   set(gca,'YTickLabel',[-1.0 0 .6]');
   %set(gca,'YLim',[-250,250])
   %set(gca,'YTick',[-250 0 250]');
   %set(gca,'YTickLabels',[-250 0 250]');

   

%%%%% Plot joint angles %%%%%%
nsd=2;
k=2;
n=1;
nstep=3;

figure(2);
%set(gcf,'Position',[240 9 816 945]);
set(gcf,'Position',[240 40 640 650]);
orient tall;

for j=1:9
%   subplot(5,2,n,'align'),errorbar(exp_data2(2:nstep:nexp,1),exp_data2(2:nstep:nexp,k),nsd*exp_data2(2:nstep:nexp,k+1),'g')
   hold on
   subplot(5,2,n,'align'),plot(sim_data2(:,1),sim_data2(:,j+1),'r--');
   figfrmt;
   box off;
   title(curve_name2(j,:));


	if j == 1
           ylabel('(m)');
	end
	if j == 2
           set(gca,'YLim',[.7 1.5]);
    end
	if j == 8 | j==9
	   xlabel('% Gait Cycle');
	   set(gca,'XTickLabel',[0 50 100 150]')
	end

   k=k+2;
   n=n+1;

end


figure(3);
%set(gcf,'Position',[240 9 816 945]);
set(gcf,'Position',[140 40 640 650]);
orient tall;

%%%%% Plot leg segment ME
	subplot(5,2,1,'align'),plot(best(50:150,20));
	hold on
	left=[best(101:150,21)',best(51:100,21)'];
	subplot(5,2,1,'align'),plot(left,'r--');
	set(gca,'YLim',[60 120]);
	figfrmt;
	box off;
	title('Limb Segment Energy');
	H1=legend('Right','Left');
	%set(H1,'Position',[ 0.467746160602969 0.158265563530436 0.169117647058824 0.0409523809523809 ]);
    
	subplot(5,2,3,'align'),plot(best(:,1),best(:,25));
	set(gca,'YLim',[750 850]);
	figfrmt;
	box off;
	title('Total System Energy');
    
 	subplot(5,2,4,'align'),plot(best(50:100,25),'b');
    hold on;
 	subplot(5,2,4,'align'),plot(best(100:150,25),'g');
	set(gca,'YLim',[750 850]);
	figfrmt;
    set(gca,'XLim',[0 50]);
	box off;
	title('Total System Energy');
   
	subplot(5,2,5,'align'),plot(best(:,1),best(:,26));
	figfrmt;
	set(gca,'YLim',[650 750]);
	box off;
	title('System Potential Energy');
    
    subplot(5,2,6,'align'),plot(best(50:100,26),'b');
    hold on;
 	subplot(5,2,6,'align'),plot(best(100:150,26),'g');
	figfrmt;
    set(gca,'XLim',[0 50]);
	set(gca,'YLim',[650 750]);
	box off;
	title('System Potential Energy');


    subplot(5,2,7,'align'),plot(best(:,1),best(:,27));
	set(gca,'YLim',[0 100]);
	figfrmt;
	box off;
	title('System Kinetic Energy');

    subplot(5,2,8,'align'),plot(best(50:100,27),'b');
    hold on;
 	subplot(5,2,8,'align'),plot(best(100:150,27),'g');
	figfrmt;
    set(gca,'XLim',[0 50]);
    set(gca,'YLim',[0 100]);
	box off;
	title('System Kinetic Energy');

    
    ylabel('Energy (J)');
	xlabel('% Gait Cycle');
	set(gca,'XTickLabel',[0 50 100 150]')
    
    

