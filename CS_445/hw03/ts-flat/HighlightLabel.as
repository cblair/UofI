package com.ts.global.view.components
{
	import com.ts.global.assets.skins.HighlightLenseSkin;
	
	import spark.components.Label;
	import spark.components.supportClasses.SkinnableComponent;
	
	[ SkinState( "over" ) 		]
	[ SkinState( "up" ) 		]
	[ SkinState( "down" ) 		]
	[ SkinState( "disabled" ) 	]
	public class HighlightLabel extends SkinnableComponent
	{
		private var _skinState:String = "up";
		
		[SkinPart( required="true")]
		public var labelDisplay:Label;
		
		private var _text:String;
		
		[Bindable]
		public function get text():String{ return _text; }
		public function set text( value:String ):void
		{
			_text = value;
			if( labelDisplay ) labelDisplay.text = value;
		}
		
		public function HighlightLabel()
		{
			super();
			_text = "";
			setStyle( "skinClass", com.ts.global.assets.skins.HighlightLabelSkin );
		}
		
		public function setSkinState( value:String ):void
		{
			if( value == "over" || value == "up" || value == "down" || value == "disabled" )
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
			super.partAdded( partName, instance )
			if( instance == labelDisplay )
			{
				labelDisplay.text = _text;
			}
		}
	}
}