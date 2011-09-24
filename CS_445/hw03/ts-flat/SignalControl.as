package com.ts.presentation.view.map.controls
{
	import com.ts.presentation.model.map.phase.vo.PhaseLinkVO;
	
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import mx.controls.Button;
	import mx.graphics.SolidColor;
	import mx.states.SetStyle;
	
	import spark.components.ToggleButton;
	
	public class SignalControl extends ToggleButton
	{
		public static const SIGNAL_OVER:String 			= "signalOver";
		public static const SIGNAL_OUT:String 			= "signalOut";
		public static const SIGNAL_SELECTED_OVER:String	= "signalSelectedOver";
		public static const SIGNAL_SELECTED_OUT:String	= "signalSelectedOut";
		public static const SIGNAL_ADD_SELECTED:String 	= "signalAddSelected";
		public static const SIGNAL_SELECT:String 		= "signalSelect";
		public static const SIGNAL_REMOVE_SELECTED:String="signalRemoveSelected";
		public static const SIGNAL_UNSELECT:String 		= "signalUnselect";
		
		private var _signalFill:SolidColor;
		
		/*private var _signalLinkData:SignalLinkVO;
		[Bindable]
		public function get signalLinkData():SignalLinkVO{ return _signalLinkData; }
		public function set signalLinkData( value:SignalLinkVO ):void
		{
			_signalLinkData = value;
		}*/
		
		private var _signalColor:uint;
		[Bindable]
		public function get signalColor():uint{ return _signalColor; }
		public function set signalColor( value:uint ):void
		{
			_signalColor = value;
		}
		
		public function get signalNumber():int{ return _signalLinkData.signalNumber; }
		public function get intersectionNumber():int{ return _signalLinkData.intersectionNum; }
		public function get controlUID():String{ return _signalLinkData.signalUID;}
		
		public function SignalControl()
		{
			super();
			setStyle( "skinClass", SignalControlSkin );
			_signalFill = new SolidColor( _signalColor );
			addEventListener( MouseEvent.ROLL_OVER, handleEvent );
			addEventListener( MouseEvent.ROLL_OUT, handleEvent );
			addEventListener( MouseEvent.CLICK, handleEvent );
		}
		
		private function handleEvent( event:Event ):void
		{
			switch( event.type )
			{
				case MouseEvent.ROLL_OVER:
					if( selected )
					{
						dispatchEvent( new Event( SIGNAL_SELECTED_OVER, true ) );
					}
					else
					{
						dispatchEvent( new Event( SIGNAL_OVER, true ) );
					}
					break;
				
				case MouseEvent.ROLL_OUT:
					if( selected )
					{
						dispatchEvent( new Event( SIGNAL_SELECTED_OUT, true ) );
					}
					else
					{
						dispatchEvent( new Event( SIGNAL_OUT, true ) );
					}
					break;
				
				case MouseEvent.CLICK:
					if( selected )
					{
						if( MouseEvent( event ).shiftKey )
						{
							dispatchEvent( new Event( SIGNAL_ADD_SELECTED, true ) );
						}
						else
						{
							dispatchEvent( new Event( SIGNAL_SELECT, true ) );
						}
					}
					else
					{
						if( MouseEvent( event ).shiftKey )
						{
							dispatchEvent( new Event( SIGNAL_REMOVE_SELECTED, true ) );
						}
						else
						{
							dispatchEvent( new Event( SIGNAL_UNSELECT, true ) );
						}
					}
					break;
					
				default:
					break;
			}
		}
	}
}