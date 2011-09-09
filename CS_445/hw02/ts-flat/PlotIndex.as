package com.ts.aspect.model.graph.plot.vo
{
	import com.ts.aspect.model.graph.plot.PlotProxy;

	public class PlotIndex
	{
		public var plots:Array;
		
		public function PlotIndex( Plots:Array )
		{
			plots = Plots;
		}
		
		public function subdue( notify:Boolean = false ):void
		{
			for each( var plot:PlotProxy in plots )
			{
				plot.subdue( notify );
			}
		}
		
		public function emphasize( notify:Boolean = false ):void
		{
			for each( var plot:PlotProxy in plots )
			{
				plot.emphasize( notify );
			}
		}
		
		public function standardize(  notify:Boolean = false ):void
		{
			for each( var plot:PlotProxy in plots )
			{
				plot.standardize( notify );
			}
		}
	}
}