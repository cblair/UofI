package com.ts.aspect.model.graph.plot.vo
{
	public class PlotDecorationVO
	{
		public var color:uint;
		public var alpha:Number;
		public var tooltip:String;
		public var weblink:String;
		
		public function PlotDecorationVO( Color:uint, Alpha:Number, Tooltip:String, Weblink:String )
		{
			color 	= Color;
			alpha 	= Alpha;
			tooltip = Tooltip;
			weblink = Weblink;
		}
	}
}