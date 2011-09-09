package com.ts.presentation.model.map.phase
{
	import com.ts.presentation.model.map.phase.vo.LinkDataVO;
	
	import flash.geom.Point;

	public class LinkIndex
	{
		public var links:Array;
		public var length:Number;
		public var startLink:LinkProxy;
		public var endLink:LinkProxy;
		
		private var _linkLookup:Object;
		
		public function LinkIndex( Links:Array )
		{
			//-- derive values
			links = Links;
			if( links.length > 0 )
			{
				startLink = links[ 0 ].startLink;
				endLink = links[ 0 ].endLink;
				length = startLink.getLengthAlongLinks();
			}
			
			//-- build link index map ( index by link number )
			_linkLookup = new Object();
			for each( var link:LinkProxy in Links )
			{
				_linkLookup[ link.linkNum ] = link;
			}
		}
		
		public function get linkData():Array
		{
			var linkData:Array = new Array();
			for each( var link:LinkProxy in links )
			{
				linkData.push( new LinkDataVO( link.pathData, link.numLanes ) );
			}
			
			linkData.push( new LinkDataVO( startLink.pathData, startLink.numLanes ) );
			linkData.push( new LinkDataVO( endLink.pathData, endLink.numLanes ) );
			
			return linkData;
		}
		
		public function getDistanceAlongPath( pos:Number, linkNum:int ):Number
		{
			return startLink.getDistanceAlongPath( 0, linkNum, pos );
		}
		
		public function getPercentAlongPath( pos:Number, linkNum:int ):Number
		{
			var distance:Number = getDistanceAlongPath( pos, linkNum );
			if( isNaN( distance ) )
			{
				return NaN;
			}
			else
			{
				return distance / length;
			}
		}
		
		public function hasLink( linkNum:int ):Boolean
		{
			if( _linkLookup[ linkNum ] ) return true;
			return false;
		}
	}
}