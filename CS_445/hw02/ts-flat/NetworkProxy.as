package com.ts.presentation.model.map
{
	import com.ts.data.model.DatabaseProxy;
	import com.ts.data.model.DataIndex;
	import com.ts.data.model.DataProxy;
	import com.ts.presentation.view.decorationAssets.converters.DataToColorConverter;
	import com.ts.presentation.view.decorationAssets.interpolators.StateColorInterpolator;
	import com.ts.presentation.view.map.controls.PhaseControl;
	import com.ts.presentation.model.map.intersection.IntersectionIndex;
	import com.ts.presentation.model.map.intersection.IntersectionProxy;
	import com.ts.presentation.model.map.phase.PhaseProxy;
	
	import org.puremvc.as3.patterns.proxy.Proxy;
	import com.ts.presentation.model.map.phase.DetectorIndex;
	
	public class NetworkProxy extends Proxy
	{
		public static const NAME:String = "networkProxy";
		
		private const DEFAULT_SIGNAL_DATA_CONVERTER:DataToColorConverter = new DataToColorConverter( new StateColorInterpolator( [ 0 ], [ 0x000000 ] ) );
		private const DEFAULT_SIGNAL_DATA_NAME:String	= "default";
		
		public var backgroundNetworkModel:BackgroundNetworkProxy;
		public var intersectionIndex:IntersectionIndex;
		public var detectorIndex:DetectorIndex;
		
		private var _signalDataConverter:DataToColorConverter;
		private var _signalDataName:String;
		private var _signalDataIndex:Object;
		private var _dataIndex:DataIndex;
		private var _phaseMap:Array;
		private var _currentInterval:int = 0;
		
		public function setSignalDataConverter( signalDataConverter:DataToColorConverter, signalDataName:String ):void
		{
			_signalDataConverter 	= signalDataConverter;
			_signalDataName 		= signalDataName;
			
			_signalDataIndex = new Object();
			
			if( !_dataIndex )
			{
				_dataIndex = DatabaseProxy( facade.retrieveProxy( DatabaseProxy.NAME ) as DatabaseProxy ).dataIndex;
			}
			
			var signalData:Array = _dataIndex.getDataAtName( _signalDataName );
			
			if( signalData )
			{
				for each( var signal:DataProxy in signalData )
				{
					_signalDataIndex[ signal.lodID + "-" + signal.intersection ] = signal.uncompressedData;
				}
			}
			updateNetworkSignals( _currentInterval );
		}
		
		public function clearSignalDataConverter():void
		{
			_signalDataConverter 	= DEFAULT_SIGNAL_DATA_CONVERTER;
			_signalDataName 		= DEFAULT_SIGNAL_DATA_NAME;
			_signalDataIndex 		= null;
			
			updateNetworkSignals( _currentInterval );
		}
		
		public function get intersections():Array{ return intersectionIndex.intersections; }
		public function get phases():Array
		{
			var phases:Array = new Array();
			for each( var intersection:IntersectionProxy in intersectionIndex.intersections )
			{
				phases = phases.concat( intersection.phaseIndex.phases );
			}
			return phases;
		}
		public function get numIntersections():int{ return intersectionIndex.intersections.length; }
		public function get numPhases():int
		{
			var runningSum:int = 0;
			for each( var intersection:IntersectionProxy in intersectionIndex.intersections )
			{
				runningSum += intersection.phaseIndex.phases.length;
			}
			return runningSum;
		}
		
		public function get backgroundNetworkPathData():Array{ return backgroundNetworkModel.networkPathData; }
		public function get systemPhaseLinkData():Array{ return intersectionIndex.systemPhaseLinkData; }
		
		public function NetworkProxy( IntersectionIndx:IntersectionIndex, BackgroundNetworkModel:BackgroundNetworkProxy, DetectorIndx:DetectorIndex )
		{
			super( NAME );
			
			backgroundNetworkModel 	= BackgroundNetworkModel;
			intersectionIndex 		= IntersectionIndx;
			detectorIndex 			= DetectorIndx;
			
			_signalDataConverter 	= DEFAULT_SIGNAL_DATA_CONVERTER;
			_signalDataName 		= DEFAULT_SIGNAL_DATA_NAME;
			
			_phaseMap = new Array();
			for each( var intersection:IntersectionProxy in intersectionIndex.intersections )
			{
				for each( var phase:PhaseProxy in intersection.phaseIndex.phases )
				{
					_phaseMap.push( phase.uniqueID );
				}
			}
		}
		
		public function hasLink( linkNum:int ):Boolean
		{
			return intersectionIndex.hasLink( linkNum );
		}
		
		public function getIntersectionNumAtLink( linkNum:int ):int
		{
			return intersectionIndex.getIntersectionNumAtLink( linkNum );
		}
		
		public function getPhaseNumAtLink( linkNum:int ):int
		{
			return intersectionIndex.getPhaseNumAtLink( linkNum );
		}
		
		public function getPhaseAtLink( linkNum:int ):PhaseProxy
		{
			return intersectionIndex.getPhaseAtLink( linkNum );
		}
		
		public function getPhaseAtUID( UID:String ):PhaseProxy
		{
			return intersectionIndex.getPhaseAtUID( UID );
		}
		
		public function get phaseNumVOs():Array{ return intersectionIndex.phaseNumVOs; }
		
		public function updateInterval( interval:int ):void
		{
			updateNetworkSignals( interval );
		}
		
		private function updateNetworkSignals( interval:int ):void
		{
			if( _signalDataName == DEFAULT_SIGNAL_DATA_NAME )
			{
				for each( var phaseUID:String in _phaseMap )
				{
					getPhaseAtUID( phaseUID ).phaseControl.signalColor = _signalDataConverter.defaultColor;
				}
			}
			else
			{
				var value:Number;
				for( phaseUID in _signalDataIndex )
				{
					value = _signalDataIndex[ phaseUID ][ interval ];
					getPhaseAtUID( phaseUID ).phaseControl.signalColor = isNaN( value ) ? DEFAULT_SIGNAL_DATA_CONVERTER.defaultColor : _signalDataConverter.convertValueToColor( value ); 
				}
			}
		}
	}
}