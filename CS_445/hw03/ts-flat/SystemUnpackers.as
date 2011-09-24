package com.ts.load.controller
{
	import com.ts.data.model.DataIndex;
	import com.ts.data.model.DataProxy;
	import com.ts.importing.deriveSystem.model.DataName;
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
	import com.ts.presentation.model.map.phase.PhaseVehicleIndex;
	import com.ts.presentation.model.map.phase.SignalIndex;
	import com.ts.presentation.model.map.phase.SignalProxy;
	import com.ts.presentation.model.map.vehicle.VehicleProxy;
	import com.ts.presentation.model.map.vehicle.vo.RequiredVehData;
	import com.ts.presentation.model.map.vehicle.vo.VehicleMetadataVO;
	import com.ts.presentation.model.map.vo.BackgroundLinkVO;
	import com.ts.presentation.model.map.vo.SystemMetadataVO;
	import com.ts.presentation.model.map.vo.TrafficMetadataVO;
	import com.ts.utilities.drawing.PathUtilities;

	public class SystemUnpackers
	{
		/**
		 * Unpacks the system XML into the System State Model.
		 * 
		 * @param systemXML The XML list containing the system.
		 * @param dataIndex The native data proxies for the simulation.
		 * 
		 * @return The SystemProxy, which manages the traffic system.
		 **/
		public static function unpackSystemProxy( systemXML:XML, dataIndex:DataIndex ):SystemProxy
		{
			var metadata:SystemMetadataVO 	= unpackSystemMetadataVO( systemXML[ SystemElement.SYSTEM_METADATA ][ 0 ] );
			var network:NetworkProxy 		= unpackNetworkProxy( systemXML[ SystemElement.NETWORK ][ 0 ], dataIndex );
			var traffic:TrafficProxy		= unpackTrafficProxy( systemXML[ SystemElement.TRAFFIC ][ 0 ], dataIndex );
			
			traffic.buildVehicleLookup();
			
			var phaseVehData:Object = buildPhaseVehMap( metadata, traffic, network );
			for( var key:String in phaseVehData )
			{
				network.getPhaseAtUID( key ).phaseVehIndex = phaseVehData[ key ];
			}
			
			return new SystemProxy( metadata, traffic, network );
		}
		
		private static function buildPhaseVehMap( metadata:SystemMetadataVO, traffic:TrafficProxy, network:NetworkProxy ):Object
		{
			var m:int = metadata.maxIterations;
			var phaseVehData:Object = new Object();
			var vehPhaseData:Array;
			var vehPosData:Array;
			var vehIntData:Array;
			var vehPhase:PhaseProxy;
			
			for each( var vehicle:VehicleProxy in traffic.vehicles )
			{
				vehPhaseData = vehicle.requiredData.phase.uncompressedData;
				vehPosData = vehicle.requiredData.percent.uncompressedData;
				vehIntData = vehicle.requiredData.intersection.uncompressedData;
				
				for( var j:int = 0; j < m; j++ )
				{
					if( !isNaN( vehPhaseData[ j ] ) )
					{
						vehPhase = network.getPhaseAtUID( String( vehPhaseData[ j ] ) + "-" + String( vehIntData[ j ] ) ); 
						if( !phaseVehData[ vehPhase.uniqueID ] ) phaseVehData[ vehPhase.uniqueID ] = new PhaseVehicleIndex( m );
						phaseVehData[ vehPhase.uniqueID ].addVehicle( vehicle, j, vehPosData[ j ] );
					}
				}
			}
			
			return phaseVehData;
		}
		
		/**
		 * Unpacks the sMetadata XML into the system's metadata.
		 * 
		 * @param sMetadataXML The XML contianing the system's general state data
		 * 
		 * @return The metadata property wrapper.
		 **/
		public static function unpackSystemMetadataVO( sMetadataXML:XML ):SystemMetadataVO
		{
			return new SystemMetadataVO( 	int( sMetadataXML.@[ SystemMetadataAttribute.MIN_ITERATIONS ] ),
											int( sMetadataXML.@[ SystemMetadataAttribute.MAX_ITERATIONS ] ),
											int( sMetadataXML.@[ SystemMetadataAttribute.NUM_INTERSECTIONS ] ),
											int( sMetadataXML.@[ SystemMetadataAttribute.NUM_PHASES ] ),
											int( sMetadataXML.@[ SystemMetadataAttribute.NUM_VEHICLES ] ),
											Number( sMetadataXML.@[ SystemMetadataAttribute.TIME_RESOLUTION ] ),
											Number( sMetadataXML.@[ SystemMetadataAttribute.PX_PER_FEET ] ),
											String( sMetadataXML.@[ SystemMetadataAttribute.ASPECT ] ) );
		}
		
		/**
		 * Unpacks the network XML into a network proxy.
		 * 
		 * @param networkXML The XML list containing the network information.
		 * @param dataIndex The native data proxies for the simulation.
		 * 
		 * @return The NetworkProxy, which manages the network in the display.
		 **/
		public static function unpackNetworkProxy( networkXML:XML, dataIndex:DataIndex ):NetworkProxy
		{
			var detectors:DetectorIndex 			= unpackDetectors( networkXML[ SystemElement.DETECTORS ] );
			var background:BackgroundNetworkProxy 	= unpackBackgroundNetwork( networkXML[ SystemElement.BACKGROUND ] );
			var intersections:IntersectionIndex	 	= unpackIntersectionIndex( networkXML[ SystemElement.INTERSECTIONS ] );
			
			return new NetworkProxy( intersections, background, detectors );
		}
		
		/**
		 * Unpacks the backgroundNetwork XMLList into a BackgroundNetworkProxy.
		 * 
		 * @param backgroundXMLList The XML list containing multiple backgroundLink elements.
		 * 
		 * @return which manages the background link map for the display.
		 **/
		public static function unpackBackgroundNetwork( backgroundXMLList:XMLList ):BackgroundNetworkProxy
		{
			var bLinks:Array = new Array();
			for each( var bLinkXML:XML in backgroundXMLList.children() )
			{
				bLinks.push( unpackBackgroundLink( bLinkXML ) );
			}
			
			return new BackgroundNetworkProxy( bLinks );
		}
		
		public static function unpackBackgroundLink( bLinkXML:XML ):BackgroundLinkVO
		{
			return new BackgroundLinkVO(
				int		( bLinkXML.@[ BackgroundAttribute.LINK_NUM ] ),
				stringToBoolean( String( bLinkXML.@[ BackgroundAttribute.IS_CONNECTOR ] ) ),
				PathUtilities.stringToPoints( String( bLinkXML.@[ BackgroundAttribute.POINTS ] ) ),
				int		( bLinkXML.@[ BackgroundAttribute.NUM_LANES ] ),
				Number	( bLinkXML.@[ BackgroundAttribute.VARIANCE ] )
				);
		}
		
		/**
		 * Unpacks the intersection XMLList into an IntersectionIndex.
		 * 
		 * @param intersectionXMLList The XML list containing multiple intersection elements.
		 * 
		 * @return which manages all the intersections for the system.
		 **/
		public static function unpackIntersectionIndex( intersectionXMLList:XMLList ):IntersectionIndex
		{
			var intersections:Array = new Array();
			for each( var intersectionXML:XML in intersectionXMLList.children() )
			{
				intersections.push( unpackIntersectionProxy( intersectionXML ) );
			}
			
			return new IntersectionIndex( intersections );
		}
		
		/**
		 * Unpacks an intersection XML element into an IntersectionProxy.
		 * 
		 * @param intersectionXML The XML element containing an intersection element.
		 * 
		 * @return manages the state of an intersection.
		 **/
		public static function unpackIntersectionProxy( intersectionXML:XML ):IntersectionProxy
		{
			var phases:PhaseIndex 		= unpackPhaseIndex( intersectionXML[ SystemElement.PHASES ] );
			var intersectionNum:int 	= int( intersectionXML.@[ IntersectionAttribute.INTERSECTION_NUM ] );
			return new IntersectionProxy( phases, intersectionNum );
		}
		
		/**
		 * Unpacks a phase XMLList into a PhaseIndex.
		 * 
		 * @param phaseXMLList The XML list containing multiple phase elements.
		 * 
		 * @return which manages all the phases in an intersection.
		 **/
		public static function unpackPhaseIndex( phaseXMLList:XMLList ):PhaseIndex
		{
			var phases:Array = new Array();
			for each( var phaseXML:XML in phaseXMLList.children() )
			{
				phases.push( unpackPhaseProxy( phaseXML ) );
			}
			
			return new PhaseIndex( phases )
		}
		
		/**
		 * Unpacks a phase XML element into a PhaseProxy.
		 * 
		 * @param phaseXML The XML element containing a phase element.
		 * 
		 * @return manages the state of a phase.
		 **/
		public static function unpackPhaseProxy( phaseXML:XML ):PhaseProxy
		{
			var links:LinkIndex 	= unpackLinks( phaseXML[ SystemElement.LINKS ]  );
			var signals:SignalIndex = unpackSignals( phaseXML[ SystemElement.SIGNALS ] );

			return new PhaseProxy( links, signals );
		}
		
		/**
		 * Unpacks the signal XML list into a SignalIndex
		 * 
		 * @param signalXMLList a list of signal XML elements
		 * 
		 * @return a collection of SignalProxies
		 **/
		public static function unpackSignals( signalXMLList:XMLList ):SignalIndex
		{
			var signals:Array = new Array();
			
			for each( var signalXML:XML in signalXMLList.children() )
			{
				signals.push( unpackSignalProxy( signalXML ) );
			}
			
			return new SignalIndex( signals );
		}
		
		/**
		 * Unpacks Signal XML into a SignalProxy
		 * 
		 * @param signalXML the XML describing a signal
		 * 
		 * @return manages the state of a signal
		 **/
		public static function unpackSignalProxy( signalXML:XML ):SignalProxy
		{
			var signal:SignalProxy = new SignalProxy(
				int		( signalXML.@[ SignalAttribute.GROUP_NUM ] ),
				int		( signalXML.@[ SignalAttribute.INTERSECTION_NUM ] ),
				int		( signalXML.@[ SignalAttribute.LINK_NUM ] ),
				PathUtilities.stringToPoint	( String( signalXML.@[ SignalAttribute.POSITION ] ) ),
				Number	( signalXML.@[ SignalAttribute.AT ] ),
				Number	( signalXML.@[ SignalAttribute.ANGLE ] ),
				int		( signalXML.@[ SignalAttribute.NUM_LANES ] )
			);
			
			return signal;
		}
		
		/**
		 * Unpacks the detector XML list into a DetectorIndex
		 * 
		 * @param detectorXMLList a list of detector XML elements
		 * 
		 * @return a collection of DetectorProxies
		 **/
		public static function unpackDetectors( detectorXMLList:XMLList ):DetectorIndex
		{
			var detectors:Array = new Array();
			
			for each( var detectorXML:XML in detectorXMLList.children() )
			{
				detectors.push( unpackDetectorProxy( detectorXML ) );
			}
			
			return new DetectorIndex( detectors );
		}
		
		/**
		 * Unpacks Detector XML into a DetectorProxy
		 * 
		 * @param detectorXML the XML describing a detector
		 * 
		 * @return manages the state of a detector
		 **/
		public static function unpackDetectorProxy( detectorXML:XML ):DetectorProxy
		{
			return new DetectorProxy(
				int		( detectorXML.@[ DetectorAttribute.GROUP_NUM ] ),
				int		( detectorXML.@[ DetectorAttribute.INTERSECTION_NUM ] ),
				int		( detectorXML.@[ DetectorAttribute.LINK_NUM ] ),
				PathUtilities.stringToPoint( String( detectorXML.@[ DetectorAttribute.POSITION ] ) ),
				Number	( detectorXML.@[ DetectorAttribute.LENGTH ] ),
				Number	( detectorXML.@[ DetectorAttribute.ANGLE ] ),
				int		( detectorXML.@[ DetectorAttribute.NUM_LANES ] )
			);
		}
		
		/**
		 * Unpacks the link network XML list into a LinkIndex
		 * 
		 * @param linksXML a list of link XML elements
		 * 
		 * @return a collection of LinkProxies
		 **/
		public static function unpackLinks( linksXML:XMLList ):LinkIndex
		{
			var links:Array = new Array();
			var linkMap:Object = new Object();
			
			for each( var linkXML:XML in linksXML.children() )
			{
				var link:LinkProxy = unpackLinkProxy( linkXML );
				links.push( link );
				linkMap[ link.linkNum ] = link;
			}
			
			for each( link in links )
			{
				if( linkMap[ link.nextLinkNum ] )
					link.nextLink = linkMap[ link.nextLinkNum ];
				
				if( linkMap[ link.prevLinkNum ] )
					link.prevLink = linkMap[ link.prevLinkNum ];
			}
			
			return new LinkIndex( links );
		}
		
		/**
		 * Unpacks Link XML into a Link Proxy
		 * 
		 * @param linkXML the XML describing a link
		 * 
		 * @return manages the state of a link
		 **/
		public static function unpackLinkProxy( linkXML:XML ):LinkProxy
		{
			var link:LinkProxy = new LinkProxy(
				int		( linkXML.@[ LinkAttribute.LINK_NUM ] ),
				int		( linkXML.@[ LinkAttribute.NUM_LANES ] ),
				PathUtilities.stringToPoints( String( linkXML.@[ LinkAttribute.POINTS ] ) ),
				Number	( linkXML.@[ LinkAttribute.LINK_LENGTH ] ),
				int		( linkXML.@[ LinkAttribute.PREV_LINK_NUM ] ),
				int		( linkXML.@[ LinkAttribute.NEXT_LINK_NUM ] ),
				Number	( linkXML.@[ LinkAttribute.FROM_ANGLE ] ),
				Number	( linkXML.@[ LinkAttribute.TO_ANGLE ] ),
				Number	( linkXML.@[ LinkAttribute.AVG_ANGLE ] ),
				Number	( linkXML.@[ LinkAttribute.ANGLE_VARIANCE ] ),
				stringToBoolean( String( linkXML.@[ LinkAttribute.IS_CONNECTOR ] ) ),
				stringToBoolean( String( linkXML.@[ LinkAttribute.HAS_SIGNAL ] ) ),
				int		( linkXML.@[ LinkAttribute.SIGNAL_NUM ] ),
				Number	( linkXML.@[ LinkAttribute.FROM_AT ] ),
				Number	( linkXML.@[ LinkAttribute.TO_AT ] )
			);
			
			link.isTerminalLink = stringToBoolean( String( linkXML.@[ LinkAttribute.IS_TERMINAL_LINK ] ) );
			
			return link;
		}
		
		/**
		 * Unpacks the traffic XML into a traffic proxy.
		 * 
		 * @param trafficXML The XML list containing multiple vehicle elements.
		 * @param dataIndex The native data proxies for the simulation.
		 * 
		 * @return The TrafficProxy, which manages all the vehicles in the display.
		 **/
		public static function unpackTrafficProxy( trafficXML:XML, dataIndex:DataIndex ):TrafficProxy
		{
			var vehicles:Array = new Array();
			for each( var vMetadataXML:XML in trafficXML[ SystemElement.VEHICLES ].children() )
			{
				vehicles.push( unpackVehicleProxy( vMetadataXML, dataIndex ) );
			}
			
			var trafficMetadata:TrafficMetadataVO = unpackTrafficMetadataVO( trafficXML[ SystemElement.TRAFFIC_METADATA ][ 0 ] );
			
			return new TrafficProxy( vehicles, trafficMetadata );
		}
		
		/**
		 * Creates a TrafficMetadaVO from traffic metadata XML.
		 * 
		 * @param tMetadataXML The XML element containg traffic metadata attributes
		 * 
		 * @return TrafficMetadataVO which contains general data for the entire traffic model
		 **/
		public static function unpackTrafficMetadataVO( tMetadataXML:XML ):TrafficMetadataVO
		{
			//-- supported properties are stored as a comma delimited string.
			var supportedProperties:String = String( tMetadataXML.@[ TrafficAttribute.SUPPORTED_PROPERTIES ] );
			return new TrafficMetadataVO( supportedProperties.split( "," ) );
		}
		
		/**
		 * Creates a VehicleProxy from vehicle metadata XML.
		 * 
		 * @param vMetadataXML The XML element containg vehicle metadata attributes
		 * @param dataIndex The native data proxies for the simulation ( used to build the vehicle's required data list )
		 * 
		 * @return VehicleProxy which manages the state of a single vehicle
		 **/
		public static function unpackVehicleProxy( vMetadataXML:XML, dataIndex:DataIndex ):VehicleProxy
		{
			//-- 1. get the vehicle's metadata;
			var vehMetadata:VehicleMetadataVO 	= unpackVehicleMetadataVO( vMetadataXML );
			
			//-- 2. use the metadata to build the rest of the vehicle proxy's assets
			var requiredVehData:RequiredVehData = new RequiredVehData( null, null, null, null );
			requiredVehData.inSystem 			= dataIndex.getVehicleDataProxy( DataName.VEH_IN_SYSTEM, vehMetadata.lodID );
			requiredVehData.intersection 		= dataIndex.getVehicleDataProxy( DataName.VEH_INTERSECTION, vehMetadata.lodID );
			requiredVehData.percent 			= dataIndex.getVehicleDataProxy( DataName.VEH_PERCENT, vehMetadata.lodID );
			requiredVehData.phase 				= dataIndex.getVehicleDataProxy( DataName.VEH_PHASE, vehMetadata.lodID );
			requiredVehData.vehRotation 		= dataIndex.getVehicleDataProxy( DataName.VEH_ROTATION, vehMetadata.lodID );
			requiredVehData.vehXPos 			= dataIndex.getVehicleDataProxy( DataName.VEH_POS_X, vehMetadata.lodID );
			requiredVehData.vehYPos 			= dataIndex.getVehicleDataProxy( DataName.VEH_POS_Y, vehMetadata.lodID );
			
			return new VehicleProxy( vehMetadata, requiredVehData );
		}
		
		/**
		 * Unpacks Vehicle Metadata XML data into a VehicleMetadataVO
		 * 
		 * @param vMetadataXML The XML element containg vehicle metadata attributes
		 * 
		 * @return VehicleMetadataVO's contain each vehicle's list of unique characteristics
		 **/
		public static function unpackVehicleMetadataVO( vMetadataXML:XML ):VehicleMetadataVO
		{
			return new VehicleMetadataVO( int	( vMetadataXML.@[ VehicleAttribute.LOD ] ),
										  int	( vMetadataXML.@[ VehicleAttribute.ENTER_SYSTEM ] ),
										  int	( vMetadataXML.@[ VehicleAttribute.EXIT_SYSTEM ] ),
										  Number( vMetadataXML.@[ VehicleAttribute.LENGTH ] ) );
		}
		
		public static function stringToBoolean( value:String ):Boolean
		{
			return value == "true";
		}
	}
}