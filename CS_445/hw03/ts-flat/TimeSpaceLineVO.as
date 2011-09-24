package com.ts.presentation.model.timeSpace
{
	public class TimeSpaceLineVO
	{
		public var coloredLineTrends:Array;
		public var vehicleNumber:int;
		
		public function TimeSpaceLineVO( VehicleNumber:int, ColoredLineTrends:Array )
		{
			coloredLineTrends	= ColoredLineTrends;
			vehicleNumber 		= VehicleNumber;
		}
	}
}