package com.ts.importing.virtualData.required
{
	import com.ts.data.model.DataIndex;
	import com.ts.data.model.DataProxy;
	import com.ts.importing.configuration.model.enumerations.ExternalImportDataName;
	import com.ts.importing.deriveSystem.model.ImportDataIndex;
	import com.ts.importing.virtualData.AbstractVirtualDataFactory;
	import com.ts.presentation.model.map.SystemProxy;
	
	public class SignalDisplayVFactory extends AbstractVirtualDataFactory
	{
		public static const NAME:String 	= "signalDisplay";
		public const REQUIREMENTS:Array 	= [ ExternalImportDataName.LDP_SIG_DISPLAY ];
		
		public function SignalDisplayVFactory()
		{
			super( NAME, REQUIREMENTS );
		}
		
		override public function deriveData( dataIndex:DataIndex, importIndex:ImportDataIndex, system:SystemProxy ):void
		{
			var signalDisplayData:Array = importIndex.getDataAtName( ExternalImportDataName.LDP_SIG_DISPLAY );
			for each( var dataProxy:DataProxy in signalDisplayData )
			{
				dataIndex.indexDataProxy( dataProxy );
			}
		}
	}
}