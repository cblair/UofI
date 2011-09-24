package com.ts.importing.virtualData
{
	import com.ts.importing.virtualData.derivations.required.RequiredDataNameEnum;
	import com.ts.importing.virtualData.required.VehLengthFactory;
	import com.ts.importing.virtualData.required.VehNumbersFactory;
	import com.ts.importing.virtualData.required.VehPhaseFactory;
	import com.ts.importing.virtualData.required.VehPosXFactory;
	import com.ts.importing.virtualData.required.VehPosYFactory;
	import com.ts.importing.virtualData.required.VehRotationFactory;

	public class DerivationPolicyFactory
	{
		public static function getDerivationPolicyByName( name:String ):AbstractVirtualDataFactory
		{
			switch( name )
			{
				case RequiredDataNameEnum.VEH_NUMBERS:			return new VehNumbersFactory();
				case RequiredDataNameEnum.VEH_POS_X:			return new VehPosXFactory();
				case RequiredDataNameEnum.VEH_POS_Y:			return new VehPosYFactory();
				case RequiredDataNameEnum.VEH_ROTATION:			return new VehRotationFactory();
				case RequiredDataNameEnum.VEH_LENGTH:			return new VehLengthFactory();
				case RequiredDataNameEnum.VEH_PHASE:			return new VehPhaseFactory();
				
				default:										return null;
			}
		}
	}
}