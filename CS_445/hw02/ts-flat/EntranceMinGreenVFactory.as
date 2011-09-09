package com.ts.importing.virtualData.railroadPreemption
{
	import com.ts.data.model.DataIndex;
	import com.ts.data.model.DataProxy;
	import com.ts.importing.configuration.model.enumerations.ExternalImportDataName;
	import com.ts.importing.deriveSystem.model.ImportDataIndex;
	import com.ts.importing.virtualData.AbstractVirtualDataFactory;
	import com.ts.presentation.model.map.SystemProxy;

	public class EntranceMinGreenVFactory extends AbstractVirtualDataFactory
	{
		public static const NAME:String 	= "entranceMinGreen";
		public const REQUIREMENTS:Array 	= [ ExternalImportDataName.LDP_ENTRANCE_MIN_GREEN ];
		
		public function EntranceMinGreenVFactory()
		{
			super( NAME, REQUIREMENTS );
		}
		
		override public function deriveData( dataIndex:DataIndex, importIndex:ImportDataIndex, system:SystemProxy ):void
		{
			var entranceMinGreenData:Array = importIndex.getDataAtName( ExternalImportDataName.LDP_ENTRANCE_MIN_GREEN );
			for each( var dataProxy:DataProxy in entranceMinGreenData )
			{
				dataIndex.indexDataProxy( dataProxy );
			}
		}
	}
}