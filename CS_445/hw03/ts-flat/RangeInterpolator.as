package com.ts.utilities.colorRanges
{
	public class RangeInterpolator extends AbstractColorInterpolator
	{
		public var iFunction = ColorInterpolation.interpolate2ColorRBG;
		public var lColor:uint;
		public var hColor:uint;
		public var lAlpha:Number;
		public var hAlpha:Number;
		public var minVal:Number;
		public var maxVal:Number;
		
		public function RangeInterpolator( LColor:uint, HColor:uint, LAlpha:Number, HAlpha:Number, MinVal:Number, MaxVal:Number )
		{
			lColor 		= LColor;
			hColor 		= HColor;
			lAlpha		= LAlpha;
			hAlpha		= HAlpha;
			minVal 		= MinVal;
			maxVal 		= MaxVal;
		}
		
		override public function getColor( value:Number ):uint
		{
			return iFunction( minVal, value, maxVal, lColor, hColor, lAlpha, hAlpha );
		}
		
		override public function toString( asList:Boolean=true ):String
		{
			
		}
	}
}