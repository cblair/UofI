package com.ts.presentation.view.graphs
{
	import com.ts.presentation.model.graphs.PlotBarVO;
	
	import spark.components.Group;
	
	public class AbstractPlotBar extends Group
	{
		protected var _plotBarVO:PlotBarVO;
		protected var _isNull:Boolean = false;
		protected var _valueRange:Number;
		protected var _valueSpan:Number;
		protected var _valueLowOffset:Number;
		
		public function AbstractPlotBar( plotBarVO:PlotBarVO = null )
		{
			_plotBarVO 				= plotBarVO;
			if( plotBarVO )
			{
				_isNull 			= isNaN( plotBarVO.highValue );
				plotBarVO.lowValue 	= isNaN( plotBarVO.lowValue ) ? plotBarVO.minValue : plotBarVO.lowValue;	//-- CONVERT LOW VALUE TO MIN VALUE IF IT IS NOT DEFINED.
				_valueRange 		= plotBarVO.maxValue - plotBarVO.minValue;
				_valueSpan 			= _isNull ? NaN : plotBarVO.highValue - plotBarVO.lowValue;
				_valueLowOffset 	= plotBarVO.lowValue / plotBarVO.maxValue;
				
				_plotBarVO.plotBar 	= this;
			}
			
		}
		
		public function subdue():void
		{
			//-- Override me!!
		}
		
		public function emphasize():void
		{
			//-- Override me!!
		}
	}
}