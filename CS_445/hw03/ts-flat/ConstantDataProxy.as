package com.ts.data.model
{
	import com.ts.data.model.vo.DataVO;
	import com.ts.data.model.vo.MetadataVO;

	public class ConstantDataProxy extends DataProxy
	{	
		public function ConstantDataProxy( metadata:MetadataVO, data:DataVO )
		{
			super( metadata, data );
		}
	}
}