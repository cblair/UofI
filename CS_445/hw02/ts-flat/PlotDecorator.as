package com.ts.aspect.model.graph.plot
{
	import com.ts.aspect.model.DecorationProxy;
	import com.ts.aspect.model.graph.plot.vo.PlotDecorationVO;

	public class PlotDecorator
	{
		private var _subdued:Boolean = false;
		private var _emphasized:Boolean = false;
		
		public var entries:Array;
		
		public function PlotDecorator( Entries:Array )
		{
			entries = Entries;
			standardize();
		}
		
		public function getDecoration( value:Number ):PlotDecorationVO
		{
			//-- Override me!!!
			return null;
		}
		
		public function subdue():void
		{
			_subdued = true;
			_emphasized = false;
			for each( var entry:DecorationProxy in entries )
			{
				entry.subdue();
			}
			
			refactorDecorator();
		}
		
		public function emphasize():void
		{
			_subdued = false;
			_emphasized = true;
			for each( var entry:DecorationProxy in entries )
			{
				entry.emphasize();
			}
			
			refactorDecorator();
		}
		
		public function standardize():void
		{
			_subdued = false;
			_emphasized = false;
			for each( var entry:DecorationProxy in entries )
			{
				entry.standardize();
			}
			
			refactorDecorator();	
		}
		
		protected function refactorDecorator():void
		{
			//-- Override me!!!
		}

	}
}