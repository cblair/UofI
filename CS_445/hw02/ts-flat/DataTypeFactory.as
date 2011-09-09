package com.ts.data.factories
{
	import com.ts.data.enumerations.DataClass;
	import com.ts.data.enumerations.DataRange;
	import com.ts.data.enumerations.DataType;
	import com.ts.data.model.vo.DataTypeVO;

	public class DataTypeFactory
	{
		public function DataTypeFactory()
		{
		}
		
		public static function getDataType( dataTypeName:String ):DataTypeVO
		{
			switch( dataTypeName )
			{
				case DataType.ANALOG:		return new DataTypeVO( DataType.ANALOG, 	DataClass.DYNAMIC, 	DataRange.UNBOUNDED );
				case DataType.BOOLEAN:		return new DataTypeVO( DataType.BOOLEAN, 	DataClass.DISCRETE, DataRange.FIXED );
				case DataType.CONSTANT:		return new DataTypeVO( DataType.CONSTANT, 	DataClass.CONSTANT, DataRange.CONSTANT );
				case DataType.COUNTING:		return new DataTypeVO( DataType.COUNTING, 	DataClass.DYNAMIC, 	DataRange.UNBOUNDED );
				case DataType.STATE:		return new DataTypeVO( DataType.STATE, 		DataClass.DISCRETE, DataRange.FIXED );
				case DataType.TIME:			return new DataTypeVO( DataType.TIME, 		DataClass.DYNAMIC, 	DataRange.UNBOUNDED );
				default:						return null;
			}
		}
	}
}