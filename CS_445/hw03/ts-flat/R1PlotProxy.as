package com.ts.aspect.model.graph.plot
{
	import com.ts.aspect.model.DecorationProxy;
	import com.ts.aspect.model.graph.plot.vo.R1PlotVO;
	import com.ts.aspect.model.vo.DecorationVO;
	import com.ts.data.model.DataProxy;
	import com.ts.utilities.colorRanges.AbstractColorInterpolator;
	
	public class R1PlotProxy extends PlotProxy
	{
		public static const NAME:String = "r1PlotProxy";
		
		public var plotData:R1PlotVO;
		public var dataSource:DataProxy;
		public var decorationSource:DataProxy;
		
		public function R1PlotProxy( Name:String, PlotData:R1PlotVO, Decorator:PlotDecorator, Decoration:DecorationVO, DataSource:DataProxy, DecorationSource:DataProxy )
		{
			plotData 			= PlotData;
			dataSource 			= DataSource;
			decorationSource 	= DecorationSource;
			plotDecorator		= Decorator;
			
			super( NAME + "/" + Name, Decorator, Decoration );
		}
	}
}