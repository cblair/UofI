package com.ts.importing.parsing.model
{
	import com.ts.data.enumerations.LevelOfDetail;
	import com.ts.data.model.vo.DataVO;
	import com.ts.importing.configuration.controller.ExternalImportConverter;
	import com.ts.importing.configuration.model.enumerations.ExternalImportDataName;
	
	import flash.sampler.NewObjectSample;
	
	import mx.utils.StringUtil;

	public class IN0ParsingPolicy extends AbstractParsingPolicy
	{
		public static const NAME:String = "IN0ParsingPolicy";
		
		private static const NULL_CHAR:String = "*";
		
		private var _linkTable:Array;
		private var _signalTable:Array;
		private var _detectorTable:Array;
		private var _tableMap:Object;
		
		public function IN0ParsingPolicy( externalImportProxies:Array, rawData:String, dataRange:int = 0, dataResolution:int = 0 )
		{
			super( externalImportProxies, rawData, NAME, dataRange, dataResolution );
			
			var rawDataRegions:Array = separateParts( rawData );
			
			_linkTable = formLinkTable( rawDataRegions[ 0 ], rawDataRegions[ 1 ] );
			_signalTable = formSignalTable( rawDataRegions[ 2 ] );
			_detectorTable = formDetectorTable( rawDataRegions[ 3 ] );
			_tableMap = formTableMap( _linkTable, _signalTable, _detectorTable );
		}
		
		public function get dataRange():int
		{
			var dataRangeString:String = _rawData.split( "SIMULATION_DURATION" )[ 1 ].split( "\r" )[ 0 ];
			return int( StringUtil.trim( dataRangeString ) );
		}
		
		public function get intervalsPerSecond():Number
		{
			var resolutionString:String = _rawData.split( "TIME_STEP" )[ 1 ].split( "\r" )[ 0 ];
			return Number( StringUtil.trim( resolutionString ) );
		}
		
		//-- create a header -> table lookup object. Key = external data name, Value = corresponding data table
		private function formTableMap( linkTable:Array, signalTable:Array, detectorTable:Array ):Object
		{
			var tableMap:Object = new Object();
			
			for each( var header:String in linkTable[ 0 ] )
			{
				tableMap[ header ] = linkTable;
			}
			
			for each( header in signalTable[ 0 ] )
			{
				tableMap[ header ] = signalTable;
			}
			
			for each( header in detectorTable[ 0 ] )
			{
				tableMap[ header ] = detectorTable;
			}
			
			return tableMap;
		}
		
		private function formLinkTable( rawLinkData:String, rawColumnData:String ):Array
		{
			//---------------------- variables -----------------------//
			
			//-- Array Table
			var linkTable:Array = new Array( [ "linkNum", "numLanes", "behavior", "linkPointX", "linkPointY", "fromLinkNum", "fromLinkAt", "toLinkNum", "toLinkAt", "isConnector" ] );
			
			//-- Gets each connector string chunk
			var connectors:Array = parseConnectors( rawColumnData );
			
			//-- Gets each link string chunck
			var links:Array = parseLinks( rawLinkData );
			
			//-- helpers
			var OVERIndex:int;
			var numberOfOvers:int;
			
			//-- Value catchers
			var linkNum:String;
			var numLanes:String;
			var fromLinkNum:String;
			var fromLinkAt:String;
			var toLinkNum:String;
			var toLinkAt:String;
			var isConnector:String;
			var behavior:String;
			var linkPointX:String;
			var linkPointY:String;
			
			//-- Parsing Helpers
			var from:Array;
			var to:Array;
			var over:Array;
			var linkNumTempArray:Array;
			var numLanesTempArray:Array;
			var overIndexes:Array;
			var currentOverString:String;
			var behaviorTypeArray:Array;
			
			
			
			//-------------------------- Parse Links --------------------------//
			
			for each( var link:String in links )
			{
				//-- These vars only apply to connectors
				fromLinkNum = fromLinkAt = toLinkNum = toLinkAt = NULL_CHAR;
				
				isConnector = "0";
				
				overIndexes = new Array();
				
				behaviorTypeArray = link.split( " BEHAVIORTYPE " );
				behaviorTypeArray = behaviorTypeArray[ 1 ].split( "DISPLAYTYPE" );
				behavior = StringUtil.trim( behaviorTypeArray[ 0 ] );
				
				from = link.split( " FROM ");
				from = from[1].split("  ");
				from = StringUtil.trim( from[ 0 ] ).split(" ");
				
				to = link.split(" TO ");
				to = to[1].split("  ");
				to = StringUtil.trim( to[ 1 ] ).split(" ");
				
				linkNumTempArray = link.split("LINK ");
				linkNumTempArray = linkNumTempArray[1].split("NAME");
				linkNum = linkNumTempArray[0].split(" ").join("");
				
				numLanesTempArray = link.split(" LANES ");
				numLanesTempArray = numLanesTempArray[1].split("LANE_WIDTH");
				numLanes = numLanesTempArray[0].split(" ").join("");
				
				//-- BEGIN calculate number of OVER elements in Link
				numberOfOvers = 0;
				OVERIndex = link.indexOf("OVER");
				overIndexes.push( OVERIndex );
				while( OVERIndex != -1 )
				{
					OVERIndex = link.indexOf("OVER", OVERIndex + 1);
					overIndexes.push( OVERIndex );
					numberOfOvers++;
				}
				//-- END calculate number of OVER elements in Link 
				
				
				//-- Create array of rows for this link
				
				linkTable.push( [  linkNum, numLanes, behavior, from[0], from[1], fromLinkNum, fromLinkAt, toLinkNum, toLinkAt, isConnector ] );
				
				if( numberOfOvers > 0 )
				{
					for( var i:int = 0; i < numberOfOvers; i++ )
					{
						//-- Create OVER variable
						currentOverString = link.slice( overIndexes[i], overIndexes[i + 1]).split("OVER").join("");
						over = currentOverString.split(" ");
						over = [ over[1], over[2] ];
						
						
						linkTable.push( [  linkNum, numLanes, behavior, over[0], over[1], fromLinkNum, fromLinkAt, toLinkNum, toLinkAt, isConnector ] );
					}
				}
				
				
				linkTable.push( [  linkNum, numLanes, behavior, to[0], to[1], fromLinkNum, fromLinkAt, toLinkNum, toLinkAt, isConnector ] );
			}
			
			//-------------------------- Parse connectors --------------------------//
			
			var at:Array
			
			for each( var connector:String in connectors )
			{
				behavior = NULL_CHAR;
				isConnector = "1";
				
				overIndexes = new Array();
				
				behaviorTypeArray = connector.split( " BEHAVIORTYPE " );
				behaviorTypeArray = behaviorTypeArray[ 1 ].split( "DISPLAYTYPE" );
				behavior = StringUtil.trim( behaviorTypeArray[ 0 ] );
				
				from = connector.split(" FROM LINK " );
				from = from[1].split(" ");
				
				fromLinkNum = from[0];
				fromLinkAt = connector.split( " AT " )[ 1 ].split( " " )[ 0 ];
				
				to = connector.split(" TO LINK ");
				to = to[1].split(" ");
				
				toLinkNum = to[0];
				toLinkAt = connector.split( " AT " )[ 2 ].split( " " )[ 0 ];
				
				linkNumTempArray = connector.split("CONNECTOR");
				linkNumTempArray = linkNumTempArray[1].split("NAME");
				linkNum = linkNumTempArray[0].split(" ").join("");
				
				//-- Connector lanes is different than Link lanes. Link lanes always has 1 value. Connector lanes has a value
				//-- for each lane ex. LANES 1 2 3 4 vs LANES 4.
				//-- Going to parse the values, and take the larges lane number.
				numLanesTempArray = connector.split("LANES");
				numLanesTempArray = numLanesTempArray[1].split("AT");
				numLanesTempArray = numLanesTempArray[ 0 ].split( " " );
				var numLanesMax:int = 1;
				for each( var string:String in numLanesTempArray )
				{
					if( !isNaN( Number( string ) ) )
					{
						numLanesMax = Math.max( Number( string ), numLanesMax );
					}
				}
				numLanes = String( numLanesMax );
				
				//-- BEGIN calculate number of OVER elements in Link
				numberOfOvers = 0;
				OVERIndex = connector.indexOf("OVER");
				overIndexes.push( OVERIndex );
				while( OVERIndex != -1 )
				{
					OVERIndex = connector.indexOf("OVER", OVERIndex + 1);
					overIndexes.push( OVERIndex );
					numberOfOvers++;
				}
				//-- END calculate number of OVER elements in Link 
				
				//-- Create array of rows for this link
				
				
				if( numberOfOvers > 0 )
				{
					for( i = 0; i < numberOfOvers; i++ )
					{
						//-- Create OVER variable
						currentOverString = connector.slice( overIndexes[i], overIndexes[i + 1]).split("OVER").join("");
						over = currentOverString.split(" ");
						over = [ over[1], over[2] ];
						
						
						linkTable.push( [  linkNum, numLanes, behavior, over[0], over[1], fromLinkNum, fromLinkAt, toLinkNum, toLinkAt, isConnector ] );
					}
				}
			}
			
			return linkTable;
		}
		
		private function formSignalTable( rawSignalData:String ):Array
		{
			//---------------------- variables -----------------------//
			
			//-- TABLE
			var signalTable:Array = new Array( [ "sGroup", "sC", "sLink", "sLane", "sAt" ] );
			
			//-- Parsing Helpers
			var signalGroupArray:Array;
			var signalControllerArray:Array;
			var signalLinkArray:Array;
			var signalLaneArray:Array;
			var signalAtArray:Array;
			
			//-- Gets each signal string chunk
			var signals:Array = parseSignals( rawSignalData );
			
			//-- Value Catchers
			var signalGroup:String;
			var signalController:String;
			var signalLink:String;
			var signalLane:String;
			var signalAt:String;
			
			//---------------------- parse signal data -----------------------//
			
			for each ( var signal:String in signals )
			{
				signalGroupArray = signal.split( "GROUP " );
				signalGroupArray = signalGroupArray[ 1 ].split( " " );
				signalGroup = StringUtil.trim( signalGroupArray[ 0 ] );
				
				signalControllerArray = signal.split( " SCJ " );
				signalControllerArray = signalControllerArray[ 1 ].split( " " );
				signalController = StringUtil.trim( signalControllerArray[ 0 ] );
				
				if( signal.match( " LINK " ) )
				{
					signalLinkArray = signal.split( " LINK " );
					signalLinkArray = signalLinkArray[ 1 ].split( " LANE " );
					signalLink = StringUtil.trim( signalLinkArray[ 0 ] );
				}
				
				if( signal.match( " LANE " ) )
				{
					signalLaneArray = signal.split( " LANE " );
					signalLaneArray = signalLaneArray[ 1 ].split( " AT " );
					signalLane = StringUtil.trim( signalLaneArray[ 0 ] );
				}
				
				if( signal.match( " AT " ) )
				{
					signalAtArray = signal.split( " AT " );
					signalAtArray = signalAtArray[ 1 ].split( " VEHICLE_CLASSES " );
					signalAt = StringUtil.trim( signalAtArray[ 0 ] );
					
					signalTable.push( [ signalGroup, signalController, signalLink, signalLane, signalAt ] );
				}
			}
			return signalTable;
		}
		
		private function formDetectorTable( rawDetectorData:String ):Array
		{
			//---------------------- variables -----------------------//
			
			//-- table
			var detectorTable:Array = new Array( [ "dGroup", "dC", "dLength", "dLink", "dLane", "dAt" ] );
			
			//-- value catchers
			var detectorGroup:String;
			var detectorController:String;
			var detectorLength:String;
			var detectorLink:String;
			var detectorLane:String;
			var detectorAt:String;
			
			//-- detector data chunks
			var detectors:Array = parseDetectors( rawDetectorData );
			
			//-- parsing helpers
			var detectorGroupArray:Array;
			var detectorControllerArray:Array;
			var detectorLengthArray:Array;
			var detectorLinkArray:Array;
			var detectorLaneArray:Array;
			var detectorAtArray:Array;
			
			//----------------------- parse detectors ---------------------------//
			
			for each( var detector:String in detectors )
			{
				detectorGroupArray = detector.split( "DETECTOR " );
				detectorGroupArray = detectorGroupArray[ 1 ].split( " NAME " );
				detectorGroup = StringUtil.trim( detectorGroupArray[ 0 ] );
				
				detectorControllerArray = detector.split( " SCJ " );
				detectorControllerArray = detectorControllerArray[ 1 ].split( " VEHICLE_CLASSES " );
				detectorController = StringUtil.trim( detectorControllerArray[ 0 ] );
				
				detectorLengthArray = detector.split( " LENGTH " );
				detectorLengthArray = detectorLengthArray[ 1 ].split( " SMOOTHING " );
				detectorLength = StringUtil.trim( detectorLengthArray[ 0 ] );
				
				detectorLinkArray = detector.split( " LINK " );
				detectorLinkArray = detectorLinkArray[ 1 ].split( " LANE " );
				detectorLink = StringUtil.trim( detectorLinkArray[ 0 ] );
				
				detectorLaneArray = detector.split( " LANE " );
				detectorLaneArray = detectorLaneArray[ 1 ].split( " AT " );
				detectorLane = StringUtil.trim( detectorLaneArray[ 0 ] );
				
				detectorAtArray = detector.split( " AT " );
				detectorAtArray = detectorAtArray[ 1 ].split( " LENGTH " );
				detectorAt = StringUtil.trim( detectorAtArray[ 0 ] );
				
				detectorTable.push( [ detectorGroup, detectorController, detectorLength, detectorLink, detectorLane, detectorAt ] );
			}
			
			return detectorTable;
		}
		
		private function parseLinks( linksString:String ):Array
		{
			var tempArray:Array = linksString.split("\r\n").join("").split("LINK");
			var linksArray:Array = new Array();
			var i:int;
			for( i = 0; i < tempArray.length; i++ )
			{
				if( tempArray[i] != "" )
				{
					tempArray[i] = "LINK" + tempArray[i];
					linksArray.push( tempArray[i] );
				}
			}
			
			return linksArray;
		}
		
		private function parseConnectors( connectorsString:String ):Array
		{
			var tempArray:Array = connectorsString.split("\r\n").join("").split("CONNECTOR");
			var connectorsArray:Array = new Array();
			var i:int;
			for( i = 0; i < tempArray.length; i++ )
			{
				if( tempArray[i] != "" )
				{
					tempArray[i] = "CONNECTOR" + tempArray[i];
					connectorsArray.push( tempArray[i] );
				}
			}
			
			return connectorsArray;
		}
		
		private function parseSignals( signalsString:String ):Array
		{
			var tempArray:Array = signalsString.split("\r\n").join("").split("SIGNAL_HEAD");
			var signalsArray:Array = new Array();
			var i:int;
			for( i = 1; i < tempArray.length; i++ )
			{
				if( tempArray[i] != "" )
				{
					tempArray[i] = "SIGNAL_HEAD" + tempArray[i];
					signalsArray.push( tempArray[i] );
				}
			}
			
			return signalsArray;
		}
		
		private function parseDetectors( detectorsString:String ):Array
		{
			var tempArray:Array = detectorsString.split("\r\n").join("").split("DETECTOR");
			var detectorsArray:Array = new Array();
			var i:int;
			for( i = 0; i < tempArray.length; i++ )
			{
				if( tempArray[i] != "" )
				{
					tempArray[i] = "DETECTOR" + tempArray[i];
					detectorsArray.push( tempArray[i] );
				}
			}
			
			return detectorsArray;
		}
		
		private function separateParts( rawData:String ):Array
		{
			var linksBeg:String = "-- Links: --\r\n------------";
			var linksEnd:String = "-- Connectors: --\r\n-----------------";
			var connectorsEnd:String = "-- Toll Pricing Calculation Models: --\r\n--------------------------------------";
			var signalBeg:String = "-- Signal Controllers (SC): --\r\n------------------------------";
			var signalEnd:String = "-- Detectors: --\r\n----------------";
			var detectorsEnd:String = "-- Public Transport: --\r\n-----------------------";
			
			var tempArray:Array = rawData.split( linksBeg );
			tempArray = tempArray[1].split( linksEnd );
			var links:String = tempArray[0];
			
			tempArray = tempArray[1].split( connectorsEnd );
			var connectors:String = tempArray[0];
			
			tempArray = rawData.split( signalBeg );
			tempArray = tempArray[ 1 ].split( signalEnd );
			var signals:String = tempArray[ 0 ];
			
			tempArray = tempArray[ 1 ].split( detectorsEnd );
			var detectors:String = tempArray[ 0 ];
			
			var separateParts:Array = [links, connectors, signals, detectors];
			return separateParts;
		}

		override protected function validateWorkingImportProxy():Boolean
		{
			//-- default, need to validate some how.
			return true;
		}
		
		override protected function parseWorkingProxy():void
		{
			// example code, dont know how much applies
			var levelOfDetail:String = LevelOfDetail.INTERSECTION;
			var lodID:int = 0;
			var valueList:Array;
			
			
			//-- get the appropriate data table, based on the name of the dataset. could be the linkTable, signalTable, or detectorTable.
			var dataTable:Array = getDataTableByName( _workingImportProxy.externalDataName );
			var index:int = getDataIndexByDataName( _workingImportProxy.externalDataName );
			var n:int = dataTable.length;
			
			
			//---------------------- get values, build data proxy -----------------------//
			
			if( index != -1 )
			{
				valueList = new Array();
				
				//-- Need to invert y values.
				if( _workingImportProxy.externalDataName == ExternalImportDataName.IN0_LINK_POINT_Y )
				{
					for( var i:int = 1; i < n; i++ )
					{
						valueList.push( String( Number( dataTable[ i ][ index ] ) * -1 ) );
					}
				}
				else
				{
					for( i = 1; i < n; i++ )
					{
						valueList.push( dataTable[ i ][ index ] );
					}
				}
				
				var data:DataVO = ExternalImportConverter.createDataVO( valueList, _workingImportProxy, 0, valueList.length );
				
				_newDataProxies.push( ExternalImportConverter.createDataProxy( data, _workingImportProxy, levelOfDetail, lodID, 0, _workingImportProxy.dataName + String( lodID ) + "0" ) );
				
				trace( _newDataProxies[ _newDataProxies.length - 1 ].toString( false ) );
			}
			
			//------------------------ call super to refresh display, and start next data set -------------------------//
			
			super.parseWorkingProxy();
		}
		
		private function getDataIndexByDataName( dataName:String ):int
		{
			return getDataTableByName( dataName )[ 0 ].indexOf( dataName );
		}
		
		private function getDataTableByName( dataName:String ):Array
		{
			return _tableMap[ dataName ];
		}
	}
}