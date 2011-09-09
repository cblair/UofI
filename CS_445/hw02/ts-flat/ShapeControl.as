package com.ts.presentation.view.annotation.controls
{
	import com.ts.presentation.view.map.controls.VehicleControlSkin;
	import com.ts.presentation.model.map.phase.vo.PhaseLinkVO;
	
	import flash.display.LineScaleMode;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	
	import mx.graphics.SolidColor;
	import mx.graphics.SolidColorStroke;
	import mx.states.SetStyle;
	
	import spark.components.Button;
	import spark.components.ToggleButton;
	import spark.primitives.Path;
	
	public class ShapeControl extends ToggleButton
	{
		public static const SHAPE_OVER:String 			= "shapeOver";
		public static const SHAPE_OUT:String 			= "shapeOut";
		public static const SHAPE_SELECTED_OVER:String 	= "shapeSelectedOver";
		public static const SHAPE_SELECTED_OUT:String 	= "shapeSelectedOut";
		public static const SHAPE_ADD_SELECTED:String 	= "shapeAddSelected";
		public static const SHAPE_SELECT:String 		= "shapeSelect";
		public static const SHAPE_REMOVE_SELECTED:String= "shapeRemoveSelected";
		public static const SHAPE_UNSELECT:String 		= "shapeUnselect";
		
		[ SkinPart( required="true" ) ]
		public var shape:Path;
		
		[ SkinPart( required="true" ) ]
		public var dragPoint:Button;
		
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
		
		private var _color:uint = 0x000000;
		private var _stroke:SolidColorStroke;
		public function set color( value:uint ):void
		{
			if( _color == value ) return;
			_color = value;
			_stroke.color = value;
		}
		
		private var _alpha:Number = 1;
		override public function set alpha( value:Number ):void
		{
			if( _alpha == value ) return;
			_alpha = value;
			_stroke.alpha = value;
		}
		
		private var _weight:Number = 1;
		public function set weight( value:Number ):void
		{
			if( _weight == value ) return;
			_weight = value;
			_stroke.weight = value;
		}
		
		private var _shapeData:String;
		public function set shapeData( value:String ):void
		{
			_shapeData = value;
			if( shape )
			{
				shape.data = value;
			}
		}
		
		private var _shapeID:int;
		public function get shapeID():int{ return _shapeID; }
		
		public function ShapeControl( shapeID:int )
		{
			super();
			_shapeID = shapeID;
			_stroke = new SolidColorStroke();
			_stroke.scaleMode = LineScaleMode.NONE;
			setStyle( "skinClass", ShapeControlSkin );
			addEventListener( MouseEvent.ROLL_OVER, handleEvent );
			addEventListener( MouseEvent.ROLL_OUT, 	handleEvent );
			addEventListener( MouseEvent.CLICK,		handleEvent );
		}
		
		private var _highlighted:Boolean = false;
		public function set highlighted( value:Boolean ):void
		{
			_highlighted = value;
		}
		
		public function set bottomRightPoint( value:Point ):void
		{
			value = globalToLocal( value );
			dragPoint.x = value.x;
			dragPoint.y = value.y;
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
					dispatchEvent( new Event( SHAPE_OUT, true ) );
				}
				if( selected )
				{
					selected = false;
					dispatchEvent( new Event( SHAPE_REMOVE_SELECTED, true ) );
				}
				removeEventListener( MouseEvent.ROLL_OVER, 	handleEvent );
				removeEventListener( MouseEvent.ROLL_OUT, 	handleEvent );
				removeEventListener( MouseEvent.CLICK,		handleEvent );
			}
		}
		
		override protected function partAdded( partName:String, instance:Object ):void
		{
			if( instance == shape )
			{
				if( _shapeData )
				{
					shape.data = _shapeData;
				}
				shape.stroke = _stroke;
			}
			if( instance == dragPoint )
			{
			}
		}
		
		private function handleEvent( event:Event ):void
		{
			switch( event.type )
			{
				case MouseEvent.ROLL_OVER:
					_highlighted = true;
					if( selected )
						dispatchEvent( new Event( SHAPE_SELECTED_OVER, true ) );
					
					else
						dispatchEvent( new Event( SHAPE_OVER, true ) );
					
					break;
				
				case MouseEvent.ROLL_OUT:
					_highlighted = false;
					if( selected )
						dispatchEvent( new Event( SHAPE_SELECTED_OUT, true ) );
					
					else
						dispatchEvent( new Event( SHAPE_OUT, true ) );
					
					break;
				
				case MouseEvent.CLICK:
					if( selected )
					{
						if( MouseEvent( event ).shiftKey )
						{
							dispatchEvent( new Event( SHAPE_ADD_SELECTED, true ) );
						}
						else
						{
							dispatchEvent( new Event( SHAPE_SELECT, true ) );
						}
					}
					else
					{
						if( MouseEvent( event ).shiftKey )
						{
							dispatchEvent( new Event( SHAPE_REMOVE_SELECTED, true ) );
						}
						else
						{
							dispatchEvent( new Event( SHAPE_UNSELECT, true ) );
						}
					}
					break;
				
				default:
					break;
			}
		}
	}
}