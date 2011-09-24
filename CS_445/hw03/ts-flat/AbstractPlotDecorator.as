package com.ts.aspect.model
{
	import com.ts.data.model.DataProxy;

	public class AbstractPlotDecorator
	{
		public var entries:Array;
		public var plotPolicy:String;
		public var dataSource:DataProxy;
		
		public function AbstractPlotDecorator( Entries:Array, PlotPolicy:String, DataSource:DataProxy )
		{
			entries 	= Entries;
			plotPolicy 	= PlotPolicy;
			dataSource	= DataSource;
			this.refactorDecorator();
		}
		
		protected function refactorDecorator():void
		{
			//-- override me
		}
		
		public function getDecorator( maxValue:Number, minValue:Number, value:Number ):PlotDecoration2VO
		{
			//-- override me!!
			return null;
		}
	}
}