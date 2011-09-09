package com.ts.importing.configuration.model.enumerations
{
	public class RequiredImportDataName
	{
		public static const FZP_VEH_TIME:String 			= "vehTime";
		public static const FZP_VEH_VELOCITY:String			= "vehVelocity";
		//public static const FZP_VEH_ACCEL:String			= "vehAcceleration";
		public static const FZP_VEH_LINK_POS:String			= "vehLinkPos";
		public static const FZP_VEH_LINK:String				= "vehLink";
		public static const FZP_VEH_NUM:String				= "vehNumber";
		public static const FZP_VEH_WORLD_X:String			= "vehWorldX";
		public static const FZP_VEH_WORLD_Y:String			= "vehWorldY";
		public static const FZP_VEH_RWORLD_X:String			= "vehRWorldX";
		public static const FZP_VEH_RWORLD_Y:String			= "vehRWorldY";
		public static const FZP_VEH_LENGTH:String			= "vehLength";
		
		//-- NEED TO PHASE OUT
		//public static const LSA_SIG_INTERSECTION:String		= "signalIntersection";
		//public static const LSA_SIG_GROUP:String			= "signalPhase";
		//public static const LSA_SIG_LINK:String				= "signalLink";
		//public static const LSA_SIG_LANE:String				= "signalLane";
		//public static const LSA_SIG_POS:String				= "signalPosition";
		//--
		
		public static const LDP_SIG_TIME:String				= "simTime";
		public static const LDP_DETECTOR:String				= "detector";
		public static const LDP_SIG_DISPLAY:String			= "signalDisplay";
		public static const LDP_MIN_GREEN:String			= "minGreen";
		public static const LDP_MAX_GREEN:String			= "maxGreen";
		public static const IN0_LINK_NUMBER:String			= "linkNum";
		public static const IN0_NUM_LANES:String			= "numLanes";
		public static const IN0_LINK_POINT_X:String			= "linkPointX";
		public static const IN0_LINK_POINT_Y:String			= "linkPointY";
		public static const IN0_FROM_LINK:String			= "fromLinkNum";
		public static const IN0_FROM_LINK_AT:String			= "fromLinkAt";
		public static const IN0_TO_LINK:String				= "toLinkNum";
		public static const IN0_TO_LINK_AT:String			= "toLinkAt";
		public static const IN0_IS_CONNECTOR:String			= "isConnector";
		public static const IN0_LINK_BEHAVIOR:String		= "behavior";
		public static const IN0_SIG_INTERSECTION:String		= "sigIntersection";
		public static const IN0_SIG_GROUP:String			= "sigPhase";
		public static const IN0_SIG_LINK:String				= "sigLink";
		public static const IN0_SIG_LANE:String				= "sigLane";
		public static const IN0_SIG_POS:String				= "sigPosition";
		public static const IN0_DET_INTERSECTION:String		= "detIntersection";
		public static const IN0_DET_GROUP:String			= "detPhase";
		public static const IN0_DET_LINK:String				= "detLink";
		public static const IN0_DET_LANE:String				= "detLane";
		public static const IN0_DET_LENGTH:String			= "detLength";
		public static const IN0_DET_POS:String				= "detPosition";
		
		public static const REQUIRED_IMPORT_NAMES:Array = [ "vehTime",
															"vehVelocity",
															"vehLinkPos",
															"vehLink",
															"vehNumber",
															"vehWorldX",
															"vehWorldY",
															"vehRWorldX",
															"vehRWorldY",
															"vehLength",
															"simTime",
															"detector",
															"minGreen",
															"maxGreen",
															"signalDisplay",
															"linkNum",
															"numLanes",
															"linkPointX",
															"linkPointY",
															"fromLinkNum",
															"fromLinkAt",
															"toLinkNum",
															"toLinkAt",
															"isConnector",
															"behavior",
															"sigIntersection",
															"sigPhase",
															"sigLink",
															"sigLane",
															"sigPosition",
															"detIntersection",
															"detPhase",
															"detLink",
															"detLane",
															"detLength",
															"detPosition" ];
		
	}
}