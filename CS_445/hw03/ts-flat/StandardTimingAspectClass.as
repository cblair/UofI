package com.ts.importing.aspectClasses
{
	import com.ts.importing.adornerClasses.StandardTimingAdorner;
	import com.ts.importing.virtualData.DerivationCollection;
	import com.ts.importing.virtualData.required.DetectorVFactory;
	import com.ts.importing.virtualData.required.MaxGreenVFactory;
	import com.ts.importing.virtualData.required.MinGreenVFactory;
	import com.ts.importing.virtualData.required.SignalDisplayVFactory;
	import com.ts.importing.virtualData.standardTiming.VehExtTimerVFactory;

	public class StandardTimingAspectClass extends AbstractAspectClass
	{
		public function StandardTimingAspectClass( aspectName:String )
		{
			var derivationCollection:DerivationCollection = new DerivationCollection();
			derivationCollection.addDeriver( MaxGreenVFactory );
			derivationCollection.addDeriver( MinGreenVFactory );
			derivationCollection.addDeriver( VehExtTimerVFactory );
			
			var adorner:StandardTimingAdorner = new StandardTimingAdorner();
			
			super( derivationCollection, adorner, aspectName );
		}
	}
}