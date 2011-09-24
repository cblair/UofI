package com.ts.presentation.view.graphs
{
	import com.ts.presentation.model.graphs.PlotBarVO;
	
	import mx.graphics.SolidColor;
	
	import spark.primitives.Rect;
	
	public class AreaFillBar extends AbstractPlotBar
	{
		private var _rect:Rect;
		
		public function AreaFillBar( plotBarVO:PlotBarVO )
		{
			super( plotBarVO );
		}
		
		override public function emphasize():void
		{
			if( !_isNull )
			{
				_rect.fill = new SolidColor( _plotBarVO.color, _plotBarVO.fillAlpha );
			}
		}
		
		override public function subdue():void
		{
			if( !_isNull )
			{
				_rect.fill = new SolidColor( _plotBarVO.subduedColor, _plotBarVO.fillAlpha );
			}
		}
		
		override protected function createChildren():void
		{
			super.createChildren();
			_rect = new Rect;
			_rect.percentWidth = 100;
			
			if( _isNull )
			{
				_rect.height = 1;
				_rect.bottom = 0;
				_rect.fill = new SolidColor( 0x000000, 0 );
			}
			else
			{
				_rect.percentHeight = _valueSpan / _valueRange * 100;
				if( _valueLowOffset == 0 )
				{
					_rect.bottom = 0;
				}
				else
				{
					updateOffset();	
				}
			}
			
			emphasize();
			addElement( _rect );
		}
		
		private function updateOffset():void
		{
			//_rect.bottom = Math.floor( _valueLowOffset * height );
		}
		
		override protected function updateDisplayList(unscaledWidth:Number, unscaledHeight:Number):void
		{
			super.updateDisplayList( unscaledWidth, unscaledHeight );
			if( !_isNull )
			{
				if( _valueLowOffset != 0 )
				{
					updateOffset();	
					trace( "updated offset" )
				}
			}
		}
	}
}