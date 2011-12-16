#include <iostream>
#include "SA.h"
#include "Look.h"
#include "system_params.h"
extern "C" SeeThis mdata;

extern "C" int PATTERN;
//////////////////////////////////////////////////////////////////////
void mod_emg(double **XR,double **YR,double **XL,double **YL,
			 double *x)

{
	int i,m,next;
	int npP0,npP1,npP2,npZ0,npZ1,nnpP1,nnpP2,nnpZ0,point;
	double tp0,tp1,tp2,ntp2;
	double xp0,xp1,xp2,xp3,xp4,xp5, xp6, xp7;
	double t_begin, t_end, t_last;
	double shift, skew, skew1, skew2;
	
	double *pBlockX1;
	double *pBlockY1;
	double *pBlockX2;
	double *pBlockY2;
	double *pX;
	double *pY;

	double **XRtemp,**YRtemp,**XLtemp,**YLtemp;

	pBlockX1=(double*)calloc(N_emg_step,sizeof(double)); if(pBlockX1 == NULL) exit(0);
	pBlockY1=(double*)calloc(N_emg_step,sizeof(double)); if(pBlockY1 == NULL) exit(0);
	pBlockX2=(double*)calloc(N_emg_step,sizeof(double)); if(pBlockX2 == NULL) exit(0);
	pBlockY2=(double*)calloc(N_emg_step,sizeof(double)); if(pBlockY2 == NULL) exit(0);
	pX=(double*)calloc(N_emg_step,sizeof(double)); if(pX == NULL) exit(0);
	pY=(double*)calloc(N_emg_step,sizeof(double)); if(pY == NULL) exit(0);


	XRtemp=(double**)calloc(N_musG,sizeof(double*));
	if(XRtemp == NULL) exit(0);
	for(i=0;i<N_musG;i++){
		XRtemp[i]=(double*)calloc(N_emg_step,sizeof(double));
		if(XRtemp[i] == NULL) exit(0);
	}

	YRtemp=(double**)calloc(N_musG,sizeof(double*));
	if(YRtemp == NULL) exit(0);
	for(i=0;i<N_musG;i++){
		YRtemp[i]=(double*)calloc(N_emg_step,sizeof(double));
		if(YRtemp[i] == NULL) exit(0);
	}

	XLtemp=(double**)calloc(N_musG,sizeof(double*));
	if(XLtemp == NULL) exit(0);
	for(i=0;i<N_musG;i++){
		XLtemp[i]=(double*)calloc(N_emg_step,sizeof(double));
		if(XLtemp[i] == NULL) exit(0);
	}

	YLtemp=(double**)calloc(N_musG,sizeof(double*));
	if(YLtemp == NULL) exit(0);
	for(i=0;i<N_musG;i++){
		YLtemp[i]=(double*)calloc(N_emg_step,sizeof(double));
		if(YLtemp[i] == NULL) exit(0);
	}

	//FILE *fp_write1;


	next = 0;
	for(m=0;m<N_musG;m++){
		for(i=0;i<mdata.ExcP[m].nPara;i++){
			mdata.ExcP[m].xp[i] = x[i + next];
		}
		next = next + mdata.ExcP[m].nPara;
	}



	for(m=0;m<N_musG;m++){

		//////////////clear (initialize)//////////////////
		for(i=0;i<N_emg_step;i++){
				pX[i] = 0.0;
				pY[i] = 0.0;
			}
		//////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////
		if(mdata.ExcP[m].type == 1){//type1
//printf("type1:%d\n",m);
			npP0 = mdata.ExcP[m].pBlock[0].nPoint;
			npZ0 = mdata.ExcP[m].zBlock[0].nPoint;
			npZ1 = mdata.ExcP[m].zBlock[1].nPoint;
			tp0 = (double)mdata.ExcP[m].pBlock[0].t1;
			xp0 = mdata.ExcP[m].xp[0]; //onset 
			xp1 = mdata.ExcP[m].xp[1]; //offset 
			xp2 = mdata.ExcP[m].xp[2]; //amplitude 
			if(PATTERN==BLOCK){//block
				if(m==0 || m==1){ //sol, gas
					xp3 = mdata.ExcP[m].xp[3]; //amplifying for 2nd block
					xp4 = mdata.ExcP[m].xp[4]; //amplifying for 3rd block
					xp5 = mdata.ExcP[m].xp[5]; //amplitude for 4th
					xp6 = mdata.ExcP[m].xp[6]; //amplitude for 5th
					xp7 = mdata.ExcP[m].xp[7]; //amplitude for 6th
				}
				
				else{ //BFsh is type 3
					xp3 = mdata.ExcP[m].xp[3]; //amplifying for 2nd block
					xp4 = mdata.ExcP[m].xp[4]; //amplifying for 3rd block
					xp5 = mdata.ExcP[m].xp[5]; //amplitude for 4th
				}
				
			}

			else{ //emg
				if(m==bfshG || m==psoasG || m==gmedG || m==perlngG || m==flxdigG || m==extdigG){
					xp3 = mdata.ExcP[m].xp[3]; //amplifying for 2nd block
					xp4 = mdata.ExcP[m].xp[4]; //amplifying for 3rd block
					xp5 = mdata.ExcP[m].xp[5]; //amplitude for 4th
				}
			}

			///////create (clear)////////
			for(i=0;i<npP0;i++){
				pBlockX1[i] = 0.0; 
				pBlockY1[i] = 0.0;
			}
			/////////////////////////////

			for(i=0;i<npP0;i++){//shift
				pBlockX1[i] = mdata.ExcP[m].pBlock[0].px[i] - tp0;
				pBlockY1[i] = mdata.ExcP[m].pBlock[0].py[i];
			}

			///////skew and amplify///////

			//first, compute "skew"
			skew=(xp1-xp0)/((double)(mdata.ExcP[m].pBlock[0].t2
				                   - mdata.ExcP[m].pBlock[0].t1));

			if(PATTERN==BLOCK){//block	
				if(m==0 || m==1){//sol, m-gas
					for(i=0;i<npP0;i++){
						pBlockX1[i] = pBlockX1[i] * skew;
					}
					point = npP0/6;
					for(i=0;i<point;i++){	 
						pBlockY1[i] = pBlockY1[i] * xp2;//1st block
					}
					for(i=point;i<2*point;i++){
						pBlockY1[i] = pBlockY1[i] * xp3;//2nd block
					}
					for(i=2*point;i<3*point;i++){
						pBlockY1[i] = pBlockY1[i] * xp4;//3rd block
					}
					for(i=3*point;i<4*point;i++){
						pBlockY1[i] = pBlockY1[i] * xp5;//4thd block
					}
					for(i=4*point;i<5*point;i++){
						pBlockY1[i] = pBlockY1[i] * xp6;//5th block
					}
					for(i=5*point;i<npP0;i++){
						pBlockY1[i] = pBlockY1[i] * xp7;//6th block
					}
				}
				else{
					for(i=0;i<npP0;i++){
						pBlockX1[i] = pBlockX1[i] * skew;
					}
					point = npP0/4;
					for(i=0;i<point;i++){	 
						pBlockY1[i] = pBlockY1[i] * xp2;//1st block
					}
					for(i=point;i<2*point;i++){
						pBlockY1[i] = pBlockY1[i] * xp3;//2nd block
					}
					for(i=2*point;i<3*point;i++){
						pBlockY1[i] = pBlockY1[i] * xp4;//3rd block
					}
					for(i=3*point;i<npP0;i++){
						pBlockY1[i] = pBlockY1[i] * xp5;//4rd block
					}
				}
			}
			else{//emg

				if(m==bfshG || m==psoasG || m==gmedG || m==perlngG || m==flxdigG || m==extdigG){
					for(i=0;i<npP0;i++){
						pBlockX1[i] = pBlockX1[i] * skew;
					}
					point = npP0/4;
					for(i=0;i<point;i++){	 
						pBlockY1[i] = pBlockY1[i] * xp2;//1st block
					}
					for(i=point;i<2*point;i++){
						pBlockY1[i] = pBlockY1[i] * xp3;//2nd block
					}
					for(i=2*point;i<3*point;i++){
						pBlockY1[i] = pBlockY1[i] * xp4;//3rd block
					}
					for(i=3*point;i<npP0;i++){
						pBlockY1[i] = pBlockY1[i] * xp5;//4rd block
					}
				}
				else{
					for(i=0;i<npP0;i++){//skew and amplify								
						pBlockX1[i] = pBlockX1[i] * skew;					
						pBlockY1[i] = pBlockY1[i] * xp2;				
					}
				}
			}
			t_end = pBlockX1[npP0-1]; 

			////////make one continuous curve///////
			for(i=0;i<npP0;i++){
				pX[i] = pBlockX1[i];
				pY[i] = pBlockY1[i];
			}

			for(i=0;i<npZ0+npZ1;i++){
				pX[i+npP0] = t_end + (double)(i+1)*(100.0-t_end)/(double)(npZ0+npZ1);
				pY[i+npP0] = 0.0;
			}
			////////////////////////////////////////	
			//shifting 
			shift = xp0;
			if(shift!=0.0){
				//curveshift(pX,pY,XR,YR,shift,m);
				curveshift(pX,pY,XRtemp,YRtemp,shift,m);
//for(i=0;i<N_emg_step;i++){
//	printf("m=%d: (%d)%f\n",m,i,YRtemp[m][i]);
//}
			}
			else{
				for(i=0;i<N_emg_step;i++){
					//XR[m][i] = pX[i];
					//YR[m][i] = pY[i];
					XRtemp[m][i] = pX[i];
					YRtemp[m][i] = pY[i];
				}
			}
		}
		//////////////////////////end of type 1//////////////////////////////

		/////////////////////////////////////////////////////////////////////
		if(mdata.ExcP[m].type == 2){//type2
//printf("type2:%d\n",m);
			npP0 = mdata.ExcP[m].pBlock[0].nPoint;
			npP1 = mdata.ExcP[m].pBlock[1].nPoint;
			npZ0 = mdata.ExcP[m].zBlock[0].nPoint;
			nnpP1 = npP1 - 1;
			nnpZ0 = npZ0 + 1;
			tp1 = (double)mdata.ExcP[m].pBlock[1].t1; //corresponds to onset
			xp0 = mdata.ExcP[m].xp[0]; //onset 
			xp1 = mdata.ExcP[m].xp[1]; //offset
			xp2 = mdata.ExcP[m].xp[2]; //amplitude 

			if(PATTERN==BLOCK){//block
				xp3 = mdata.ExcP[m].xp[3]; //amplitude 
				xp4 = mdata.ExcP[m].xp[4]; //amplitude 
				xp5 = mdata.ExcP[m].xp[5]; //amplitude 
				
				if((m==0) || (m==1))
				{
					xp6 = mdata.ExcP[m].xp[6]; //amplitude 
					xp7 = mdata.ExcP[m].xp[7]; //amplitude 
				}
			}

			else{ //emg
				if(m==bfshG || m==psoasG || m==gmedG || m==perlngG || m==flxdigG || m==extdigG){
					xp3 = mdata.ExcP[m].xp[3]; //amplifying for 2nd block
					xp4 = mdata.ExcP[m].xp[4]; //amplifying for 3rd block
					xp5 = mdata.ExcP[m].xp[5]; //amplitude 
				}
			}

			///////create (clear)////////
			for(i=0;i<npP0+nnpP1;i++){
				pBlockX1[i] = 0.0; 
				pBlockY1[i] = 0.0;
			}
			/////////////////////////////

			//first, compute "skew"
			skew=(100.0-xp0+xp1)/((100.0 - (double)mdata.ExcP[m].pBlock[1].t1 
				                   + (double)mdata.ExcP[m].pBlock[0].t2));
			for(i=0;i<nnpP1;i++){//shift
				pBlockX1[i] = mdata.ExcP[m].pBlock[1].px[i] - tp1;
				pBlockY1[i] = mdata.ExcP[m].pBlock[1].py[i];
			}
			for(i=0;i<npP0;i++){//shift
				pBlockX1[i+nnpP1] = 100.0 - tp1 + (double)i;
				pBlockY1[i+nnpP1] = mdata.ExcP[m].pBlock[0].py[i];
			}

			if(PATTERN==BLOCK){//block
				if(m==0 || m==1){//sol, m-gas
					//for(i=0;i<npP0;i++){
					for(i=0;i<npP0+nnpP1;i++){
						pBlockX1[i] = pBlockX1[i] * skew;
					}
					point = (npP0+nnpP1)/6;
					for(i=0;i<point;i++){	 
						pBlockY1[i] = pBlockY1[i] * xp2;//1st block
					}
					for(i=point;i<2*point;i++){
						pBlockY1[i] = pBlockY1[i] * xp3;//2nd block
					}
					for(i=2*point;i<3*point;i++){
						pBlockY1[i] = pBlockY1[i] * xp4;//3rd block
					}
					for(i=3*point;i<4*point;i++){
						pBlockY1[i] = pBlockY1[i] * xp5;//4th block
					}
					for(i=4*point;i<5*point;i++){
						pBlockY1[i] = pBlockY1[i] * xp6;//5th block
					}
					for(i=5*point;i<(npP0+nnpP1);i++){
						pBlockY1[i] = pBlockY1[i] * xp7;//6th block
					}
					
				}
				else{ //BFsh is type3
					for(i=0;i<npP0+nnpP1;i++){//skew and amplify
						pBlockX1[i] = pBlockX1[i] * skew;
					}
					point = (npP0+nnpP1)/4;
					for(i=0;i<point;i++){	 
						pBlockY1[i] = pBlockY1[i] * xp2;//1st block
					}
					for(i=point;i<2*point;i++){
						pBlockY1[i] = pBlockY1[i] * xp3;//2nd block
					}
					for(i=2*point;i<3*point;i++){
						pBlockY1[i] = pBlockY1[i] * xp4;//3rd block
					}
					for(i=3*point;i<npP0+nnpP1;i++){
						pBlockY1[i] = pBlockY1[i] * xp5;//4th block
					}
				}
				//else{
				//	for(i=0;i<npP0+nnpP1;i++){//skew and amplify
				//		pBlockX1[i] = pBlockX1[i] * skew;
				//		pBlockY1[i] = pBlockY1[i] * xp2;
				//	}
				//}
			}
			else{//emg
				if(m==bfshG || m==psoasG || m==gmedG || m==perlngG || m==flxdigG || m==extdigG){
					for(i=0;i<npP0+nnpP1;i++){
						pBlockX1[i] = pBlockX1[i] * skew;
					}

					point = (npP0+nnpP1)/4;
					for(i=0;i<point;i++){	 
						pBlockY1[i] = pBlockY1[i] * xp2;//1st block
					}
					for(i=point;i<2*point;i++){
						pBlockY1[i] = pBlockY1[i] * xp3;//2nd block
					}
					for(i=2*point;i<3*point;i++){
						pBlockY1[i] = pBlockY1[i] * xp4;//3rd block
					}
					for(i=3*point;i<npP0+nnpP1;i++){
						pBlockY1[i] = pBlockY1[i] * xp5;//4th block
					}
					
				}
				else{
					for(i=0;i<(npP0+nnpP1);i++){//skew and amplify				
						pBlockX1[i] = pBlockX1[i] * skew;
						pBlockY1[i] = pBlockY1[i] * xp2;

					}
				}
				
			}
			t_end = pBlockX1[npP0+nnpP1-1]; 

			////////make one continuous curve///////
			for(i=0;i<npP0+nnpP1;i++){
				pX[i] = pBlockX1[i];
				pY[i] = pBlockY1[i];
			}

			for(i=0;i<nnpZ0;i++){
				pX[i+npP0+nnpP1] = t_end + (double)(i+1)*(100.0-t_end)/(double)(nnpZ0);
				pY[i+npP0+nnpP1] = 0.0;
			}
			////////////////////////////////////////

			//shifting 
			shift = xp0;
			if(shift!=0.0){
				//curveshift(pX,pY,XR,YR,shift,m);
				curveshift(pX,pY,XRtemp,YRtemp,shift,m);

//for(i=0;i<N_emg_step;i++){
//	printf("m=%d: (%d)%f\n",m,i,YRtemp[m][i]);
//}

			}
			else{
				for(i=0;i<N_emg_step;i++){
					//XR[m][i] = pX[i];
					//YR[m][i] = pY[i];
					XRtemp[m][i] = pX[i];
					YRtemp[m][i] = pY[i];
				}
			}
		}
		////////////////////////end of type 2/////////////////////////////

		//////////////////////////////////////////////////////////////////
		if(mdata.ExcP[m].type == 3){//type3
//printf("type3:%d\n",m);
			npP0 = mdata.ExcP[m].pBlock[0].nPoint;
			npP1 = mdata.ExcP[m].pBlock[1].nPoint;
			npP2 = mdata.ExcP[m].pBlock[2].nPoint;
			npZ0 = mdata.ExcP[m].zBlock[0].nPoint;
			npZ1 = mdata.ExcP[m].zBlock[1].nPoint;
			nnpP2 = npP2 - 1;
			nnpZ0 = npZ0 + 1;
			tp1 = (double)mdata.ExcP[m].pBlock[1].t1; //center lobe onset
			tp2 = (double)mdata.ExcP[m].pBlock[2].t1; //side lobe onset
			ntp2 = tp2 - tp1;
			xp0 = mdata.ExcP[m].xp[0]; //offset (1st side lobe)
			xp1 = mdata.ExcP[m].xp[1]; //onset (center lobe)
			xp2 = mdata.ExcP[m].xp[2]; //offset (center lobe)
			xp3 = mdata.ExcP[m].xp[3]; //onset (2nd side lobe)
			xp4 = mdata.ExcP[m].xp[4]; //amplitude (side lobe)
			xp5 = mdata.ExcP[m].xp[5]; //amplitude (center lobe)
			///////create (clear)////////
			for(i=0;i<npP1;i++){
				pBlockX1[i] = 0.0; 
				pBlockY1[i] = 0.0;
			}
			for(i=0;i<npP0+nnpP2;i++){
				pBlockX2[i] = 0.0; 
				pBlockY2[i] = 0.0;
			}
			/////////////////////////////
			
			//compute "skew" for center lobe
			skew1=(xp2-xp1)/((double)(mdata.ExcP[m].pBlock[1].t2
				                   - mdata.ExcP[m].pBlock[1].t1));
			//compute "skew" for side lobe
			skew2=(100.0-xp3+xp0)/((100.0 - (double)mdata.ExcP[m].pBlock[2].t1 
				                   + (double)mdata.ExcP[m].pBlock[0].t2));
		
			for(i=0;i<npP1;i++){//shift (center)
				pBlockX1[i] = mdata.ExcP[m].pBlock[1].px[i] - tp1;
				pBlockY1[i] = mdata.ExcP[m].pBlock[1].py[i];
			}
			for(i=0;i<nnpP2;i++){//shift by the same amount (2nd side burst)
				pBlockX2[i] = mdata.ExcP[m].pBlock[2].px[i] - tp1;
				pBlockY2[i] = mdata.ExcP[m].pBlock[2].py[i];

			}
			for(i=0;i<npP0;i++){//shift (1st side burst)
				pBlockX2[i+nnpP2] = 100.0 - tp1 + (double)i;
				pBlockY2[i+nnpP2] = mdata.ExcP[m].pBlock[0].py[i];
			}

			for(i=0;i<npP1;i++){//skew and amplify for center lobe (now 1st lobe)
				pBlockX1[i] = pBlockX1[i] * skew1;
				pBlockY1[i] = pBlockY1[i] * xp5;
			}
			t_end = pBlockX1[npP1-1];

			for(i=0;i<npP0+nnpP2;i++){//skew, shift and amplify for side lobe (now 2nd lobe)
				pBlockX2[i] = (pBlockX2[i] - ntp2)*skew2 + (xp3-xp1);//ntp2 + (xp3-tp2);
				pBlockY2[i] = pBlockY2[i]*xp4;
			}
			t_begin = pBlockX2[0];
			t_last = pBlockX2[npP0+nnpP2-1];
			////////make one continuous curve///////
			for(i=0;i<npP1;i++){
				pX[i] = pBlockX1[i];
				pY[i] = pBlockY1[i];
			}

			for(i=0;i<npZ1;i++){
				pX[i+npP1] = t_end + (double)(i+1)*(t_begin-t_end)/(double)(npZ1+1);
				pY[i+npP1] = 0.0;
			}

			for(i=0;i<npP0+nnpP2;i++){
				pX[i+npP1+npZ1] = pBlockX2[i];
				pY[i+npP1+npZ1] = pBlockY2[i];
			}

			for(i=0;i<nnpZ0;i++){
				pX[i+npP1+npZ1+npP0+nnpP2] = t_last + 
									(double)(i+1)*(100.0-t_last)/(double)nnpZ0;
				pY[i+npP1+npZ1+npP0+nnpP2] = 0.0;
			}
			////////////////////////////////////////

			//shifting 
			shift = xp1;
			if(shift!=0.0){
				//curveshift(pX,pY,XR,YR,shift,m);
				curveshift(pX,pY,XRtemp,YRtemp,shift,m);
//for(i=0;i<N_emg_step;i++){
//	printf("m=%d: %f\n",m,YRtemp[m][i]);
//}
			}
			else{
				for(i=0;i<N_emg_step;i++){
					//XR[m][i] = pX[i];
					//YR[m][i] = pY[i];
					XRtemp[m][i] = pX[i];
					YRtemp[m][i] = pY[i];

				}
			}
		}
		///////////////////////end of type 3////////////////////////

		//Updated for Gauss
		//////////////////////////////Start of Type 4 (Gauss Type 1) - Jeff R /////////////////////////////////////////
		if(mdata.ExcP[m].type == 4){//type4

			//Obtain the current magnitudes for each of the variables in the excitation pattern
			xp0 = mdata.ExcP[m].xp[0];//start time
			xp1 = mdata.ExcP[m].xp[1];//end time
			xp2 = mdata.ExcP[m].xp[2];//magnitude
			xp3 = mdata.ExcP[m].xp[3];//shape factor
			
			//Obtain other values needed for calculations
			//npP0 = mdata.ExcP[m].pBlock[0].nPoint;//number of points in block (=101)
			//tp0 = (double)mdata.ExcP[m].pBlock[0].t1;//starting point in block (=0)

					
			//Shift the curve so that it is centered around a different value.
			shift = xp0;
			if(shift != 0.0)
			{
				curveshift(pX,pY,XR,YR,shift,m);
			}
			else
			{
				for(i=0;i<N_emg_step;i++){
					XR[m][i] = pX[i];
					YR[m][i] = pY[i];
				}
			}
		}
		//////////////////////////////End of Type 4 (Gauss Type 1) - Jeff R/////////////////////////////////////////////
	} //end of for-loop (m)
	


	for(m=0;m<N_musG;m++){
		/////////////make left leg/////////////
		//comp_left(XR,YR,XL,YL,m);
		comp_left(XRtemp,YRtemp,XLtemp,YLtemp,m);
		///////////////////////////////////////

		if(m==solG){
			for(i=0;i<N_emg_step;i++){
				XR[sol][i]=XRtemp[m][i];	YR[sol][i]=YRtemp[m][i]; 
				XL[sol][i]=XLtemp[m][i];	YL[sol][i]=YLtemp[m][i];
				XR[tp][i]=XRtemp[m][i];		YR[tp][i]=YRtemp[m][i]; //Tibpost
				XL[tp][i]=XLtemp[m][i];		YL[tp][i]=YLtemp[m][i];
			}
		}

		if(m==gasG){
			for(i=0;i<N_emg_step;i++){
				XR[mgas][i]=XRtemp[m][i]; YR[mgas][i]=YRtemp[m][i]; 
				XL[mgas][i]=XLtemp[m][i]; YL[mgas][i]=YLtemp[m][i];
				XR[lgas][i]=XRtemp[m][i]; YR[lgas][i]=YRtemp[m][i]; 
				XL[lgas][i]=XLtemp[m][i]; YL[lgas][i]=YLtemp[m][i];
			}
		}

		if(m==smG){
			for(i=0;i<N_emg_step;i++){
				XR[sm][i]=XRtemp[m][i];		YR[sm][i]=YRtemp[m][i]; 
				XL[sm][i]=XLtemp[m][i];		YL[sm][i]=YLtemp[m][i];
				XR[bflh][i]=XRtemp[m][i];	YR[bflh][i]=YRtemp[m][i]; 
				XL[bflh][i]=XLtemp[m][i];	YL[bflh][i]=YLtemp[m][i];
			}
		}

		if(m==bfshG){
			for(i=0;i<N_emg_step;i++){
				XR[bfsh][i]=XRtemp[m][i];		YR[bfsh][i]=YRtemp[m][i]; 
				XL[bfsh][i]=XLtemp[m][i];		YL[bfsh][i]=YLtemp[m][i];
			}
		}

		if(m==psoasG){
			for(i=0;i<N_emg_step;i++){
				XR[psoas][i]=XRtemp[m][i]; YR[psoas][i]=YRtemp[m][i]; 
				XL[psoas][i]=XLtemp[m][i]; YL[psoas][i]=YLtemp[m][i];
				XR[iliac][i]=XRtemp[m][i]; YR[iliac][i]=YRtemp[m][i]; 
				XL[iliac][i]=XLtemp[m][i]; YL[iliac][i]=YLtemp[m][i];
			}
		}

		if(m==taG){//ta
			for(i=0;i<N_emg_step;i++){
				XR[ta][i]=XRtemp[m][i];			YR[ta][i]=YRtemp[m][i];
				XL[ta][i]=XLtemp[m][i];			YL[ta][i]=YLtemp[m][i];
				XR[pertert][i]=XRtemp[m][i];	YR[pertert][i]=YRtemp[m][i]; 
				XL[pertert][i]=XLtemp[m][i];	YL[pertert][i]=YLtemp[m][i];
				XR[extdig][i]=XRtemp[m][i];		YR[extdig][i]=YRtemp[m][i]; 
				XL[extdig][i]=XLtemp[m][i];		YL[extdig][i]=YLtemp[m][i];
				XR[exthal][i]=XRtemp[m][i];		YR[exthal][i]=YRtemp[m][i]; 
				XL[exthal][i]=XLtemp[m][i];		YL[exthal][i]=YLtemp[m][i];
			}
		}

		if(m==rfG){
			for(i=0;i<N_emg_step;i++){
				XR[rf][i]=XRtemp[m][i];		YR[rf][i]=YRtemp[m][i]; 
				XL[rf][i]=XLtemp[m][i];		YL[rf][i]=YLtemp[m][i];
			}
		}

		if(m==vas1G){//vas
			for(i=0;i<N_emg_step;i++){
				XR[vas1][i]=XRtemp[m][i]; YR[vas1][i]=YRtemp[m][i]; 
				XL[vas1][i]=XLtemp[m][i]; YL[vas1][i]=YLtemp[m][i];
			}
		}

		if(m==vas2G){//vas
			for(i=0;i<N_emg_step;i++){
				XR[vas2][i]=XRtemp[m][i]; YR[vas2][i]=YRtemp[m][i]; 
				XL[vas2][i]=XLtemp[m][i]; YL[vas2][i]=YLtemp[m][i];
			}
		}

		if(m==vas3G){//vas
			for(i=0;i<N_emg_step;i++){
				XR[vas3][i]=XRtemp[m][i]; YR[vas3][i]=YRtemp[m][i]; 
				XL[vas3][i]=XLtemp[m][i]; YL[vas3][i]=YLtemp[m][i];
			}
		}

		if(m==gmaxG){//gmax
			for(i=0;i<N_emg_step;i++){
				XR[gmax][i]=XRtemp[m][i];	YR[gmax][i]=YRtemp[m][i]; 
				XL[gmax][i]=XLtemp[m][i];	YL[gmax][i]=YLtemp[m][i];
				XR[addmag][i]=XRtemp[m][i]; YR[addmag][i]=YRtemp[m][i]; 
				XL[addmag][i]=XLtemp[m][i]; YL[addmag][i]=YLtemp[m][i];
			}
		}

		if(m==gmedG){//gmed
			for(i=0;i<N_emg_step;i++){
				XR[gmeda][i]=XRtemp[m][i]; YR[gmeda][i]=YRtemp[m][i]; 
				XL[gmeda][i]=XLtemp[m][i]; YL[gmeda][i]=YLtemp[m][i];
				XR[gmedp][i]=XRtemp[m][i]; YR[gmedp][i]=YRtemp[m][i]; 
				XL[gmedp][i]=XLtemp[m][i]; YL[gmedp][i]=YLtemp[m][i];
			}
		}

		if(m==perlngG){//Perolong
			for(i=0;i<N_emg_step;i++){
				XR[perlng][i]=XRtemp[m][i];		YR[perlng][i]=YRtemp[m][i]; 
				XL[perlng][i]=XLtemp[m][i];		YL[perlng][i]=YLtemp[m][i];
				XR[perbrev][i]=XRtemp[m][i];	YR[perbrev][i]=YRtemp[m][i]; 
				XL[perbrev][i]=XLtemp[m][i];	YL[perbrev][i]=YLtemp[m][i];
			}
		}

		if(m==flxdigG){
			for(i=0;i<N_emg_step;i++){
				XR[flxdig][i]=XRtemp[m][i]; YR[flxdig][i]=YRtemp[m][i]; 
				XL[flxdig][i]=XLtemp[m][i]; YL[flxdig][i]=YLtemp[m][i];
				XR[flxhal][i]=XRtemp[m][i]; YR[flxhal][i]=YRtemp[m][i]; 
				XL[flxhal][i]=XLtemp[m][i]; YL[flxhal][i]=YLtemp[m][i];
			}
		}


		if(m==extdigG){
			for(i=0;i<N_emg_step;i++){
				XR[extdig][i]=XRtemp[m][i]; YR[extdig][i]=YRtemp[m][i]; 
				XL[extdig][i]=XLtemp[m][i]; YL[extdig][i]=YLtemp[m][i];
				XR[exthal][i]=XRtemp[m][i]; YR[exthal][i]=YRtemp[m][i]; 
				XL[exthal][i]=XLtemp[m][i]; YL[exthal][i]=YLtemp[m][i];
			}
		}

		
	
	}//end of loop for m

	free(pBlockX1);
	free(pBlockX2);
	free(pBlockY1);
	free(pBlockY2);
	free(pX);
	free(pY);

	for(i=0;i<N_musG;i++){
  	   free(XRtemp[i]);
    }
	free(XRtemp);

	for(i=0;i<N_musG;i++){
  	   free(XLtemp[i]);
    }
	free(XLtemp);

	for(i=0;i<N_musG;i++){
  	   free(YRtemp[i]);
    }
	free(YRtemp);

	for(i=0;i<N_musG;i++){
  	   free(YLtemp[i]);
    }
	free(YLtemp);


}
//////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
void comp_left(double **bezXR,double **bezYR,
			   double **bezXL,double **bezYL,
			   int m)
{
	double **bezXLdummy;
	int i, j, memo;
	double index, diff1,diff2;

	bezXLdummy=(double**)calloc(N_mus,sizeof(double*));
	if(bezXLdummy == NULL) exit(0);
	for(i=0;i<N_mus;i++){
		bezXLdummy[i]=(double*)calloc(N_emg_step,sizeof(double));
		if(bezXLdummy[i] == NULL) exit(0);
	}


	memo=0;
	index=0;

	for(i=0;i<N_emg_step;i++){
		bezXLdummy[m][i] = bezXR[m][i] + 50.0;

		if(bezXLdummy[m][i] >= 100.0){
			if(index==0){
				diff1=bezXLdummy[m][i] - 100.0;
				diff2=100.0 - bezXLdummy[m][i-1];
				memo=i-1;
				index = 1;
			}
		
		}
	}

	j=0;
	i=N_emg_step-1;
	while(j<=memo){
		bezXL[m][i] = bezXLdummy[m][memo-j]+diff2;
		bezYL[m][i] = bezYR[m][memo-j];
		j++;
		i--;
	}

	j=0;
	i=0;
	while(j<N_emg_step-1 - memo){
		bezXL[m][i] = bezXR[m][memo+1+i]-50.0 -diff1; 
		bezYL[m][i] = bezYR[m][memo+1+i];

		j++;
		i++;
	}


	bezXL[m][STARTCYCLE]=0.0;     //to make sure (I think these are not needed, though}
	bezXL[m][ENDCYCLE]=100.0;

	for(i=0;i<N_mus;i++){
  	   free(bezXLdummy[i]);
    }
	free(bezXLdummy);


}

///////////////////////////////////////////////////////////////
//Change initial velocities (to be optimized parameters)
void change_init(double *x, int N_params)
{
	//last nine of x are the initial velocities

	mdata.legTx_v = x[N_params-13];
    mdata.legTy_v = x[N_params-12];
    mdata.pelvis_w = x[N_params-11];
	mdata.hipR_w = x[N_params-10];
    mdata.kneeR_w = x[N_params-9];
    mdata.ankleR_w = x[N_params-8];
	mdata.midR_w = x[N_params-7];
	mdata.mtpR_w = x[N_params-6];
    mdata.hipL_w = x[N_params-5];
    mdata.kneeL_w = x[N_params-4];
    mdata.ankleL_w = x[N_params-3];
	mdata.midL_w = x[N_params-2];
	mdata.mtpL_w = x[N_params-1];
    
}
///////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
void curveshift(double pX[N_emg_step], double pY[N_emg_step],
				double **XR,double **YR,
				double shift, int m)
{
	int i,pass_point,flag,overpoints;
	double *shiftX ;
	double *shiftY;
	double *overX;
	double *overY;
	double difference1, difference2;

	shiftX=(double*)calloc(N_emg_step,sizeof(double)); if(shiftX == NULL) exit(0);
	shiftY=(double*)calloc(N_emg_step,sizeof(double)); if(shiftY == NULL) exit(0);
	overX=(double*)calloc(N_emg_step,sizeof(double)); if(overX == NULL) exit(0);
	overY=(double*)calloc(N_emg_step,sizeof(double)); if(overY == NULL) exit(0);
	

	////////////////////////////////////////////////////////	
	if(shift>0.0){

		if((pX[0] + shift)>=100.0){
			for(i=0;i<N_emg_step;i++){
				pX[i] = pX[i] - 100.0;
			}
		}

		flag = 0;
		for(i=0;i<N_emg_step;i++){
			shiftX[i] = pX[i] + shift;
			shiftY[i] = pY[i];
			if(shiftX[i] > 100.0){
				if(flag==0){
					pass_point = i;
					difference1 = shiftX[pass_point] - 100.0;
					difference2 = 100.0 - shiftX[pass_point-1];					
					overX[0] = shiftX[pass_point];//future start point
					overY[0] = shiftY[pass_point];
					overpoints = 1;
					flag=1;
				}
				else{
					overX[i-pass_point] = shiftX[i] - 100.0;
					overY[i-pass_point] = shiftY[i];
					overpoints = overpoints + 1;
				}
			}
		}
	
		for(i=0;i<overpoints;i++){
			XR[m][i] = overX[i] - difference1;
			YR[m][i] = overY[i];
		}
		for(i=overpoints;i<N_emg_step;i++){
			XR[m][i] = shiftX[i-overpoints] + difference2;
			YR[m][i] = shiftY[i-overpoints];
		}
	}
	/////////////////////////////////////////////////////////////////////
	else{//shift<0.0

		if((pX[0] + shift) <= -100.0){
			for(i=0;i<N_emg_step;i++){
				pX[i] = pX[i] + 100.0;
			}
		}
		flag = 0;
		for(i=0;i<N_emg_step;i++){
			shiftX[i] = pX[i] + shift;
			shiftY[i] = pY[i];

			if(flag==0){

				if(shiftX[i] >= 0.0){
					pass_point = i;
					difference1 = shiftX[pass_point] - 0.0;
					difference2 = 0.0 - shiftX[pass_point-1]; //negative value					
					flag=1;
				}
				else{ //shiftX[i]<0.0
					overX[i] = shiftX[i];
					overY[i] = shiftY[i];
				}
			}
		}
		for(i=0;i<N_emg_step-pass_point;i++){
			XR[m][i] = shiftX[i+pass_point] - difference1;
			YR[m][i] = shiftY[i+pass_point];
		}
		for(i=0;i<pass_point;i++){
			XR[m][i+N_emg_step-pass_point] = overX[i] + difference2 + 100.0;
			YR[m][i+N_emg_step-pass_point] = overY[i];
		}
	}//end of else

	//check the first and last point
	XR[m][0] = 0.0;
	XR[m][N_emg_step-1] = 100.0;
	YR[m][N_emg_step-1] = YR[m][0];

	

	free(shiftX);
	free(shiftY);
	free(overX);
	free(overY);
}




