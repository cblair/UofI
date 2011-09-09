package com.ts.presentation.model.map
{
	import com.ts.data.model.DatabaseProxy;
	import com.ts.data.model.DataIndex;
	import com.ts.data.model.DataProxy;
	import com.ts.presentation.model.map.vehicle.VehicleProxy;
	import com.ts.presentation.model.map.vo.TrafficMetadataVO;
	import com.ts.presentation.view.decorationAssets.converters.DataToColorConverter;
	import com.ts.presentation.view.decorationAssets.interpolators.StateColorInterpolator;
	import com.ts.presentation.view.map.controls.VehicleControl;
	
	import flash.geom.PerspectiveProjection;
	
	import org.puremvc.as3.patterns.proxy.Proxy;
	
	public class TrafficProxy extends Proxy
	{
		public static const NAME:String	=	"trafficProxy";
		
		private const DEFAULT_TRAFFIC_DATA_CONVERTER:DataToColorConverter = new DataToColorConverter( new StateColorInterpolator( [ 0 ], [ 0x000000 ] ) );
		private const DEFAULT_TRAFFIC_DATA_NAME:String	= "default";
		
		private var _enterSystemIndex:Object;
		private var _exitSystemIndex:Object;
		private var _vehicleIndex:Object;
		private var _vehicleNumbers:Array;
		private var _vehicleDataHash:Array;
		private var _colorDataHash:Array;
		private var _defColorHash:Array;
		
		private var _trafficDataConverter:DataToColorConverter;
		private var _trafficDataName:String;
		private var _dataIndex:DataIndex;
		private var _currentInterval:int = 0;
		
		private var _selectedPhaseMap:Object = new Object();
		
		public var vehicles:Array;
		public var metadata:TrafficMetadataVO;
		public var numVehicles:int;
		public var numPropsPerVehicle:int = 7;
		public var numPropsPerColor:int = 2;
		
		public function get supportedPropertyNames():Array{ return metadata.supportedProperties; }
		public function set selectedPhases( value:Array ):void
		{
			_selectedPhaseMap = new Object();
			if( value )
			{
				for each( var phaseUID:String in value )
				{
					_selectedPhaseMap[ phaseUID ] = true;
				}
			}
			updateInterval( _currentInterval );
		}
		
		public function setTrafficDataConverter( trafficDataConverter:DataToColorConverter, trafficDataName:String ):void
		{
			_trafficDataConverter 	= trafficDataConverter;
			_trafficDataName 		= trafficDataName;
			
			if( !_dataIndex )
			{
				_dataIndex = DatabaseProxy( facade.retrieveProxy( DatabaseProxy.NAME ) as DatabaseProxy ).dataIndex;
			}
			
			_colorDataHash = new Array();
			for( var i:int = 0; i < _vehicleDataHash.length; i++ )
			{
				_colorDataHash.push( new Array() );
			}
			
			var colorData:DataProxy;
			var rawColorData:Array;
			var inSystemData:Array;
			var value:Number;
			for each( var vehicle:VehicleProxy in vehicles )
			{
				colorData 		= _dataIndex.getVehicleDataProxy( _trafficDataName, vehicle.lodID );
				if( colorData )
				{
					inSystemData 	= vehicle.requiredData.inSystem.uncompressedData;
					rawColorData	= colorData.uncompressedData;
				
					for( i = 0; i < _vehicleDataHash.length; i++ )
					{
						if( inSystemData[ i ] == 1 )
						{
							value = rawColorData[ i ];
							_colorDataHash[ i ].push( value, isNaN( value ) ? DEFAULT_TRAFFIC_DATA_CONVERTER.defaultColor : _trafficDataConverter.convertValueToColor( value ) );
						}
					}
				}	
			}
			
			updateInterval( _currentInterval );
		}
		
		public function clearSignalDataConverter():void
		{
			_trafficDataConverter 	= DEFAULT_TRAFFIC_DATA_CONVERTER;
			_trafficDataName 		= DEFAULT_TRAFFIC_DATA_NAME;
			_colorDataHash			= _defColorHash;
			
			updateInterval( _currentInterval );
		}
		
		public function getVehicleAtVehNum( num:int ):VehicleProxy
		{
			return _vehicleIndex[ num ];
		}
	
		public function TrafficProxy( Vehicles:Array, Metadata:TrafficMetadataVO )
		{
			super( NAME );
			vehicles = Vehicles;
			metadata = Metadata;
			
			//-- if this is a new simulation, add the default name. if this is a loaded simulation, it will already be there.
			if( metadata.supportedProperties.indexOf( DEFAULT_TRAFFIC_DATA_NAME ) == -1 )
			{
				metadata.supportedProperties.push( DEFAULT_TRAFFIC_DATA_NAME );
			}
			
			numVehicles = vehicles.length;
			buildIndex( Vehicles );
		}
		
		private function buildIndex( vehicles:Array ):void
		{
			//-- get the total number of records
			var totalRecords:int = VehicleProxy( vehicles[ 0 ] ).requiredData.inSystem.uncompressedData.length;

			_enterSystemIndex = new Object();
			_exitSystemIndex = new Object();
			_vehicleIndex = new Object();
			_vehicleNumbers = new Array();
			
			var workingEnterIndex:int;
			var workingExitIndex:int;
			
			for each( var vehicle:VehicleProxy in vehicles )
			{
				//-- Build vehicle index
				_vehicleIndex[ vehicle.lodID ] = vehicle;
				_vehicleNumbers.push( vehicle.lodID );
				
				//-- Build enterSystemIndex
				workingEnterIndex = vehicle.enterSystemIndex;
				if( !_enterSystemIndex[ workingEnterIndex ] )
				{
					_enterSystemIndex[ workingEnterIndex ] = new Array();
				}
				
				_enterSystemIndex[ workingEnterIndex ].push( vehicle );
				
				//-- Build exitSystemIndex
				workingExitIndex = vehicle.exitSystemIndex;
				if( !_exitSystemIndex[ workingExitIndex ] )
				{
					_exitSystemIndex[ workingExitIndex ] = new Array();
				}
				
				_exitSystemIndex[ workingExitIndex ].push( vehicle );
			}
		}
		
		public function buildVehicleLookup():void
		{
			//-- get the total number of records
			var totalRecords:int = VehicleProxy( vehicles[ 0 ] ).requiredData.inSystem.uncompressedData.length;
			
			//-- add an empty array to each index of the datahash
			_vehicleDataHash 	= new Array();
			_defColorHash 		= new Array();
			for( var i:int = 0; i < totalRecords; i++ )
			{
				_vehicleDataHash.push( new Array() );
				_defColorHash.push( new Array() );
			}
			
			var vehicleXData:Array;
			var vehicleYData:Array;
			var vehicleRotationData:Array;
			var inSystemData:Array;
			var phaseData:Array;
			var percentData:Array;
			var intersectionData:Array;
			
			for each( var vehicle:VehicleProxy in vehicles )
			{
				vehicleXData 		= vehicle.requiredData.vehXPos.uncompressedData;
				vehicleYData 		= vehicle.requiredData.vehYPos.uncompressedData;
				vehicleRotationData = vehicle.requiredData.vehRotation.uncompressedData;
				inSystemData 		= vehicle.requiredData.inSystem.uncompressedData;
				phaseData			= vehicle.requiredData.phase.uncompressedData;
				percentData			= vehicle.requiredData.percent.uncompressedData;
				intersectionData	= vehicle.requiredData.intersection.uncompressedData;
				
				for( i = 0; i < totalRecords; i++ )
				{
					if( inSystemData[ i ] == 1 )
					{
						_vehicleDataHash[ i ].push( vehicle.vehicleControl,
												    vehicle.lodID,
												    inSystemData[ i ],
												    vehicleXData[ i ],
												    vehicleYData[ i ],
												    vehicleRotationData[ i ],
												    String( phaseData[ i ] ) + "-" + String( intersectionData[ i ] ) );
						
						_defColorHash[ i ].push(    0, 
											        DEFAULT_TRAFFIC_DATA_CONVERTER.defaultColor );
					}
				}
			}
		}
		
		//-- using short var names for optimization
		private var _pT:Object = new Object(); 	//-- previousTraffic
		private var _tempT:Object;				//-- tempTraffic
		private var _cT:Array;					//-- currentTraffic
		private var _cC:Array;					//-- currentColor
		private var _dC:Array;					//-- currentDefaultColor
		private var _cV:VehicleControl;			//-- currentVehicle
		private var _key:int;					//-- previousTrafficKey
		private var _pos:int;					//-- currentHashIndexOffset
		private var _cPos:int;					//-- currentColorHashIndexOffset
		private var _dPos:int;					//-- currentDefaultColorHashIndexOffset
		
		public function updateInterval( interval:int ):void
		{
			_currentInterval = interval;
			_tempT = new Object();
			_cT = _vehicleDataHash[ interval ];
			_cC = _colorDataHash[ interval ];
			_dC = _defColorHash[ interval ];
			
			var n:int = _cT.length / numPropsPerVehicle;
			
			//-- 1. check all new incoming vehicles, check them off the "prev list" as they are encountered ( delete ), and push them onto a "temp list" <-- will become new "prev list";
			for( var i:int = 0; i < n; i++ )
			{
				_pos 	= i * numPropsPerVehicle;
				_cPos	= i * numPropsPerColor;
				_dPos	= i * numPropsPerColor;
				_cV		= _cT[ _pos ];
				_key 	= _cT[ _pos + 1 ]; //-- veh num
				
				//-- vehicle was visible last iteration, and is still visible in this iteration.
				if( _pT[ _key ] )
				{	
					delete _pT[ _key ]; 	
				}
				//-- vehicle was not visible last interation, update visibility
				else
				{
					_cV.visible 		= true;
					_cV.includeInLayout = true;
				}
				
				_tempT[ _key ] 	= _cV;
				
				_cV.x 		= _cT[ _pos + 3 ];
				_cV.y		= _cT[ _pos + 4 ];
				_cV.rotation= _cT[ _pos + 5 ];
				
				if( _selectedPhaseMap[ _cT[ _pos + 6 ] ] )
				{
					_cV.vehColor = _cC[ _cPos + 1 ];
				}
				else
				{
					_cV.vehColor = _dC[ _dPos + 1 ];
				}
			}
			
			//-- 2. remove all vehicles that are no longer in the system ( all remaining keys on the "prev list" )
			for each( var oldV:VehicleControl in _pT )
			{
				oldV.visible = false;
				oldV.includeInLayout = false;
			}
			
			//-- 3. temp list becomes new prev list
			_pT = _tempT;
		}
	}
}