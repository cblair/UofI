package com.ts.global.view.layout
{
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	
	import spark.components.Button;
	import spark.components.ToggleButton;
	import spark.components.supportClasses.SkinnableComponent;
	
	[SkinState( "openOver" )]
	[SkinState( "openOut" )]
	[SkinState( "closedOver" )]
	[SkinState( "closedOut" )]

	public class PaneControlBase extends SkinnableComponent
	{
		public static const OPEN:String = "open";
		public static const CLOSE:String = "close";
		public static const START_DRAG:String = "startDrag";
		public static const UPDATE_DRAG:String = "updateDrag";
		public static const END_DRAG:String = "endDrag";
		
		[SkinPart( required="true" )]
		public var DragBtn:Button;
		
		[SkinPart( required="false" )]
		public var OpenCloseBtn:ToggleButton;
		
		private var _componentState:String = "closedOut";
		private var _isDragging:Boolean = false;
		
		
		//-- IS OPEN --//
		private var _isOpen:Boolean = false;
		/**
		 * 
		 **/
		[Bindable]
		public function get IsOpen():Boolean{ return _isOpen; }
		public function set IsOpen( value:Boolean ):void
		{
			if( _isOpen == value ) return;
			_isOpen = value;
			if( value )openPane();
			else closePane();
			trace( "Component Changed: " + value );
		}
		
		override public function set enabled(value:Boolean):void
		{
			super.enabled = value;
			if( value )
			{
				addEventListener( MouseEvent.ROLL_OVER, onOver );
				addEventListener( MouseEvent.ROLL_OUT, onOut );
				if( DragEnabled && DragBtn ) enableDragBtn(); 
				if( OpenCloseEnabled && OpenCloseBtn ) enableOpenCloseBtn();
			}
			else
			{
				if( _isDragging )stopPanelDrag();
				removeEventListener( MouseEvent.ROLL_OVER, onOver );
				removeEventListener( MouseEvent.ROLL_OUT, onOut );
				if( DragEnabled && DragBtn ) disableDragBtn();
				if( OpenCloseEnabled && OpenCloseBtn ) disableOpenCloseBtn();
			}
		}
		
		
		//-- DRAGBTN ALPHA MULTIPLIER --//
		private var _dragBtnAlphaMultiplier:int = 1;
		/**
		 * 
		 **/
		[Bindable]
		public function get DragBtnAlphaMultiplier():int{ return _dragBtnAlphaMultiplier; }
		public function set DragBtnAlphaMultiplier( value:int ):void{ _dragBtnAlphaMultiplier = value; }
		
		
		//-- OCBTN ALPHA MULTIPLIER --//
		private var _OCBtnAlphaMultiplier:int = 1;
		/**
		 * 
		 **/
		[Bindable]
		public function get OCBtnAlphaMultiplier():int{ return _OCBtnAlphaMultiplier; }
		public function set OCBtnAlphaMultiplier( value:int ):void{ _OCBtnAlphaMultiplier = value; }
		
		
		//-- CONTROL LABEL --//
		private var _controlLabel:String = "";
		/**
		 * 
		 **/
		[Bindable]
		public function get ControlLabel():String{ return _controlLabel; }
		public function set ControlLabel( value:String ):void{ _controlLabel = value; }
		
		
		//-- DRAG ENABLED --//
		private var _dragEnabled:Boolean = true;
		private var _dragEnabledChanged:Boolean = true;
		/**
		 * 
		 **/
		public function get DragEnabled():Boolean{ return _dragEnabled; }
		public function set DragEnabled( value:Boolean ):void
		{
			if( _dragEnabled == value ) return;
			_dragEnabled = value;
			_dragEnabledChanged = true;
			invalidateProperties();
		}
		
		
		//-- DRAG VISIBLE --//
		private var _dragVisible:Boolean = true;
		private var _dragVisibleChanged:Boolean = true;
		/**
		 * 
		 **/
		public function get DragVisible():Boolean{ return _dragVisible; }
		public function set DragVisible( value:Boolean ):void
		{
			if( _dragVisible == value ) return;
			_dragVisible = value;
			_dragVisibleChanged = true;
			invalidateProperties();
		}
		
		
		//-- OPEN CLOSE ENABLED --//
		private var _openCloseEnabled:Boolean = true;
		private var _openCloseEnabledChanged:Boolean = true;
		/**
		 * 
		 **/
		public function get OpenCloseEnabled():Boolean{ return _openCloseEnabled; }
		public function set OpenCloseEnabled( value:Boolean ):void
		{
			if( _openCloseEnabled == value ) return;
			_openCloseEnabled = value;
			_openCloseEnabledChanged = true;
			invalidateProperties();
		}
		
		
		//-- OPEN CLOSE VISIBLE --//
		private var _openCloseVisible:Boolean = true;
		private var _openCloseVisibleChanged:Boolean = true;
		/**
		 * 
		 **/
		public function get OpenCloseVisible():Boolean{ return _openCloseVisible; }
		public function set OpenCloseVisible( value:Boolean ):void
		{
			if( _openCloseVisible == value ) return;
			_openCloseVisible = value;
			_openCloseVisibleChanged = true;
			invalidateProperties();
		}
		
		
		//-- CONTROL SPAN --//
		private var _controlSpan:Number = 20;
		/**
		 * 
		 **/
		[Bindable]
		public function get ControlSpan():Number{ return _controlSpan; }
		public function set ControlSpan( value:Number ):void
		{
			_controlSpan = value;
		}
		
		
		//-- LAYOUT MODE --//
		private var _layoutMode:String = "vertical"; //-- Control lays out horizontal, but moves vertical
		private var _layoutModeChanged:Boolean = true;
		/**
		 * 
		 **/
		[Bindable]
		public function get LayoutMode():String{ return _layoutMode; }
		public function set LayoutMode( value:String ):void
		{
			if( _layoutMode == value ) return;
			_layoutMode = ( value == "vertical" || value == "horizontal" ) ? value : "vertical";
			if( _layoutMode == "vertical" ) _openMode = ( _openMode == "right" || _openMode == "left" ) ? "down" : _openMode;
			else _openMode = ( _openMode == "down" || _openMode == "up" ) ? "right" : _openMode;
			_layoutModeChanged = true;
			invalidateProperties();
		}
		
		
		//-- OPEN MODE --//
		private var _openMode:String = "down";
		private var _openModeChanged:Boolean = true;
		/**
		 * 
		 **/
		public function get OpenMode():String{ return _openMode; }
		public function set OpenMode( value:String ):void
		{
			if( _openMode == value ) return;
			_openMode = ( value == "right" || value == "down" || value == "left" || value == "up" ) ? value : "down";
			if( _openMode == "right" || _openMode == "left" ) _layoutMode = "horizontal";
			else _layoutMode = "vertical";
			_openModeChanged = true;
			invalidateProperties();
		}
		
		public function PaneControlBase()
		{
			super();
		}
		
		public function closePane():void
		{
			this.dispatchEvent( new Event( CLOSE ) );
			setCurrentSkinState( isOverControl() ? "closedOver" : "closedOut" );
			if( OpenCloseBtn && OpenCloseEnabled ) OpenCloseBtn.selected = false;
		}
		
		public function openPane():void
		{
			this.dispatchEvent( new Event( OPEN ) );
			setCurrentSkinState( isOverControl() ? "openOver" : "openOut" );
			if( OpenCloseBtn && OpenCloseEnabled ) OpenCloseBtn.selected = true;
		}
		
		override protected function partAdded(partName:String, instance:Object):void
		{
			super.partAdded( partName, instance );
			if( instance == DragBtn )
			{
				_dragEnabledChanged = true;
				_dragVisibleChanged = true;
				invalidateProperties();
			}
			else if( instance == OpenCloseBtn )
			{
				_openCloseEnabledChanged = true;
				_openCloseVisibleChanged = true;
				if( IsOpen ) openPane();
				invalidateProperties();
			}
		}
		
		override protected function partRemoved(partName:String, instance:Object):void
		{
			super.partRemoved( partName, instance );
			if( instance == DragBtn )
			{
				disableDragBtn();
			}
			else if( instance == OpenCloseBtn )
			{
				disableOpenCloseBtn();
			}
		}
		
		override protected function commitProperties():void
		{
			super.commitProperties();
			if( _dragEnabledChanged || _dragVisibleChanged )
			{
				if( DragBtn )
				{
					if( _dragEnabledChanged )
					{
						if( _dragEnabled ) 		enableDragBtn();
						else 					disableDragBtn();
						_dragEnabledChanged = false;
					}
					if( _dragVisibleChanged )
					{
						if( _dragVisible )	 	DragBtnAlphaMultiplier = 1;
						else					DragBtnAlphaMultiplier = 0;
						_dragVisibleChanged = false;
					}
				}
			}
			if( _openCloseEnabledChanged || _openCloseVisibleChanged )
			{
				if( OpenCloseBtn )
				{
					if( _openCloseEnabledChanged )
					{
						if( _openCloseEnabled ) enableOpenCloseBtn();
						else 					disableOpenCloseBtn();
						_openCloseEnabledChanged = false;
					}
					if( _openCloseVisibleChanged )
					{
						if( _openCloseVisible )	OCBtnAlphaMultiplier = 1;
						else					OCBtnAlphaMultiplier = 0;
						_openCloseVisibleChanged = false;
					}
				}
			}
			if( _layoutModeChanged || _openModeChanged )
			{
				_layoutModeChanged = false;
				_openModeChanged = false;
				updateControlLayout();
			}
		}
		
		protected function setCurrentSkinState( value:String ):void
		{
			_componentState = value;
			invalidateSkinState();
		}
		
		override protected function getCurrentSkinState():String
		{
			return _componentState;
		}
		
		private function enableDragBtn():void
		{
			DragBtn.enabled = true;
			DragBtn.mouseEnabled = true;
			DragBtn.mouseChildren = true;
			DragBtn.addEventListener( MouseEvent.MOUSE_DOWN, onDown );
			DragBtn.addEventListener( MouseEvent.MOUSE_UP, onUp );
		}
		
		private function enableOpenCloseBtn():void
		{
			OpenCloseBtn.enabled = true;
			OpenCloseBtn.mouseEnabled = true;
			OpenCloseBtn.mouseChildren = true;
			OpenCloseBtn.addEventListener( MouseEvent.CLICK, onToggle, true );
		}
		
		private function disableDragBtn():void
		{
			DragBtn.enabled = false;
			DragBtn.mouseEnabled = false;
			DragBtn.mouseChildren = false;
			DragBtn.removeEventListener( MouseEvent.MOUSE_DOWN, onDown );
			DragBtn.removeEventListener( MouseEvent.MOUSE_UP, onUp );
		}
		
		private function disableOpenCloseBtn():void
		{
			OpenCloseBtn.enabled = false;
			OpenCloseBtn.mouseEnabled = false;
			OpenCloseBtn.mouseChildren = false;
			OpenCloseBtn.removeEventListener( MouseEvent.CLICK, onToggle, true );
			if( !IsOpen ) IsOpen = true;
		}
		
		private function updateControlLayout():void
		{
			if( _layoutMode == "vertical" )
			{
				left = 0;
				right = 0;
				height = ControlSpan;
				width = NaN;
				if( _openMode == "down" )
				{
					top = 0;
					bottom = NaN;
				}
				else
				{
					top = NaN;
					bottom = 0;
				}
			}
			else
			{
				top = 0;
				bottom = 0;
				height = NaN;
				width = ControlSpan;
				if( _openMode == "right" )
				{
					left = 0;
					right = NaN;
				}
				else
				{
					right = 0;
					left = NaN;
				}
			}
		}
		
		private function onToggle( event:MouseEvent ):void
		{
			OpenCloseBtn.selected = !OpenCloseBtn.selected;
			IsOpen = OpenCloseBtn.selected ? true : false;
		}
		
		private function onOver( event:MouseEvent ):void
		{
			setCurrentSkinState( IsOpen ? "openOver" : "closedOver" );
		}
		
		private function onOut( event:MouseEvent ):void
		{
			if( !_isDragging )setCurrentSkinState( IsOpen ? "openOut" : "closedOut" );
		}
		
		private function onDown( event:MouseEvent ):void
		{
			stage.addEventListener( MouseEvent.MOUSE_UP, onUp );
			startPanelDrag();
		}
		
		private function onUp( event:MouseEvent ):void
		{
			stage.removeEventListener( MouseEvent.MOUSE_UP, onUp );
			stopPanelDrag();
		}
		
		private function startPanelDrag():void
		{
			_isDragging = true;
			this.startDrag();
			this.dispatchEvent( new Event( START_DRAG ) );
		}
		
		private function updatePanelDrag():void
		{
			this.dispatchEvent( new Event( UPDATE_DRAG ) );
		}
		
		private function stopPanelDrag():void
		{
			stopDrag();
			this.dispatchEvent( new Event( END_DRAG ) );
			_isDragging = false;
		}
		
		private function isOverControl():Boolean
		{
			var gPoint:Point = localToGlobal( new Point( mouseX, mouseY ) );
			return ( _isDragging || hitTestPoint( gPoint.x, gPoint.y ) );
		}
	}
}