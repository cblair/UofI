package com.ts.aspect.model.graph.plot
{

	public class PlotPolicy
	{
		public static const AREA_FILL:String		= "areaFill";
		public static const LINE:String				= "line";
		public static const AREA_LINE_FILL:String	= "areaLineFill";
		public static const STATE_REPEATING:String	= "stateRepeating";
		public static const STATE_STACKED:String	= "stateStacked";
		public static const STATE_CLIMBING:String	= "stateClimbing";
		
		public static const PLOT_BAR_POLICIES:Array = [ "areaFill",
														"line",
														"areaLineFill",
														"stateRepeating",
														"stateStacked",
														"stateClimbing" ];
		
		public static const PLOT_BAR_MENU_NAMES:Array = [ "Area Fill",
														  "Line Plot",
														  "Area & Line Plot",
														  "Repeating State Plot",
														  "Stacked State Plot",
														  "Climbing State Plot" ];
		
	}
}