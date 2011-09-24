package com.ts.presentation.view.map.controls
{
	import com.ts.presentation.model.map.phase.vo.PhaseLinkVO;
	
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import mx.graphics.SolidColor;
	import mx.states.SetStyle;
	
	import spark.components.Group;
	import spark.components.ToggleButton;
	import spark.primitives.Rect;
	
	public class VehicleControl extends ToggleButton
	{
		public static const VEH_OVER:String 			= "vehOver";
		public static const VEH_OUT:String 				= "vehOut";
		public static const VEH_SELECTED_OVER:String 	= "vehSelectedOver";
		public static const VEH_SELECTED_OUT:String 	= "vehSelectedOut";
		public static const VEH_ADD_SELECTED:String 	= "vehAddSelected";
		public static const VEH_SELECT:String 			= "vehSelect";
		public static const VEH_REMOVE_SELECTED:String 	= "vehRemoveSelected";
		public static const VEH_UNSELECT:String 		= "vehUnselect";
		
		[ SkinPart( required="true" ) ]
		public var vehicle:Rect;
		
		[ SkinPart( required="true" ) ]
		public var background:Group;
		
		override public function set x(value:Number):void
		{
			if( x == value ) return;
			super.x = value;
		}
		
		override public function set y(value:Number):void
		{
			if( y == value ) return;
			super.y = value;
		}
		
		override public function set rotation(value:Number):void
		{
			if( rotation == value ) return;
			super.rotation = value;
		}
		
		private var _vehColor:uint = 0x000000;
		private var _vehFill:SolidColor;
		private var _vehColorChanged:Boolean = false;
		public function set vehColor( value:uint ):void
		{
			if( _vehColor == value ) return;
			_vehColor = value;
			_vehFill.color = value;
			//_vehColorChanged = true;
			//invalidateDisplayList();
		}
		
		private var _vehAlpha:Number = 1;
		private var _vehAlphaChanged:Boolean = false;
		public function set vehAlpha( value:Number ):void
		{
			_vehAlpha = value;
			_vehAlphaChanged = true;
			invalidateDisplayList();
		}
		
		private var _vehLength:Number;
		private var _vehLengthChanged:Boolean = false;
		public function set vehLength( value:Number ):void
		{
			_vehLength = value;
			_vehLengthChanged = true;
			invalidateDisplayList();
		}
		
		private var _vehWidth:Number;
		private var _vehWidthChanged:Boolean = false;
		public function set vehWidth( value:Number ):void
		{
			_vehWidth = value;
			_vehWidthChanged = true;
			invalidateDisplayList();
		}
		
		private var _controlUID:String;
		public function get controlUID():String{ return _controlUID; }
		
		public function VehicleControl( ControlUID:String, VehWidth:Number, VehLength:Number )
		{
			super();
			_controlUID = ControlUID;
			vehWidth 	= VehWidth;
			vehLength 	= VehLength;
			
			setStyle( "skinClass", VehicleControlSkin );
			addEventListener( MouseEvent.ROLL_OVER, handleEvent );
			addEventListener( MouseEvent.ROLL_OUT, 	handleEvent );
			addEventListener( MouseEvent.CLICK,		handleEvent );
		}
		
		private var _highlighted:Boolean = false;
		public function set highlighted( value:Boolean ):void
		{
			_highlighted = value;
		}
		
		override public function set visible(value:Boolean):void
		{
			super.visible = value;
			if( visible )
			{
				addEventListener( MouseEvent.ROLL_OVER, handleEvent );
				addEventListener( MouseEvent.ROLL_OUT, 	handleEvent );
				addEventListener( MouseEvent.CLICK,		handleEvent );
			}
			else
			{
				if( _highlighted )
				{
					_highlighted = false;
					dispatchEvent( new Event( VEH_OUT, true ) );
				}
				if( selected )
				{
					selected = false;
					dispatchEvent( new Event( VEH_REMOVE_SELECTED, true ) );
				}
				removeEventListener( MouseEvent.ROLL_OVER, 	handleEvent );
				removeEventListener( MouseEvent.ROLL_OUT, 	handleEvent );
				removeEventListener( MouseEvent.CLICK,		handleEvent );
			}
		}
		
		override protected function updateDisplayList(unscaledWidth:Number, unscaledHeight:Number):void
		{
			super.updateDisplayList( unscaledWidth, unscaledHeight );
			
			if( _vehColorChanged || _vehAlphaChanged )
			{
				vehicle.fill 	 = new SolidColor( _vehColor, _vehAlpha );
				_vehColorChanged = false;
				_vehAlphaChanged = false;
			}
		}
		
		override protected function partAdded( partName:String, instance:Object ):void
		{
			if( instance == vehicle )
			{
				_vehFill		= new SolidColor( _vehColor, _vehAlpha );
				vehicle.fill 	= _vehFill;
				vehicle.width 	= _vehLength;
				vehicle.height 	= _vehWidth;
				vehicle.x 		= -( _vehLength / 2 );
				vehicle.y 		= -( _vehWidth / 2 );
			}
			if( instance == background )
			{
				background.width 	= _vehLength + 4;
				background.height 	= _vehWidth + 4;
				background.x 		= -( ( _vehLength + 4 ) / 2 - .5 );
				background.y 		= -( ( _vehWidth + 4 ) / 2 - .5 );
			}
		}
		
		private function handleEvent( event:Event ):void
		{
			switch( event.type )
			{
				case MouseEvent.ROLL_OVER:
					_highlighted = true;
					if( selected )
						dispatchEvent( new Event( VEH_SELECTED_OVER, true ) );
					
					else
						dispatchEvent( new Event( VEH_OVER, true ) );
					
					break;
				
				case MouseEvent.ROLL_OUT:
					_highlighted = false;
					if( selected )
						dispatchEvent( new Event( VEH_SELECTED_OUT, true ) );
					
					else
						dispatchEvent( new Event( VEH_OUT, true ) );
					
					break;
				
				case MouseEvent.CLICK:
					if( selected )
					{
						if( MouseEvent( event ).shiftKey )
						{
							dispatchEvent( new Event( VEH_ADD_SELECTED, true ) );
						}
						else
						{
							dispatchEvent( new Event( VEH_SELECT, true ) );
						}
					}
					else
					{
						if( MouseEvent( event ).shiftKey )
						{
							dispatchEvent( new Event( VEH_REMOVE_SELECTED, true ) );
						}
						else
						{
							dispatchEvent( new Event( VEH_UNSELECT, true ) );
						}
					}
					break;
				
				default:
					break;
			}
		}
	}
}