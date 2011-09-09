package com.ts.aspect.model.graph.plot.vo
{
	import com.ts.data.model.DataProxy;

	public class R1PlotVO
	{
		public var policy:String;
		public var dataSrcName:String;
		public var decorationSrcName:String;
		public var decType:String;
		public var decOrientation:String;
		
		public function R1PlotVO( Policy:String, DataSrcName:String, DecorationSrcName:String, DecType:String, DecOrientation:String )
		{
			policy 				= Policy;
			dataSrcName 		= DataSrcName;
			decorationSrcName 	= DecorationSrcName;
			decType 			= DecType;
			decOrientation 		= DecOrientation;
		}
	}
}