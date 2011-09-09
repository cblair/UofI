package com.ts.data.model
{
	import com.ts.data.enumerations.LevelOfDetail;
	import com.ts.data.model.DataProxy;

	public class DataIndex
	{
		private const PROXY_TABLE:int = 0;
		private const NAME_TABLE:int = 1;
		private const LOD_NUMBERS:int = 2;
		
		private var _uniqueNameTable:Object;
		
		private var _dataProxyTable:Object;
		private var _dataProxyNames:Array;
		
		private var _vehicleProxyTable:Object;
		private var _vehicleNameTable:Object;
		private var _vehicleNumbers:Array;
		
		private var _phaseProxyTable:Object;
		private var _phaseNameTable:Object;
		private var _phaseNumbers:Array;
		private var _phaseIntersections:Array;
		
		private var _intersectionProxyTable:Object;
		private var _intersectionNameTable:Object;
		private var _intersectionNumbers:Array;
		
		public var dataProxies:Array;
		
		public function get vehicleNumbers():Array{ return _vehicleNumbers; }
		public function get phaseNumbers():Array{ return _phaseNumbers; }
		public function get phaseIntersections():Array{ return _phaseIntersections; }
		public function get itersectionNumbers():Array{ return _intersectionNumbers; }
		public function get dataNames():Array{ return _dataProxyNames; }
		
		public function DataIndex( parsedDataProxies:Array )
		{
			_uniqueNameTable	= new Object();
			_dataProxyTable 	= new Object();
			_dataProxyNames 	= new Array();
			
			_vehicleProxyTable 	= new Object();
			_vehicleNameTable 	= new Object();
			_vehicleNumbers 	= new Array();
			
			_phaseProxyTable 	= new Object();
			_phaseNameTable  	= new Object();
			_phaseNumbers 		= new Array();
			_phaseIntersections = new Array();
			
			_intersectionProxyTable 	= new Object();
			_intersectionNameTable  	= new Object();
			_intersectionNumbers 		= new Array();
			
			dataProxies = new Array();
			
			parsedDataProxies == parsedDataProxies ? parsedDataProxies : new Array();
			
			for each( var dataProxy:DataProxy in parsedDataProxies )
			{
				indexDataProxy( dataProxy );
			}
		}
		
		public function indexDataProxy( dataProxy:DataProxy ):void
		{
			addToDataNameHash( dataProxy );
			addToLodHash( dataProxy, dataProxy.levelOfDetail );
			dataProxies.push( dataProxy );
		}
		
		public function getDataAtUniqueID( dataName:String, lodID:int, intersectionNumber:int ):DataProxy
		{
			return _uniqueNameTable[ dataName + String( lodID ) + String( intersectionNumber ) ]; 
		}
		
		public function getDataAtName( dataName:String ):Array
		{
			var data:Array = _dataProxyTable [ dataName ];
			if( data )
			{
				return data;
			}
			else
			{
				trace( dataName + " not found" );
				return [];
			}
		}
		
		public function getVehicleDataProxy( dataName:String, vehNum:int ):DataProxy 
		{ 
			if( _vehicleNameTable[ vehNum ] )
				return _vehicleProxyTable[ vehNum ][ _vehicleNameTable[ vehNum ].indexOf( dataName ) ];
			
			return null;
		}
		
		public function getPhaseDataProxy( dataName:String, phaseNum:int ):DataProxy 
		{ 
			if( _phaseProxyTable[ phaseNum ] )
				return _phaseProxyTable[ phaseNum ][ _phaseNameTable[ phaseNum ].indexOf( dataName ) ]; 
			
			return null;
		}
		
		public function getIntersectionDataProxy( dataName:String, intNum:int ):DataProxy 
		{ 
			if( _intersectionProxyTable[ intNum ] )
				return _intersectionProxyTable[ intNum ][ _intersectionNameTable[ intNum ].indexOf( dataName ) ]; 
			
			return null;
		}
		
		public function getDataForAllVehicles( dataName:String )		:Array{ return getDataForAllLod( dataName, LevelOfDetail.VEHICLE ); }
		public function getDataForAllPhases( dataName:String )			:Array{ return getDataForAllLod( dataName, LevelOfDetail.PHASE ); }
		public function getDataForAllIntersections( dataName:String )	:Array{ return getDataForAllLod( dataName, LevelOfDetail.INTERSECTION ); }
		
		public function getAllDataNamesAtVehicleNum( vehNum:int )		:Array{ return getAllDataNamesAtLodID( vehNum, LevelOfDetail.VEHICLE ); }
		public function getAllDataNamesAtPhaseNum( phaseNum:int )		:Array{ return getAllDataNamesAtLodID( phaseNum, LevelOfDetail.PHASE ); }
		public function getAllDataNamesAtIntersectionNum( intNum:int )	:Array{ return getAllDataNamesAtLodID( intNum, LevelOfDetail.INTERSECTION ); }
		
		public function getAllDataAtVehicleNum( vehNum:int )		:Array{ return getAllDataAtLodID( vehNum, LevelOfDetail.VEHICLE ); }
		public function getAllDataAtPhaseNum( phaseNum:int )		:Array{ return getAllDataAtLodID( phaseNum, LevelOfDetail.PHASE ); }
		public function getAllDataAtIntersectionNum( intNum:int )	:Array{ return getAllDataAtLodID( intNum, LevelOfDetail.INTERSECTION ); }
		
		private function addToDataNameHash( dataProxy:DataProxy ):void
		{
			_uniqueNameTable[ dataProxy.uniqueID ] = dataProxy;
			
			if( !_dataProxyTable[ dataProxy.dataName ] )
			{
				_dataProxyTable[ dataProxy.dataName ] = new Array();
			}
			_dataProxyTable[ dataProxy.dataName ].push( dataProxy );
			_dataProxyNames.push( dataProxy.dataName );
		}
		
		private function addToLodHash( dataProxy:DataProxy, levelOfDetail:String ):void
		{
			var workingDataIndex:Array = getWorkingDataIndex( levelOfDetail );
			var workingPhaseNum:int = dataProxy.lodID;
			
			if( !workingDataIndex[ PROXY_TABLE ][ workingPhaseNum ] )
			{
				workingDataIndex[ PROXY_TABLE ][ workingPhaseNum ] = new Array();
				workingDataIndex[ NAME_TABLE ][ workingPhaseNum ] = new Array();
				workingDataIndex[ LOD_NUMBERS ].push( dataProxy.lodID );
				if( workingDataIndex[ LOD_NUMBERS ] == _phaseNumbers )
				{
					_phaseIntersections.push( dataProxy.intersection );
				}
			}
			
			workingDataIndex[ PROXY_TABLE ][ workingPhaseNum ].push( dataProxy );
			workingDataIndex[ NAME_TABLE ][ workingPhaseNum ].push( dataProxy.dataName );
		}
		
		private function getDataForAllLod( dataName:String, levelOfDetail:String ):Array
		{
			var workingDataIndex:Array = getWorkingDataIndex( levelOfDetail );
			var proxies:Array = new Array();
			var index:int;
			
			for each( var lodID:Number in workingDataIndex[ LOD_NUMBERS ] )
			{
				index = workingDataIndex[ NAME_TABLE ][ lodID ].indexOf( dataName )
				proxies.push( workingDataIndex[ PROXY_TABLE ][ lodID ][ index ] );
			}
			
			return proxies;
		}
		
		private function getAllDataNamesAtLodID( lodID:int, levelOfDetail:String ):Array
		{
			var workingDataIndex:Array = getWorkingDataIndex( levelOfDetail );
			
			return workingDataIndex[ NAME_TABLE ][ lodID ];
		}
		
		private function getAllDataAtLodID( lodID:int, levelOfDetail:String ):Array
		{
			var workingDataIndex:Array = getWorkingDataIndex( levelOfDetail );
			var proxies:Array = new Array();
			var n:int = workingDataIndex[ NAME_TABLE ][ lodID ].length;
			
			for( var i:int = 0; i < n; i++ )
			{
				proxies.push( workingDataIndex[ PROXY_TABLE ][ i ] );
			}
			
			return proxies;
		}
		
		private function getWorkingDataIndex( levelOfDetail:String ):Array
		{
			switch( levelOfDetail )
			{
				case LevelOfDetail.VEHICLE:
					return [ _vehicleProxyTable, _vehicleNameTable, _vehicleNumbers ];
					
				case LevelOfDetail.PHASE:
					return [ _phaseProxyTable, _phaseNameTable, _phaseNumbers ];
					
				case LevelOfDetail.INTERSECTION:
					return [ _intersectionProxyTable, _intersectionNameTable, _intersectionNumbers ];
					
				default:
					return null;
			}
		}
	}
}