package com.ts.presentation.model.perdue
{
	public class SignalCycleVO
	{
		public var startRed:int;
		public var startYellow:int;
		public var startGreen:int;
		public var cycleLength:int;
		
		public function SignalCycleVO( StartRed:int, StartYellow:int, StartGreen:int, CycleLength:int )
		{
			startRed 	= StartRed;
			startYellow = StartYellow;
			startGreen 	= StartGreen;
			cycleLength = CycleLength;
		}
	}
}