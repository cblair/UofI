package com.ts.global.view.components
{
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import spark.components.Button;
	import spark.components.Label;
	import spark.components.supportClasses.SkinnableComponent;
	
	[State( "over" )]
	[State( "up" )]
	[Event( name="decreaseSpeed", 	type="flash.events.Event" )]
	[Event( name="increaseSpeed", 	type="flash.events.Event" )]
	public class SpeedControl extends SkinnableComponent
	{
		[SkinPart( required="true" ) ]
		public var decrementBtn:Button;
		
		[SkinPart( requried="true" ) ]
		public var incrementBtn:Button;
		
		[SkinPart( required="true" ) ]
		public var speedIndicator:Label;

		private var _skinState:String = "up";
		
		[Bindable] public var speedTxt:String = "1x";
		public function set speed( value:Number ):void
		{
			speedTxt = value.toPrecision( 3 ) + "x";
		}
		
		public function SpeedControl()
		{
			super();
		}
		
		public function setSkinState( value:String ):void
		{
			if( value == "over" || value == "up" )
			{
				_skinState = value;
				invalidateSkinState();
			}
		}
		
		override protected function getCurrentSkinState():String
		{
			return _skinState;
		}
		
		override protected function partAdded(partName:String, instance:Object):void
		{
			super.partAdded( partName, instance );
			
			if( instance == decrementBtn )
			{
				decrementBtn.addEventListener( MouseEvent.CLICK, handleEvent );
			}
			if( instance == incrementBtn )
			{
				incrementBtn.addEventListener( MouseEvent.CLICK, handleEvent );
			}
		}
		
		override protected function partRemoved(partName:String, instance:Object):void
		{
			super.partRemoved( partName, instance );
			
			if( instance == decrementBtn )
			{
				decrementBtn.removeEventListener( MouseEvent.CLICK, handleEvent );
			}
			if( instance == incrementBtn )
			{
				incrementBtn.removeEventListener( MouseEvent.CLICK, handleEvent );
			}
		}
		
		private function handleEvent( event:Event ):void
		{
			switch( event.type )
			{
				case MouseEvent.CLICK:
				{
					if( event.target == decrementBtn )
					{
						dispatchEvent( new Event( "decreaseSpeed" ) );
					}
					else if( event.target == incrementBtn )
					{
						dispatchEvent( new Event( "increaseSpeed" ) );
					}
				}
				
				default:
					break;
			}
		}
	}
}