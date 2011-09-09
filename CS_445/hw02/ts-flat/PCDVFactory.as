package com.ts.importing.virtualData.signalCoordination
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
	import com.ts.importing.configuration.model.enumerations.ExternalImportDataName;
	import com.ts.importing.deriveSystem.command.DerivedDataProxyFactory;
	import com.ts.importing.deriveSystem.model.ImportDataIndex;
	import com.ts.importing.deriveSystem.model.SignalCoordinationDataName;
	import com.ts.importing.virtualData.AbstractVirtualDataFactory;
	import com.ts.presentation.model.map.SystemProxy;

	public class PCDVFactory extends AbstractVirtualDataFactory
	{
		public static const NAME:String 	= "PCDVFactory";
		public const REQUIREMENTS:Array 	= [ ExternalImportDataName.MER_DATA_COL_POINT,
												ExternalImportDataName.MER_LINK_NUM,
												ExternalImportDataName.MER_DATA_CP,
												ExternalImportDataName.MER_T_ENTER,
												ExternalImportDataName.MER_VEH_NO ]
		
		public function PCDVFactory()
		{
			super( NAME, REQUIREMENTS );
		}
		
		override public function deriveData( dataIndex:DataIndex, importIndex:ImportDataIndex, system:SystemProxy ):void
		{
			var headerCP:Array 		= importIndex.getDataAtName( ExternalImportDataName.MER_DATA_COL_POINT );
			var headerLink:Array 	= importIndex.getDataAtName( ExternalImportDataName.MER_LINK_NUM );
			var collectionP:Array 	= importIndex.getDataAtName( ExternalImportDataName.MER_DATA_CP );
			var vehNum:Array 		= importIndex.getDataAtName( ExternalImportDataName.MER_VEH_NO );
			var time:Array 			= importIndex.getDataAtName( ExternalImportDataName.MER_T_ENTER );
			
			//-- will terminate derivator if all required assets are not defined.
			if( !( headerCP[ 0 ] && headerLink[ 0 ] && collectionP[ 0 ] && vehNum[ 0 ] && time[ 0 ] ) ) return;
			
			headerCP 		= headerCP[ 0 ].uncompressedData;
			headerLink 		= headerLink[ 0 ].uncompressedData;
			collectionP 	= collectionP[ 0 ].uncompressedData;
			vehNum 			= vehNum[ 0 ].uncompressedData;
			time 			= time[ 0 ].uncompressedData;
			
			var cpMap:Object = new Object();
			
			var phaseNumbers:Array = system.phaseNumVOs;
			for( var i:int = 0; i < headerCP.length; i++ )
			{
				cpMap[ headerCP[ i ] ] = [ system.getPhaseNumAtLink( headerLink[ i ] ), system.getIntersectionNumAtLink( headerLink[ i ] ) ];
			}
			
			var workingCP:Array = new Array();
			var workingVehNum:Array = new Array();
			var workingTime:Array = new Array();
			
			for( i = 0; i < time.length; i ++ )
			{
				if( time[ i ] != -1 )
				{
					workingCP.push( collectionP[ i ] );
					workingVehNum.push( vehNum[ i ] );
					workingTime.push( Math.round( time[ i ] * 10 ) );
				}
			}
			
			for( i = 0; i < phaseNumbers.length; i ++ )
			{
				var tempTime:Array = new Array();
				var tempVehNum:Array = new Array();
				
				for( var j:int = 0; j < workingCP.length; j ++ )
				{
					if( cpMap[ workingCP[ j ] ][ 0 ] == phaseNumbers[ i ].phaseNum && cpMap[ workingCP[ j ] ][ 1 ] == phaseNumbers[ i ].intersectionNum )
					{
						tempTime.push( workingTime[ j ] );
						tempVehNum.push( workingVehNum[ j ] );
					}
				}
				
				if( tempTime.length > 0 )
				{
					var detTimeMetaData:MetadataVO = new MetadataVO( 	SignalCoordinationDataName.DETECTION_TIME, DataType.ANALOG, DataClass.DYNAMIC, DataRange.UNBOUNDED, 
																		"Detection Time", "Det T", "Time of Detection", "Detection Time", 
																		LevelOfDetail.PHASE, phaseNumbers[ i ].phaseNum, phaseNumbers[ i ].intersectionNum, 
																		SignalCoordinationDataName.DETECTION_TIME +  String(phaseNumbers[ i ].phaseNum) + String(phaseNumbers[ i ].intersectionNum),
																		"", "", "");
					var detTimeData:DataVO = DataConverter.dataListToDataVO( tempTime, 0, tempTime.length );
					var detVehNumMetaData:MetadataVO = new MetadataVO( 	SignalCoordinationDataName.DETECTION_VEH_NUM, DataType.ANALOG, DataClass.DYNAMIC, DataRange.UNBOUNDED, 
																		"Detection Vehicle Number", "Det V", "Vehicle Number Detected", "Vehicle Detected", 
																		LevelOfDetail.PHASE, phaseNumbers[ i ].phaseNum, phaseNumbers[ i ].intersectionNum, 
																		SignalCoordinationDataName.DETECTION_VEH_NUM +  String(phaseNumbers[ i ].phaseNum) + String(phaseNumbers[ i ].intersectionNum),
																		"", "", "");
					var detVehNumData:DataVO = DataConverter.dataListToDataVO( tempVehNum, 0, tempVehNum.length );
					
					var detTimeDataProxy:DataProxy = DerivedDataProxyFactory.createDataProxy( detTimeMetaData, detTimeData );
					var detVehNumDataProxy:DataProxy = DerivedDataProxyFactory.createDataProxy( detVehNumMetaData, detVehNumData );
					
					dataIndex.indexDataProxy( detTimeDataProxy );
					dataIndex.indexDataProxy( detVehNumDataProxy );
				}
			}
			
			
			
			/*
			//-- 1. build the MetadataVO
			posXMetadata 	= new MetadataVO( 	RequiredDataName.VEH_POS_X, DataType.ANALOG, DataClass.DYNAMIC, DataRange.UNBOUNDED, "Vehicle X Position", "X Pos", "", "", 
												worldXData[ i ].levelOfDetail, worldXData[ i ].lodID, worldXData[ i ].intersection, RequiredDataName.VEH_POS_X + String( worldXData[ i ].lodID ), worldXData[ i ].aspect, "A vehicle's current x point.", "" );
			//-- 2. build the dataVO
			posXData 		= DataConverter.dataListToDataVO( workingPosXData, 0, worldXData[ i ].numValues );
			
			//-- 3. build the dataProxy from the MetadataVO & DataVO
			var dataProxy:DataProxy = DerivedDataProxyFactory.createDataProxy( posXMetadata, posXData ) );
			 
			//-- 4. add the dataProxy to the index
			dataIndex.indexDataProxy( dataProxy )
			*/
		}
	}
}