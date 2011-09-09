package com.ts.importing.virtualData.railroadPreemption
{
	import com.ts.data.model.DataIndex;
	import com.ts.data.model.DataProxy;
	import com.ts.importing.configuration.model.enumerations.ExternalImportDataName;
	import com.ts.importing.deriveSystem.model.ImportDataIndex;
	import com.ts.importing.virtualData.AbstractVirtualDataFactory;
	import com.ts.presentation.model.map.SystemProxy;

	public class ActivePhaseVFactory extends AbstractVirtualDataFactory
	{
		public static const NAME:String 	= "activePhase";
		public const REQUIREMENTS:Array 	= [ ExternalImportDataName.LDP_ACTIVE_PHASE ];
		
		public function ActivePhaseVFactory()
		{
			super( NAME, REQUIREMENTS );
		}
		
		override public function deriveData( dataIndex:DataIndex, importIndex:ImportDataIndex, system:SystemProxy ):void
		{
			var activePhaseData:Array = importIndex.getDataAtName( ExternalImportDataName.LDP_ACTIVE_PHASE );
			for each( var dataProxy:DataProxy in activePhaseData )
			{
				dataIndex.indexDataProxy( dataProxy );
			}
		}
	}
}