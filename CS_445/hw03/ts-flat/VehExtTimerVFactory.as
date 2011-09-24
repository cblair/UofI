package com.ts.importing.virtualData.standardTiming
{
	import com.ts.data.model.DataIndex;
	import com.ts.data.model.DataProxy;
	import com.ts.importing.configuration.model.enumerations.ExternalImportDataName;
	import com.ts.importing.deriveSystem.model.ImportDataIndex;
	import com.ts.importing.virtualData.AbstractVirtualDataFactory;
	import com.ts.presentation.model.map.SystemProxy;
	
	public class VehExtTimerVFactory extends AbstractVirtualDataFactory
	{
		public static const NAME:String 	= "vehExtTimer";
		public const REQUIREMENTS:Array 	= [ ExternalImportDataName.LDP_VEH_EXT_TIMER ];
		
		public function VehExtTimerVFactory()
		{
			super( NAME, REQUIREMENTS );
		}
		
		override public function deriveData( dataIndex:DataIndex, importIndex:ImportDataIndex, system:SystemProxy ):void
		{
			var vehExtTimerData:Array = importIndex.getDataAtName( ExternalImportDataName.LDP_VEH_EXT_TIMER );
			for each( var dataProxy:DataProxy in vehExtTimerData )
			{
				dataIndex.indexDataProxy( dataProxy );
			}
		}
	}
}