package com.ts.aspect.development
{
	import com.ts.data.enumerations.DataClass;
	import com.ts.data.model.DataProxy;
	import com.ts.data.model.DiscreteDataProxy;
	import com.ts.data.model.vo.StateVO;
	import com.ts.data.model.vo.StatesVO;
	import com.ts.utilities.drawing.PathUtilities;
	
	import flash.geom.Point;

	public class HistoryPlotMask
	{
		public var plotData:DataProxy;
		public var policyList:Array;
		private var _plotPolicy:String;
		private var _plotFunction:Function;

		private var _stateMap:Object;
		private var _numStates:int;
		
		public function set plotPolicy( value:String ):void
		{
			if( _plotPolicy != value )
			{
				_plotPolicy = value;
				switch( value )
				{
					case HistoryPlotPolicy.LINEAR_FILL:
						_plotFunction = linearFill;
						break;
					
					case HistoryPlotPolicy.LINEAR_LINE:
						break;
					
					case HistoryPlotPolicy.STEP_FILL:
						break;
					
					case HistoryPlotPolicy.STEP_LINE:
						break;
					
					case HistoryPlotPolicy.STACKED_BAR:
						_plotFunction = stackedBar;
						break;
					
					case HistoryPlotPolicy.HORIZONTAL_BAR:
						_plotFunction = horizontalBar;
						break;
					
					default:
						_plotPolicy = HistoryPlotPolicy.LINEAR_FILL;
						_plotFunction = linearFill;
						break;
				}
			}
		}
		
		public function HistoryPlotMask( PlotData:DataProxy, PlotPolicy:String, StatePlotMap:Array = null )
		{
			if( PlotData.dataClass == DataClass.DISCRETE )
			{
				policyList = HistoryPlotPolicy.DISCRETE_POLICIES;
				
				_stateMap = new Object();
				
				//-- if a state map was provided, use its array order to map state values
				if( StatePlotMap )
				{
					for( i = 1; i <= StatePlotMap.length; i++ )
					{
						_stateMap[ StatePlotMap[ i - 1 ] ] = i;
					}
					
					_numStates = StatePlotMap.length;
				}
				//-- if a state map was not provided, map the actual state numbers in ascending order
				else
				{
					var states:Array = DiscreteDataProxy( PlotData ).states.states;
					states = states.sortOn( "stateNumber" );
					
					for( var i:int = 1; i <= states.length; i++ )
					{
						_stateMap[ states[ i - 1 ].stateNumber ] = i;
					}
					
					_numStates = states.length;
				}
			}
			else
			{
				//-- some plot policies are only supported by discrete data types. 
				if( PlotPolicy == HistoryPlotPolicy.HORIZONTAL_BAR || PlotPolicy == HistoryPlotPolicy.STACKED_BAR )
				{
					plotPolicy = HistoryPlotPolicy.LINEAR_FILL;
				}
				else plotPolicy = PlotPolicy;
				policyList = HistoryPlotPolicy.DYNAMIC_POLICIES;
			}
			
			plotData = PlotData;
			plotPolicy = PlotPolicy;
		}
		
		public function buildHistoryPlotMask():HistoryPlotMaskVO
		{
			return _plotFunction( plotData.uncompressedData );
		}
		
		private function linearFill( dataSet:Array ):HistoryPlotMaskVO
		{
			var pointPairGroups:Array = new Array();
			var pointPairs:Array = new Array()
			var n:int = dataSet.length;
			var maxValue:Number = plotData.maxValue;
			var minValue:Number = plotData.minValue;
			var value:Number;
			
			var openGroup:Boolean = false;
			for( var i:int = 0; i < n; i++ )
			{
				value = dataSet[ i ];
				
				if( openGroup )
				{
					if( !isNaN( value ) )
					{
						if( value == maxValue )
						{
							pointPairs.push( [ new Point( i, maxValue - dataSet[ i - 1 ] ), new Point( i, maxValue ) ] );
							pointPairs.push( [ new Point( i, 0 ), new Point( i, maxValue ) ] );
						}
						else
						{
							pointPairs.push( [ new Point( i, maxValue - value ), new Point( i, maxValue ) ] );
						}
					}
					else
					{
						//-- first encounter of a NaN, need to close the current group, and take the shape down to the bottom of the plot.
						openGroup = false;
						pointPairs.push( [ new Point( i - 1, maxValue ), new Point( i - 1, maxValue ) ] );
						pointPairGroups.push( pointPairs );
					}
				}
				else
				{
					if( !isNaN( value ) )
					{
						if( value == maxValue )
						{
							pointPairs = new Array();
							pointPairs.push( [ new Point( i, maxValue ), new Point( i, maxValue ) ] );
							pointPairs.push( [ new Point( i, 0 ), new Point( i, maxValue ) ] );
							openGroup = true;
						}
						else
						{
							pointPairs = new Array();
							pointPairs.push( [ new Point( i, maxValue - value ), new Point( i, maxValue ) ] );
							openGroup = true;
						}
					}
				}
			}
			
			openGroup = false;
			pointPairs.push( [ new Point( i - 1, maxValue ), new Point( i - 1, maxValue ) ] );
			pointPairGroups.push( pointPairs );
			
			return new HistoryPlotMaskVO( PathUtilities.sideStitchPointPairGroups( pointPairGroups ) );
		}
		
		private function stackedBar( dataSet:Array ):HistoryPlotMaskVO
		{
			var pointPairGroups:Array = new Array();
			var pointPairs:Array = new Array()
			var n:int = dataSet.length;
			var maxValue:Number = _numStates;
			var minValue:Number = 0;
			var value:Number;
			
			var openGroup:Boolean = false;
			var lastValue:Number = dataSet[ 0 ];
			for( var i:int = 0; i < n; i++ )
			{
				value = dataSet[ i ];
				
				if( openGroup )
				{
					if( !isNaN( value ) )
					{
						//-- stack bar is changing. close off the shape with one last line, but immediately starting a new shape, so keep group open.
						if( value != lastValue )
						{
							pointPairs.push( [ new Point( i, maxValue - dataSet[ i - 1 ] ), new Point( i, maxValue - dataSet[ i - 1 ] + 1 ) ] );
							pointPairGroups.push( pointPairs );
							pointPairs = new Array();
						}
						
						pointPairs.push( [ new Point( i, maxValue - value ), new Point( i, maxValue - value + 1 ) ] );
					}
					else
					{
						//-- first encounter of a NaN, need to close the current group, and take the shape down to the bottom of the plot.
						pointPairs.push( [ new Point( i, maxValue - dataSet[ i - 1 ] ), new Point( i, maxValue - dataSet[ i - 1 ] + 1 ) ] );
						pointPairGroups.push( pointPairs );
						openGroup = false;
					}
				}
				else
				{
					if( !isNaN( value ) )
					{
						pointPairs = new Array();
						pointPairs.push( [ new Point( i, maxValue - value ), new Point( i, maxValue - value + 1 ) ] );
						openGroup = true;
					}
				}
				
				lastValue = value;
			}
			
			openGroup = false;
			pointPairs.push( [ new Point( i, maxValue - dataSet[ i - 1 ] ), new Point( i, maxValue - dataSet[ i - 1 ] + 1 ) ] );
			pointPairGroups.push( pointPairs );
			
			return new HistoryPlotMaskVO( PathUtilities.sideStitchPointPairGroups( pointPairGroups ) );
		}
		
		private function horizontalBar( dataSet:Array ):HistoryPlotMaskVO
		{
			var pathDataPoints:Array = [ [ new Point( 0, 1 ), new Point( 0, 0 ) ],
										 [ new Point( dataSet.length, 1 ), new Point( dataSet.length, 0 ) ] ];
			
			return new HistoryPlotMaskVO( PathUtilities.sideStitchPointPairs( pathDataPoints ) );
		}
	}
}