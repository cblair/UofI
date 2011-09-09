package com.ts.presentation.model.map.phase
{
	public class DetectorIndex
	{
		public var detectors:Array;
		public var groupNum:int;
		public var intersectionNum:int;
		
		public function DetectorIndex( Detectors:Array )
		{
			if( Detectors )
			{
				if( Detectors.length > 0 )
				{
					detectors = Detectors;
					groupNum = DetectorProxy( detectors[ 0 ] ).groupNum;
					intersectionNum = DetectorProxy( detectors[ 0 ] ).intersectionNum;
				}
				else
				{
					detectors = [];
					groupNum = -1;
					intersectionNum = -1;
				}
			}
			else
			{
				detectors = [];
				groupNum = -1;
				intersectionNum = -1;
			}
		}
	}
}