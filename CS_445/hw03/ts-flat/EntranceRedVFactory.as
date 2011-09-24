package com.ts.importing.virtualData.railroadPreemption
{
	import com.ts.data.model.DataIndex;
	import com.ts.data.model.DataProxy;
	import com.ts.importing.configuration.model.enumerations.ExternalImportDataName;
	import com.ts.importing.deriveSystem.model.ImportDataIndex;
	import com.ts.importing.virtualData.AbstractVirtualDataFactory;
	import com.ts.presentation.model.map.SystemProxy;

	public class EntranceRedVFactory extends AbstractVirtualDataFactory
	{
		public static const NAME:String 	= "entranceRed";
		public const REQUIREMENTS:Array 	= [ ExternalImportDataName.LDP_ENTRANCE_RED ];
		
		public function EntranceRedVFactory()
		{
			super( NAME, REQUIREMENTS );
		}
		
		override public function deriveData( dataIndex:DataIndex, importIndex:ImportDataIndex, system:SystemProxy ):void
		{
			var entranceRedData:Array = importIndex.getDataAtName( ExternalImportDataName.LDP_ENTRANCE_RED );
			for each( var dataProxy:DataProxy in entranceRedData )
			{
				dataIndex.indexDataProxy( dataProxy );
			}
		}
	}
}