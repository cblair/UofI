package com.ts.global.view.layout
{
	import spark.components.Group;
	
	public class DynamicLayout extends Group
	{	
		private var _panelLayout:IPanelLayout = new VerticalPanelLayout();
		private var _layoutChanged:Boolean = true;
		public function get Layout():IPanelLayout{ return _panelLayout; }
		public function set Layout( value:IPanelLayout ):void
		{
			_panelLayout = value;
			_layoutChanged = true;
			
			invalidateProperties();
		}
		
		private var _panes:Array = new Array();
		private var _panesChanged:Boolean = false;
		public function get DynamicPanes():Array{ return _panes; }
		public function set DynamicPanes( value:Array ):void
		{
			_panes = value;
			_panesChanged = true;
			
			invalidateProperties();
		}
		
		public function DynamicLayout()
		{
			super();
		}
		
		override protected function commitProperties():void
		{
			super.commitProperties();
			
			if( _panesChanged )
			{
				_panesChanged = false;
			}
			
			if( _layoutChanged )
			{
				_layoutChanged = false;
			}
		}
	}
}