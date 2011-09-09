package com.ts.importing.aspectClasses
{
	import com.ts.importing.adornerClasses.AbstractAdorner;
	import com.ts.importing.adornerClasses.RRPreemptionAdorner;
	import com.ts.importing.adornerClasses.StandardTimingAdorner;
	import com.ts.importing.virtualData.DerivationCollection;
	import com.ts.importing.virtualData.railroadPreemption.ActivePhaseVFactory;
	import com.ts.importing.virtualData.railroadPreemption.EntranceMinGreenVFactory;
	import com.ts.importing.virtualData.railroadPreemption.EntranceRedVFactory;
	import com.ts.importing.virtualData.railroadPreemption.EntranceYellowVFactory;
	import com.ts.importing.virtualData.railroadPreemption.PreemptCallVFactory;
	import com.ts.importing.virtualData.railroadPreemption.PreemptSequenceEventVFactory;
	import com.ts.importing.virtualData.railroadPreemption.PreemptSequenceStateVFactory;
	import com.ts.importing.virtualData.railroadPreemption.TrackClearMinGrnVFactory;
	import com.ts.importing.virtualData.standardTiming.VehExtTimerVFactory;

	public class RailroadPreemptionAspectClass extends AbstractAspectClass
	{
		public function RailroadPreemptionAspectClass( aspectName:String )
		{
			var derivationCollection:DerivationCollection = new DerivationCollection();
			derivationCollection.addDeriver( ActivePhaseVFactory );
			derivationCollection.addDeriver( EntranceMinGreenVFactory );
			derivationCollection.addDeriver( EntranceYellowVFactory );
			derivationCollection.addDeriver( EntranceRedVFactory );
			derivationCollection.addDeriver( PreemptCallVFactory );
			derivationCollection.addDeriver( TrackClearMinGrnVFactory );
			derivationCollection.addDeriver( PreemptSequenceStateVFactory );
			derivationCollection.addDeriver( PreemptSequenceEventVFactory );
			derivationCollection.addDeriver( VehExtTimerVFactory );
			
			var adorner:RRPreemptionAdorner = new RRPreemptionAdorner();
			
			super( derivationCollection, adorner, aspectName );
		}
	}
}