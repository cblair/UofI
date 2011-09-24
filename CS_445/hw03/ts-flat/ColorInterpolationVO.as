package com.ts.utilities.colorRanges
{
	public class ColorInterpolationVO
	{
		public var iType:String;
		public var iThresholds:Array;
		public var iColors:Array;
		
		public function ColorInterpolationVO( IType:String, IThresholds:Array, IColors:Array )
		{
			iType 		= IType;
			iThresholds = IThresholds;
			iColors 	= IColors;
		}
	}
}