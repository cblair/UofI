package com.ts.importing.virtualData.volumeDensity
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
	import com.ts.data.model.vo.StateVO;
	import com.ts.data.model.vo.StatesVO;
	import com.ts.importing.configuration.model.enumerations.ExternalImportDataName;
	import com.ts.importing.deriveSystem.command.DerivedDataProxyFactory;
	import com.ts.importing.deriveSystem.model.ImportDataIndex;
	import com.ts.importing.deriveSystem.model.VolumeDensityDataName;
	import com.ts.importing.virtualData.AbstractVirtualDataFactory;
	import com.ts.presentation.model.map.SystemProxy;
	
	public class GapInEffectStateVFactory extends AbstractVirtualDataFactory
	{
		public static const NAME:String 	= "gapInEffectState";
		public const REQUIREMENTS:Array 	= [ ExternalImportDataName.LDP_SIG_DISPLAY,
												ExternalImportDataName.LDP_TIME_TO_REDUCE, 
												ExternalImportDataName.LDP_TIME_B4_REDUCE ];
		
		public function GapInEffectStateVFactory()
		{
			super( NAME, REQUIREMENTS );
		}
		
		override public function deriveData(dataIndex:DataIndex, importIndex:ImportDataIndex, system:SystemProxy):void
		{
			var tb4rData:Array = importIndex.getDataAtName( ExternalImportDataName.LDP_TIME_B4_REDUCE );
			var ttrData:Array = importIndex.getDataAtName( ExternalImportDataName.LDP_TIME_TO_REDUCE );
			var sigDisplayData:Array = importIndex.getDataAtName( ExternalImportDataName.LDP_SIG_DISPLAY );
			/*
			var longestArray:Array = new Array();
			
			for each( var dataArray:Array in new Array( [ tb4rData, ttrData, sigDisplayData ] ) )
			{
				if( dataArray.length >  longestArray.length )
				{
					longestArray = dataArray;
					n = dataArray.length;
				}
			}*/
				
			var valueCollection:Array;
			var longestArray:Array = tb4rData.length >= ttrData.length ? tb4rData : ttrData;
			var m:int;
			var n:int = longestArray.length;
			
			
			for( var i:int = 0; i < n; i++ )
			{
				var workingTB4RData:DataProxy = importIndex.getPhaseDataProxy( ExternalImportDataName.LDP_TIME_B4_REDUCE, longestArray[ i ].lodID );
				var workingTTRData:DataProxy = importIndex.getPhaseDataProxy( ExternalImportDataName.LDP_TIME_TO_REDUCE, longestArray[ i ].lodID );
				var workingSigDisplay:DataProxy = importIndex.getPhaseDataProxy( ExternalImportDataName.LDP_SIG_DISPLAY, longestArray[ i ].lodID );
				
				if( workingTTRData && workingTTRData && workingSigDisplay )
				{
					var uncompressedTB4R:Array = workingTB4RData.uncompressedData;
					var uncompressedTTR:Array = workingTTRData.uncompressedData;
					var uncompressedSigDisplay:Array = workingSigDisplay.uncompressedData;
					
					m = uncompressedSigDisplay.length;
					valueCollection = new Array();
					
					for( var j:int = 0; j < m; j++ )
					{
						if( uncompressedSigDisplay[ j ] != 1 )
						{
							valueCollection.push( NaN );
						}
						else
						{
							if( !isNaN( uncompressedTB4R[ j ] ) && isNaN( uncompressedTTR[ j ] ) )
							{
								valueCollection.push( 1 );
							}
							else if( !isNaN( uncompressedTTR[ j ] ) && isNaN( uncompressedTB4R[ j ] ) )
							{
								valueCollection.push( 2 );
							}
							else if( isNaN( uncompressedTTR[ j ] ) && isNaN( uncompressedTB4R[ j ] ) )
							{
								valueCollection.push( 3 );
							}
						}
					}
					
					var metadata:MetadataVO = new MetadataVO( VolumeDensityDataName.GAP_IN_EFFECT_STATE, DataType.STATE, DataClass.DISCRETE, DataRange.FIXED, "Gap In Effect State", "GIE.S", "", "",
						LevelOfDetail.PHASE, workingSigDisplay.lodID, workingSigDisplay.intersection, VolumeDensityDataName.GAP_IN_EFFECT_STATE + workingSigDisplay.lodID + workingSigDisplay.intersection, workingSigDisplay.aspect, "", "" );
					
					var maxData:DataVO = DataConverter.dataListToDataVO( valueCollection, 0, m );
					
					//var nullState:StateVO = new StateVO( "null", "null", "null", "null", "null", "null", 0, "null", null );
					var beforeReduction:StateVO = new StateVO( "beforeReduction", "Time Before Reduction", "T.B.R.", "Time Before Gap Reduction", "Time Before Reduction", "Before Reduction", 1, "The Veh Ext timer has not yet started to reduce", null );
					var reducing:StateVO = new StateVO( "reducing", "Veh Ext Timer Reducing", "Rdc", "Veh Ext Timer is reducing", "Veh Ext timer reducing", "Reducing", 2, "The Veh Ext timer is being reduced", null );
					var afterReduction:StateVO = new StateVO( "afterReduction", "After Reduction", "T.A.R.", "Time After Gap Reduction", "After gap reduction", "After Reduction", 3, "The Veh Ext timer has finished being reduced", null );
					
					var states:StatesVO = new StatesVO( [ beforeReduction, reducing, afterReduction ] );
					
					dataIndex.indexDataProxy( DerivedDataProxyFactory.createDataProxy( metadata, maxData, states ) );
				}
			}
		}
	}
}