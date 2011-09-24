package com.ts.importing.virtualData.railroadPreemption
{
	import com.ts.data.model.DataIndex;
	import com.ts.data.model.DataProxy;
	import com.ts.importing.configuration.model.enumerations.ExternalImportDataName;
	import com.ts.importing.deriveSystem.model.ImportDataIndex;
	import com.ts.importing.virtualData.AbstractVirtualDataFactory;
	import com.ts.presentation.model.map.SystemProxy;

	public class PreemptCallVFactory extends AbstractVirtualDataFactory
	{
		public static const NAME:String 	= "preemptCall";
		public const REQUIREMENTS:Array 	= [ ExternalImportDataName.LDP_PREEMPT_CALL ];
		
		public function PreemptCallVFactory()
		{
			super( NAME, REQUIREMENTS );
		}
		
		override public function deriveData( dataIndex:DataIndex, importIndex:ImportDataIndex, system:SystemProxy ):void
		{
			var preemptCallData:Array = importIndex.getDataAtName( ExternalImportDataName.LDP_PREEMPT_CALL );
			for each( var dataProxy:DataProxy in preemptCallData )
			{
				dataIndex.indexDataProxy( dataProxy );
			}
		}
	}
}