package com.ts.importing.aspectClasses
{
	import com.ts.importing.adornerClasses.StandardTimingAdorner;
	import com.ts.importing.adornerClasses.VolumeDensityAdorner;
	import com.ts.importing.virtualData.DerivationCollection;
	import com.ts.importing.virtualData.volumeDensity.GapInEffectStateVFactory;
	import com.ts.importing.virtualData.volumeDensity.GapInEffectVFactory;
	import com.ts.importing.virtualData.volumeDensity.TimeBeforeReduceVFactory;
	import com.ts.importing.virtualData.volumeDensity.TimeToReduceVFactory;
	
	public class VolumeDensityAspectClass extends AbstractAspectClass
	{
		public function VolumeDensityAspectClass( aspectName:String )
		{
			var derivationCollection:DerivationCollection = new DerivationCollection();
			derivationCollection.addDeriver( GapInEffectVFactory );
			derivationCollection.addDeriver( TimeToReduceVFactory );
			derivationCollection.addDeriver( TimeBeforeReduceVFactory );
			derivationCollection.addDeriver( GapInEffectStateVFactory );
			
			var adorner:VolumeDensityAdorner = new VolumeDensityAdorner();
			
			super( derivationCollection, adorner, aspectName );
		}
	}
}