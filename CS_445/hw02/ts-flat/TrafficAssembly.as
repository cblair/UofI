package com.ts.importing.deriveSystem.command
{
	import com.ts.data.enumerations.DataClass;
	import com.ts.data.enumerations.DataRange;
	import com.ts.data.enumerations.DataType;
	import com.ts.data.enumerations.LevelOfDetail;
	import com.ts.data.model.DataConverter;
	import com.ts.data.model.DataIndex;
	import com.ts.data.model.DataProxy;
	import com.ts.data.model.vo.DataVO;
	import com.ts.data.model.vo.HashVO;
	import com.ts.data.model.vo.MetadataVO;
	import com.ts.data.model.vo.StateVO;
	import com.ts.data.model.vo.StatesVO;
	import com.ts.importing.configuration.model.enumerations.ExternalImportDataName;
	import com.ts.importing.configuration.model.enumerations.RequiredImportDataName;
	import com.ts.importing.deriveSystem.model.DataName;
	import com.ts.importing.deriveSystem.model.ImportDataIndex;
	import com.ts.importing.deriveSystem.vo.TrafficHelperDataVO;
	import com.ts.presentation.model.map.TrafficProxy;
	import com.ts.presentation.model.map.vehicle.VehicleProxy;
	import com.ts.presentation.model.map.vehicle.vo.RequiredVehData;
	import com.ts.presentation.model.map.vehicle.vo.VehicleDataIndex;
	import com.ts.presentation.model.map.vehicle.vo.VehicleMetadataVO;
	import com.ts.presentation.model.map.vo.TrafficMetadataVO;
	import com.ts.utilities.StringFormatters;
	import com.ts.utilities.drawing.TrigCalculations;
	
	import flash.geom.Point;

	public class TrafficAssembly
	{
		private static const SUPPORTED_PROPERTY_LIST:Array = [ ExternalImportDataName.FZP_VEH_VELOCITY,
															   ExternalImportDataName.FZP_VEH_DELAY_TIME,
															   ExternalImportDataName.FZP_VEH_ACCEL,
															   ExternalImportDataName.FZP_VEH_QUEUE,
															   ExternalImportDataName.FZP_VEH_TOTAL_Q_TIME ];
		/**
		 * Takes the importDataIndex, calculates additional data proxies to complete the vehicle model. Adds
		 * each new dataProxy to the "dataIndex", as well as any Imported proxies that are ready as is.
		 * ( only the dataProxy will continue on to the application ).
		 * 
		 * @param importDataIndex The helper Index containing the imported data proxies
		 * 
		 * @param dataIndex The nativeDataIndex that will continue on to become the actual data model.
		 * 
		 * @param vehicleHelperData A virtual object containing helper data for calculating the virtual data.
		 * 
		 * @return Returns the nativeDataIndex after the new data proxies have been appended.
		 **/

		public static function buildTrafficModel( importDataIndex:ImportDataIndex, dataIndex:DataIndex, helperData:TrafficHelperDataVO ):TrafficProxy
		{
			dataIndex = createVehInSystemData( importDataIndex.getDataForAllVehicles( RequiredImportDataName.FZP_VEH_WORLD_X ), dataIndex );
			dataIndex = createVehTrajectoryData( importDataIndex, dataIndex, helperData );
			var vehicles:Array = createVehicles( importDataIndex, dataIndex );
			var vehicleProperties:Array = setSupportedProperties( importDataIndex, dataIndex );
			return new TrafficProxy( vehicles, new TrafficMetadataVO( vehicleProperties ) );
		}
		
		private static function createVehInSystemData( vehFrontXList:Array, dataIndex:DataIndex ):DataIndex
		{
			var rawData:Array;
			var workingData:Array;
			var data:DataVO;
			var metadata:MetadataVO;
			
			var inSys:StateVO = new StateVO( "inSystem", "In System", "In Sys.", "Vehicle is in the system", "Vehicle in system", "true", 1, "Vehicle is in the system", null );
			var outSys:StateVO = new StateVO( "outSystem", "Out of System", "Out Sys.", "Vehicle is out of the system", "Vehicle out of system", "false", 0, "Vehicle is out of the system", null );
			var states:StatesVO = new StatesVO( [ outSys, inSys ] );
			
			for each( var dataProxy:DataProxy in vehFrontXList )
			{
				metadata = new MetadataVO( 	DataName.VEH_IN_SYSTEM, DataType.STATE, DataClass.DISCRETE, DataRange.FIXED, "Vehicle In System", "Veh in Sys", "", "", 
											dataProxy.levelOfDetail, dataProxy.lodID, dataProxy.intersection, DataName.VEH_IN_SYSTEM + String( dataProxy.lodID ), dataProxy.aspect, "Flag indicating if a vehicle is in system or out.", "" );
				
				rawData = dataProxy.uncompressedData;
				workingData = new Array();
				for each( var value:Number in rawData )
				{
					workingData.push( isNaN( value ) ? 0 : 1 )
				}
				
				data = DataConverter.dataListToDataVO( workingData, 0, dataProxy.numValues );
				dataIndex.indexDataProxy( DerivedDataProxyFactory.createDataProxy( metadata, data, states ) );
			}
			return dataIndex;
		}
		
		
		private static var _vehLengthMap:Object;
		private static function createVehTrajectoryData( importDataIndex:ImportDataIndex, dataIndex:DataIndex, helperData:TrafficHelperDataVO ):DataIndex
		{
			var worldXData:Array 	= importDataIndex.getDataForAllVehicles( RequiredImportDataName.FZP_VEH_WORLD_X );
			var worldYData:Array 	= importDataIndex.getDataForAllVehicles( RequiredImportDataName.FZP_VEH_WORLD_Y );
			var worldRXData:Array 	= importDataIndex.getDataForAllVehicles( RequiredImportDataName.FZP_VEH_RWORLD_X );
			var worldRYData:Array 	= importDataIndex.getDataForAllVehicles( RequiredImportDataName.FZP_VEH_RWORLD_Y );
			
			var rawXData:Array;
			var rawYData:Array;
			var rawRXData:Array;
			var rawRYData:Array;
			
			var workingPosXData:Array;
			var workingPosYData:Array;
			var workingRotationData:Array;
			
			var posXMetadata:MetadataVO;
			var posYMetadata:MetadataVO;
			var rotationMetadata:MetadataVO;
			
			var posXData:DataVO;
			var posYData:DataVO;
			var rotationData:DataVO;
			
			var xOffset:Number 		= helperData.intersectionAnalysis.topLeftPoint.x;
			var yOffset:Number 		= helperData.intersectionAnalysis.topLeftPoint.y;
			var n:int = worldXData.length;
			var m:int;
			
			var positionPoints:Array = new Array();
			var position:Point;
			var rotation:Number;
			var startPoint:Point;
			var endPoint:Point;
			
			_vehLengthMap = new Object();
			var captureLength:Boolean = true;
			
			for( var i:int = 0; i < n; i++ )
			{				
				rawXData 	= worldXData[ i ].uncompressedData;
				rawYData 	= worldYData[ i ].uncompressedData;
				rawRXData 	= worldRXData[ i ].uncompressedData;
				rawRYData 	= worldRYData[ i ].uncompressedData;
				
				workingPosXData  	= new Array();
				workingPosYData  	= new Array();
				workingRotationData = new Array();
				
				captureLength = true;
				m = worldXData[ i ].numValues;
				for( var j:int = 0; j < m; j++ )
				{
					startPoint 	= new Point( rawXData[ j ],  rawYData[ j ] );
					endPoint 	= new Point( rawRXData[ j ], rawRYData[ j ] );
					position 	= Point.interpolate( startPoint, endPoint, 0.5 );
					rotation 	= TrigCalculations.getAngleInDegrees( startPoint, endPoint );
					
					workingPosXData.push( Math.round( ( position.x - xOffset ) * 10 ) / 10 );
					workingPosYData.push( Math.round( ( position.y - yOffset ) * 10 ) / 10 );
					workingRotationData.push( Math.round( rotation * 10 ) / 10 );
					
					//-- capture the vehicle's length once per j loop. ( once for each vehicle )
					//-- only capture it when the data is not NaN.
					if( captureLength && !isNaN( startPoint.x ) )
					{
						_vehLengthMap[ worldXData[ i ].lodID ] = StringFormatters.fixedNumber( Point.distance( startPoint, endPoint ), 3 );
						captureLength = false;
					}
				}
				
				posXMetadata 	 = new MetadataVO( 	DataName.VEH_POS_X, DataType.ANALOG, DataClass.DYNAMIC, DataRange.UNBOUNDED, "Vehicle X Position", "X Pos", "", "", 
													worldXData[ i ].levelOfDetail, worldXData[ i ].lodID, worldXData[ i ].intersection, DataName.VEH_POS_X + String( worldXData[ i ].lodID ), worldXData[ i ].aspect, "A vehicle's current x point.", "" );
				posYMetadata 	 = new MetadataVO( 	DataName.VEH_POS_Y, DataType.ANALOG, DataClass.DYNAMIC, DataRange.UNBOUNDED, "Vehicle Y Position", "Y Pos", "", "", 
													worldXData[ i ].levelOfDetail, worldXData[ i ].lodID, worldXData[ i ].intersection, DataName.VEH_POS_Y + String( worldXData[ i ].lodID ), worldXData[ i ].aspect, "A vehicle's current y point.", "" );
				rotationMetadata = new MetadataVO( 	DataName.VEH_ROTATION, DataType.ANALOG, DataClass.DYNAMIC, DataRange.UNBOUNDED, "Vehicle Rotation", "Rotation", "", "", 
													worldXData[ i ].levelOfDetail, worldXData[ i ].lodID, worldXData[ i ].intersection, DataName.VEH_ROTATION + String( worldXData[ i ].lodID ), worldXData[ i ].aspect, "A vehicle's current rotation.", "" );
				
				posXData 		= DataConverter.dataListToDataVO( workingPosXData, 		0, worldXData[ i ].numValues );
				posYData 		= DataConverter.dataListToDataVO( workingPosYData, 		0, worldXData[ i ].numValues );
				rotationData 	= DataConverter.dataListToDataVO( workingRotationData, 	0, worldXData[ i ].numValues );
				
				dataIndex.indexDataProxy( DerivedDataProxyFactory.createDataProxy( posXMetadata, 		posXData ) );
				dataIndex.indexDataProxy( DerivedDataProxyFactory.createDataProxy( posYMetadata, 		posYData ) );
				dataIndex.indexDataProxy( DerivedDataProxyFactory.createDataProxy( rotationMetadata, 	rotationData ) );
			}
			return dataIndex;
		}
		
		private static function addSupportedProperty( importDataIndex:ImportDataIndex, dataIndex:DataIndex, dataName:String ):void
		{
			var supportedPropertyList:Array = importDataIndex.getDataAtName( dataName );
			
			for each( var data:DataProxy in supportedPropertyList )
			{
				dataIndex.indexDataProxy( data );
			}
		}
		
		private static function createVehicles( importDataIndex:ImportDataIndex, dataIndex:DataIndex ):Array
		{
			var vehicles:Array = new Array();
			var workingMetadata:VehicleMetadataVO;
			var inSystemValuePattern:Array;
			
			var posXData:Array = dataIndex.getDataAtName( DataName.VEH_POS_X );
			var posYData:Array = dataIndex.getDataAtName( DataName.VEH_POS_Y );
			var rotationData:Array = dataIndex.getDataAtName( DataName.VEH_ROTATION );
			var inSystemData:Array = dataIndex.getDataAtName( DataName.VEH_IN_SYSTEM );
			
			var n:int = posXData.length;
			
			for( var i:int = 0; i < n; i++ )
			{
				//-- Gets the indexes where the vehicle enters and leaves the system
				inSystemValuePattern = DataProxy( inSystemData[ i ] ).changeIndexOf( 1 );
				
				workingMetadata = new VehicleMetadataVO( posXData[ i ].lodID, inSystemValuePattern[ 0 ], inSystemValuePattern[ 1 ], _vehLengthMap[ posXData[ i ].lodID ] );
				vehicles.push( new VehicleProxy( workingMetadata, new RequiredVehData( inSystemData[ i ], posXData[ i ], posYData[ i ], rotationData[ i ] ) ) );
			}
			return vehicles;
		}
		
		private static function setSupportedProperties( importDataIndex:ImportDataIndex, dataIndex:DataIndex ):Array
		{
			var properties:Array = new Array();
			
			for each( var dataName:String in SUPPORTED_PROPERTY_LIST )
			{
				if( importDataIndex.getDataAtName( dataName ).length > 0 )
				{
					properties.push( dataName );
					addSupportedProperty( importDataIndex, dataIndex, dataName );
				}
			}
			return properties;
		}
	}
}