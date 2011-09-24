package com.ts.aspect.model
{
	public class PlotDecoration2VO
	{
		public var color:uint;
		public var alpha:Number;
		public var tooltip:String;
		public var weblink:String;
		public var maxValue:Number;
		public var minValue:Number;
		public var highValue:Number;
		public var lowValue:Number;
		
		public function PlotDecoration2VO( MaxValue:Number, MinValue:Number, HighValue:Number, LowValue:Number, Color:uint, Alpha:Number, Tooltip:String, Weblink:String )
		{
			color 		= Color;
			alpha 		= Alpha;
			tooltip 	= Tooltip;
			weblink 	= Weblink;
			maxValue	= MaxValue;
			minValue	= MinValue;
			highValue	= HighValue;
			lowValue	= LowValue;
		}
	}
}