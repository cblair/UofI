package com.ts.presentation.model.map.vo
{
	public class BackgroundLinkVO
	{
		public var linkNumber:int;
		public var isConnector:Boolean;
		public var points:Array;
		public var numLanes:int;
		public var variance:Number;
		
		public function BackgroundLinkVO( LinkNumber:int, IsConnector:Boolean, Points:Array, NumLanes:int, Variance:Number )
		{
			points 		= Points;
			numLanes	= NumLanes;
			linkNumber 	= LinkNumber;
			isConnector	= IsConnector;
			variance	= Variance;
		}
	}
}