%checking excitation manipulation

clear all;

load 'ExcPattern'

numm=25; %number of muscles

for j=1:101
    for m=1:numm
        xr(j,m)=ExcPattern(j,4*m-3);
        yr(j,m)=ExcPattern(j,4*m-2);
        xl(j,m)=ExcPattern(j,4*m-1);
        yl(j,m)=ExcPattern(j,4*m);
    end
end

for m=1:numm
    subplot(5,5,m);
    plot(xr(:,m),yr(:,m),xl(:,m),yl(:,m),':');
    axis([0,100,0,1.0])
end

% x1R=ExcPattern(:,1);
% sol_R=ExcPattern(:,2);
% 
% x1L=ExcPattern(:,3);
% sol_L=ExcPattern(:,4);
% 
% x2R=ExcPattern(:,5);
% Mgas_R=ExcPattern(:,6);
% 
% x2L=ExcPattern(:,7);
% Mgas_L=ExcPattern(:,8);
% 
% x3R=ExcPattern(:,9);
% bflh_R=ExcPattern(:,10);
% 
% x3L=ExcPattern(:,11);
% bflh_L=ExcPattern(:,12);
% 
% x4R=ExcPattern(:,13);
% bfsh_R=ExcPattern(:,14);
% 
% x4L=ExcPattern(:,15);
% bfsh_L=ExcPattern(:,16);
% 
% x5R=ExcPattern(:,17);
% psoas_R=ExcPattern(:,18);
% 
% x5L=ExcPattern(:,19);
% psoas_L=ExcPattern(:,20);
% 
% x6R=ExcPattern(:,21);
% ta_R=ExcPattern(:,22);
% 
% x6L=ExcPattern(:,23);
% ta_L=ExcPattern(:,24);
% 
% x7R=ExcPattern(:,25);
% rf_R=ExcPattern(:,26);
% 
% x7L=ExcPattern(:,27);
% rf_L=ExcPattern(:,28);
% 
% x8R=ExcPattern(:,29);
% vas1_R=ExcPattern(:,30);
% 
% x8L=ExcPattern(:,31);
% vas1_L=ExcPattern(:,32);
% 
% x9R=ExcPattern(:,33);
% gmax_R=ExcPattern(:,34);
% 
% x9L=ExcPattern(:,35);
% gmax_L=ExcPattern(:,36);
% 
% x10R=ExcPattern(:,37);
% Lgas_R=ExcPattern(:,38);
% 
% x10L=ExcPattern(:,39);
% Lgas_L=ExcPattern(:,40);
% 
% x11R=ExcPattern(:,41);
% sm_R=ExcPattern(:,42);
% 
% x11L=ExcPattern(:,43);
% sm_L=ExcPattern(:,44);
% 
% x12R=ExcPattern(:,45);
% ilacus_R=ExcPattern(:,46);
% 
% x12L=ExcPattern(:,47);
% ilacus_L=ExcPattern(:,48);
% 
% x13R=ExcPattern(:,49);
% vas2_R=ExcPattern(:,50);
% 
% x13L=ExcPattern(:,51);
% vas2_L=ExcPattern(:,52);
% 
% x14R=ExcPattern(:,53);
% vas3_R=ExcPattern(:,54);
% 
% x14L=ExcPattern(:,55);
% vas3_L=ExcPattern(:,56);
% 
% x15R=ExcPattern(:,57);
% addmag_R=ExcPattern(:,58);
% 
% x15L=ExcPattern(:,59);
% addmag_L=ExcPattern(:,60);
% 
% 
% figure(100)
% 
% subplot(5,3,1)
% plot(x1R,sol_R,x1L,sol_L,':')
% axis([0,max(x1R),0,1.0])
% 
% subplot(5,3,2)
% plot(x2R,Mgas_R,x2L,Mgas_L,':')
% axis([0,max(x2R),0,1.0])
% 
% subplot(5,3,3)
% plot(x3R,bflh_R,x3L,bflh_L,':')
% axis([0,max(x3R),0,1.0])
% 
% subplot(5,3,4)
% plot(x4R,bfsh_R,x4L,bfsh_L,':')
% axis([0,max(x4R),0,1.0])
% 
% subplot(5,3,5)
% plot(x5R,psoas_R,x5L,psoas_L,':')
% axis([0,max(x5R),0,1.0])
% 
% subplot(5,3,6)
% plot(x6R,ta_R,x6L,ta_L,':')
% axis([0,max(x6R),0,1.0])
% 
% subplot(5,3,7)
% plot(x7R,rf_R,x7L,rf_L,':')
% axis([0,max(x7R),0,1.0])
% 
% subplot(5,3,8)
% plot(x8R,vas1_R,x8L,vas1_L,':')
% axis([0,max(x8R),0,1.0])
% 
% subplot(5,3,9)
% plot(x9R,gmax_R,x9L,gmax_L,':')
% axis([0,max(x9R),0,1.0])
% 
% subplot(5,3,10)
% plot(x10R,Lgas_R,x10L,Lgas_L,':')
% axis([0,max(x10R),0,1.0])
% 
% subplot(5,3,11)
% plot(x11R,sm_R,x11L,sm_L,':')
% axis([0,max(x11R),0,1.0])
% 
% subplot(5,3,12)
% plot(x12R,ilacus_R,x12L,ilacus_L,':')
% axis([0,max(x12R),0,1.0])
% 
% subplot(5,3,13)
% plot(x13R,vas2_R,x13L,vas2_L,':')
% axis([0,max(x13R),0,1.0])
% 
% subplot(5,3,14)
% plot(x14R,vas3_R,x14L,vas3_L,':')
% axis([0,max(x14R),0,1.0])
% 
% subplot(5,3,15)
% plot(x15R,addmag_R,x15L,addmag_L,':')
% axis([0,max(x15R),0,1.0])
