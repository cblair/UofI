package com.ts.presentation.view.map.controls
{
	import flash.events.Event;
	
	public class MapEvent extends Event
	{
		public var name:String;
		
		public static const VEH_OVER:String 			= "vehOver";
		public static const VEH_OUT:String 				= "vehOut";
		public static const VEH_SELECTED_OVER:String 	= "vehSelectedOver";
		public static const VEH_SELECTED_OUT:String 	= "vehSelectedOut";
		public static const VEH_ADD_SELECTED:String 	= "vehAddSelected";
		public static const VEH_SELECT:String 			= "vehSelect";
		public static const VEH_REMOVE_SELECTED:String 	= "vehRemoveSelected";
		public static const VEH_UNSELECT:String 		= "vehUnselect";
		
		public static const PHASE_OVER:String 			= "phaseOver";
		public static const PHASE_OUT:String 			= "phaseOut";
		public static const PHASE_SELECTED_OVER:String	= "phaseSelectedOver";
		public static const PHASE_SELECTED_OUT:String	= "phaseSelectedOut";
		public static const PHASE_ADD_SELECTED:String 	= "phaseAddSelected";
		public static const PHASE_SELECT:String 		= "phaseSelect";
		public static const PHASE_REMOVE_SELECTED:String= "phaseRemoveSelected";
		public static const PHASE_UNSELECT:String 		= "phaseUnselect";
		
		public static const SIGNAL_OVER:String 			= "signalOver";
		public static const SIGNAL_OUT:String 			= "signalOut";
		public static const SIGNAL_SELECTED_OVER:String	= "signalSelectedOver";
		public static const SIGNAL_SELECTED_OUT:String	= "signalSelectedOut";
		public static const SIGNAL_ADD_SELECTED:String 	= "signalAddSelected";
		public static const SIGNAL_SELECT:String 		= "signalSelect";
		public static const SIGNAL_REMOVE_SELECTED:String= "signalRemoveSelected";
		public static const SIGNAL_UNSELECT:String 		= "signalUnselect";
		
		public function MapEvent(type:String, Name:String, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
			name = Name;
		}
		
		override public function clone():Event
		{
			return new MapEvent( type, name, bubbles, cancelable );
		}
	}
}