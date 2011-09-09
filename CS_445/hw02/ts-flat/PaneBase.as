package com.ts.global.view.layout
{
	import spark.components.SkinnableContainer;
	
	public class PaneBase extends SkinnableContainer
	{
		//-- IS DRAGGABLE --//
		private var _isDraggable:Boolean = true;
		private var _isDraggableChanged:Boolean = true;
		/**
		 * 
		 **/
		[Bindable]
		public function get IsDraggable():Boolean{ return _isDraggable; }
		public function set IsDraggable( value:Boolean ):void
		{
			if( _isDraggable != value )
			{
				_isDraggable = value;
				_isDraggableChanged = true;
				invalidateProperties();
			}
		}
		
		
		//-- IS CLOSABLE --//
		private var _isClosable:Boolean = true;
		private var _isClosableChanged:Boolean = true;
		/**
		 * 
		 **/
		[Bindable] 
		public function get IsClosable():Boolean{ return _isClosable; }
		public function set IsClosable( value:Boolean ):void
		{
			if( _isClosable != value )
			{
				_isClosable = value;
				_isClosableChanged = true;
				invalidateProperties();
			}
		}
		
		public function PaneBase()
		{
			super();
		}
	}
}