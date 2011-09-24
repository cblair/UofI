package com.ts.presentation.model.graphs
{
	import com.ts.presentation.view.graphs.AbstractPlotBar;

	[Bindable]
	public class PlotBarVO
	{
		public var color:uint;
		public var subduedColor:uint;
		public var width:Number;
		public var lowValue:Number;
		public var lastValue:Number;
		public var highValue:Number;
		public var minValue:Number;
		public var maxValue:Number;
		public var plotPolicy:String;
		public var weight:Number;
		public var fillAlpha:Number;
		public var lineAlpha:Number;
		public var plotBar:AbstractPlotBar;
		
		public function PlotBarVO( Color:uint, 			SubduedColor:uint,		Width:Number, 			LowValue:Number,	LastValue:Number, 	
								   HighValue:Number, 	MinValue:Number, 		MaxValue:Number, 		PlotPolicy:String,	Weight:Number = 1,	
								   FillAlpha:Number = 1,LineAlpha:Number = 1 )
		{
			color 		= Color;
			subduedColor= SubduedColor;
			width 		= Width;
			lowValue 	= LowValue;
			lastValue	= LastValue;
			highValue 	= HighValue;
			minValue 	= MinValue;
			maxValue 	= MaxValue;
			plotPolicy 	= PlotPolicy;
			weight		= Weight;
			fillAlpha	= FillAlpha;
			lineAlpha	= LineAlpha;
		}
	}
}