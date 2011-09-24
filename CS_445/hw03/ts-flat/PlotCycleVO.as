package com.ts.presentation.model.perdue
{
	public class PlotCycleVO
	{
		public var dataCycle:DataCycleVO;
		public var signalCycle:SignalCycleVO;
		public var markers:Array;
		
		public function PlotCycleVO( DataCycle:DataCycleVO, SignalCycle:SignalCycleVO, Markers:Array )
		{
			dataCycle 		= DataCycle;
			signalCycle 	= SignalCycle;
			markers 		= Markers;
		}
	}
}