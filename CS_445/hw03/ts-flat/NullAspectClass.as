package com.ts.importing.aspectClasses
{
	import com.ts.importing.adornerClasses.AbstractAdorner;
	import com.ts.importing.virtualData.DerivationCollection;
	import com.ts.importing.virtualData.required.DetectorVFactory;
	import com.ts.importing.virtualData.required.MaxGreenVFactory;
	import com.ts.importing.virtualData.required.MinGreenVFactory;
	import com.ts.importing.virtualData.required.SignalDisplayVFactory;
	import com.ts.importing.virtualData.standardTiming.VehExtTimerVFactory;

	public class NullAspectClass extends AbstractAspectClass
	{
		public function NullAspectClass( aspectName:String )
		{
			var derivationCollection:DerivationCollection = new DerivationCollection();
			var adorner:AbstractAdorner = new AbstractAdorner( "null" );
			super( derivationCollection, adorner, aspectName );
		}
	}
}