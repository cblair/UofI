package com.ts.presentation.model.map.vo
{
	[Bindable]
	public class PathDataVO
	{
		public var pathData:String;
		public var weight:Number;
		public var color:uint;
		public var alpha:Number;
		public var linkNumber:int;
		public var isConnector:Boolean;
		public var variance:Number;
		
		public function PathDataVO( PathData:String, Weight:Number, Color:uint, Alpha:Number, LinkNumber:int, IsConnector:Boolean, Variance:Number )
		{
			pathData 	= PathData;
			weight 		= Weight;
			color 		= Color;
			alpha 		= Alpha;
			linkNumber	= LinkNumber;
			isConnector	= IsConnector;
			variance 	= Math.round( Variance * 100 ) / 100;
		}
	}
}