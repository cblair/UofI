package com.ts.importing.virtualData.railroadPreemption
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
	import com.ts.importing.deriveSystem.model.RailroadPreemptionDataName;
	import com.ts.importing.deriveSystem.model.VolumeDensityDataName;
	import com.ts.importing.virtualData.AbstractVirtualDataFactory;
	import com.ts.presentation.model.map.SystemProxy;
	
	public class PreemptSequenceStateVFactory extends AbstractVirtualDataFactory
	{
		public static const NAME:String 	= "preemptSequenceState";
		public const REQUIREMENTS:Array 	= [ ExternalImportDataName.LDP_SIG_TIME,
												ExternalImportDataName.LDP_ENTRANCE_MIN_GREEN,
												ExternalImportDataName.LDP_ENTRANCE_RED,
												ExternalImportDataName.LDP_ENTRANCE_YELLOW,
												ExternalImportDataName.LDP_TRACK_CLEAR_MIN_GRN,
												ExternalImportDataName.LDP_DETECTOR,
												ExternalImportDataName.LDP_ACTIVE_PREEMPT,
												ExternalImportDataName.LDP_TIME_B4_REDUCE ];
		
		public function PreemptSequenceStateVFactory()
		{
			super( NAME, REQUIREMENTS );
		}
		
		override public function deriveData( dataIndex:DataIndex, importIndex:ImportDataIndex, system:SystemProxy ):void
		{
			var simTime:Array			= importIndex.getDataAtName( ExternalImportDataName.LDP_SIG_TIME );
			var forLOD:DataProxy 		= importIndex.getDataAtName( ExternalImportDataName.LDP_GATE_SIGNAL )[ 0 ];
			var entranceMinG:Array 		= importIndex.getDataAtName( ExternalImportDataName.LDP_ENTRANCE_MIN_GREEN );
			var entranceYellow:Array 	= importIndex.getDataAtName( ExternalImportDataName.LDP_ENTRANCE_YELLOW );
			var entranceRed:Array 		= importIndex.getDataAtName( ExternalImportDataName.LDP_ENTRANCE_RED );
			var trackClearMinG:Array 	= importIndex.getDataAtName( ExternalImportDataName.LDP_TRACK_CLEAR_MIN_GRN );
			var gateSignal:Array 		= importIndex.getDataAtName( ExternalImportDataName.LDP_GATE_SIGNAL );
			var activePhase1:Array		= [ importIndex.getPhaseDataProxy( ExternalImportDataName.LDP_ACTIVE_PHASE, 1 ) ];
			var activePhase2:Array		= [ importIndex.getPhaseDataProxy( ExternalImportDataName.LDP_ACTIVE_PHASE, 2 ) ];
			
			if( simTime && entranceMinG && entranceYellow && entranceRed && trackClearMinG && gateSignal && activePhase1 && activePhase2 )
			{
				simTime 		= simTime[0].uncompressedData;
				entranceMinG 	= entranceMinG[0].uncompressedData;
				entranceYellow 	= entranceYellow[0].uncompressedData;
				entranceRed		= entranceRed[0].uncompressedData;
				trackClearMinG 	= trackClearMinG[0].uncompressedData;
				gateSignal 		= gateSignal[0].uncompressedData;
				activePhase1 	= activePhase1[0].uncompressedData;
				activePhase2 	= activePhase2[0].uncompressedData;
			}
			
			for( var  i:int = 0; i < activePhase1.length; i++ )
			{
				if( isNaN( activePhase1[ i ] ) )
				{
					activePhase1[ i ] = -1;
				}
				
				if( isNaN( activePhase2[ i ] ) )
				{
					activePhase2[ i ] = -1;
				}
			}
			
			
			
			var valueCollection:Array;
			var numberOfPhases:int = importIndex.phaseNumbers.length;
			var m:int;
			var n:int = numberOfPhases;
			var isNull:Boolean;
			var clearPhases:Array = new Array();
			var dwellPhases:Array = new Array();
			var exitPhases:Array = new Array();
			
			
			for( i = 0; i < n; i++ )
			{
				m = simTime.length;
				valueCollection = new Array();
				var captureClearPhases:Boolean = false;
				
				for( var j:int = 0; j < m; j++ )
				{
					/*isNull = false;
					if( 
						isNaN( entranceMinG[ j ] ) && 
						isNaN( entranceRed[ j ] ) && 
						isNaN( entranceYellow[ j ] ) &&
						isNaN( trackClearMinG[ j ] ) )
					{
						isNull = true;
					}
					
					if( isNull )
					{
						valueCollection.push( NaN );
					}
					else
					{*/
					if( ( !isNaN( entranceMinG[ j ] )  || !isNaN( entranceRed[ j ] )  || !isNaN( entranceYellow[ j ] ) ) && isNaN( trackClearMinG[ j ] ) )
					{
						valueCollection.push( 1 );
					}
					else if ( !isNaN(trackClearMinG[ j ] ) || captureClearPhases == true ) // || ( activePhase1 [ j ] == clearPhases[ 0 ] && activePhase2 == clearPhases[ 1 ] ) )
					{
						captureClearPhases = true;
						if( isNaN(trackClearMinG[ j ] ) )
						{
							captureClearPhases = false;
							if( activePhase1[ j ] == clearPhases[ 0 ] && activePhase2[ j ] == clearPhases[ 1 ] )
							{
								valueCollection.push( 2 );
								clearPhases = [ activePhase1[ j ], activePhase2[ j ] ];
							}
							else
							{
								valueCollection.push( 3 );
								dwellPhases = [ activePhase1[ j ], activePhase2[ j ] ];
							}
						}
						else
						{
							valueCollection.push( 2 );
							clearPhases = [ activePhase1[ j ], activePhase2[ j ] ];
						}
					}
					else if( activePhase1[ j ] == clearPhases[ 0 ] && activePhase2[ j ] == clearPhases[ 1 ] )
					{
						valueCollection.push( 2 );
						if( activePhase1.length > j + 1 )
						{
							dwellPhases = [ activePhase1[ j + 1 ], activePhase2[ j + 1 ] ];
						}
					}
					else if( activePhase1[ j ] == dwellPhases[ 0 ] && activePhase2[ j ] == dwellPhases[ 1 ] )
					{
						valueCollection.push( 3 );
						if( activePhase1.length > j + 1 )
						{
							exitPhases = [ activePhase1[ j + 1 ], activePhase2[ j + 1 ] ];
						}
					}
					else if( activePhase1[ j ] == exitPhases[ 0 ] && activePhase2[ j ] == exitPhases[ 1 ] )
					{
						valueCollection.push( 4 );
					}
					/*else if ( dwellPhases.length > 0 && ( activePhase1 [ j ] != clearPhases[ 0 ] && activePhase2[ j ] != clearPhases[ 1 ] ) && ( activePhase1 [ j ] != dwellPhases[ 0 ] && activePhase2[ j ] != dwellPhases[ 1 ] ) )
					{
						valueCollection.push( 4 );
					}*/
					else
					{
						valueCollection.push( NaN );
					}
				}
				
				dwellPhases = new Array();
				clearPhases = new Array();
				exitPhases = new Array();
				isNull = false;
				
				var metadata:MetadataVO = new MetadataVO( RailroadPreemptionDataName.PREEMPT_SEQUENCE_STATE, DataType.STATE, DataClass.DISCRETE, DataRange.FIXED, "Railroad Preemption Sequence State", "PRE.S.S", "", "",
					LevelOfDetail.PHASE, importIndex.phaseNumbers[ i ], importIndex.phaseIntersections[ i ], RailroadPreemptionDataName.PREEMPT_SEQUENCE_STATE + importIndex.phaseNumbers[ i ] + importIndex.phaseIntersections[ i ], forLOD.aspect, "", "" );
				
				var maxData:DataVO = DataConverter.dataListToDataVO( valueCollection, 0, m );
				
				var rightOfWay:StateVO = new StateVO( "rightOfWayTransfer", "Right of Way Transfer", "RoW", "Right of Way Transfer/Entrance", "Right of Way Trans.", "Right of Way Trans.", 1, "Right of Way Transfer/Entrance", null );
				var trackClearance:StateVO = new StateVO( "trackClearance", "Track Clearance", "T.Clear", "Track Clearance", "Track Clearance", "Track Clearance", 2, "Track Clearance", null);
				var dwell:StateVO = new StateVO( "Dwell", "Dwell Phase", "Dwell", "Dwell Phase", "Dwell", "Dwell", 3, "Dwell Phase", null );
				var exit:StateVO = new StateVO( "Exit", "Exit Phase", "Exit", "Exit Phase", "Exit", "Exit", 4, "Exit Phase", null );
				
				var states:StatesVO = new StatesVO( [ rightOfWay, trackClearance, dwell, exit ] );
				
				dataIndex.indexDataProxy( DerivedDataProxyFactory.createDataProxy( metadata, maxData, states ) );
			}
		}
	}
}