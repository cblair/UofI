package com.ts.presentation.model.map
{
	import com.ts.presentation.model.map.vo.BackgroundLinkVO;
	import com.ts.presentation.model.map.vo.PathDataVO;
	import com.ts.utilities.drawing.PathUtilities;
	
	import org.puremvc.as3.patterns.proxy.Proxy;

	public class BackgroundNetworkProxy extends Proxy
	{
		static public const NAME:String = "backgroundNetworkProxy";
		
		public var backgroundLinks:Array;
		
		public function BackgroundNetworkProxy( BackgroundLinks:Array )
		{
			backgroundLinks = BackgroundLinks;
			super( NAME );
		}
		
		public function get networkPathData():Array
		{
			var pathData:Array = new Array();
			var workingAlpha:Number;
			for each( var link:BackgroundLinkVO in backgroundLinks )
			{
				//workingAlpha = link.isConnector && link.variance > 5 ? 0 : .5;
				//workingAlpha = link.isConnector ? 0 : .5;
				/*pathData.push( new PathDataVO( PathUtilities.pointsToPathData( link.points, false ), link.numLanes * .5, 
							   link.isConnector ? 0x880000 : 0x888888, workingAlpha, link.linkNumber, link.isConnector, link.variance ) );*/
				pathData.push( new PathDataVO( PathUtilities.pointsToPathData( link.points, false ), link.numLanes * 4, 
							   0xCCCCCC, 1, link.linkNumber, link.isConnector, link.variance ) );
			}
			
			return pathData;
		}
	}
}