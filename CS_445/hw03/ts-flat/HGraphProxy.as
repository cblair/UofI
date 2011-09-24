package com.ts.aspect.model.graph.hGraph
{
	import com.ts.aspect.model.DisplayProxy;
	import com.ts.aspect.model.graph.plot.vo.PlotIndex;
	import com.ts.aspect.model.vo.DecorationVO;
	import com.ts.aspect.model.vo.DisplayVO;
	
	public class HGraphProxy extends DisplayProxy
	{
		
		public static const NAME:String	= "hGraphProxy";
		
		public var plotIndex:PlotIndex;
		
		public function HGraphProxy( Name:String, PlotIndx:PlotIndex, Display:DisplayVO, Decoration:DecorationVO )
		{
			plotIndex = PlotIndx;
			super( NAME + Name, Display, Decoration );
		}
		
		override public function subdue( notify:Boolean=false ):void
		{
			plotIndex.subdue( false );
			super.subdue( true );
		}
		
		override public function emphasize( notify:Boolean=false ):void
		{
			plotIndex.emphasize( false );
			super.emphasize( true );
		}
		
		override public function standardize(notify:Boolean=false):void
		{
			plotIndex.standardize( false );
			super.standardize( true );
		}
	}
}