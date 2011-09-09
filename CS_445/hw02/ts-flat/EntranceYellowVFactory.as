package com.ts.importing.virtualData.railroadPreemption
{
	import com.ts.data.model.DataIndex;
	import com.ts.data.model.DataProxy;
	import com.ts.importing.configuration.model.enumerations.ExternalImportDataName;
	import com.ts.importing.deriveSystem.model.ImportDataIndex;
	import com.ts.importing.virtualData.AbstractVirtualDataFactory;
	import com.ts.presentation.model.map.SystemProxy;

	public class EntranceYellowVFactory extends AbstractVirtualDataFactory
	{
		public static const NAME:String 	= "entranceYellow";
		public const REQUIREMENTS:Array 	= [ ExternalImportDataName.LDP_ENTRANCE_YELLOW ];
		
		public function EntranceYellowVFactory()
		{
			super( NAME, REQUIREMENTS );
		}
		
		override public function deriveData( dataIndex:DataIndex, importIndex:ImportDataIndex, system:SystemProxy ):void
		{
			var entranceYellowData:Array = importIndex.getDataAtName( ExternalImportDataName.LDP_ENTRANCE_YELLOW );
			for each( var dataProxy:DataProxy in entranceYellowData )
			{
				dataIndex.indexDataProxy( dataProxy );
			}
		}
	}
}