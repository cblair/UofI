package com.ts.presentation.model.perdue
{
	import com.ts.data.model.DatabaseProxy
	import com.ts.data.model.DataIndex;
	import com.ts.data.model.DataProxy;
	import com.ts.presentation.view.decorationAssets.converters.DataToColorConverter;
	import com.ts.presentation.view.decorationAssets.interpolators.StateColorInterpolator;
	import com.ts.presentation.view.decorationAssets.plotters.ColoredMarkerVO;
	import com.ts.presentation.view.decorationAssets.plotters.ScatterPlot;
	
	import org.puremvc.as3.patterns.proxy.Proxy;
	
	public class PerdueProxy extends Proxy
	{
		public static const NAME:String = "perdueProxy";
		
		private const DEFAULT_PLOT_DATA_CONVERTER:DataToColorConverter = new DataToColorConverter( new StateColorInterpolator( [ 0 ], [ 0x000000 ] ) );
		private const DEFAULT_PLOT_DATA_NAME:String	= "default";
		
		private var _plotDataConverter:DataToColorConverter;
		private var _plotDataName:String;
		private var _dataIndex:DataIndex;
		private var _phasePlotMap:Object;
		
		public function setPlotDataConverter( plotDataConverter:DataToColorConverter, vehDataName:String ):void
		{
			_plotDataConverter 	= plotDataConverter;
			_plotDataName 		= vehDataName;
			
			updatePhasePlots();
		}
		
		public function clearPlotDataConverter():void
		{
			_plotDataConverter 	= DEFAULT_PLOT_DATA_CONVERTER;
			_plotDataName 		= DEFAULT_PLOT_DATA_NAME;
			
			updatePhasePlots();
		}
		
		public function getPlotAtPhaseUID( phaseUID:String ):PerduePlotVO
		{
			return _phasePlotMap[ phaseUID ] as PerduePlotVO;
		}
		
		public function PerdueProxy()
		{
			super( NAME );
			
			_plotDataConverter 	= DEFAULT_PLOT_DATA_CONVERTER;
			_plotDataName	 	= DEFAULT_PLOT_DATA_NAME;

			_phasePlotMap 		= new Object();
			
			var database:DatabaseProxy 	= facade.retrieveProxy( DatabaseProxy.NAME ) as DatabaseProxy;
			_dataIndex 					= database.dataIndex;
		}
		
		public function addPhasePlot( phaseUID:String, detectedVehicles:DataProxy, detectedTimes:DataProxy, signalData:DataProxy ):void
		{
			var signalCycles:Array = buildCycles( signalData );
			var dataCycles:Array 	= seperateDataCycles( signalCycles, detectedVehicles.uncompressedData, detectedTimes.uncompressedData );
			var plotCycles:Array 	= buildPlotCycles( dataCycles, signalCycles );
			
			_phasePlotMap[ phaseUID ] = new PerduePlotVO( plotCycles );
		}
		
		private function buildCycles( signalData:DataProxy ):Array
		{
			var cycles:Array = new Array();
			
			var changeIndex:Array = signalData.dataVO.hashIndex;
			var changeTable:Object = signalData.dataVO.hashTable;
			
			var firstChangeToRed:int = 0;
			while( changeTable[ firstChangeToRed ] != 3 && firstChangeToRed < changeIndex.length )
			{
				firstChangeToRed++;
			}
			
			//-- if red is not the first signal displayed for this phase
			if( firstChangeToRed != 0 )
			{
				cycles.push( new SignalCycleVO( 0, 0, 0, changeIndex[ firstChangeToRed ] - 1 ) );
			}
			
			var lastSignal:int = 1;
			var workingCycle:SignalCycleVO;
			for( var i:int = firstChangeToRed; i < changeIndex.length; i++ )
			{
				if( !isNaN( changeTable[ i ] ) && changeTable[ i ] != lastSignal )
				{
					switch( changeTable[ i ] )
					{
						case 3:
							if( workingCycle )
							{
								workingCycle.cycleLength = changeIndex[ i ] - workingCycle.startRed - 1;
								cycles.push( workingCycle );
							}
							workingCycle = new SignalCycleVO( changeIndex[ i ], 0, 0, 0 );
							break;
						
						case 2:
							workingCycle.startYellow = changeIndex[ i ];
							break;
						
						case 1:
							workingCycle.startGreen = changeIndex[ i ];
							break;
						
						default:
							break;
					}
					lastSignal = changeTable[ i ];
				}
			}
			
			if( workingCycle )
			{
				workingCycle.cycleLength = signalData.numValues - workingCycle.startRed - 1;
				cycles.push( workingCycle )
			}

			return cycles;
		}
		
		private function seperateDataCycles( signalCycles:Array, detectedVeh:Array, detectionTimes:Array ):Array
		{
			var dataCycles:Array = new Array();
			
			var workingVehArray:Array;
			var workingTimeArray:Array;
			
			var detectionMap:Object = new Object();
			for( var i:int = 0; i < detectionTimes.length; i++ )
			{
				detectionMap[ detectionTimes[ i ] ] = detectedVeh[ i ];
			}
			
			for each( var cycle:SignalCycleVO in signalCycles )
			{
				workingVehArray = new Array();
				workingTimeArray = new Array();
				for( i = cycle.startRed; i <= ( cycle.startRed + cycle.cycleLength ); i++ )
				{
					if( detectionMap[ i ] )
					{
						workingVehArray.push( detectionMap[ i ] );
						workingTimeArray.push( i );
					}
				}
				dataCycles.push( new DataCycleVO( workingVehArray, workingTimeArray ) );
			}
			return dataCycles;
		}
		
		private function buildPlotCycles( dataCycles:Array, signalCycles:Array ):Array
		{
			var plotCycles:Array = new Array();
			
			for( var i:int = 0; i < dataCycles.length; i++ )
			{
				var markers:Array = buildMarkers( dataCycles[ i ] );
				plotCycles.push( new PlotCycleVO( dataCycles[ i ], signalCycles[ i ], markers ) );
			}
			return plotCycles;
		}
		
		private function buildMarkers( dataCycle:DataCycleVO ):Array
		{
			var markers:Array = new Array();
			if( _plotDataName == DEFAULT_PLOT_DATA_NAME )
			{
				for each( var vehicle:int in dataCycle.detectedVeh )
				{
					markers.push( ScatterPlot.createPointMarker( _plotDataConverter.defaultColor ) );
				}
			}
			else
			{
				var colorValue:Number;
				for( var i:int = 0; i < dataCycle.detectedVeh.length; i++ )
				{
					colorValue = _dataIndex.getVehicleDataProxy( _plotDataName, dataCycle.detectedVeh[ i ] ).getValueAt( dataCycle.detectionTimes[ i ] );
					
					markers.push( ScatterPlot.createPointMarker( isNaN( colorValue ) ? _plotDataConverter.convertValueToColor( colorValue ) : _plotDataConverter.defaultColor ) );
				}
			}
			
			return markers;
		}
		
		private function updatePhasePlots():void
		{
			for each( var plot:PerduePlotVO in _phasePlotMap )
			{
				for each( var cycle:PlotCycleVO in plot.plotCycles )
				{
					cycle.markers = buildMarkers( cycle.dataCycle );
				}
			}
			updatePerdue();
		}
		
		private function updatePerdue():void
		{
			
		}
	}
}