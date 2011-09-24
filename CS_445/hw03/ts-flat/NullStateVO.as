package com.ts.data.model.vo
{
	import com.ts.data.enumerations.StateAttribute;
	import com.ts.data.enumerations.XMLElement;
	
	import flash.text.StaticText;

	public class NullStateVO extends StateVO
	{
		
		public function NullStateVO()
		{
			super( "null", "null state", "null", "null state", "null state", null, NaN, "null", null ); 
		}
	}
}