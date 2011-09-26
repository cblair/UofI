var someObject;
var someObject:*;
var someObject:Object;

function TestOne ()
{ 
	var aspectName; //= AspectName;
};

function TestTwo ()
{
	test = Test;
};

package test
{

}

package mx.core 
{
	import test;

	class UIObject extends Something
    	{
		public static const test;
		
		public function UIObject ()
		{
			super(NAME);
		}

		public function TestOne( )
		{

		}    
	}
}
