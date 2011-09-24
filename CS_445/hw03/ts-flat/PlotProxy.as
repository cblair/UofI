package com.ts.aspect.model.graph.plot
{
	import com.ts.aspect.model.DecorationProxy;
	import com.ts.aspect.model.vo.DecorationVO;
	
	public class PlotProxy extends DecorationProxy
	{
		public static const NAME:String = "plotProxy";
		public var plotDecorator:PlotDecorator;
		
		public function PlotProxy( Name:String, Decorator:PlotDecorator, Decoration:DecorationVO )
		{
			super( NAME + "/" + Name, Decoration );
			plotDecorator = Decorator;
		}
		
		override public function subdue( notify:Boolean=false ):void
		{
			super.subdue();
			plotDecorator.subdue();
		}
		
		override public function emphasize( notify:Boolean=false ):void
		{
			super.emphasize();
			plotDecorator.emphasize();
		}
		
		override public function standardize(notify:Boolean=false):void
		{
			super.standardize();
			plotDecorator.standardize();
		}
	}
}