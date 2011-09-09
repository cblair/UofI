package com.ts.importing.virtualData.railroadPreemption
{
	import com.ts.data.model.DataIndex;
	import com.ts.data.model.DataProxy;
	import com.ts.importing.configuration.model.enumerations.ExternalImportDataName;
	import com.ts.importing.deriveSystem.model.ImportDataIndex;
	import com.ts.importing.virtualData.AbstractVirtualDataFactory;
	import com.ts.presentation.model.map.SystemProxy;

	public class TrackClearMinGrnVFactory extends AbstractVirtualDataFactory
	{
		public static const NAME:String 	= "trackClearMinGreen";
		public const REQUIREMENTS:Array 	= [ ExternalImportDataName.LDP_TRACK_CLEAR_MIN_GRN ];
		
		public function TrackClearMinGrnVFactory()
		{
			super( NAME, REQUIREMENTS );
		}
		
		override public function deriveData( dataIndex:DataIndex, importIndex:ImportDataIndex, system:SystemProxy ):void
		{
			var trackClearMinGrnData:Array = importIndex.getDataAtName( ExternalImportDataName.LDP_TRACK_CLEAR_MIN_GRN );
			for each( var dataProxy:DataProxy in trackClearMinGrnData )
			{
				dataIndex.indexDataProxy( dataProxy );
			}
		}
	}
}