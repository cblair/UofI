package com.ts.aspect.model
{
	import com.ts.aspect.model.vo.DecorationVO;
	import com.ts.aspect.model.vo.DisplayVO;
	
	public class DisplayProxy extends DecorationProxy
	{
		public static const NAME:String = "displayProxy";
		
		public var display:DisplayVO;
		
		
		public function DisplayProxy( Name:String, Display:DisplayVO, Decoration:DecorationVO )
		{
			display = Display;
			
			super( Name, Decoration );
		}
		
		override public function toString( asList:Boolean = true ):String
		{
			var string:String = "--[ " + proxyName + " ] -- \n";
			
			string += decoration.toString( asList );
			string += "\n";
			string += display.toString( asList );
			string += "\n";
			
			return string;
		}
	}
}