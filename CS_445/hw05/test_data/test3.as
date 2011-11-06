package com.its
{
	import test_data.import_test;
	
	public class XMLElement
	{
		public static const METADATA:String = "md";
		public static const DATA:String	= "d";

		public static const NAME:String	= "abstractAdorner";
		public var aspectName:String;

		public var s:String;
		
		public function test_func(AspectName:String, name2:int)
		{
			var j = 1;
			j = 2;
			j = 3.0;
			s = "Help";
			s = call_me(s);
		}

		public function call_me(test:String)
		{
			var j = 1;
		}
	}
}

function test_func2(AspectName:String, name2:int)
{

}
