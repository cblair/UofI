package com.ts.presentation.model.perdue
{
	public class DataCycleVO
	{
		public var detectedVeh:Array;
		public var detectionTimes:Array;
		
		public function DataCycleVO( DetectedVeh:Array, DetectionTimes:Array )
		{
			detectedVeh		= DetectedVeh;
			detectionTimes 	= DetectionTimes;
		}
	}
}