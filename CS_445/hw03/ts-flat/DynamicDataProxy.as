package com.ts.data.model
{
	import com.ts.data.model.vo.DataVO;
	import com.ts.data.model.vo.MetadataVO;

	public class DynamicDataProxy extends DataProxy
	{	
		public function DynamicDataProxy( metadata:MetadataVO, data:DataVO )
		{
			super( metadata, data );
		}
	}	
}