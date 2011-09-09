package com.ts.importing.virtualData.volumeDensity
{
	import com.ts.data.model.DataIndex;
	import com.ts.data.model.DataProxy;
	import com.ts.importing.configuration.model.enumerations.ExternalImportDataName;
	import com.ts.importing.deriveSystem.model.ImportDataIndex;
	import com.ts.importing.virtualData.AbstractVirtualDataFactory;
	import com.ts.presentation.model.map.SystemProxy;
	
	public class TimeToReduceVFactory extends AbstractVirtualDataFactory
	{
		public static const NAME:String 	= "timeToReduce";
		public const REQUIREMENTS:Array 	= [ ExternalImportDataName.LDP_TIME_TO_REDUCE ];
		
		public function TimeToReduceVFactory()
		{
			super( NAME, REQUIREMENTS );
		}
		
		override public function deriveData( dataIndex:DataIndex, importIndex:ImportDataIndex, system:SystemProxy ):void
		{
			var timeToReduceData:Array = importIndex.getDataAtName( ExternalImportDataName.LDP_TIME_TO_REDUCE );
			for each( var dataProxy:DataProxy in timeToReduceData )
			{
				dataIndex.indexDataProxy( dataProxy );
			}
		}
	}
}