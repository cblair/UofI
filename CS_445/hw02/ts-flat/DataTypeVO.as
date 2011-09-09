package com.ts.data.model.vo
{
	public class DataTypeVO
	{
		public var dataType:String;
		public var dataClass:String;
		public var dataRange:String;
		
		public function DataTypeVO( dType:String, dClass:String, dRange:String )
		{
			dataType 	= dType;
			dataClass 	= dClass;
			dataRange 	= dRange;
		}
	}
}