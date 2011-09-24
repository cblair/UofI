package com.ts.importing.aspectClasses
{
	import com.ts.importing.virtualData.DerivationCollection;
	import com.ts.importing.virtualData.required.DetectorVFactory;
	import com.ts.importing.virtualData.required.MaxGreenVFactory;
	import com.ts.importing.virtualData.required.MinGreenVFactory;
	import com.ts.importing.virtualData.required.SignalDisplayVFactory;
	
	public class RequiredAspectClass extends AbstractAspectClass
	{
		public function RequiredAspectClass()
		{
			var derivationCollection:DerivationCollection = new DerivationCollection();
			derivationCollection.addDeriver( SignalDisplayVFactory );
			derivationCollection.addDeriver( DetectorVFactory );
			derivationCollection.addDeriver( MaxGreenVFactory );
			derivationCollection.addDeriver( MinGreenVFactory );
			
			super( derivationCollection, null, "required" );
		}
	}
}