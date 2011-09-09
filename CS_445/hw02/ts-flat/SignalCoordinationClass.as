package com.ts.importing.aspectClasses
{
	import com.ts.importing.adornerClasses.AbstractAdorner;
	import com.ts.importing.adornerClasses.SignalCoordinationAdorner;
	import com.ts.importing.virtualData.DerivationCollection;
	import com.ts.importing.virtualData.signalCoordination.PCDVFactory;
	import com.ts.importing.virtualData.required.MaxGreenVFactory;
	import com.ts.importing.virtualData.required.MinGreenVFactory;

	public class SignalCoordinationClass extends AbstractAspectClass
	{
		public function SignalCoordinationClass( aspectName:String )
		{
			var derivationCollection:DerivationCollection = new DerivationCollection();
			derivationCollection.addDeriver( MaxGreenVFactory );
			derivationCollection.addDeriver( MinGreenVFactory );
			derivationCollection.addDeriver( PCDVFactory );
			
			var adorner:AbstractAdorner = new SignalCoordinationAdorner();
			
			super( derivationCollection, adorner, aspectName );
		}
	}
}