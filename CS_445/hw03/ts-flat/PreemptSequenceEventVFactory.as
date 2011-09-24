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
	
	public class PreemptSequenceEventVFactory extends AbstractVirtualDataFactory
	{
		public static const NAME:String 	= "preemptSequenceEvent";
		public const REQUIREMENTS:Array 	= [ ExternalImportDataName.LDP_SIG_TIME,
												ExternalImportDataName.LDP_ACTIVE_PREEMPT];
		
		public function PreemptSequenceEventVFactory()
		{
			super( NAME, REQUIREMENTS );
		}
		
		override public function deriveData(dataIndex:DataIndex, importIndex:ImportDataIndex, system:SystemProxy):void
		{
			var simTime:Array			= importIndex.getDataAtName( ExternalImportDataName.LDP_SIG_TIME );
			var activePreempt:Array		= importIndex.getDataAtName( ExternalImportDataName.LDP_ACTIVE_PREEMPT );
			var sigDisp:Array			= importIndex.getDataAtName( ExternalImportDataName.LDP_VAP_SIG_DISPLAY );
			var gateSigDisp:Array		= importIndex.getDataAtName( ExternalImportDataName.LDP_GATE_SIGNAL );
			var trainSigDisp:Array;
			
			if( simTime && sigDisp && gateSigDisp && activePreempt )
			{
				simTime 		= simTime[0].uncompressedData;
				activePreempt 	= activePreempt[0].uncompressedData;
				trainSigDisp 	= sigDisp[0].uncompressedData;
				gateSigDisp		= gateSigDisp[0].uncompressedData;
			}
			
			
			
			var valueCollection:Array;
			var numberOfPhases:int = importIndex.phaseNumbers.length;
			var m:int;
			var n:int = numberOfPhases;
			var checkInDetected:Boolean = false;
			var gatesAreClosed:Boolean = false;
			var trainHasArrived:Boolean = false;
			var checkOutDetected:Boolean = false;
			
			for( var i:int = 0; i < n; i++ )
			{
				m = simTime.length;
				valueCollection = new Array();
				
				for( var j:int = 0; j < m; j++ )
				{
					if( checkInDetected == false && !isNaN( activePreempt[ j ] ) )
					{
						checkInDetected = true;
					}
					if( trainSigDisp[ j ] == 1 )
					{
						if( trainHasArrived == false )
						{
							trainHasArrived = true;
						}
					}
					if( checkInDetected == true && isNaN( activePreempt[ j ] ) )
					{
						if( checkOutDetected == false )
						{
							checkOutDetected = true;
						}
					}
					
					
					if( gateSigDisp[ j ] == 3 )
					{
						valueCollection.push( 2 );
						if( gatesAreClosed == false )
						{
							gatesAreClosed = true;
						}
					}
					else if( gatesAreClosed && checkInDetected && !trainHasArrived )
					{
						valueCollection.push( 2 );
					}
					else if( !isNaN( activePreempt[ j ] ) && trainSigDisp[ j ] != 0 && gatesAreClosed == false )
					{
						//-- 1 - CHECK IN
						valueCollection.push( 1 );
					}
					else if( trainSigDisp[ j ] == 1 )
					{
						//-- 3 - TRAIN ARRIVES
						valueCollection.push( 3 );
					}
					else if( gatesAreClosed && checkInDetected && trainHasArrived && !checkOutDetected )
					{
						valueCollection.push( 3 );
					}
					else if( checkInDetected == true && isNaN( activePreempt[ j ] ) )
					{
						//-- 4 - CHECK OUT
						valueCollection.push( 4 );
					}
					else
					{
						valueCollection.push( NaN );
					}
				}
				
				checkInDetected = false;
				gatesAreClosed = false;
				trainHasArrived = false;
				checkOutDetected = false;
				
				var metadata:MetadataVO = new MetadataVO( RailroadPreemptionDataName.PREEMPT_SEQUENCE_EVENT, DataType.STATE, DataClass.DISCRETE, DataRange.FIXED, "Railroad Preemption Sequence Event", "PRE.S.E", "", "",
					LevelOfDetail.PHASE, importIndex.phaseNumbers[ i ], importIndex.phaseIntersections[ i ], RailroadPreemptionDataName.PREEMPT_SEQUENCE_EVENT + importIndex.phaseNumbers[ i ] + importIndex.phaseIntersections[ i ], "", "", "" );
				
				var maxData:DataVO = DataConverter.dataListToDataVO( valueCollection, 0, m );
				
				var checkIn:StateVO = new StateVO( "checkIn", "Check In", "Ch.In", "Train Check In", "Check In", "Check In", 1, "Check In", "" );
				var gatesClose:StateVO = new StateVO( "gatesClose", "Gates Close", "Gt.Cls", "Gates Close", "Gates Close", "Gates Close", 2, "Gates Close", "" );
				var trainArrives:StateVO = new StateVO( "trainArrives", "Train Arrives", "Tr.Arr", "Train arrives", "Train Arr.", "Train Arrives", 3, "Train Arrives", "" );
				var checkOut:StateVO = new StateVO( "checkOut", "Check Out", "Ch.Out", "Train Check Out", "Check Out", "Check Out", 4, "Check Out", "" );
				
				var states:StatesVO = new StatesVO( [ checkIn, gatesClose, trainArrives, checkOut ] );
				
				dataIndex.indexDataProxy( DerivedDataProxyFactory.createDataProxy( metadata, maxData, states ) );
			}
		}
	}
}