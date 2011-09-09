package com.ts.save.controller
{
	import com.ts.load.xml.BackgroundAttribute;
	import com.ts.load.xml.DetectorAttribute;
	import com.ts.load.xml.IntersectionAttribute;
	import com.ts.load.xml.LinkAttribute;
	import com.ts.load.xml.SignalAttribute;
	import com.ts.load.xml.SystemElement;
	import com.ts.load.xml.SystemMetadataAttribute;
	import com.ts.load.xml.TrafficAttribute;
	import com.ts.load.xml.VehicleAttribute;
	import com.ts.presentation.model.map.BackgroundNetworkProxy;
	import com.ts.presentation.model.map.NetworkProxy;
	import com.ts.presentation.model.map.SystemProxy;
	import com.ts.presentation.model.map.TrafficProxy;
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
	import com.ts.presentation.model.map.vehicle.VehicleProxy;
	import com.ts.presentation.model.map.vo.BackgroundLinkVO;
	import com.ts.presentation.model.map.vo.SystemMetadataVO;
	import com.ts.presentation.model.map.vo.TrafficMetadataVO;
	import com.ts.utilities.drawing.PathUtilities;
	
	import mx.core.mx_internal;

	public class SystemPackers
	{
		/**
		 * Packs the SystemModel ( systemProxy ) into a savable XML element
		 * 
		 * @param systemModel the system's state manager
		 * @param rootElement the system's root xml element - <system/>
		 * 
		 * @return a system XML element
		 **/
		public static function packSystem( systemModel:SystemProxy, rootElement:XML ):XML
		{
			rootElement.appendChild( packSystemMetadataVO( systemModel.metadata ) );
			rootElement.appendChild( packNetwork( systemModel.networkModel ) );
			rootElement.appendChild( packTraffic( systemModel.trafficModel ) );
			
			return rootElement;
		}
		
		/**
		 * Packs a SystemMetadataVO into an XML element
		 * 
		 * @param sMetadataVO the system's metadata
		 * 
		 * @return a system metadata XML element
		 **/
		public static function packSystemMetadataVO( sMetadataVO:SystemMetadataVO ):XML
		{
			var sMetadataXML:XML										= new XML( "<" + SystemElement.SYSTEM_METADATA + "/>" );
			sMetadataXML.@[ SystemMetadataAttribute.MIN_ITERATIONS ]	= sMetadataVO.minIterations;
			sMetadataXML.@[ SystemMetadataAttribute.MAX_ITERATIONS ]	= sMetadataVO.maxIterations;
			sMetadataXML.@[ SystemMetadataAttribute.NUM_INTERSECTIONS ]	= sMetadataVO.numIntersections;
			sMetadataXML.@[ SystemMetadataAttribute.NUM_PHASES ]		= sMetadataVO.numPhases;
			sMetadataXML.@[ SystemMetadataAttribute.NUM_VEHICLES ]		= sMetadataVO.numVehicles;
			sMetadataXML.@[ SystemMetadataAttribute.TIME_RESOLUTION ]	= sMetadataVO.timeResolution;
			sMetadataXML.@[ SystemMetadataAttribute.PX_PER_FEET ]		= sMetadataVO.pxPerFt;
			sMetadataXML.@[ SystemMetadataAttribute.ASPECT ]			= sMetadataVO.aspect;
			
			return sMetadataXML;
		}
		
		/**
		 * Packs the NetworkProxy into an XML element
		 * 
		 * @param network the system's network state manager
		 * 
		 * @return a network XML element
		 **/
		public static function packNetwork( network:NetworkProxy ):XML
		{
			var networkXML:XML = new XML( "<" + SystemElement.NETWORK + "/>" );
			
			networkXML.appendChild( packIntersections( network.intersectionIndex ) );
			networkXML.appendChild( packBackgroundNetwork( network.backgroundNetworkModel ) );
			networkXML.appendChild( packDetectors( network.detectorIndex ) );
			
			return networkXML;
		}
		
		/**
		 * Packs a BackgroundNetwork into an XML List of backgroundLink elements
		 * 
		 * @param backgroundNetwork a collection of backgroundLinks
		 * 
		 * @return a list of XML backgroundLinks
		 **/
		public static function packBackgroundNetwork( backgroundNetwork:BackgroundNetworkProxy ):XMLList
		{
			var openTag:String 		= "<" + SystemElement.BACKGROUND + ">";
			var dataString:String 	= "";
			var closeTag:String 	= "</" + SystemElement.BACKGROUND + ">";
			
			for each( var link:BackgroundLinkVO in backgroundNetwork.backgroundLinks )
			{
				dataString += packBackgroundLinkVO( link ).toXMLString(); 
			}
			
			return new XMLList( openTag + dataString + closeTag );
		}
		
		/**
		 * Packs a backgroundLinkVO into an XML element
		 * 
		 * @param bLink defines a background link
		 * 
		 * @return a background link XML element
		 **/
		public static function packBackgroundLinkVO( bLink:BackgroundLinkVO ):XML
		{
			var bLinkXML:XML 			= new XML( "<" + SystemElement.BACKGROUND_LINK + "/>" );
			
			bLinkXML.@[ BackgroundAttribute.LINK_NUM ] 		= bLink.linkNumber;
			bLinkXML.@[ BackgroundAttribute.IS_CONNECTOR ] 	= bLink.isConnector.toString();
			bLinkXML.@[ BackgroundAttribute.POINTS ] 		= PathUtilities.pointsToString( bLink.points );
			bLinkXML.@[ BackgroundAttribute.NUM_LANES ] 	= bLink.numLanes;
			bLinkXML.@[ BackgroundAttribute.VARIANCE ] 		= bLink.variance;
			
			return bLinkXML;
		}
		
		/**
		 * Packs a DetectorIndex into an XML List of detector elements
		 * 
		 * @param detectorIndex a collection of detectorProxies
		 * 
		 * @return a list of XML detectors
		 **/
		public static function packDetectors( detectorIndex:DetectorIndex ):XMLList
		{
			var openTag:String 		= "<" + SystemElement.DETECTORS + ">";
			var dataString:String 	= "";
			var closeTag:String 	= "</" + SystemElement.DETECTORS + ">";
			
			for each( var detector:DetectorProxy in detectorIndex.detectors )
			{
				dataString += packDetectorProxy( detector ).toXMLString(); 
			}
			
			return new XMLList( openTag + dataString + closeTag );
		}
		
		/**
		 * Packs a DetectorProxy into an XML element
		 * 
		 * @param detector manages the state of a detector
		 * 
		 * @return a detector XML element
		 **/
		public static function packDetectorProxy( detector:DetectorProxy ):XML
		{
			var detectorXML:XML 								= new XML( "<" + SystemElement.DETECTOR + "/>" );
			detectorXML.@[ DetectorAttribute.GROUP_NUM ] 		= detector.groupNum;
			detectorXML.@[ DetectorAttribute.INTERSECTION_NUM ] = detector.intersectionNum;
			detectorXML.@[ DetectorAttribute.LINK_NUM ]			= detector.linkNum;
			detectorXML.@[ DetectorAttribute.POSITION ] 		= PathUtilities.pointToString( detector.position );
			detectorXML.@[ DetectorAttribute.LENGTH ] 			= detector.length;
			detectorXML.@[ DetectorAttribute.ANGLE ] 			= detector.angle;
			detectorXML.@[ DetectorAttribute.NUM_LANES ] 		= detector.numLanes;
			
			return detectorXML;
		}
		
		/**
		 * Packs an IntersectionIndex into an XML List of intersection elements
		 * 
		 * @param intersectionIndex a collection of intersectionProxies
		 * 
		 * @return a list of XML intersectionProxies
		 **/
		public static function packIntersections( intersectionIndex:IntersectionIndex ):XMLList
		{
			var openTag:String 		= "<" + SystemElement.INTERSECTIONS + ">";
			var dataString:String 	= "";
			var closeTag:String 	= "</" + SystemElement.INTERSECTIONS + ">";
			
			for each( var intersection:IntersectionProxy in intersectionIndex.intersections )
			{
				dataString += packIntersectionProxy( intersection ).toXMLString(); 
			}
			
			return new XMLList( openTag + dataString + closeTag );
		}
		
		/**
		 * Packs an IntersectionProxy into an XML element
		 * 
		 * @param intersection manages the state of an intersection
		 * 
		 * @return an intersection XML element
		 **/
		public static function packIntersectionProxy( intersection:IntersectionProxy ):XML
		{
			var intersectionXML:XML 			= new XML( "<" + SystemElement.INTERSECTION + "/>" );
			
			intersectionXML.@[ IntersectionAttribute.INTERSECTION_NUM ] = intersection.intersectionNumber;
			intersectionXML.appendChild( packPhases( intersection.phaseIndex ) );
			
			return intersectionXML;
		}
		
		/**
		 * Packs a PhaseIndex into an XML List of phase elements
		 * 
		 * @param phaseIndex a collection of phaseProxies
		 * 
		 * @return a list of XML phaseProxies
		 **/
		public static function packPhases( phaseIndex:PhaseIndex ):XMLList
		{
			var openTag:String 		= "<" + SystemElement.PHASES + ">";
			var dataString:String 	= "";
			var closeTag:String 	= "</" + SystemElement.PHASES + ">";
			
			for each( var phase:PhaseProxy in phaseIndex.phases )
			{
				dataString += packPhaseProxy( phase ).toXMLString(); 
			}
			
			return new XMLList( openTag + dataString + closeTag );
		}
		
		/**
		 * Packs a PhaseProxy into an XML element
		 * 
		 * @param phase manages the state of a phase
		 * 
		 * @return a phase XML element
		 **/
		public static function packPhaseProxy( phase:PhaseProxy ):XML
		{
			var phaseXML:XML 			= new XML( "<" + SystemElement.PHASE + "/>" );
			
			phaseXML.appendChild( packSignals( phase.signalIndex ) );
			phaseXML.appendChild( packLinks( phase.linkIndex ) );
			
			return phaseXML;
		}
		
		/**
		 * Packs a SignalIndex into an XML List of signal elements
		 * 
		 * @param signalIndex a collection of signalProxies
		 * 
		 * @return a list of XML signals
		 **/
		public static function packSignals( signalIndex:SignalIndex ):XMLList
		{
			var openTag:String 		= "<" + SystemElement.SIGNALS + ">";
			var dataString:String 	= "";
			var closeTag:String 	= "</" + SystemElement.SIGNALS + ">";
			
			for each( var signal:SignalProxy in signalIndex.signals )
			{
				dataString += packSignalProxy( signal ).toXMLString(); 
			}
			
			return new XMLList( openTag + dataString + closeTag );
		}
		
		/**
		 * Packs a SignalProxy into an XML element
		 * 
		 * @param signal manages the state of a signal
		 * 
		 * @return a signal XML element
		 **/
		public static function packSignalProxy( signal:SignalProxy ):XML
		{
			var signalXML:XML 								= new XML( "<" + SystemElement.SIGNAL + "/>" );
			signalXML.@[ SignalAttribute.GROUP_NUM ] 		= signal.groupNum;
			signalXML.@[ SignalAttribute.INTERSECTION_NUM ] = signal.intersectionNum;
			signalXML.@[ SignalAttribute.LINK_NUM ] 		= signal.linkNum;
			signalXML.@[ SignalAttribute.POSITION ] 		= PathUtilities.pointToString( signal.position );
			signalXML.@[ SignalAttribute.AT ] 				= signal.at;
			signalXML.@[ SignalAttribute.ANGLE ] 			= signal.angle;
			signalXML.@[ SignalAttribute.NUM_LANES ] 		= signal.numLanes;
			
			return signalXML;
		}
		
		/**
		 * Packs a LinkIndex into an XML List of link elements
		 * 
		 * @param linkIndex a collection of linkProxies
		 * 
		 * @return a list of XML links
		 **/
		public static function packLinks( linkIndex:LinkIndex ):XMLList
		{
			var openTag:String 		= "<" + SystemElement.LINKS + ">";
			var dataString:String 	= "";
			var closeTag:String 	= "</" + SystemElement.LINKS + ">";
			
			for each( var link:LinkProxy in linkIndex.links )
			{
				dataString += packLinkProxy( link ).toXMLString(); 
			}
			
			return new XMLList( openTag + dataString + closeTag );
		}
		
		/**
		 * Packs a LinkProxy into an XML element
		 * 
		 * @param link data that describes a path
		 * 
		 * @return a link XML element
		 **/
		public static function packLinkProxy( link:LinkProxy ):XML
		{
			var linkXML:XML 							= new XML( "<" + SystemElement.LINK + "/>" );
			linkXML.@[ LinkAttribute.LINK_NUM ] 		= link.linkNum;
			linkXML.@[ LinkAttribute.NUM_LANES ] 		= link.numLanes;
			linkXML.@[ LinkAttribute.POINTS ] 			= PathUtilities.pointsToString( link.points );
			linkXML.@[ LinkAttribute.LINK_LENGTH ] 		= link.linkLength;
			linkXML.@[ LinkAttribute.FORK_LENGTH ] 		= link.forkLength;
			linkXML.@[ LinkAttribute.FROM_AT ] 			= link.fromAt;
			linkXML.@[ LinkAttribute.TO_AT ] 			= link.toAt;
			linkXML.@[ LinkAttribute.PREV_LINK_NUM ] 	= link.prevLinkNum;
			linkXML.@[ LinkAttribute.NEXT_LINK_NUM ] 	= link.nextLinkNum;
			linkXML.@[ LinkAttribute.FROM_ANGLE ] 		= link.fromAngle;
			linkXML.@[ LinkAttribute.TO_ANGLE ] 		= link.toAngle;
			linkXML.@[ LinkAttribute.AVG_ANGLE ] 		= link.avgAngle;
			linkXML.@[ LinkAttribute.ANGLE_VARIANCE ] 	= link.angleVariance;
			linkXML.@[ LinkAttribute.IS_CONNECTOR ] 	= link.isConnector.toString();
			linkXML.@[ LinkAttribute.HAS_SIGNAL ] 		= link.hasSignal.toString();
			linkXML.@[ LinkAttribute.SIGNAL_NUM ] 		= link.signalNum;
			linkXML.@[ LinkAttribute.IS_TERMINAL_LINK ] = link.isTerminalLink.toString();
			
			return linkXML;
		}
		
		/**
		 * Packs the TrafficProxy into an XML element
		 * 
		 * @param traffic the system's traffic state manager
		 * 
		 * @return a traffic XML element
		 **/
		public static function packTraffic( traffic:TrafficProxy ):XML
		{
			var trafficXML:XML	= new XML( "<" + SystemElement.TRAFFIC + "/>" );
			
			trafficXML.appendChild( packTrafficMetadataVO( traffic.metadata ) );
			trafficXML.appendChild( packVehicles( traffic.vehicles ) );
			
			return trafficXML;
		}
		
		/**
		 * Packs the TrafficMetadataVO into an XML element
		 * 
		 * @param tMetadata the system's traffic metadata
		 * 
		 * @return a trafficMetadata XML element
		 **/
		public static function packTrafficMetadataVO( tMetadata:TrafficMetadataVO ):XML
		{
			var tMetadataXML:XML	= new XML( "<" + SystemElement.TRAFFIC_METADATA + "/>" );
			
			var supportedProperties:String = tMetadata.supportedProperties.toString();
			
			tMetadataXML.@[ TrafficAttribute.SUPPORTED_PROPERTIES ]	= supportedProperties;
			
			return tMetadataXML;
		}
		
		/**
		 * Packs each vehicleProxy into a vehicleMetadata XML element
		 * 
		 * @param vehicles a list of the system's vehicle proxies.
		 * 
		 * @return a vehicle XML List
		 **/
		public static function packVehicles( vehicles:Array ):XMLList
		{
			var openTag:String 		= "<" + SystemElement.VEHICLES + ">";
			var dataString:String 	= "";
			var closeTag:String 	= "</" + SystemElement.VEHICLES + ">";
			
			for each( var vehicle:VehicleProxy in vehicles )
			{
				dataString += packVehicleProxy( vehicle ).toXMLString(); 
			}
			
			return new XMLList( openTag + dataString + closeTag );
		}
		
		/**
		 * Packs a single vehicleProxy into a vehicleMetadata XML element
		 * 
		 * @param vehicle a vehicle's state manager.
		 * 
		 * @return a vehicle XML Element.
		 **/
		public static function packVehicleProxy( vehicle:VehicleProxy ):XML
		{
			var vehicleXML:XML	= new XML( "<" + SystemElement.VEHICLE + "/>" );
			
			vehicleXML.@[ VehicleAttribute.LOD ] 			= vehicle.lodID;
			vehicleXML.@[ VehicleAttribute.ENTER_SYSTEM ] 	= vehicle.enterSystemIndex;
			vehicleXML.@[ VehicleAttribute.EXIT_SYSTEM ] 	= vehicle.exitSystemIndex;
			vehicleXML.@[ VehicleAttribute.LENGTH ] 		= vehicle.vehLength;
			
			return vehicleXML;
		}
	}
}