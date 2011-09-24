package com.ts.aspect.model
{
	import com.ts.aspect.model.graph.plot.PlotPolicy;
	import com.ts.aspect.model.graph.plot.vo.PlotDecorationVO;
	import com.ts.data.model.DataProxy;
	
	public class StateMapPlotDecorator extends AbstractPlotDecorator
	{
		private var _stateMap:Object;
		private var _entryMap:Object;
		private var _entry:DecorationEntry;
		private var _state:int;
		private var _adornerData:DataProxy;
		private var _decorationFunction:Function;
		
		public function StateMapPlotDecorator( Entries:Array, plotPolicy:String, valueData:DataProxy, adornerData:DataProxy )
		{
			_stateMap = new Object();
			_entryMap = new Object();
			
			var i:int = 1;
			for each( var entry:DecorationEntry in Entries )
			{
				_entryMap[ entry.value ] = entry;
				_stateMap[ entry.value ] = i;
				i++;
			}
			
			super( Entries, plotPolicy, dataProxy );
			
			switch( plotPolicy )
			{
				case PlotPolicy.STATE_REPEATING:
					_decorationFunction = repeatingDecorator;
					break;
				
				case PlotPolicy.STATE_CLIMBING:
					_decorationFunction = climbingDecorator;
					break;
				
				case PlotPolicy.STATE_STACKED:
					_decorationFunction = stackedDecorator;
					break;
				
				default:
					_decorationFunction = climbingDecorator;
					break;
			}
		}
		
		override protected function refactorDecorator():void
		{
			
		}
		
		override public function getDecorator( maxValue:Number, minValue:Number, value:Number ):PlotDecoration2VO
		{
			if( isNaN( value ) ) return new NullDecoration( maxValue, minValue );
			
			_entry = _entryMap[ value ];
			_state = _stateMap[ value ]
			
			if( _entry )
			{
				return _decorationFunction( maxValue, minValue, value );
			}
			else
			{
				return new NullDecoration( maxValue, minValue );
			}
		}
		
		private function repeatingDecorator( maxValue:Number, minValue:Number, value:Number ):PlotDecoration2VO
		{
			return new PlotDecoration2VO( maxValue, minValue, maxValue, minValue, _entry.color, _entry.alpha, _entry.tooltip, _entry.weblink );
		}
		
		private function climbingDecorator( maxValue:Number, minValue:Number, value:Number ):PlotDecoration2VO
		{
			return new PlotDecoration2VO( maxValue, minValue, _state, minValue, _entry.color, _entry.alpha, _entry.tooltip, _entry.weblink );
		}
		
		private function stackedDecorator( maxValue:Number, minValue:Number, value:Number ):PlotDecoration2VO
		{
			return new PlotDecoration2VO( maxValue, minValue, _state, _state - 1, _entry.color, _entry.alpha, _entry.tooltip, _entry.weblink );
		}
	}
}