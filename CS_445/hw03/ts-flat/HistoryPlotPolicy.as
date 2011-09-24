package com.ts.aspect.development
{
	public class HistoryPlotPolicy
	{
		public static const LINEAR_FILL:String		= "linearFill";
		public static const LINEAR_LINE:String		= "linearLine";
		public static const STEP_FILL:String		= "stepFill";
		public static const STEP_LINE:String		= "stepLine";
		public static const HORIZONTAL_BAR:String	= "horizontalBar";
		public static const STACKED_BAR:String		= "stackedBar";
		
		public static const DYNAMIC_POLICIES:Array 	= [ "linearFill", "linearLine", "stepFill", "stepLine" ];
		public static const DISCRETE_POLICIES:Array = [ "linearFill", "linearLine", "stepFill", "stepLine", "horizontalBar", "stackedBar" ];
	}
}