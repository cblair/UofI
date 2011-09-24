package com.ts.presentation.model.timeSpace
{
	import com.ts.data.model.DatabaseProxy;
	import com.ts.data.model.DataIndex;
	import com.ts.presentation.view.decorationAssets.converters.DataToColorConverter;
	import com.ts.presentation.view.decorationAssets.interpolators.StateColorInterpolator;
	import com.ts.presentation.view.decorationAssets.plotters.LinePlot;
	import com.ts.presentation.model.map.phase.PhaseVehicleIndex;
	
	import org.puremvc.as3.patterns.proxy.Proxy;
	
	public class TimeSpaceProxy extends Proxy
	{
		public static const NAME:String	= "timeSpaceProxy";
		public static const NOTE_TIME_SPACE_CHANGED:String	= NAME + "/note/timeSpaceChanged";
		public static const NOTE_TIME_SPACE_INACTIVE:String	= NAME + "/note/timeSpaceInactive";
		
		private const DEFAULT_PLOT_DATA_CONVERTER:DataToColorConverter = new DataToColorConverter( new StateColorInterpolator( [ 0 ], [ 0x000000 ] ) );
		private const DEFAULT_PLOT_DATA_NAME:String	= "default";
		
		private var _phaseTSMap:Object;
		private var _phasePlotMap:Object
		private var _signalDataConverter:DataToColorConverter;
		private var _plotDataConverter:DataToColorConverter;
		private var _plotDataName:String;
		private var _dataIndex:DataIndex;
		
		private var _selectedPhases:Array;
		
		public function setPlotDataConverter( plotDataConverter:DataToColorConverter, vehDataName:String ):void
		{
			_plotDataConverter = plotDataConverter;
			_plotDataName = vehDataName;
			
			updatePhasePlots();
		}
		
		public function clearPlotDataConverter():void
		{
			_plotDataConverter = DEFAULT_PLOT_DATA_CONVERTER;
			_plotDataName = DEFAULT_PLOT_DATA_NAME;
			
			updatePhasePlots();
		}
		
		public function getPlotAtPhaseUID( phaseUID:String ):TimeSpacePlotVO
		{
			return _phasePlotMap[ phaseUID ] as TimeSpacePlotVO;
		}
		
		public function TimeSpaceProxy()
		{
			super( NAME );
			_phaseTSMap = new Object();
			_phasePlotMap = new Object();
			_plotDataConverter = DEFAULT_PLOT_DATA_CONVERTER;
			_plotDataName = DEFAULT_PLOT_DATA_NAME;
			
			var database:DatabaseProxy = facade.retrieveProxy( DatabaseProxy.NAME ) as DatabaseProxy;
			_dataIndex = database.dataIndex;
			
			_selectedPhases = new Array();
		}
		
		public function selectPhase( phaseUID:String ):void
		{
			_selectedPhases = [ phaseUID ];
			updateTimeSpace();
		}
		
		public function addPhaseSelection( phaseUID:String ):void
		{
			var index:int = _selectedPhases.indexOf( phaseUID );
			if( index == -1 )
			{
				_selectedPhases.push( phaseUID );
				updateTimeSpace();
			}
		}
		
		public function removePhaseSelection( phaseUID:String ):void
		{
			var index:int = _selectedPhases.indexOf( phaseUID );
			if( index != -1 )
			{
				_selectedPhases.splice( index, 1 );
				updateTimeSpace();
			}
		}
		
		public function unselectPhases():void
		{
			_selectedPhases = [];
			updateTimeSpace();
		}
		
		public function addPhasePlot( phaseUID:String, phaseVehIndex:PhaseVehicleIndex, phaseLength:Number, phaseSignalPos:Number, signalLength:int, phaseSignalData:Array, signalDataConverter:DataToColorConverter ):void
		{
			if( phaseVehIndex )
			{
				var timeSpaceVehIndex:TimeSpaceVehicleIndex = new TimeSpaceVehicleIndex( phaseVehIndex.vehRangeTable, phaseLength );
				var signalTrend:Array = buildSignalTrend( phaseSignalPos, phaseLength, signalLength, phaseSignalData, signalDataConverter );
				var tsTrends:Array = buildTSTrends( timeSpaceVehIndex );
				var tsPlot:TimeSpacePlotVO = new TimeSpacePlotVO( tsTrends, signalTrend );
				
				_phaseTSMap[ phaseUID ] = timeSpaceVehIndex;
				_phasePlotMap[ phaseUID ] = tsPlot;
			}
		}
		
		private function buildSignalTrend( signalPos:Number, phaseLength:Number, signalLength:int, signalData:Array, signalDataConverter:DataToColorConverter ):Array
		{
			return LinePlot.buildConstantLinearTrend( signalPos, phaseLength, signalLength, signalData, signalDataConverter );
		}
		
		private function buildTSTrends( vehIndex:TimeSpaceVehicleIndex ):Array
		{
			var tsTrends:Array = new Array();
			if( _plotDataName == DEFAULT_PLOT_DATA_NAME )
			{
				for each( var vehicle:int in vehIndex.vehicleIndex )
				{
					tsTrends.push( new TimeSpaceLineVO( vehicle, LinePlot.buildDynamicLinearTrend( vehIndex.timeTable[ vehicle ], vehIndex.spaceTable[ vehicle ], vehIndex.phaseLength, null, DEFAULT_PLOT_DATA_CONVERTER ) ) );
				}
			}
			else
			{
				for each( vehicle in vehIndex.vehicleIndex )
				{
					tsTrends.push( new TimeSpaceLineVO( vehicle, LinePlot.buildDynamicLinearTrend( vehIndex.timeTable[ vehicle ], vehIndex.spaceTable[ vehicle ], vehIndex.phaseLength, 
																	 							   _dataIndex.getVehicleDataProxy( _plotDataName, vehicle ).uncompressedData, _plotDataConverter ) ) );
				}
			}
			
			return tsTrends;
		}
		
		private function updatePhasePlots():void
		{
			var signalTrend:Array;
			var tsTrends:Array;
			for( var phaseUID:String in _phaseTSMap )
			{
				signalTrend = _phasePlotMap[ phaseUID ].signalPlot;
				tsTrends = buildTSTrends( _phaseTSMap[ phaseUID ] );
				_phasePlotMap[ phaseUID ] = new TimeSpacePlotVO( tsTrends, signalTrend ); 
			}
			
			updateTimeSpace();
		}
		
		private function updateTimeSpace():void
		{
			if( _selectedPhases.length > 0 )
			{
				var selectedPlots:Array = new Array();
				for each( var phaseUID:String in _selectedPhases )
				{
					selectedPlots.push( _phasePlotMap[ phaseUID ] );
				}
				sendNotification( NOTE_TIME_SPACE_CHANGED, selectedPlots );
			}
			else
			{
				sendNotification( NOTE_TIME_SPACE_INACTIVE );
			}
		}
	}
}