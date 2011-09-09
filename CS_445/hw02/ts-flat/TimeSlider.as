package com.ts.global.view.components
{
	import com.ts.global.assets.ColorAssets;
	
	import flash.display.DisplayObject;
	import flash.events.Event;
	
	import mx.events.FlexEvent;
	
	import spark.components.HSlider;
	
	public class TimeSlider extends HSlider
	{
		[Bindable] public var percentComplete:Number = 0;
		
		public function TimeSlider()
		{
			super();
		}
		
		override public function set value( newValue:Number ):void
		{
			super.value = newValue;
			updateProgress( newValue );
		}
		
		private function updateProgress( value:Number ):void
		{
			value = Math.max( minimum, Math.min( maximum, value ) );
			percentComplete = value / ( maximum - minimum );
			//trace( percentComplete );
		}
		
		override protected function partAdded(partName:String, instance:Object):void
		{
			super.partAdded( partName, instance );
			if( instance == thumb )
			{
				addEventListener( Event.CHANGE, handleEvent );
			}
		}
		
		override protected function partRemoved(partName:String, instance:Object):void
		{
			super.partRemoved( partName, instance );
			if( instance == thumb )
			{
				removeEventListener( Event.CHANGE, handleEvent );
			}
		}
		
		private function handleEvent( event:Event ):void
		{
			switch( event.type )
			{
				case Event.CHANGE:
					updateProgress( value );
					break;
				
				default:
					break;

			}
		}
	}
}