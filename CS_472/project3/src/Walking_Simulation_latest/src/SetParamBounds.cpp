#include <iostream>
#include "ExcGroups.h"
#include "SAstructs.h"

void set_opt_param_bounds(double *LB, double *UB){

/*	if (mdata.ExcPatterns[i].pattern_type == 3){
	//SOLG
	LB[0]=70.0; UB[0]=100.0;  //onset
	LB[1]=25.0; UB[1]=70.0;  //offset
	LB[2]=0.1;  UB[2]=1.0; //amplitude
	LB[3]=2.0;  UB[3]=20.0; //shape factor
	
	//GASG 
	LB[4]=90.0; UB[4]=100.0;  //onset
	LB[5]=40.0; UB[5]=80.0;  //offset
	LB[6]=0.1;  UB[6]=1.0; //amplitude
	LB[7]=2.0;  UB[7]=20.0; //shape factor

	//HAMG
	LB[8]=70.0; UB[8]=95.0;    //onset
	LB[9]=30.0; UB[9]=60.0;  //offset
	LB[10]=0.1;  UB[10]=1.0; //amplitude
	LB[11]=2.0;  UB[11]=20.0; //shape factor

	//BFSHG
	LB[12]=60.0; UB[12]=100.0;   //onset
	LB[13]=0.0;  UB[13]=30.0;  //offset
	LB[14]=0.1;  UB[14]=1.0; //amplitude
	LB[15]=2.0;  UB[15]=20.0; //shape factor

	//ILG
	LB[16]=35.0; UB[16]=60.0;   //onset
	LB[17]=60.0; UB[17]=90.0;  //offset
	LB[18]=0.1;  UB[18]=1.0; //amplitude
	LB[19]=2.0;  UB[19]=20.0; //shape factor

	//TAG
	LB[20]=40.0; UB[20]=70.0;   //onset
	LB[21]=0.0;  UB[21]=40.0;  //offset
	LB[22]=0.1;  UB[22]=1.0; //amplitude	
	LB[23]=2.0;  UB[23]=20.0; //shape factor

	//RFG
	LB[24]=80.0; UB[24]=100.0;   //onset
	LB[25]=30.0; UB[25]=50.0;  //offset
	LB[26]=0.1;  UB[26]=1.0; //amplitude
	LB[27]=2.0;  UB[27]=20.0; //shape factor

	//VMG
	LB[28]=80.0; UB[28]=100.0;   //onset
	LB[29]=30.0; UB[39]=50.0;  //offset
	LB[30]=0.1; UB[30]=1.0; //amplitude
	LB[31]=2.0; UB[31]=20.0; //shape factor

	//VLG
	LB[32]=80.0; UB[32]=100.0;   //onset
	LB[33]=30.0; UB[33]=50.0;  //offset
	LB[34]=0.1;  UB[34]=1.0; //amplitude
	LB[35]=2.0;  UB[35]=20.0; //shape factor

	//GMAXG
	LB[36]=70.0; UB[36]=100.0;   //onset
	LB[37]=5.0;  UB[37]=40.0;  //offset
	LB[38]=0.1;  UB[38]=1.0; //amplitude
	LB[39]=2.0;  UB[39]=20.0; //shape factor

	//GMEDG
	LB[40]=75.0; UB[40]=100.0;   //onset
	LB[41]=30.0; UB[41]=60.0;  //offset
	LB[42]=0.1;  UB[42]=1.0; //amplitude
	LB[43]=2.0;  UB[43]=20.0; //shape factor

	//PERLNGG
	LB[44]=70.0; UB[44]=100.0;  //onset
	LB[45]=25.0; UB[45]=70.0;  //offset
	LB[46]=0.1;  UB[46]=1.0; //amplitude
	LB[47]=2.0;  UB[47]=20.0; //shape factor
	
	//FLGDIGG
	LB[48]=70.0; UB[48]=100.0;  //onset
	LB[49]=25.0; UB[49]=70.0;  //offset
	LB[50]=0.1;  UB[50]=1.0; //amplitude
	LB[51]=2.0;  UB[51]=20.0; //shape factor

	//EXTDIGG
	LB[52]=40.0; UB[52]=70.0;   //onset
	LB[53]=0.0;  UB[53]=40.0;  //offset
	LB[54]=0.1;  UB[54]=1.0; //amplitude	
	LB[55]=2.0;  UB[55]=20.0; //shape factor


	//ALG_R
//	LB[4]=20.0; UB[4]=40.0;   //onset
//	LB[5]=40.0; UB[5]=70.0;  //offset
//	LB[6]=0.1; UB[6]=1.0; //amplitude
//	LB[7]=2.0; UB[7]=20.0; //shape factor

	//AMG_R
//	LB[8]=-30.0; UB[8]=20.0;   //onset
//	LB[9]=20.0; UB[9]=70.0;  //offset
//	LB[10]=0.1; UB[10]=1.0; //amplitude
//	LB[11]=2.0; UB[11]=20.0; //shape factor
	
	//SARG_R
//	LB[12]=15.0; UB[12]=40.0;   //onset
//	LB[13]=55.0; UB[13]=85.0;  //offset
//	LB[14]=0.1; UB[14]=1.0; //amplitude.
//	LB[15]=2.0; UB[15]=20.0; //shape factor

/*	//RFG
	LB[16]=20.0; UB[16]=45.0;   //offset (1st side burst)
	LB[17]=50.0; UB[17]=75.0;   //onset (center burst)
	LB[18]=75.0; UB[18]=90.0;  //offset (center burst)
	LB[19]=90.0; UB[19]=100.0; //onset (2nd side burst)
	LB[20]=0.1; UB[20]=1.0;  //amplitude (side burst)
	LB[21]=0.1; UB[21]=1.0; //amplitude (center burst)

  //TFLG_R
	LB[31]=-20.0; UB[31]=30.0;   //onset
	LB[32]=30.0; UB[32]=80.0;  //offset
	LB[33]=0.1; UB[33]=1.0; //amplitude
	LB[34]=2.0; UB[34]=20.0; //shape factor

  //BFLHG_R
	LB[42]=70.0; UB[42]=100.0;    //onset
	LB[43]=35.0; UB[43]=70.0;  //offset	
	LB[44]=0.1; UB[44]=1.0; //amplitude

  //LGG_R (Type 2 for IMH26)
	LB[52]=80.0; UB[52]=100.0;  //onset
	LB[53]=40.0; UB[53]=80.0;  //offset
	LB[54]=0.1; UB[54]=1.0; //amplitude
*/



//	if (SYMMETRIC == 0){//set left leg parameters

/*		//SHOULD BE UPDATED ACCORDING TO THE DESIRES OF THE USER JEFF R
		//ILG_L
		LB[61]=85.0; UB[61]=100.0;   //onset
		LB[62]=10.0; UB[62]=40.0;  //offset
		LB[63]=0.1; UB[63]=1.0; //amplitude
		LB[64]=2.0; UB[64]=20.0; //shape factor

		//ALG_L
		LB[65]=70.0; UB[65]=90.0;   //onset
		LB[66]=-10.0; UB[66]=20.0;  //offset
		LB[67]=0.1; UB[67]=1.0; //amplitude
		LB[68]=2.0; UB[68]=20.0; //shape factor

		//AMG_L
		LB[69]=50.0; UB[69]=100.0;   //onset
		LB[70]=0.0; UB[70]=50.0;  //offset
		LB[71]=0.1; UB[71]=1.0; //amplitude
		LB[72]=2.0; UB[72]=20.0; //shape factor
		
		//SARG_L
		LB[73]=65.0; UB[73]=90.0;   //onset
		LB[74]=5.0; UB[74]=25.0;  //offset
		LB[75]=0.1; UB[75]=1.0; //amplitude
		LB[76]=2.0; UB[76]=20.0; //shape factor

		//RFG_L
		LB[77]=0.0; UB[77]=40.0;   //offset (1st side burst)
		LB[78]=40.0; UB[78]=55.0;   //onset (center burst)
		LB[79]=55.0; UB[79]=85.0;  //offset (center burst)
		LB[80]=85.0; UB[80]=100.0; //onset (2nd side burst)
		LB[81]=0.1; UB[81]=1.0;  //amplitude (side burst)
		LB[82]=0.1; UB[82]=1.0; //amplitude (center burst)

		//VMG_L
		LB[83]=10.0; UB[83]=40.0;   //onset
		LB[84]=70.0; UB[84]=100.0;  //offset
		LB[85]=0.1; UB[85]=1.0; //amplitude

		//VLG_L
		LB[86]=15.0; UB[86]=40.0;   //onset
		LB[87]=60.0; UB[87]=100.0;  //offset
		LB[88]=0.1; UB[88]=1.0; //amplitude

		//GMEDG_L (Type 2 for IMH26)
		LB[89]=25.0; UB[89]=55.0;   //onset
		LB[90]=-15.0; UB[90]=20.0;  //offset
		LB[91]=0.1; UB[91]=1.0; //amplitude

		//TFLG_L
		LB[92]=50.0; UB[92]=100.0;   //onset
		LB[93]=0.0; UB[93]=50.0;  //offset
		LB[94]=0.1; UB[94]=1.0; //amplitude
		LB[95]=2.0; UB[95]=20.0; //shape factor

		//GMAXG_L
		LB[96]=20.0; UB[96]=50.0;   //onset
		LB[97]=50.0; UB[97]=90.0;  //offset
		LB[98]=0.1; UB[98]=1.0; //amplitude
		LB[99]=2.0; UB[99]=20.0; //shape factor

		//MHG_L
		LB[100]=20.0; UB[100]=45.0;    //onset
		LB[101]=75.0; UB[101]=100.0;  //offset
		LB[102]=0.1; UB[102]=1.0; //amplitude

		//BFLHG_L (Type 2 for IMH26)
		LB[103]=25.0; UB[103]=55.0;    //onset
		LB[104]=-10.0; UB[104]=25.0;  //offset	
		LB[105]=0.1; UB[105]=1.0; //amplitude

		//BFSHG_L
		LB[106]=20.0; UB[106]=50.0;   //onset
		LB[107]=50.0; UB[107]=90.0;  //offset
		LB[108]=0.1; UB[108]=1.0; //amplitude
		LB[109]=2.0; UB[109]=20.0; //shape factor

		//MGG_L
		LB[110]=25.0; UB[110]=60.0;  //onset
		LB[111]=-10.0; UB[111]=25.0;  //offset
		LB[112]=0.1; UB[112]=1.0; //amplitude

		//LGG_L
		LB[113]=25.0; UB[113]=60.0;  //onset
		LB[114]=-10.0; UB[114]=25.0;  //offset
		LB[115]=0.1; UB[115]=1.0; //amplitude

		//SOLG_L
		LB[116]=35.0; UB[116]=70.0;  //onset
		LB[117]=0.0; UB[117]=35.0;  //offset
		LB[118]=0.1; UB[118]=1.0; //amplitude
		
		//TAG_L
		LB[119]=0.0; UB[119]=30.0;   //onset
		LB[120]=50.0; UB[120]=75.0;  //offset
		LB[121]=0.1; UB[121]=1.0; //amplitude	
	}//end if SYMMETRIC == 0
*/	
/*
/////INITIAL CONDITIONS//////
		//leg_tx 
        LB[N_EXC_PARAMS+0] = 1.0; UB[N_EXC_PARAMS+0] = 1.5;

		//leg_ty
        LB[N_EXC_PARAMS+1] = -0.5; UB[N_EXC_PARAMS+1] =  0.0;

		//pelvis_rotation
        LB[N_EXC_PARAMS+2] = -1.0; UB[N_EXC_PARAMS+2] =  1.0;

		//hip_angle_r  
        LB[N_EXC_PARAMS+3] = -3.0; UB[N_EXC_PARAMS+3] =  3.0; 

		//knee_angle_r
        LB[N_EXC_PARAMS+4] = -5.0; UB[N_EXC_PARAMS+4] =  5.0;

		//ankle_angle_r  
        LB[N_EXC_PARAMS+5] = -5.0; UB[N_EXC_PARAMS+5] =  5.0;

		//subtalar_angle_r  
        LB[N_EXC_PARAMS+6] = -5.0; UB[N_EXC_PARAMS+6] =  5.0;

		//mtp_angle_r  
        LB[N_EXC_PARAMS+7] = -5.0; UB[N_EXC_PARAMS+7] =  5.0;

		//hip_angle_l
        LB[N_EXC_PARAMS+8] = -3.0; UB[N_EXC_PARAMS+8] =  3.0;

		//knee_angle_l  
        LB[N_EXC_PARAMS+9] = -2.0; UB[N_EXC_PARAMS+9] = 5.0; 

		//ankle_angle_l
        LB[N_EXC_PARAMS+10] = -1.0; UB[N_EXC_PARAMS+10] = 6.0;

		//subtalar_angle_l  
        LB[N_EXC_PARAMS+11] = -5.0; UB[N_EXC_PARAMS+11] =  5.0;

		//mtp_angle_l  
        LB[N_EXC_PARAMS+12] = -5.0; UB[N_EXC_PARAMS+12] =  5.0;
	} //end source 3 loop
*/
//	else if (mdata.ExcPatterns[i].pattern_type == 4){
	//SOLG  1
	LB[0]=80.0;   UB[0]=5.0;  //onset
	LB[1]=15.0;   UB[1]=65.0;  //offset
	LB[2]=0.01;  UB[2]=1.0; //amplitude
	LB[3]=0.0;   UB[3]=35.0;  //onset
	LB[4]=36.0;   UB[4]=75.0;  //offset
	LB[5]=0.01;  UB[5]=1.0; //amplitude
	
	//GASG  2
	LB[6]=80.0;  UB[6]=5.0;  //onset
	LB[7]=15.0;  UB[7]=65.0;  //offset
	LB[8]=0.01;  UB[8]=1.0; //amplitude
	LB[9]=0.0;  UB[9]=35.0;  //onset
	LB[10]= 36.0; UB[10]=65.0;  //offset
	LB[11]=0.01; UB[11]=1.0; //amplitude

	//HAMG   3
	LB[12]=65.0;   UB[12]=95.0;    //onset
	LB[13]=5.0;   UB[13]=30.0;  //offset
	LB[14]=0.01;  UB[14]=1.0; //amplitude
	LB[15]=50.0;   UB[15]=85.0;    //onset
	LB[16]=86.0;   UB[16]=30.0;  //offset
	LB[17]=0.01;  UB[17]=1.0; //amplitude

	//BFSHG  4
	LB[18]=65.0;   UB[18]=95.0;   //onset
	LB[19]=5.0;   UB[19]=30.0;  //offset
	LB[20]=0.01;  UB[20]=1.0; //amplitude
	LB[21]=35.0;   UB[21]=50.0;   //onset
	LB[22]=51.0;   UB[22]=75.0;  //offset
	LB[23]=0.01;  UB[23]=1.0; //amplitude

	//ILG   5
	LB[24]=35.0;   UB[24]=55.0;   //onset
	LB[25]=56.0;   UB[25]=75.0;  //offset
	LB[26]=0.01;  UB[26]=1.0; //amplitude
	LB[27]=30.0;   UB[27]=45.0;   //onset
	LB[28]=46.0;   UB[286]=75.0;  //offset
	LB[29]=0.01;  UB[29]=0.05; //amplitude

	//TAG   6
	LB[30]=75.0;   UB[30]=95.0;   //onset
	LB[31]=0.0;   UB[31]=25.0;  //offset
	LB[32]=0.01;  UB[32]=0.40; //amplitude	
	LB[33]=40.0;   UB[33]=75.0;   //onset
	LB[34]=76.0;   UB[34]=5.0;  //offset
	LB[35]=0.01;  UB[35]=0.40; //amplitude

	//RFG   7
	LB[36]=70.0;   UB[36]=100.0;   //onset
	LB[37]=5.0;   UB[37]=35.0;  //offset
	LB[38]=0.01;  UB[38]=1.0; //amplitude
	LB[39]=45.0;   UB[39]=55.0;   //onset
	LB[40]=56.0;   UB[40]=80.0;  //offset
	LB[41]=0.01;  UB[41]=1.0; //amplitude

	//VMG   8
	LB[42]=70.0;   UB[42]=100.0;   //onset
	LB[43]=5.0;   UB[43]=45.0;  //offset
	LB[44]=0.01;  UB[44]=1.0; //amplitude
	LB[45]=70.0;   UB[45]=100.0;   //onset
	LB[46]=5.0;   UB[46]=45.0;  //offset
	LB[47]=0.01;  UB[47]=1.0; //amplitude

	//VLG   9
	LB[48]=70.0;   UB[48]=100.0;   //onset
	LB[49]=5.0;  UB[49]=45.0;  //offset
	LB[50]=0.01;  UB[50]=1.0; //amplitude
	LB[51]=70.0;   UB[51]=100.0;   //onset
	LB[52]=5.0;   UB[52]=45.0;  //offset
	LB[53]=0.01;  UB[53]=1.0; //amplitude

	//GMAXG   10
	LB[54]=70.0;   UB[54]=100.0;   //onset
	LB[55]=5.0;   UB[55]=55.0;  //offset
	LB[56]=0.01;  UB[56]=1.0; //amplitude
	LB[57]=45.0;   UB[57]=60.0;   //onset
	LB[58]=61.0;   UB[58]=90.0;  //offset
	LB[59]=0.01;  UB[59]=1.0; //amplitude
 
	//GMEDG   11
	LB[60]=70.0;   UB[60]=100.0;   //onset
	LB[61]=5.0;   UB[61]=55.0;  //offset
	LB[62]=0.01;  UB[62]=1.0; //amplitude
	LB[63]=45.0;   UB[63]=60.0;   //onset
	LB[64]=61.0;   UB[64]=90.0;  //offset
	LB[65]=0.01;  UB[65]=1.0; //amplitude

	//PERLNGG  12
	LB[66]=80.0;   UB[66]=5.0;  //onset
	LB[67]=15.0;   UB[67]=65.0;  //offset
	LB[68]=0.01;  UB[68]=1.0; //amplitude
	LB[69]=0.0;   UB[69]=35.0;  //onset
	LB[70]=36.0;   UB[70]=75.0;  //offset
	LB[71]=0.01;  UB[71]=1.0; //amplitude
	
	//FLGDIGG  13
	LB[72]=75.0;   UB[72]=95.0;  //onset
	LB[73]=0.0;   UB[73]=25.0;  //offset
	LB[74]=0.01;  UB[74]=1.0; //amplitude
	LB[75]=40.0;   UB[75]=75.0;  //onset
	LB[76]=76.0;   UB[76]=5.0;  //offset
	LB[77]=0.01;  UB[77]=1.0; //amplitude

	//EXTDIGG  14
	LB[78]=80.0; UB[78]=100.0;   //onset
	LB[79]=15.0;  UB[79]=65.0;  //offset
	LB[80]=0.1;  UB[80]=1.0; //amplitude	
	LB[81]=0.0; UB[81]=35.0;   //onset
	LB[82]=36.0;  UB[82]=75.0;  //offset
	LB[83]=0.1;  UB[83]=1.0; //amplitude


	//ALG_R
//	LB[4]=20.0; UB[4]=40.0;   //onset
//	LB[5]=40.0; UB[5]=70.0;  //offset
//	LB[6]=0.1; UB[6]=1.0; //amplitude
//	LB[7]=2.0; UB[7]=20.0; //shape factor

	//AMG_R
//	LB[8]=-30.0; UB[8]=20.0;   //onset
//	LB[9]=20.0; UB[9]=70.0;  //offset
//	LB[10]=0.1; UB[10]=1.0; //amplitude
//	LB[11]=2.0; UB[11]=20.0; //shape factor
	
	//SARG_R
//	LB[12]=15.0; UB[12]=40.0;   //onset
//	LB[13]=55.0; UB[13]=85.0;  //offset
//	LB[14]=0.1; UB[14]=1.0; //amplitude.
//	LB[15]=2.0; UB[15]=20.0; //shape factor

/*	//RFG
	LB[16]=20.0; UB[16]=45.0;   //offset (1st side burst)
	LB[17]=50.0; UB[17]=75.0;   //onset (center burst)
	LB[18]=75.0; UB[18]=90.0;  //offset (center burst)
	LB[19]=90.0; UB[19]=100.0; //onset (2nd side burst)
	LB[20]=0.1; UB[20]=1.0;  //amplitude (side burst)
	LB[21]=0.1; UB[21]=1.0; //amplitude (center burst)

  //TFLG_R
	LB[31]=-20.0; UB[31]=30.0;   //onset
	LB[32]=30.0; UB[32]=80.0;  //offset
	LB[33]=0.1; UB[33]=1.0; //amplitude
	LB[34]=2.0; UB[34]=20.0; //shape factor

  //BFLHG_R
	LB[42]=70.0; UB[42]=100.0;    //onset
	LB[43]=35.0; UB[43]=70.0;  //offset	
	LB[44]=0.1; UB[44]=1.0; //amplitude

  //LGG_R (Type 2 for IMH26)
	LB[52]=80.0; UB[52]=100.0;  //onset
	LB[53]=40.0; UB[53]=80.0;  //offset
	LB[54]=0.1; UB[54]=1.0; //amplitude
*/



	if (SYMMETRIC == 0){//set left leg parameters

/*		//SHOULD BE UPDATED ACCORDING TO THE DESIRES OF THE USER JEFF R
		//ILG_L
		LB[61]=85.0; UB[61]=100.0;   //onset
		LB[62]=10.0; UB[62]=40.0;  //offset
		LB[63]=0.1; UB[63]=1.0; //amplitude
		LB[64]=2.0; UB[64]=20.0; //shape factor

		//ALG_L
		LB[65]=70.0; UB[65]=90.0;   //onset
		LB[66]=-10.0; UB[66]=20.0;  //offset
		LB[67]=0.1; UB[67]=1.0; //amplitude
		LB[68]=2.0; UB[68]=20.0; //shape factor

		//AMG_L
		LB[69]=50.0; UB[69]=100.0;   //onset
		LB[70]=0.0; UB[70]=50.0;  //offset
		LB[71]=0.1; UB[71]=1.0; //amplitude
		LB[72]=2.0; UB[72]=20.0; //shape factor
		
		//SARG_L
		LB[73]=65.0; UB[73]=90.0;   //onset
		LB[74]=5.0; UB[74]=25.0;  //offset
		LB[75]=0.1; UB[75]=1.0; //amplitude
		LB[76]=2.0; UB[76]=20.0; //shape factor

		//RFG_L
		LB[77]=0.0; UB[77]=40.0;   //offset (1st side burst)
		LB[78]=40.0; UB[78]=55.0;   //onset (center burst)
		LB[79]=55.0; UB[79]=85.0;  //offset (center burst)
		LB[80]=85.0; UB[80]=100.0; //onset (2nd side burst)
		LB[81]=0.1; UB[81]=1.0;  //amplitude (side burst)
		LB[82]=0.1; UB[82]=1.0; //amplitude (center burst)

		//VMG_L
		LB[83]=10.0; UB[83]=40.0;   //onset
		LB[84]=70.0; UB[84]=100.0;  //offset
		LB[85]=0.1; UB[85]=1.0; //amplitude

		//VLG_L
		LB[86]=15.0; UB[86]=40.0;   //onset
		LB[87]=60.0; UB[87]=100.0;  //offset
		LB[88]=0.1; UB[88]=1.0; //amplitude

		//GMEDG_L (Type 2 for IMH26)
		LB[89]=25.0; UB[89]=55.0;   //onset
		LB[90]=-15.0; UB[90]=20.0;  //offset
		LB[91]=0.1; UB[91]=1.0; //amplitude

		//TFLG_L
		LB[92]=50.0; UB[92]=100.0;   //onset
		LB[93]=0.0; UB[93]=50.0;  //offset
		LB[94]=0.1; UB[94]=1.0; //amplitude
		LB[95]=2.0; UB[95]=20.0; //shape factor

		//GMAXG_L
		LB[96]=20.0; UB[96]=50.0;   //onset
		LB[97]=50.0; UB[97]=90.0;  //offset
		LB[98]=0.1; UB[98]=1.0; //amplitude
		LB[99]=2.0; UB[99]=20.0; //shape factor

		//MHG_L
		LB[100]=20.0; UB[100]=45.0;    //onset
		LB[101]=75.0; UB[101]=100.0;  //offset
		LB[102]=0.1; UB[102]=1.0; //amplitude

		//BFLHG_L (Type 2 for IMH26)
		LB[103]=25.0; UB[103]=55.0;    //onset
		LB[104]=-10.0; UB[104]=25.0;  //offset	
		LB[105]=0.1; UB[105]=1.0; //amplitude

		//BFSHG_L
		LB[106]=20.0; UB[106]=50.0;   //onset
		LB[107]=50.0; UB[107]=90.0;  //offset
		LB[108]=0.1; UB[108]=1.0; //amplitude
		LB[109]=2.0; UB[109]=20.0; //shape factor

		//MGG_L
		LB[110]=25.0; UB[110]=60.0;  //onset
		LB[111]=-10.0; UB[111]=25.0;  //offset
		LB[112]=0.1; UB[112]=1.0; //amplitude

		//LGG_L
		LB[113]=25.0; UB[113]=60.0;  //onset
		LB[114]=-10.0; UB[114]=25.0;  //offset
		LB[115]=0.1; UB[115]=1.0; //amplitude

		//SOLG_L
		LB[116]=35.0; UB[116]=70.0;  //onset
		LB[117]=0.0; UB[117]=35.0;  //offset
		LB[118]=0.1; UB[118]=1.0; //amplitude
		
		//TAG_L
		LB[119]=0.0; UB[119]=30.0;   //onset
		LB[120]=50.0; UB[120]=75.0;  //offset
		LB[121]=0.1; UB[121]=1.0; //amplitude	
*/	}//end if SYMMETRIC == 0
	

/////INITIAL CONDITIONS//////
		//leg_tx 
        LB[N_EXC_PARAMS+0] = 1.0; UB[N_EXC_PARAMS+0] = 1.5;

		//leg_ty
        LB[N_EXC_PARAMS+1] = -0.5; UB[N_EXC_PARAMS+1] =  0.0;

		//pelvis_rotation
        LB[N_EXC_PARAMS+2] = -1.0; UB[N_EXC_PARAMS+2] =  1.0;

		//hip_angle_r  
        LB[N_EXC_PARAMS+3] = -3.0; UB[N_EXC_PARAMS+3] =  3.0; 

		//knee_angle_r
        LB[N_EXC_PARAMS+4] = -5.0; UB[N_EXC_PARAMS+4] =  5.0;

		//ankle_angle_r  
        LB[N_EXC_PARAMS+5] = -5.0; UB[N_EXC_PARAMS+5] =  5.0;

		//subtalar_angle_r  
        LB[N_EXC_PARAMS+6] = -5.0; UB[N_EXC_PARAMS+6] =  5.0;

		//mtp_angle_r  
        LB[N_EXC_PARAMS+7] = -5.0; UB[N_EXC_PARAMS+7] =  5.0;

		//hip_angle_l
        LB[N_EXC_PARAMS+8] = -3.0; UB[N_EXC_PARAMS+8] =  3.0;

		//knee_angle_l  
        LB[N_EXC_PARAMS+9] = -2.0; UB[N_EXC_PARAMS+9] = 5.0; 

		//ankle_angle_l
        LB[N_EXC_PARAMS+10] = -1.0; UB[N_EXC_PARAMS+10] = 6.0;

		//subtalar_angle_l  
        LB[N_EXC_PARAMS+11] = -5.0; UB[N_EXC_PARAMS+11] =  5.0;

		//mtp_angle_l  
        LB[N_EXC_PARAMS+12] = -5.0; UB[N_EXC_PARAMS+12] =  5.0;
//	} //end source 4 loop


}//end set_opt_param_bounds

