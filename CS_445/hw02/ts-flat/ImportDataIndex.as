package com.ts.importing.deriveSystem.model
{
	import com.ts.data.enumerations.LevelOfDetail;
	import com.ts.data.model.DataProxy;
	
	/**
	 * The importDataIndex only exists to help calculate virtual data proxies during import. After the model is ready, the importDataIndex is trashed.
	 * During the import process, the application maintaines 2 indexes: the importDataIndex, and the nativeDataIndex. The importDataIndex is populated with
	 * data proxies fresh from the parsing policies. Any raw import proxies that need to perpetuate to the nativeDataIndex must be transferred manually.
	 * 
	 * <p>The index optimizes direct lookup by data name, as well as levelOfDetail.</p>
	 * 
	 * <p>the proxies stored in each level of detail can be accessed individually ( by providing the levelOfDetail string, and a lodID index ).
	 * they can also be batch accessed, for instance, if a list of 1 type of proxy was needed for each item in a level of detail ( ex. the vehPosY for every vehicle ).</p>
	 * 
	 * <p>A master array of every dataName is accessable as well as lists containing the vehicle, phase, and intersection numbers.</p>
	 * 
	 * <p>Note, because multiple data proxies exist with the same dataName ( i.e. vehPosY ), dataNames are stored as a proxy's name combined with its lodID. The
	 * default lodID is 1, and this is set as the default, so only the dataName is required for unique proxies. Non unique will always return the first lodID of a type,
	 * unless another lodID is supplied. When requiring "levelOfDetail" data, it is best to use the levelOfDetail accessors. ( i.e. get vehicleDataProxy vs getDataProxy</p>
	 * 
	 **/
	public class ImportDataIndex
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
		
		public var importDataProxies:Array;
		
		/**
		 * An Array list of all indexed vehicle numbers ( lodID's for the "vehicle" level of detail ).
		 * ( Useful to dynamically access vehicle data )
		 **/
		public function get vehicleNumbers():Array{ return _vehicleNumbers; }
		
		/**
		 * An Array list of all indexed phase numbers ( lodID's for the "phase" level of detail )
		 * ( Useful to dynamically access phase data )
		 **/
		public function get phaseNumbers():Array{ return _phaseNumbers; }
		
		public function get phaseIntersections():Array{ return _phaseIntersections; }
		
		/**
		 * An Array list of all indexed intersection numbers ( lodID's for the "intersection" level of detail )
		 * ( Useful to dynamically access intersection data )
		 **/
		public function get itersectionNumbers():Array{ return _intersectionNumbers; }
		
		/**
		 * An Array list of all indexed dataNames + lodID ( i.e. vehPosX13 )
		 * ( Useful to dynamically access the entire data model )
		 **/
		public function get dataNames():Array{ return _dataProxyNames; }
		
		/**
		 * Constructor
		 * 
		 * @param parsedDataProxies The raw array of dataProxies generated from the ImportParserProxy
		 **/
		public function ImportDataIndex( parsedDataProxies:Array )
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
			
			importDataProxies = new Array()
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
			importDataProxies.push( dataProxy );
		}
		
		public function getDataAtUniqueID( uniqueID:String ):DataProxy
		{
			return _uniqueNameTable[ uniqueID ]; 
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
					trace( dataProxy.levelOfDetail + ", " + String( dataProxy.lodID ) + ", " + String( dataProxy.intersection ) );
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