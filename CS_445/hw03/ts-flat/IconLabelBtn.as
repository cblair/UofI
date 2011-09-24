package com.ts.global.view.components
{
	import com.ts.utilities.layout.Margin;
	import com.ts.global.assets.skins.DefIconLabelBtnSkin;
	
	import mx.controls.Image;
	
	import spark.components.Button;
	import spark.components.Group;
	
	/**
	 * IconLabelBtn is intended to provide visual redundancy in the control by providing both an icon and label
	 * description of the component's action.
	 * 
	 * Default skin is IconLabelBtnSkin.
	 **/
	[Bindable]
	public class IconLabelBtn extends Button
	{
		/**
		 * contentContainer wraps around the icon and label.
		 * it is in charge of keeping the content layed out consistantly, and allowing padding to be set around the boundaries of the button
		 * ciontrol.
		 **/
		[ SkinPart( required="true" ) ]
		public var contentContainer:Group;
		
		/**
		 * holds the icon for the button
		 **/
		[ SkinPart( required="true" ) ]
		public var icon:Image;
		private var _iconSource:Object;
		
		public var margin:Margin;
		
		/**
		 * get and set the source object of the icon display
		 **/
		public function get iconSource():Object{ return _iconSource; }
		public function set iconSource( value:Object ):void
		{ 
			_iconSource = value;
			if( icon ) icon.source = _iconSource; 
		}
		
		/**
		 * use this margin array to set the control's content layout within the skin.
		 **/
		public function get marginArray():Array{ return margin.marginArray; }
		public function set marginArray( value:Array ):void{ margin.marginArray = value; }
		
		public function IconLabelBtn()
		{
			super();
			margin = new Margin();
			this.setStyle( "skinClass", com.ts.global.assets.skins.DefIconLabelBtnSkin );
		}
		
		override protected function partAdded(partName:String, instance:Object):void
		{
			super.partAdded( partName, instance );
			if( instance == icon )
			{
				if( _iconSource ) icon.source = _iconSource;
			}
		}
	}
}