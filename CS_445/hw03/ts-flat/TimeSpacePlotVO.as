package com.ts.presentation.model.timeSpace
{
	public class TimeSpacePlotVO
	{
		public var plotLines:Array;
		public var signalPlot:Array;
		public function TimeSpacePlotVO( PlotLines:Array, SignalPlot:Array )
		{
			plotLines 	= PlotLines;
			signalPlot 	= SignalPlot;
		}
	}
}