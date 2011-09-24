package com.ts.importing.aspectClasses
{

	public class AspectFactory
	{
		public static const STANDARD_TIMING:String = "standardTiming";
		public static const SOE:String = "soe";
		public static const DYNAMIC_GAP:String = "dynamicGap";
		public static const CORRIDOR:String = "corridor";
		
		public function AspectFactory()
		{
		}
		
		public static function getAspectClass( value:String ):AbstractAspectClass
		{
			switch( value )
			{
				case "standardTiming":
					return new StandardTimingAspectClass( value );
					
				case "dynamicGap":
					return new VolumeDensityAspectClass( value );
					
				case "corridor":
					return new SignalCoordinationClass( value );
					
				case "soe":
					return new RailroadPreemptionAspectClass( value );
					
				default:
					return new NullAspectClass( "null" );
			}
		}
	}
}