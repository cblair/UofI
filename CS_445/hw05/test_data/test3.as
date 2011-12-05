package test3
{
	//import test_data.import_test;
	
	public var XMLElement;

	public class test3
	{
		public static const METADATA:String = "md";
		public static const DATA:String	= "d";

		public static const NAME:String	= "abstractAdorner";
		public var aspectName:String;

		public var i:int = 1;
		
		public function test_func(AspectName:String, name2:int)
		{
			call_me("test", name2, 1);

		}

		public function call_me(test:String, y:int, z)
		{
			var x;
			var a;
			var b;
			var c;
			x = y + z;
			x = y * z + a - b / 2;
		}
	}
}

function test_func2(AspectName:String, name2:int)
{
	var x;
	var a;
	var b;
	var c;
	x = y + z;
	x = y * z + a - b / 2;
}
