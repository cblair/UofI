package com.ts.global.view.layout
{
	import flash.events.MouseEvent;
	import flash.geom.Point;
	
	import mx.controls.Button;
	import mx.core.IVisualElement;
	
	import spark.components.Group;
	import spark.components.SkinnableContainer;
	import spark.components.ToggleButton;
	import spark.primitives.Rect;
	
	[SkinState( "openOver" )]
	[SkinState( "openOut" )]
	[SkinState( "closed" )]
	public class PaneContainerBase extends SkinnableContainer
	{
		[SkinPart( required="false" )]
		public var ContentBackground:Group;
		
		[SkinPart( reguired="true" )]
		public var ContentGroup:Group;
		
		private var _componentState:String = "closed";
		
		
		//-- LAYOUT MODE --//
		private var _layoutMode:String = "vertical";
		private var _layoutModeChanged:Boolean = true;
		/**
		 * 
		 **/
		[Bindable]
		public function get LayoutMode():String{ return _layoutMode; }
		public function set LayoutMode( value:String ):void
		{
			if( _layoutMode == value )return;
			_layoutMode = value;
			_layoutModeChanged = true;
			invalidateProperties();
		}
		
		
		override public function set enabled(value:Boolean):void
		{
			super.enabled = value;
			ContentGroup.enabled = value;
			if( value )
			{
				addEventListener( MouseEvent.ROLL_OVER, onOver );
				addEventListener( MouseEvent.ROLL_OUT, onOut );
			}
			else
			{
				removeEventListener( MouseEvent.ROLL_OVER, onOver );
				removeEventListener( MouseEvent.ROLL_OUT, onOut );
			}
		}
		
		
		//-- IS OPEN --//
		private var _isOpen:Boolean = false;
		/**
		 * 
		 **/
		public function get IsOpen():Boolean{ return _isOpen; }
		public function set IsOpen( value:Boolean ):void
		{
			if( _isOpen == value )return;
			_isOpen = value;
			enabled = value;
			if( value ) setCurrentSkinState( isOverControl() ? "over" : "out" );
			else setCurrentSkinState( "closed" );
		}
		
		
		//-- CONTAINER SPAN --//
		private var _containerSpan:Number = 20;
		private var _containerSpanChanged:Boolean = true;
		/**
		 * 
		 **/
		[Bindable]
		public function get ContainerSpan():Number{ return _containerSpan; }
		public function set ContainerSpan( value:Number ):void
		{
			_containerSpan = Math.max( 0, value );
			_containerSpanChanged = true;
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
			if( _openMode == value )return;
			_openMode = value;
			_openModeChanged = true;
			invalidateProperties();
		}
		
		
		//-- OVERLAP CONTROL --//
		private var _overlapControl:Boolean = true;
		private var _overlapControlChanged:Boolean = true;
		/**
		 * 
		 **/
		[Bindable]
		public function get OverlapControl():Boolean{ return _overlapControl; }
		public function set OverlapControl( value:Boolean ):void
		{
			if( _overlapControl == value )return;
			_overlapControl = value;
			_overlapControlChanged = true;
			invalidateProperties();
		}

		
		//-- CONTENT ARRAY --//
		private var _contentArray:Array = new Array();
		private var _contentChanged:Boolean = false;
		/**
		 * 
		 **/
		public function get Content():Array{ return _contentArray; }
		public function set Content( value:Array ):void
		{
			_contentArray = value;
			_contentChanged = true;
			invalidateProperties();
		}
		
		public function PaneContainerBase()
		{
			super();
		}
		
		override protected function createChildren():void
		{
			super.createChildren();
			//addElement( _content );
		}
		
		override protected function commitProperties():void
		{
			super.commitProperties();
			
			if( _contentChanged )
			{
				_contentChanged = false;
				ContentGroup.mxmlContent = _contentArray;
			}
			
			if( _overlapControlChanged )
			{
				_overlapControlChanged = false;
			}
			
			if( _layoutModeChanged )
			{
				_layoutModeChanged = false;
			}
			
			if( _openModeChanged )
			{
				_openModeChanged = false;
			}
			if( _containerSpanChanged )
			{
				_containerSpanChanged = false;
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
		
		private function onOver( event:MouseEvent ):void
		{
			setCurrentSkinState( "openOver" );
		}
		
		private function onOut( event:MouseEvent ):void
		{
			setCurrentSkinState( "openOut" );
		}
		
		private function isOverControl():Boolean
		{
			var gPoint:Point =  this.localToGlobal( new Point( mouseX, mouseY ) );
			return ( hitTestPoint( gPoint.x, gPoint.y ) );
		}
	}
}