package com.ts.presentation.model.map.phase.vo
{
	public class LinkDataVO
	{
		public var pathData:String;
		public var numLanes:int;
		
		public function LinkDataVO( PathData:String, NumLanes:int )
		{
			pathData = PathData;
			numLanes = NumLanes;
		}
	}
}