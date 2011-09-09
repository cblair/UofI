package com.ts.importing.virtualData.required
{
	import com.ts.data.model.DataIndex;
	import com.ts.data.model.DataProxy;
	import com.ts.importing.configuration.model.enumerations.ExternalImportDataName;
	import com.ts.importing.deriveSystem.model.ImportDataIndex;
	import com.ts.importing.virtualData.AbstractVirtualDataFactory;
	import com.ts.presentation.model.map.SystemProxy;
	
	public class MinGreenVFactory extends AbstractVirtualDataFactory
	{
		public static const NAME:String 	= "minGreen";
		public const REQUIREMENTS:Array 	= [ ExternalImportDataName.LDP_MIN_GREEN ];
		
		public function MinGreenVFactory()
		{
			super( NAME, REQUIREMENTS );
		}
		
		override public function deriveData( dataIndex:DataIndex, importIndex:ImportDataIndex, system:SystemProxy ):void
		{
			var minGreenData:Array = importIndex.getDataAtName( ExternalImportDataName.LDP_MIN_GREEN );
			for each( var dataProxy:DataProxy in minGreenData )
			{
				dataIndex.indexDataProxy( dataProxy );
			}
		}
	}
}