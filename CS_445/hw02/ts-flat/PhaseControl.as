package com.ts.presentation.view.map.controls
{
	import com.ts.presentation.model.map.phase.vo.PhaseLinkVO;
	
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import mx.controls.Button;
	import mx.graphics.SolidColor;
	import mx.states.SetStyle;
	
	import spark.components.ToggleButton;
	
	public class PhaseControl extends ToggleButton
	{
		public static const PHASE_OVER:String 			= "phaseOver";
		public static const PHASE_OUT:String 			= "phaseOut";
		public static const PHASE_SELECTED_OVER:String	= "phaseSelectedOver";
		public static const PHASE_SELECTED_OUT:String	= "phaseSelectedOut";
		public static const PHASE_ADD_SELECTED:String 	= "phaseAddSelected";
		public static const PHASE_SELECT:String 		= "phaseSelect";
		public static const PHASE_REMOVE_SELECTED:String= "phaseRemoveSelected";
		public static const PHASE_UNSELECT:String 		= "phaseUnselect";
		
		private var _phaseLinkData:PhaseLinkVO;
		[Bindable]
		public function get phaseLinkData():PhaseLinkVO{ return _phaseLinkData; }
		public function set phaseLinkData( value:PhaseLinkVO ):void
		{
			_phaseLinkData = value;
		}
		
		private var _signalColor:uint;
		public var signalFill:SolidColor;
		[Bindable]
		public function get signalColor():uint{ return _signalColor; }
		public function set signalColor( value:uint ):void
		{
			if( _signalColor == value ) return;
			_signalColor = value;
			signalFill.color = value;
		}
		
		public function get phaseNumber():int{ return _phaseLinkData.phaseNumber; }
		public function get intersectionNumber():int{ return _phaseLinkData.intersectionNum; }
		public function get controlUID():String{ return _phaseLinkData.phaseUID;}
		
		public function PhaseControl()
		{
			super();
			setStyle( "skinClass", PhaseControlSkin );
			signalFill = new SolidColor();
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
						dispatchEvent( new Event( PHASE_SELECTED_OVER, true ) );
					}
					else
					{
						dispatchEvent( new Event( PHASE_OVER, true ) );
					}
					break;
				
				case MouseEvent.ROLL_OUT:
					if( selected )
					{
						dispatchEvent( new Event( PHASE_SELECTED_OUT, true ) );
					}
					else
					{
						dispatchEvent( new Event( PHASE_OUT, true ) );
					}
					break;
				
				case MouseEvent.CLICK:
					if( selected )
					{
						if( MouseEvent( event ).shiftKey )
						{
							dispatchEvent( new Event( PHASE_ADD_SELECTED, true ) );
						}
						else
						{
							dispatchEvent( new Event( PHASE_SELECT, true ) );
						}
					}
					else
					{
						if( MouseEvent( event ).shiftKey )
						{
							dispatchEvent( new Event( PHASE_REMOVE_SELECTED, true ) );
						}
						else
						{
							dispatchEvent( new Event( PHASE_UNSELECT, true ) );
						}
					}
					break;
					
				default:
					break;
			}
		}
	}
}