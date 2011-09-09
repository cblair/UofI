package com.ts.importing.deriveSystem.command
{
	import com.fManagement.utilities.StringUtilities;
	import com.ts.data.model.DataIndex;
	import com.ts.data.model.DataProxy;
	import com.ts.importing.configuration.model.enumerations.ExternalImportDataName;
	import com.ts.importing.configuration.model.enumerations.RequiredImportDataName;
	import com.ts.importing.deriveSystem.model.ImportDataIndex;
	import com.ts.importing.deriveSystem.vo.NetworkHelperDataVO;
	import com.ts.importing.virtualData.required.DetectorVFactory;
	import com.ts.importing.virtualData.required.SignalDisplayVFactory;
	import com.ts.presentation.model.map.BackgroundNetworkProxy;
	import com.ts.presentation.model.map.NetworkProxy;
	import com.ts.presentation.model.map.intersection.IntersectionIndex;
	import com.ts.presentation.model.map.intersection.IntersectionProxy;
	import com.ts.presentation.model.map.phase.DetectorIndex;
	import com.ts.presentation.model.map.phase.DetectorProxy;
	import com.ts.presentation.model.map.phase.LinkIndex;
	import com.ts.presentation.model.map.phase.LinkProxy;
	import com.ts.presentation.model.map.phase.PhaseIndex;
	import com.ts.presentation.model.map.phase.PhaseProxy;
	import com.ts.presentation.model.map.phase.SignalIndex;
	import com.ts.presentation.model.map.phase.SignalProxy;
	import com.ts.presentation.model.map.vo.BackgroundLinkVO;
	import com.ts.utilities.StringFormatters;
	import com.ts.utilities.drawing.PathUtilities;
	import com.ts.utilities.drawing.TrigCalculations;
	
	import flash.geom.Point;

	public class NetworkAssembly
	{
		private static var _linkMap:Object;
		
		public static function buildNetworkModel( importDataIndex:ImportDataIndex, nativeDataIndex:DataIndex, networkHelperData:NetworkHelperDataVO ):NetworkProxy
		{
			//loadDataIndex( importDataIndex, nativeDataIndex );
			return buildLinks( importDataIndex, networkHelperData.intersectionAnalysis.topLeftPoint );
		}
		
		private static function buildLinks( importIndex:ImportDataIndex, topLeftPoint:Point ):NetworkProxy
		{
			var linkNumbers:DataProxy 	= importIndex.getDataAtName( RequiredImportDataName.IN0_LINK_NUMBER )[ 0 ];
			var numLanes:DataProxy		= importIndex.getDataAtName( RequiredImportDataName.IN0_NUM_LANES )[ 0 ];
			var linkPointX:DataProxy	= importIndex.getDataAtName( RequiredImportDataName.IN0_LINK_POINT_X )[ 0 ];
			var linkPointY:DataProxy	= importIndex.getDataAtName( RequiredImportDataName.IN0_LINK_POINT_Y )[ 0 ];
			var fromLink:DataProxy		= importIndex.getDataAtName( RequiredImportDataName.IN0_FROM_LINK )[ 0 ];
			var fromLinkAt:DataProxy	= importIndex.getDataAtName( RequiredImportDataName.IN0_FROM_LINK_AT )[ 0 ];
			var toLink:DataProxy		= importIndex.getDataAtName( RequiredImportDataName.IN0_TO_LINK )[ 0 ];
			var toLinkAt:DataProxy		= importIndex.getDataAtName( RequiredImportDataName.IN0_TO_LINK_AT )[ 0 ];
			var isConnector:DataProxy	= importIndex.getDataAtName( RequiredImportDataName.IN0_IS_CONNECTOR )[ 0 ];
			var behavior:DataProxy		= importIndex.getDataAtName( RequiredImportDataName.IN0_LINK_BEHAVIOR )[ 0 ];
			var signalLink:DataProxy	= importIndex.getDataAtName( RequiredImportDataName.IN0_SIG_LINK )[ 0 ];
			
			var linkNumData:Array 		= linkNumbers.uncompressedData;
			var numLanesData:Array 		= numLanes.uncompressedData;
			var linkXData:Array 		= linkPointX.uncompressedData;
			var linkYData:Array 		= linkPointY.uncompressedData;
			var fromLinkData:Array 		= fromLink.uncompressedData;
			var fromLinkAtData:Array 	= fromLinkAt.uncompressedData;
			var toLinkData:Array 		= toLink.uncompressedData;
			var toLinkAtData:Array 		= toLinkAt.uncompressedData;
			var isConnectorData:Array 	= isConnector.uncompressedData;
			var behaviorData:Array 		= behavior.uncompressedData;
			var signalLinkData:Array	= signalLink.uncompressedData;
			
			
			var links:Array = new Array();
			var backgroundLinks:Array = new Array();
			var connectorLookup:Object = new Object();
			
			var n:int = linkNumbers.numValues;
			var h:int;
			var currentLinkNum:int = linkNumData[ 0 ];
			var currentPoints:Array = new Array();
			var currentLink:LinkProxy;
			var runningLength:Number = 0;
			var linkAngles:Array = new Array();
			
			var connectedLinks:Object = getConnectedLinks( fromLinkData, toLinkData );
			
			for( var i:int = 0; i < n; i++ )
			{
				//-- New Link
				if( currentLinkNum != linkNumData[ i ] )
				{
					h = i - 1;
					if( behaviorData[ h ] == 6 || ( behaviorData[ h ] == 1 && ( connectedLinks[ currentLinkNum ] == true || isConnectorData[ h ] ) ) )
					{
						var hasSignal:Boolean = ( signalLinkData.indexOf( linkNumData[ h ] ) != -1 )
						currentLink = new LinkProxy( linkNumData[ h ], numLanesData[ h ], currentPoints, StringFormatters.fixedNumber( runningLength, 3 ), -1, -1,
													 StringFormatters.fixedNumber( TrigCalculations.getAngleInDegrees( currentPoints[ 0 ], currentPoints[ 1 ] ), 3 ),
													 StringFormatters.fixedNumber( TrigCalculations.getAngleInDegrees( currentPoints[ currentPoints.length - 1 ], currentPoints[ currentPoints.length - 2 ] ), 3 ),
													 StringFormatters.fixedNumber( TrigCalculations.getAngleInDegrees( currentPoints[ 0 ], currentPoints[ currentPoints.length - 1 ] ), 3 ),
													 StringFormatters.fixedNumber( TrigCalculations.getVariance( linkAngles ), 3 ), Boolean( isConnectorData[ h ] ), hasSignal, 0, 
													 fromLinkAtData[ h ], toLinkAtData[ h ] );
						
						
						
						if( isConnectorData[ h ] && fromLinkData[ h ] != toLinkData[ h ] )
						{
							connectorLookup[ linkNumData[ h ] ] = new Array( linkNumData[ h ], fromLinkData[ h ], fromLinkAtData[ h ], toLinkData[ h ], toLinkAtData[ h ] )
						}
						
						links.push( currentLink );
						backgroundLinks.push( new BackgroundLinkVO( linkNumData[ h ], Boolean( isConnectorData[ h ] ), currentPoints, numLanesData[ h ], currentLink.angleVariance ) )
					}
					
					currentPoints = new Array();
					linkAngles = new Array();
					runningLength = 0;
					currentLinkNum = linkNumData[ i ];
					
				}
				
				currentPoints.push( new Point( StringFormatters.fixedNumber( linkXData[ i ] - topLeftPoint.x, 3 ), StringFormatters.fixedNumber( linkYData[ i ] - topLeftPoint.y, 3 ) ) );
				
				if( currentPoints.length > 1 )
				{
					runningLength += Point.distance( currentPoints[ currentPoints.length - 2 ], currentPoints[ currentPoints.length - 1 ] );
					linkAngles.push( TrigCalculations.getRelativeAngleInDegrees( currentPoints[ currentPoints.length - 2 ], currentPoints[ currentPoints.length - 1 ] ) );
				}
				trace( "linkNumber = " + currentLinkNum + "runningLength = " + runningLength );
			}
			
			//-- Add the last link
			h = i - 1;
			if( behaviorData[ h ] == 6 || ( behaviorData[ h ] == 1 && ( connectedLinks[ currentLinkNum ] == true || isConnectorData[ h ] ) ) )

			{
				hasSignal = ( signalLinkData.indexOf( linkNumData[ h ] ) != -1 )
				currentLink = new LinkProxy( linkNumData[ h ], numLanesData[ h ], currentPoints, StringFormatters.fixedNumber( runningLength, 3 ), -1, -1,
											 StringFormatters.fixedNumber( TrigCalculations.getAngleInDegrees( currentPoints[ 0 ], currentPoints[ 1 ] ), 3 ),
											 StringFormatters.fixedNumber( TrigCalculations.getAngleInDegrees( currentPoints[ currentPoints.length - 1 ], currentPoints[ currentPoints.length - 2 ] ), 3 ),
											 StringFormatters.fixedNumber( TrigCalculations.getAngleInDegrees( currentPoints[ 0 ], currentPoints[ currentPoints.length - 1 ] ), 3 ),
											 StringFormatters.fixedNumber( TrigCalculations.getVariance( linkAngles ), 3 ), Boolean( isConnectorData[ h ] ), hasSignal, 0, 
											 fromLinkAtData[ h ], toLinkAtData[ h ] );
	
				if(  isConnectorData[ h ] && fromLinkData[ h ] != toLinkData[ h ] )
				{
					connectorLookup[ linkNumData[ h ] ] = new Array( linkNumData[ h ], fromLinkData[ h ], fromLinkAtData[ h ], toLinkData[ h ], toLinkAtData[ h ] )
				}
				
				links.push( currentLink );
				backgroundLinks.push( new BackgroundLinkVO( linkNumData[ h ], Boolean( isConnectorData[ h ] ), currentPoints, numLanesData[ h ], currentLink.angleVariance ) );
			}
			
			//-- Makes a link lookup map, get links by name instead of by searching an array.
			//-- Key = linkNum, value = linkProxy
			_linkMap = new Object();
			for each( var link:LinkProxy in links )
			{
				_linkMap[ link.linkNum ] = link;
			}
			

			//-- Store a light copy of each link to draw the network background layer
			var backgroundNetworkModel:BackgroundNetworkProxy = new BackgroundNetworkProxy( backgroundLinks );
			var phases:Array = getPhases( importIndex, links, connectorLookup );
			var intersections:Array = getIntersections( phases );
			var detectorIndex:DetectorIndex = getDetectorIndex( importIndex );
				
			return new NetworkProxy( new IntersectionIndex( intersections ), backgroundNetworkModel, detectorIndex );
		}
		
		//-- Returns an object with key value pairs of all links with connections. Key = LinkNum, value = True or null
		private static function getConnectedLinks( fromLinks:Array, toLinks:Array ):Object
		{
			var uniqueList:Object = new Object();
			
			for each( var linkNum:int in fromLinks )
			{
				uniqueList[ linkNum ] = true;
			}
			
			for each( linkNum in toLinks )
			{
				uniqueList[ linkNum ] = true;
			}
			
			return uniqueList;
		}
		
		private static function getDetectorIndex( importIndex:ImportDataIndex ):DetectorIndex
		{
			var detectorNum:Array 	= importIndex.getDataAtName( RequiredImportDataName.IN0_DET_GROUP )[ 0 ].uncompressedData;
			var detectorLink:Array 	= importIndex.getDataAtName( RequiredImportDataName.IN0_DET_LINK )[ 0 ].uncompressedData;
			var detectorInt:Array 	= importIndex.getDataAtName( RequiredImportDataName.IN0_DET_INTERSECTION )[ 0 ].uncompressedData;
			var detectorPos:Array 	= importIndex.getDataAtName( RequiredImportDataName.IN0_DET_POS )[ 0 ].uncompressedData;
			var detectorLane:Array 	= importIndex.getDataAtName( RequiredImportDataName.IN0_DET_LANE )[ 0 ].uncompressedData;
			var detectorLength:Array= importIndex.getDataAtName( RequiredImportDataName.IN0_DET_LENGTH )[ 0 ].uncompressedData;
			
			var detectorGroups:Object = new Object();
			
			var detectors:Array = new Array();
			for( var i:int = 0; i < detectorNum.length; i++ )
			{
				var detLink:LinkProxy = _linkMap[ detectorLink[ i ] ]
					
				var detPos:Point = detLink.getPointAtDistance( detectorPos[ i ] );
				detPos.x = StringFormatters.fixedNumber( detPos.x, 3 );
				detPos.y = StringFormatters.fixedNumber( detPos.y, 3 );
				
				detectors.push( new DetectorProxy( detectorNum[ i ],
												   detectorInt[ i ],
												   detectorLink[ i ],
												   detPos,
												   StringFormatters.fixedNumber( detectorLength[ i ], 3 ),
												   StringFormatters.fixedNumber( detLink.getTangilinearAngle( detectorPos[ i ] ), 3 ),
												   detLink.numLanes ) );
			}
			/*for( i = 0; i < detectorNum.length; i++ )
			{
				key = String( detectorInt[ i ] ) + "-" + String( detectorNum[ i ] );
				if( detectorGroups[ key ] == null )
				{
					detectorGroups[ key ] = new Array();
				}
				
				var detLink:LinkProxy = _linkMap[ detectorLink[ i ] ]
				detectorGroups[ key ].push( new DetectorProxy( detectorNum[ i ],
															   detectorInt[ i ],
															   detectorLink[ i ],
															   detLink.getPointAtDistance( detectorPos[ i ] ),
															   detLink.getTangilinearAngle( detectorPos[ i ] ),
															   detLink.numLanes ) );
			}*/
			return new DetectorIndex( detectors );			
		}
		
		private static function getPhases( importIndex:ImportDataIndex, links:Array, connections:Object ):Array
		{
			var signalNum:Array = importIndex.getDataAtName( RequiredImportDataName.IN0_SIG_GROUP )[ 0 ].uncompressedData;
			var signalLink:Array = importIndex.getDataAtName( RequiredImportDataName.IN0_SIG_LINK )[ 0 ].uncompressedData;
			var intersection:Array = importIndex.getDataAtName( RequiredImportDataName.IN0_SIG_INTERSECTION )[ 0 ].uncompressedData;
			var signalPos:Array = importIndex.getDataAtName( RequiredImportDataName.IN0_SIG_POS )[ 0 ].uncompressedData;
			var signalLane:Array = importIndex.getDataAtName( RequiredImportDataName.IN0_SIG_LANE )[ 0 ].uncompressedData;
			
			var signalGroups:Object = new Object();
			var signalGroupKeys:Array = new Array();
			
			var key:String;
			for( var i:int = 0; i < signalNum.length; i++ )
			{
				key = String( intersection[ i ] ) + "-" + String( signalNum[ i ] );
				if( signalGroups[ key ] == null )
				{
					signalGroups[ key ] = new Array();
				}
				
				var sigLink:LinkProxy = _linkMap[ signalLink[ i ] ]
				var sigPos:Point = sigLink.getPointAtDistance( signalPos[ i ] );
				sigPos.x = StringFormatters.fixedNumber( sigPos.x, 3 );
				sigPos.y = StringFormatters.fixedNumber( sigPos.y, 3 );
				
				signalGroups[ key ].push( new SignalProxy( signalNum[ i ],
														   intersection[ i ],
														   signalLink[ i ],
														   sigPos,
														   signalPos[ i ],
														   StringFormatters.fixedNumber( sigLink.getTangilinearAngle( signalPos[ i ] ), 3 ),
														   sigLink.numLanes ) );
			}
			
			//-- want to order all even phases before odd phases.
			var oddPhases:Array = new Array();
			var evenPhases:Array = new Array();
			for( key in signalGroups )
			{
				if( int( key.split( "-" )[ 1 ] ) % 2 == 0 )
				{
					evenPhases.push( key );
				}
				else
				{
					oddPhases.push( key );
				}
			}
			
			//-- have do this in the correct order, adding odd phases to the end of the even phases.
			signalGroupKeys = evenPhases.concat( oddPhases );
			
			var fromConnections:Object = new Object();
			var toConnections:Object = new Object();
			
			var fromLink:int;
			var toLink:int;
			var connector:int;
			
			//-- Makes 2 lookup maps, key = link, value = array of link proxys that are also connectors
			//-- a "to" map and a "from" map
			for each( var connection:Array in connections )
			{
				connector = connection[ 0 ];
				fromLink = connection[ 1 ];
				toLink = connection[ 3 ];
				
				if( fromConnections[ fromLink ] == null )
				{
					fromConnections[ fromLink ] = new Array();
				}
				
				if( toConnections[ toLink ] == null )
				{
					toConnections[ toLink ] = new Array();
				}
				
				fromConnections[ fromLink ].push( _linkMap[ connector ] );
				toConnections[ toLink ].push( _linkMap[ connector ] );
			}
			
			//-- Assets are built, now link the system together --//
			
			var sigNum:int;
			var sigLinkNum:int;
			var sigIntersection:int;
			var sigPosition:Number;
			var sigLinks:Array;
			
			var phases:Array = new Array();
			var signal:SignalProxy
			
			for each( var signalKey:String in signalGroupKeys )
			{
				//-- get the signal group on the link with the most lanes - probably the arterial signal group. want that signal to represent the phase.
				signal = signalGroups[ signalKey ][ 0 ];
				for( i = 1; i < signalGroups[ signalKey ].length; i++ )
				{
					if( signal.numLanes < signalGroups[ signalKey ][ i ].numLanes )
					{
						signal = signalGroups[ signalKey ][ i ];
					}
				}
				
				sigNum 			= signal.groupNum;
				sigLinkNum 		= signal.linkNum;
				sigIntersection = signal.intersectionNum;
				sigLink 		= _linkMap[ signal.linkNum ];
				sigLinks 		= new Array();
				sigLinks.push( sigLink );
				trace( sigNum );
				if( sigNum % 2 == 0 )
				{
					sigLinks = linkFrom( sigLinks, sigLink, toConnections, connections, _linkMap );
					sigLinks = linkTo( sigLinks, sigLink, fromConnections, connections, _linkMap );
				}
				
				var linkIndex:LinkIndex = new LinkIndex( sigLinks );
				var signals:Array = new Array();
				for each( signal in signalGroups[ signalKey ] )
				{
					signal.at = linkIndex.getDistanceAlongPath( signal.at, signal.linkNum );
					signals.push( signal );
				}
				
				
				phases.push( new PhaseProxy( linkIndex, new SignalIndex( signals ) ) );
			}
			
			return phases;
		}
		
		private static function linkTo( links:Array, currentLink:LinkProxy, fromConnections:Object, connections:Object, linkMap:Object ):Array
		{
			//-- Add the link to the links array
			if( links.indexOf( currentLink ) == -1 )
			{
				links.push( currentLink );
			}
			
			//-- From link is the next link. currentLink's "TO" connects to next's "FROM"
			var nextLink:LinkProxy;
			if( currentLink.isConnector )
			{
				nextLink = linkMap[ connections[ currentLink.linkNum ][ 3 ] ];
				if( nextLink )
				{
					if( nextLink.hasSignal || nextLink.isTerminalLink || nextLink.prevLink )
					{
						currentLink.isTerminalLink = true;
						return links;
					}
					
					currentLink.nextLink = nextLink;
					nextLink.prevLink = currentLink;
				}
				else
				{
					currentLink.isTerminalLink = true;
					return links;
				}
			}
			else
			{
				var fromLinks:Array = fromConnections[ currentLink.linkNum ];
				if( fromLinks )
				{
					nextLink = fromLinks[ 0 ];
					for each( var link:LinkProxy in fromLinks )
					{
						//-- If there are more than 1 connector, pick one with lowest variance.
						nextLink = nextLink.angleVariance <= link.angleVariance ? nextLink : link;
					}
					
					//-- end if the next signal has a link
					if( nextLink.hasSignal || nextLink.isTerminalLink || nextLink.prevLink )
					{
						currentLink.isTerminalLink = true;
						return links;
					}
					
					currentLink.nextLink = nextLink;
					nextLink.prevLink = currentLink;
				}
				//-- no more connectors, end Recursion
				else
				{
					currentLink.isTerminalLink = true;
					return links;
				}
			}
			
			//-- Recurse, pass in nextLink as the next currentLink
			return linkTo( links, nextLink, fromConnections, connections, linkMap );
		}
		
		private static function linkFrom( links:Array, currentLink:LinkProxy, toConnections:Object, connections:Object, linkMap:Object ):Array
		{
			//-- Add the link to the links array
			if( links.indexOf( currentLink ) == -1 )
			{
				links.push( currentLink );
			}
			
			//-- To link is the prev link. currentLink's "FROM" connects to prev's "TO"
			var prevLink:LinkProxy;
			if( currentLink.isConnector )
			{
				prevLink = linkMap[ connections[ currentLink.linkNum ][ 1 ] ];
				if( prevLink )
				{
					if( prevLink.hasSignal || prevLink.isTerminalLink || prevLink.nextLink )
					{
						currentLink.isTerminalLink = true;
						return links;
					}
				
					currentLink.prevLink = prevLink;
					prevLink.nextLink = currentLink;
				}
				else
				{
					currentLink.isTerminalLink = true;
					return links;
				}
			}
			else
			{
				var toLinks:Array = toConnections[ currentLink.linkNum ];
				if( toLinks )
				{
					prevLink = toLinks[ 0 ];
					for each( var link:LinkProxy in toLinks )
					{
						//-- If there are more than 1 connector, pick one with lowest variance.
						prevLink = prevLink.angleVariance <= link.angleVariance ? prevLink : link;
					}
					if( prevLink.hasSignal || prevLink.isTerminalLink || prevLink.nextLink )
					{
						currentLink.isTerminalLink == true;
						return links;
					}
					
					currentLink.prevLink = prevLink;
					prevLink.nextLink = currentLink;
				}
				//-- no more connectors, end Recursion
				else
				{
					currentLink.isTerminalLink == true;
					return links;
				}
			}
			
			//-- Recurse, pass in prevLink as the next currentLink
			return linkFrom( links, prevLink, toConnections, connections, linkMap );
		}
		
/*		private static function linkTo( links:Array, currentLink:LinkProxy, fromConnections:Object, connections:Object, linkMap:Object ):Array
		{
			//-- Add the link to the links array
			if( links.indexOf( currentLink ) == -1 )
			{
				links.push( currentLink );
			}
			
			//-- From link is the next link. currentLink's "TO" connects to next's "FROM"
			var nextLink:LinkProxy;
			if( currentLink.isConnector )
			{
				nextLink = linkMap[ connections[ currentLink.linkNum ][ 3 ] ];
				if( nextLink.hasSignal == true || nextLink.prevLink != null ) return links;
				
				currentLink.nextLink = nextLink;
				nextLink.prevLink = currentLink;
			}
			else
			{
				var fromLinks:Array = fromConnections[ currentLink.linkNum ];
				if( fromLinks )
				{
					nextLink = fromLinks[ 0 ];
					for each( var link:LinkProxy in fromLinks )
					{
						//-- If there are more than 1 connector, pick one with lowest variance.
						nextLink = nextLink.angleVariance <= link.angleVariance ? nextLink : link;
					}
					
					//-- end if the next signal has a link
					if( nextLink.hasSignal == true || nextLink.prevLink != null ) return links;
					
					currentLink.nextLink = nextLink;
					nextLink.prevLink = currentLink;
				}
					//-- no more connectors, end Recursion
				else
				{
					return links;
				}
			}
			
			//-- Recurse, pass in nextLink as the next currentLink
			linkTo( links, nextLink, fromConnections, connections, linkMap );
			return links;
		}
		
		private static function linkFrom( links:Array, currentLink:LinkProxy, toConnections:Object, connections:Object, linkMap:Object ):Array
		{
			//-- Add the link to the links array
			if( links.indexOf( currentLink ) == -1 )
			{
				links.push( currentLink );
			}
			
			//-- To link is the prev link. currentLink's "FROM" connects to prev's "TO"
			var prevLink:LinkProxy;
			if( currentLink.isConnector )
			{
				prevLink = linkMap[ connections[ currentLink.linkNum ][ 1 ] ];
				if( prevLink.hasSignal == true || prevLink.nextLink != null ) return links;
				
				currentLink.prevLink = prevLink;
				prevLink.nextLink = currentLink;
			}
			else
			{
				var toLinks:Array = toConnections[ currentLink.linkNum ];
				if( toLinks )
				{
					prevLink = toLinks[ 0 ];
					for each( var link:LinkProxy in toLinks )
					{
						//-- If there are more than 1 connector, pick one with lowest variance.
						prevLink = prevLink.angleVariance <= link.angleVariance ? prevLink : link;
					}
					if( prevLink.hasSignal == true || prevLink.nextLink != null ) return links;
					
					currentLink.prevLink = prevLink;
					prevLink.nextLink = currentLink;
				}
					//-- no more connectors, end Recursion
				else
				{
					return links;
				}
			}
			
			//-- Recurse, pass in prevLink as the next currentLink
			linkFrom( links, prevLink, toConnections, connections, linkMap );
			return links;
		}*/
		
		private static function getIntersections( phases:Array ):Array
		{
			var intersectionMap:Object = new Object();
			var intersections:Array = new Array();
			var workingIntersection:int;
			
			//-- group phases by intersection. Key = intersectionNumber, Value = array of phases
			for each( var phase:PhaseProxy in phases )
			{
				workingIntersection = phase.intersectionNumber;
				
				if( intersectionMap[ workingIntersection ] == null )
				{
					intersectionMap[ workingIntersection ] = new Array();
				}
				
				intersectionMap[ workingIntersection ].push( phase );
			}
			
			//-- Make an intersection around each phase group
			for each( var phaseGroup:Array in intersectionMap )
			{
				intersections.push( new IntersectionProxy( new PhaseIndex( phaseGroup ), phaseGroup[ 0 ].intersectionNumber ) );
			}
			
			return intersections;
		}
		
		private static function loadDataIndex( importIndex:ImportDataIndex, dataIndex:DataIndex ):void
		{
			
			var transferData:Array = importIndex.getDataAtName( ExternalImportDataName.LDP_SIG_DISPLAY );
			transferData = transferData.concat( importIndex.getDataAtName( ExternalImportDataName.LDP_DETECTOR ) );
			
			for each( var proxy:DataProxy in transferData )
			{
				dataIndex.indexDataProxy( proxy );
			}
		}
	}
}