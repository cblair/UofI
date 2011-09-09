package com.ts.presentation.model.graphs
{
	import com.ts.presentation.view.graphs.AbstractPlotBar;
	import com.ts.presentation.view.graphs.AreaFillBar;
	import com.ts.presentation.view.graphs.AreaFillPlotSkin;
	import com.ts.presentation.view.graphs.AreaLineBar;
	import com.ts.presentation.view.graphs.RepeatingStateBar;
	import com.ts.presentation.view.graphs.StackedStateBar;

	public class PlotBarPolicy
	{
		public static const AREA_FILL:String		= "areaFill";
		public static const LINE:String				= "line";
		public static const AREA_LINE_FILL:String	= "areaLineFill";
		public static const STATE_REPEATING:String	= "stateRepeating";
		public static const STATE_STACKED:String	= "stateStacked";
		
		public static const PLOT_BAR_POLICIES:Array = [ "areaFill",
														"line",
														"areaLineFill",
														"stateRepeating",
														"stateStacked" ];
		
		public static const PLOT_BAR_MENU_NAMES:Array = [ "Area Fill",
														  "Line Plot",
														  "Area & Line Plot",
														  "Repeating State Plot",
														  "Stacked State Plot" ];
		
		public static function getPlotClass( plotBarVO:PlotBarVO ):AbstractPlotBar
		{
			switch( plotBarVO.plotPolicy )
			{
				case AREA_FILL:
					return new AreaFillBar( plotBarVO );
				
				case LINE:
					return new AreaFillBar( plotBarVO );
					
				case AREA_LINE_FILL:
					return new AreaLineBar( plotBarVO );
					
				case STATE_REPEATING:
					return new RepeatingStateBar( plotBarVO );
					
				case STATE_STACKED:
					return new StackedStateBar( plotBarVO );
					
				default:
					return null;
			}
		}
	}
}