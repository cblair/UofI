#include <iostream>
#include "SA.h"
#include "Look.h"
#include "system_params.h"
extern "C" SeeThis mdata; //new global data structure

extern "C" int PATTERN;

void set_patterns(void)
{
	int i,j,m; 

	//////////customize each right leg muscles (template)//////////////

	////SOL
	/*
	mdata.ExcP[0].pBlock = new Lump[1];
	mdata.ExcP[0].zBlock = new Zero[2];

	mdata.ExcP[0].type = 1; //one excitation hill (for right leg)
	mdata.ExcP[0].nLump = 1; //number of excitation lumps
	mdata.ExcP[0].nZero = 2; //number of zero-parts
	mdata.ExcP[0].pBlock[0].t1=STARTCYCLE;
	mdata.ExcP[0].pBlock[0].t2=56;
	mdata.ExcP[0].pBlock[0].nPoint = mdata.ExcP[0].pBlock[0].t2 - 
									mdata.ExcP[0].pBlock[0].t1 + 1;
	mdata.ExcP[0].zBlock[0].nPoint = mdata.ExcP[0].pBlock[0].t1;
	mdata.ExcP[0].zBlock[1].nPoint = 100 - mdata.ExcP[0].pBlock[0].t2;
	*/
	mdata.ExcP[0].pBlock = new Lump[2];
	mdata.ExcP[0].zBlock = new Zero[1];

	mdata.ExcP[0].type = 2; //two excitation hills (for right leg)
	mdata.ExcP[0].nLump = 2; //number of excitation lumps
	mdata.ExcP[0].nZero = 1; //number of zero-parts
	mdata.ExcP[0].pBlock[0].t1=STARTCYCLE;
	mdata.ExcP[0].pBlock[0].t2=60;
	mdata.ExcP[0].pBlock[1].t1=88;
	mdata.ExcP[0].pBlock[1].t2=ENDCYCLE;
	mdata.ExcP[0].pBlock[0].nPoint = mdata.ExcP[0].pBlock[0].t2 + 1;
	mdata.ExcP[0].pBlock[1].nPoint = 100 - mdata.ExcP[0].pBlock[1].t1 + 1;
	mdata.ExcP[0].zBlock[0].nPoint = mdata.ExcP[0].pBlock[1].t1 - 
									 mdata.ExcP[0].pBlock[0].t2 - 1;
	

	////m_GAS
	/*
	mdata.ExcP[1].pBlock = new Lump[1];
	mdata.ExcP[1].zBlock = new Zero[2];

	mdata.ExcP[1].type = 1; //one excitation hill (for right leg)
	mdata.ExcP[1].nLump = 1; //number of excitation lumps
	mdata.ExcP[1].nZero = 2; //number of zero-parts
	mdata.ExcP[1].pBlock[0].t1=STARTCYCLE;
	mdata.ExcP[1].pBlock[0].t2=56;
	mdata.ExcP[1].pBlock[0].nPoint = mdata.ExcP[1].pBlock[0].t2 - 
									mdata.ExcP[1].pBlock[0].t1 + 1;
	mdata.ExcP[1].zBlock[0].nPoint = mdata.ExcP[1].pBlock[0].t1;
	mdata.ExcP[1].zBlock[1].nPoint = 100 - mdata.ExcP[1].pBlock[0].t2;
	*/
	mdata.ExcP[1].pBlock = new Lump[2];
	mdata.ExcP[1].zBlock = new Zero[1];

	mdata.ExcP[1].type = 2; //two excitation hills (for right leg)
	mdata.ExcP[1].nLump = 2; //number of excitation lumps
	mdata.ExcP[1].nZero = 1; //number of zero-parts
	mdata.ExcP[1].pBlock[0].t1=STARTCYCLE;
	mdata.ExcP[1].pBlock[0].t2=55;
	mdata.ExcP[1].pBlock[1].t1=88;
	mdata.ExcP[1].pBlock[1].t2=ENDCYCLE;
	mdata.ExcP[1].pBlock[0].nPoint = mdata.ExcP[1].pBlock[0].t2 + 1;
	mdata.ExcP[1].pBlock[1].nPoint = 100 - mdata.ExcP[1].pBlock[1].t1 + 1;
	mdata.ExcP[1].zBlock[0].nPoint = mdata.ExcP[1].pBlock[1].t1 - 
									 mdata.ExcP[1].pBlock[0].t2 - 1;
	
	

	////BFLH
	mdata.ExcP[2].pBlock = new Lump[2];
	mdata.ExcP[2].zBlock = new Zero[1];

	mdata.ExcP[2].type = 2; //two excitation hills (for right leg)
	mdata.ExcP[2].nLump = 2; //number of excitation lumps
	mdata.ExcP[2].nZero = 1; //number of zero-parts
	mdata.ExcP[2].pBlock[0].t1=STARTCYCLE;
	mdata.ExcP[2].pBlock[0].t2=23;
	mdata.ExcP[2].pBlock[1].t1=78;
	mdata.ExcP[2].pBlock[1].t2=ENDCYCLE;
	mdata.ExcP[2].pBlock[0].nPoint = mdata.ExcP[2].pBlock[0].t2 + 1;
	mdata.ExcP[2].pBlock[1].nPoint = 100 - mdata.ExcP[2].pBlock[1].t1 + 1;
	mdata.ExcP[2].zBlock[0].nPoint = mdata.ExcP[2].pBlock[1].t1 - 
									 mdata.ExcP[2].pBlock[0].t2 - 1;

	////BFSH (block)
	mdata.ExcP[3].pBlock = new Lump[3];
	mdata.ExcP[3].zBlock = new Zero[2];

	mdata.ExcP[3].type = 3; //two excitation hills (for right leg)
	mdata.ExcP[3].nLump = 3; //number of excitation lumps
	mdata.ExcP[3].nZero = 2; //number of zero-parts
	mdata.ExcP[3].pBlock[0].t1=STARTCYCLE;
	mdata.ExcP[3].pBlock[0].t2=23;
	mdata.ExcP[3].pBlock[1].t1=44;
	mdata.ExcP[3].pBlock[1].t2=68; 
	mdata.ExcP[3].pBlock[2].t1=83; 
	mdata.ExcP[3].pBlock[2].t2=ENDCYCLE;
	mdata.ExcP[3].pBlock[0].nPoint = mdata.ExcP[3].pBlock[0].t2 + 1;
	mdata.ExcP[3].pBlock[1].nPoint = mdata.ExcP[3].pBlock[1].t2 -
									 mdata.ExcP[3].pBlock[1].t1 + 1; 
	mdata.ExcP[3].pBlock[2].nPoint = 100 - mdata.ExcP[3].pBlock[2].t1 + 1;
	mdata.ExcP[3].zBlock[0].nPoint = mdata.ExcP[3].pBlock[1].t1 -
									 mdata.ExcP[3].pBlock[0].t2 -1;
	mdata.ExcP[3].zBlock[1].nPoint = mdata.ExcP[3].pBlock[2].t1 -
									 mdata.ExcP[3].pBlock[1].t2 -1;




	////PSOAS (block)
	mdata.ExcP[4].pBlock = new Lump[1];
	mdata.ExcP[4].zBlock = new Zero[2];

	mdata.ExcP[4].type = 1; //one excitation hill (for right leg)
	mdata.ExcP[4].nLump = 1; //number of excitation lumps
	mdata.ExcP[4].nZero = 2; //number of zero-parts
	mdata.ExcP[4].pBlock[0].t1=50;
	mdata.ExcP[4].pBlock[0].t2=80;
	mdata.ExcP[4].pBlock[0].nPoint = mdata.ExcP[4].pBlock[0].t2 - 
									mdata.ExcP[4].pBlock[0].t1 + 1;
	mdata.ExcP[4].zBlock[0].nPoint = mdata.ExcP[4].pBlock[0].t1;
	mdata.ExcP[4].zBlock[1].nPoint = 100 - mdata.ExcP[4].pBlock[0].t2;
	

	////TA
	mdata.ExcP[5].pBlock = new Lump[2];
	mdata.ExcP[5].zBlock = new Zero[1];

	mdata.ExcP[5].type = 2; //two excitation hills (for right leg)
	mdata.ExcP[5].nLump = 2; //number of excitation lumps
	mdata.ExcP[5].nZero = 1; //number of zero-parts
	mdata.ExcP[5].pBlock[0].t1=STARTCYCLE;
	mdata.ExcP[5].pBlock[0].t2=15;//25;
	mdata.ExcP[5].pBlock[1].t1=56;
	mdata.ExcP[5].pBlock[1].t2=ENDCYCLE;
	mdata.ExcP[5].pBlock[0].nPoint = mdata.ExcP[5].pBlock[0].t2 + 1;
	mdata.ExcP[5].pBlock[1].nPoint = 100 - mdata.ExcP[5].pBlock[1].t1 + 1;
	mdata.ExcP[5].zBlock[0].nPoint = mdata.ExcP[5].pBlock[1].t1 - 
									 mdata.ExcP[5].pBlock[0].t2 - 1;

	////RF
	/*
	mdata.ExcP[6].pBlock = new Lump[1];
	mdata.ExcP[6].zBlock = new Zero[2];

	mdata.ExcP[6].type = 1; //one excitation hill (for right leg)
	mdata.ExcP[6].nLump = 1; //number of excitation lumps
	mdata.ExcP[6].nZero = 2; //number of zero-parts
	mdata.ExcP[6].pBlock[0].t1=51;
	mdata.ExcP[6].pBlock[0].t2=73;
	mdata.ExcP[6].pBlock[0].nPoint = mdata.ExcP[6].pBlock[0].t2 - 
									mdata.ExcP[6].pBlock[0].t1 + 1;
	mdata.ExcP[6].zBlock[0].nPoint = mdata.ExcP[6].pBlock[0].t1;
	mdata.ExcP[6].zBlock[1].nPoint = 100 - mdata.ExcP[6].pBlock[0].t2;
	*/
	mdata.ExcP[6].pBlock = new Lump[3];
	mdata.ExcP[6].zBlock = new Zero[2];

	mdata.ExcP[6].type = 3; //two excitation hills (for right leg)
	mdata.ExcP[6].nLump = 3; //number of excitation lumps
	mdata.ExcP[6].nZero = 2; //number of zero-parts
	mdata.ExcP[6].pBlock[0].t1=STARTCYCLE;
	mdata.ExcP[6].pBlock[0].t2=25;
	mdata.ExcP[6].pBlock[1].t1=50;
	mdata.ExcP[6].pBlock[1].t2=75; 
	mdata.ExcP[6].pBlock[2].t1=86; 
	mdata.ExcP[6].pBlock[2].t2=ENDCYCLE;
	mdata.ExcP[6].pBlock[0].nPoint = mdata.ExcP[6].pBlock[0].t2 + 1;
	mdata.ExcP[6].pBlock[1].nPoint = mdata.ExcP[6].pBlock[1].t2 -
									 mdata.ExcP[6].pBlock[1].t1 + 1; 
	mdata.ExcP[6].pBlock[2].nPoint = 100 - mdata.ExcP[6].pBlock[2].t1 + 1;
	mdata.ExcP[6].zBlock[0].nPoint = mdata.ExcP[6].pBlock[1].t1 -
									 mdata.ExcP[6].pBlock[0].t2 -1;
	mdata.ExcP[6].zBlock[1].nPoint = mdata.ExcP[6].pBlock[2].t1 -
									 mdata.ExcP[6].pBlock[1].t2 -1;



	////VAS1	
	mdata.ExcP[7].pBlock = new Lump[2];
	mdata.ExcP[7].zBlock = new Zero[1];

	mdata.ExcP[7].type = 2; //two excitation hills (for right leg)
	mdata.ExcP[7].nLump = 2; //number of excitation lumps
	mdata.ExcP[7].nZero = 1; //number of zero-parts
	mdata.ExcP[7].pBlock[0].t1=STARTCYCLE;
	mdata.ExcP[7].pBlock[0].t2=20;//27;
	mdata.ExcP[7].pBlock[1].t1=83;
	mdata.ExcP[7].pBlock[1].t2=ENDCYCLE;
	mdata.ExcP[7].pBlock[0].nPoint = mdata.ExcP[7].pBlock[0].t2 + 1;
	mdata.ExcP[7].pBlock[1].nPoint = 100 - mdata.ExcP[7].pBlock[1].t1 + 1;
	mdata.ExcP[7].zBlock[0].nPoint = mdata.ExcP[7].pBlock[1].t1 - 
									 mdata.ExcP[7].pBlock[0].t2 - 1;


	////VAS2	
	mdata.ExcP[8].pBlock = new Lump[2];
	mdata.ExcP[8].zBlock = new Zero[1];

	mdata.ExcP[8].type = 2; //two excitation hills (for right leg)
	mdata.ExcP[8].nLump = 2; //number of excitation lumps
	mdata.ExcP[8].nZero = 1; //number of zero-parts
	mdata.ExcP[8].pBlock[0].t1=STARTCYCLE;
	mdata.ExcP[8].pBlock[0].t2=20;//27;
	mdata.ExcP[8].pBlock[1].t1=83;
	mdata.ExcP[8].pBlock[1].t2=ENDCYCLE;
	mdata.ExcP[8].pBlock[0].nPoint = mdata.ExcP[8].pBlock[0].t2 + 1;
	mdata.ExcP[8].pBlock[1].nPoint = 100 - mdata.ExcP[8].pBlock[1].t1 + 1;
	mdata.ExcP[8].zBlock[0].nPoint = mdata.ExcP[8].pBlock[1].t1 - 
									 mdata.ExcP[8].pBlock[0].t2 - 1;


	////VAS3	
	mdata.ExcP[9].pBlock = new Lump[2];
	mdata.ExcP[9].zBlock = new Zero[1];

	mdata.ExcP[9].type = 2; //two excitation hills (for right leg)
	mdata.ExcP[9].nLump = 2; //number of excitation lumps
	mdata.ExcP[9].nZero = 1; //number of zero-parts
	mdata.ExcP[9].pBlock[0].t1=STARTCYCLE;
	mdata.ExcP[9].pBlock[0].t2=20;//27;
	mdata.ExcP[9].pBlock[1].t1=83;
	mdata.ExcP[9].pBlock[1].t2=ENDCYCLE;
	mdata.ExcP[9].pBlock[0].nPoint = mdata.ExcP[9].pBlock[0].t2 + 1;
	mdata.ExcP[9].pBlock[1].nPoint = 100 - mdata.ExcP[9].pBlock[1].t1 + 1;
	mdata.ExcP[9].zBlock[0].nPoint = mdata.ExcP[9].pBlock[1].t1 - 
									 mdata.ExcP[9].pBlock[0].t2 - 1;


	///GMAX
	mdata.ExcP[10].pBlock = new Lump[2];
	mdata.ExcP[10].zBlock = new Zero[1];

	mdata.ExcP[10].type = 2; //two excitation hills (for right leg)
	mdata.ExcP[10].nLump = 2; //number of excitation lumps
	mdata.ExcP[10].nZero = 1; //number of zero-parts
	mdata.ExcP[10].pBlock[0].t1=STARTCYCLE;
	mdata.ExcP[10].pBlock[0].t2=20;//38;//17;
	mdata.ExcP[10].pBlock[1].t1=89;
	mdata.ExcP[10].pBlock[1].t2=ENDCYCLE;
	mdata.ExcP[10].pBlock[0].nPoint = mdata.ExcP[10].pBlock[0].t2 + 1;
	mdata.ExcP[10].pBlock[1].nPoint = 100 - mdata.ExcP[10].pBlock[1].t1 + 1;
	mdata.ExcP[10].zBlock[0].nPoint = mdata.ExcP[10].pBlock[1].t1 - 
									 mdata.ExcP[10].pBlock[0].t2 - 1;


	///GMED
	mdata.ExcP[11].pBlock = new Lump[2];
	mdata.ExcP[11].zBlock = new Zero[1];

	mdata.ExcP[11].type = 2; //two excitation hills (for right leg)
	mdata.ExcP[11].nLump = 2; //number of excitation lumps
	mdata.ExcP[11].nZero = 1; //number of zero-parts
	mdata.ExcP[11].pBlock[0].t1=STARTCYCLE;
	mdata.ExcP[11].pBlock[0].t2=30; 
	mdata.ExcP[11].pBlock[1].t1=85;
	mdata.ExcP[11].pBlock[1].t2=ENDCYCLE;
	mdata.ExcP[11].pBlock[0].nPoint = mdata.ExcP[11].pBlock[0].t2 + 1;
	mdata.ExcP[11].pBlock[1].nPoint = 100 - mdata.ExcP[11].pBlock[1].t1 + 1;
	mdata.ExcP[11].zBlock[0].nPoint = mdata.ExcP[11].pBlock[1].t1 - 
									 mdata.ExcP[11].pBlock[0].t2 - 1;

	///PEROLONG	
	mdata.ExcP[12].pBlock = new Lump[1];
	mdata.ExcP[12].zBlock = new Zero[2];

	mdata.ExcP[12].type = 1; //one excitation hill (for right leg)
	mdata.ExcP[12].nLump = 1; //number of excitation lumps
	mdata.ExcP[12].nZero = 2; //number of zero-parts
	mdata.ExcP[12].pBlock[0].t1=STARTCYCLE;
	mdata.ExcP[12].pBlock[0].t2=56;
	mdata.ExcP[12].pBlock[0].nPoint = mdata.ExcP[12].pBlock[0].t2 - 
									mdata.ExcP[12].pBlock[0].t1 + 1;
	mdata.ExcP[12].zBlock[0].nPoint = mdata.ExcP[12].pBlock[0].t1;
	mdata.ExcP[12].zBlock[1].nPoint = 100 - mdata.ExcP[12].pBlock[0].t2;


	///FlexDig
	mdata.ExcP[13].pBlock = new Lump[1];
	mdata.ExcP[13].zBlock = new Zero[2];

	mdata.ExcP[13].type = 1; //one excitation hill (for right leg)
	mdata.ExcP[13].nLump = 1; //number of excitation lumps
	mdata.ExcP[13].nZero = 2; //number of zero-parts
	mdata.ExcP[13].pBlock[0].t1=20;
	mdata.ExcP[13].pBlock[0].t2=60;
	mdata.ExcP[13].pBlock[0].nPoint = mdata.ExcP[13].pBlock[0].t2 - 
									mdata.ExcP[13].pBlock[0].t1 + 1;
	mdata.ExcP[13].zBlock[0].nPoint = mdata.ExcP[13].pBlock[0].t1;
	mdata.ExcP[13].zBlock[1].nPoint = 100 - mdata.ExcP[13].pBlock[0].t2;


	///ExtDig
	mdata.ExcP[14].pBlock = new Lump[2];
	mdata.ExcP[14].zBlock = new Zero[1];

	mdata.ExcP[14].type = 2; //two excitation hills (for right leg)
	mdata.ExcP[14].nLump = 2; //number of excitation lumps
	mdata.ExcP[14].nZero = 1; //number of zero-parts
	mdata.ExcP[14].pBlock[0].t1=STARTCYCLE;
	mdata.ExcP[14].pBlock[0].t2=25;
	mdata.ExcP[14].pBlock[1].t1=56;
	mdata.ExcP[14].pBlock[1].t2=ENDCYCLE;
	mdata.ExcP[14].pBlock[0].nPoint = mdata.ExcP[14].pBlock[0].t2 + 1;
	mdata.ExcP[14].pBlock[1].nPoint = 100 - mdata.ExcP[14].pBlock[1].t1 + 1;
	mdata.ExcP[14].zBlock[0].nPoint = mdata.ExcP[14].pBlock[1].t1 - 
									 mdata.ExcP[14].pBlock[0].t2 - 1;
	



	////////////////////////Initialize lump and zero//////////////////////////////////////	
	for(m=0;m<N_musG;m++){
		for(i=0;i<mdata.ExcP[m].nLump;i++){//memory allocation for lump
			mdata.ExcP[m].pBlock[i].px = new double[mdata.ExcP[m].pBlock[i].nPoint];
			mdata.ExcP[m].pBlock[i].py = new double[mdata.ExcP[m].pBlock[i].nPoint];
		}
		
		///set template values for lump
		for(j=0;j<mdata.ExcP[m].nLump;j++){
			for(i=0;i<mdata.ExcP[m].pBlock[j].nPoint;i++){
				mdata.ExcP[m].pBlock[j].py[i] = 1.0;//initialize with 1.0 (dummy);
				mdata.ExcP[m].pBlock[j].px[i] = (double)(mdata.ExcP[m].pBlock[j].t1 + i);	
			}

		}

	}

	//////////////Number of parameters to be simulated-annealed////////////
	for(m=0;m<N_musG;m++){

		
		if((mdata.ExcP[m].type == 1) || (mdata.ExcP[m].type == 2)){			

			mdata.ExcP[m].nPara = 3;
			//mdata.ExcP[m].nPara = 6;
			if(m==bfshG || m==psoasG || m==gmedG || m==perlngG || m==flxdigG || m==extdigG){//block
				mdata.ExcP[m].nPara = 6;
			}
		}

			
		if((mdata.ExcP[m].type == 3) || (mdata.ExcP[m].type == 4))
			mdata.ExcP[m].nPara = 6;

		if((m==solG)||(m==gasG)){ //SOL or mGAS
			if(PATTERN==EMG) mdata.ExcP[m].nPara = 3; //emg pattern
			if(PATTERN==BLOCK) mdata.ExcP[m].nPara = 8; //block pattrern
		}



	}

	/*	//set the number of parameter values that are to be optimized based on the pattern type and source and allocate the necessary space
	for(i=0;i<N_musG;i++){
		if((mdata.ExcPatterns[i].pattern_type == 1) || (mdata.ExcPatterns[i].pattern_type == 2)){
			if (mdata.ExcPatterns[i].pattern_source == 3){
				mdata.ExcPatterns[i].num_excite_params = 4; //on, off, magnitude, shape
			}
			else{			
				mdata.ExcPatterns[i].num_excite_params = 3; //on, off, and magnitude
			}
		}
		else if(mdata.ExcPatterns[i].pattern_type == 3){
			if (mdata.ExcPatterns[i].pattern_source == 3){			
				mdata.ExcPatterns[i].num_excite_params = 8; //on, off, magnitude, shape for first region & on, off, mag, shape for second region
			}
			else{
				mdata.ExcPatterns[i].num_excite_params = 6; //on, off, magnitude for first region & on, off, mag for second region
			}			
		}
		else{
			printf("Incompatible Muscle Pattern Type or Source.  Please Fix Values\n");
			printf("Muscle Group Number %i\n",i);
			exit(0);
		}
		mdata.ExcPatterns[i].excite_params = new double[mdata.ExcPatterns[i].num_excite_params];
//		printf("Group %i -> Parameters %i\n",i,mdata.ExcPatterns[i].num_excite_params);
	}//end loop through the muscles
*/
	//allocate memory
	for(m=0;m<N_musG;m++){
		mdata.ExcP[m].xp = new double[mdata.ExcP[m].nPara];
	}

				
}
/////////////////////////////////////////////////////////////////////
