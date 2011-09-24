package com.ts.global.view.components
{
	import com.ts.global.assets.skins.HighlightLenseSkin;
	
	import spark.components.supportClasses.SkinnableComponent;
	
	[ SkinState( "over" ) 		]
	[ SkinState( "up" ) 		]
	[ SkinState( "down" ) 		]
	[ SkinState( "disabled" ) 	]
	public class HighlightLense extends SkinnableComponent
	{
		private var _skinState:String = "up";
		
		public function HighlightLense()
		{
			super();
			setStyle( "skinClass", com.ts.global.assets.skins.HighlightLenseSkin );
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
	}
}