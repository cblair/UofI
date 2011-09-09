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
	import com.ts.data.model.vo.MetadataVO;
	import com.ts.importing.configuration.model.enumerations.RequiredImportDataName;
	import com.ts.importing.deriveSystem.model.DataName;
	import com.ts.importing.deriveSystem.model.ImportDataIndex;
	import com.ts.importing.deriveSystem.vo.NetworkHelperDataVO;
	import com.ts.importing.deriveSystem.vo.TrafficHelperDataVO;
	import com.ts.importing.parsing.model.FZPParsingPolicy;
	import com.ts.presentation.model.map.NetworkProxy;
	import com.ts.presentation.model.map.SystemProxy;
	import com.ts.presentation.model.map.TrafficProxy;
	import com.ts.presentation.model.map.phase.PhaseProxy;
	import com.ts.presentation.model.map.phase.PhaseVehicleIndex;
	import com.ts.presentation.model.map.vehicle.VehicleProxy;
	import com.ts.presentation.model.map.vo.SystemMetadataVO;
	import com.ts.utilities.StringFormatters;
	
	import flash.geom.Point;

	public class SystemAssembly
	{
		public static function buildNetworkModel( importDataIndex:ImportDataIndex, nativeDataIndex:DataIndex, networkHelperData:NetworkHelperDataVO ):NetworkProxy
		{
			return NetworkAssembly.buildNetworkModel( importDataIndex, nativeDataIndex, networkHelperData );
		}
		
		public static function buildTrafficModel( importDataIndex:ImportDataIndex, nativeDataIndex:DataIndex, trafficHelperData:TrafficHelperDataVO ):TrafficProxy
		{
			return TrafficAssembly.buildTrafficModel( importDataIndex, nativeDataIndex, trafficHelperData );
		}
		
		public static function buildMetadata( importDataIndex:ImportDataIndex, networkModel:NetworkProxy, trafficModel:TrafficProxy, aspectName:String, numIterations:int, timeResolution:Number ):SystemMetadataVO
		{	
			var vehLengthInFt:DataProxy = importDataIndex.getDataAtName( RequiredImportDataName.FZP_VEH_LENGTH )[ 0 ];
			var veh:VehicleProxy = trafficModel.getVehicleAtVehNum( vehLengthInFt.lodID );
			
			var pxPerFt:Number = StringFormatters.fixedNumber( veh.vehLength / vehLengthInFt.maxValue, 5 );
			
			return new SystemMetadataVO( 0, numIterations, networkModel.numIntersections, networkModel.numPhases, trafficModel.numVehicles, 
										 1/timeResolution, pxPerFt, aspectName );
		}
		
		public static function FinalizeSystem( systemModel:SystemProxy, importDataIndex:ImportDataIndex, nativeDataIndex:DataIndex ):void
		{
			var networkModel:NetworkProxy = systemModel.networkModel;
			var trafficModel:TrafficProxy = systemModel.trafficModel;
			var pxPerFt:Number = systemModel.pxPerFt;
			
			
			//-- link all vehicles to phases, and percents along those phases
			
			var vehLinkData:Array 	= importDataIndex.getDataForAllVehicles( RequiredImportDataName.FZP_VEH_LINK );
			var vehLinkPosData:Array= importDataIndex.getDataForAllVehicles( RequiredImportDataName.FZP_VEH_LINK_POS );
			
			var rawLinkData:Array;
			var rawLinkPosData:Array;
			
			var workingVehPhaseData:Array;
			var workingVehPhasePosData:Array;
			var workingVehIntersectionData:Array;
			
			var vehPhaseMetadata:MetadataVO;
			var vehPhasePosMetadata:MetadataVO;
			var vehIntersectionMetadata:MetadataVO;
			
			var vehPhaseData:DataVO;
			var vehPhasePosData:DataVO;
			var vehIntersectionData:DataVO;
			
			var vehPhase:PhaseProxy;
			var vehNum:int;
			var vehicle:VehicleProxy;
			
			var n:int = vehLinkData.length; //-- number of link records = number of vehicles.
			var m:int = systemModel.maxIterations;
			
			var phaseVehData:Object = new Object();
			
			for( var i:int = 0; i < n; i++ )
			{				
				rawLinkData	= vehLinkData[ i ].uncompressedData;
				rawLinkPosData = vehLinkPosData[ i ].uncompressedData;
				
				workingVehPhaseData 		= new Array();
				workingVehPhasePosData		= new Array();
				workingVehIntersectionData 	= new Array();
				
				//m = rawLinkData.length; //-- number of data points in the uncompressed data set.
				for( var j:int = 0; j < m; j++ )
				{
					vehPhase 	= networkModel.getPhaseAtLink( rawLinkData[ j ] );
					vehNum 		= vehLinkData[ i ].lodID;
					vehicle		= trafficModel.getVehicleAtVehNum( vehNum )
					
					if( vehPhase )
					{
						workingVehPhaseData.push( vehPhase.signalNumber );
						workingVehPhasePosData.push( StringFormatters.fixedNumber( vehPhase.getPercentAlongPath( rawLinkPosData[ j ] * pxPerFt, rawLinkData[ j ] ), 4 ) );
						workingVehIntersectionData.push( vehPhase.intersectionNumber );
						
						if( !phaseVehData[ vehPhase.uniqueID ] ) phaseVehData[ vehPhase.uniqueID ] = new PhaseVehicleIndex( m );
						phaseVehData[ vehPhase.uniqueID ].addVehicle( vehicle, j, workingVehPhasePosData[ j ] );
					}
					else
					{
						workingVehPhaseData.push( NaN );
						workingVehPhasePosData.push( NaN );
						workingVehIntersectionData.push( NaN );
					}
				}
				
				vehPhaseMetadata 		= new MetadataVO( 	DataName.VEH_PHASE,   DataType.ANALOG, DataClass.DYNAMIC, DataRange.UNBOUNDED, "Vehicle phase number", "Phase", "", "", 
												    		LevelOfDetail.VEHICLE, vehNum, 0, DataName.VEH_PHASE + String( vehNum ), vehLinkData[ i ].aspect, "A vehicle's current phase.", "" );
				vehPhasePosMetadata 	= new MetadataVO( 	DataName.VEH_PERCENT, DataType.ANALOG, DataClass.DYNAMIC, DataRange.UNBOUNDED, "Vehicle percent along phase", "Percent", "", "", 
															LevelOfDetail.VEHICLE, vehNum, 0, DataName.VEH_PERCENT + String( vehNum ), vehLinkData[ i ].aspect, "A vehicle's current percent along a phase.", "" );
				vehIntersectionMetadata = new MetadataVO( 	DataName.VEH_INTERSECTION, DataType.ANALOG, DataClass.DYNAMIC, DataRange.UNBOUNDED, "Vehicle intersection number", "Intersection", "", "", 
															LevelOfDetail.VEHICLE, vehNum, 0, DataName.VEH_INTERSECTION + String( vehNum ), vehLinkData[ i ].aspect, "A vehicle's current intersection.", "" );
				
				vehPhaseData		= DataConverter.dataListToDataVO( workingVehPhaseData,			0, m );
				vehPhasePosData		= DataConverter.dataListToDataVO( workingVehPhasePosData,		0, m );
				vehIntersectionData	= DataConverter.dataListToDataVO( workingVehIntersectionData,	0, m );
				
				var vehPhaseP:DataProxy 		= DerivedDataProxyFactory.createDataProxy( vehPhaseMetadata,		vehPhaseData );
				var vehPercentP:DataProxy 		= DerivedDataProxyFactory.createDataProxy( vehPhasePosMetadata,		vehPhasePosData );
				var vehIntersectionP:DataProxy 	= DerivedDataProxyFactory.createDataProxy( vehIntersectionMetadata,	vehIntersectionData );
				
				
				nativeDataIndex.indexDataProxy( vehPhaseP );
				nativeDataIndex.indexDataProxy( vehPercentP );
				nativeDataIndex.indexDataProxy( vehIntersectionP );
				
				vehicle.vehiclePhase 		= vehPhaseP;
				vehicle.vehiclePercent 		= vehPercentP;
				vehicle.vehicleIntersection = vehIntersectionP;
			}
			
			trafficModel.buildVehicleLookup();
			
			for( var key:String in phaseVehData )
			{
				networkModel.getPhaseAtUID( key ).phaseVehIndex = phaseVehData[ key ];
			}
		}
	}
}