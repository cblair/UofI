package com.ts.importing.virtualData.volumeDensity
{
	import com.ts.data.model.DataIndex;
	import com.ts.data.model.DataProxy;
	import com.ts.importing.configuration.model.enumerations.ExternalImportDataName;
	import com.ts.importing.deriveSystem.model.ImportDataIndex;
	import com.ts.importing.virtualData.AbstractVirtualDataFactory;
	import com.ts.presentation.model.map.SystemProxy;
	
	public class GapInEffectVFactory extends AbstractVirtualDataFactory
	{
		public static const NAME:String 	= "gapInEffect";
		public const REQUIREMENTS:Array 	= [ ExternalImportDataName.LDP_EFFECTIVE_GAP ];
		
		public function GapInEffectVFactory()
		{
			super( NAME, REQUIREMENTS );
		}
		
		override public function deriveData( dataIndex:DataIndex, importIndex:ImportDataIndex, system:SystemProxy ):void
		{
			var gapInEffectData:Array = importIndex.getDataAtName( ExternalImportDataName.LDP_EFFECTIVE_GAP );
			for each( var dataProxy:DataProxy in gapInEffectData )
			{
				dataIndex.indexDataProxy( dataProxy );
			}
		}
	}
}