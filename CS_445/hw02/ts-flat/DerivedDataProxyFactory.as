package com.ts.importing.deriveSystem.command
{
	import com.ts.data.enumerations.DataClass;
	import com.ts.data.model.ConstantDataProxy;
	import com.ts.data.model.DataProxy;
	import com.ts.data.model.DiscreteDataProxy;
	import com.ts.data.model.DynamicDataProxy;
	import com.ts.data.model.vo.DataVO;
	import com.ts.data.model.vo.MetadataVO;
	import com.ts.data.model.vo.StatesVO;

	public class DerivedDataProxyFactory
	{
		public static function createDataProxy( metadata:MetadataVO, data:DataVO, states:StatesVO = null ):DataProxy
		{
			switch( metadata.dataClass )
			{
				case DataClass.CONSTANT:
					return new ConstantDataProxy( metadata, data );
					
				case DataClass.DISCRETE:
					return new DiscreteDataProxy( metadata, data, states );
					
				case DataClass.DYNAMIC:
					return new DynamicDataProxy( metadata, data );
					
				default:
					return null;
			}
		}
	}
}