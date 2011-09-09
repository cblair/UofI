package com.ts.aspect.development
{
	public class HistoryPlotProxy
	{
		public var plotMask:HistoryPlotMaskVO;
		public var infoPlotParts:Array;
		public var decorationPlotParts:Array;
		
		public function HistoryPlotProxy( PlotMask:HistoryPlotMaskVO, InfoPlotParts:Array, DecorationPlotParts:Array )
		{
			plotMask	 		= PlotMask;
			infoPlotParts 		= InfoPlotParts;
			decorationPlotParts = DecorationPlotParts;
		}
	}
}