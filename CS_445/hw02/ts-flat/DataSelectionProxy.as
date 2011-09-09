package com.ts.presentation.model
{
	import com.ts.aspect.DisplayAdornerVO;
	import com.ts.aspect.model.AspectProxy;
	import com.ts.data.enumerations.LevelOfDetail;
	import com.ts.data.model.DataIndex;
	import com.ts.data.model.DataProxy;
	import com.ts.presentation.model.map.SystemProxy;
	
	import org.puremvc.as3.patterns.proxy.Proxy;
	
	public class DataSelectionProxy extends Proxy
	{
		public static const NAME:String	= "dataSelectionProxy";
		
		public static const NOTE_REQUEST_DATA_CHANGE:String  	= NAME + "/note/requestDataChange";
		public static const NOTE_UPDATE_DISPLAY:String 			= NAME + "/note/updateDisplay";
		
		public var system:SystemProxy;
		public var dataLookup:DataIndex;
		public var aspectModel:AspectProxy;
		
		public var currentLevelOfDetail:String;
		public var currentIntersection:int;
		public var currentLodID:int;
		public var currentAspect:String;
		public var currentDisplayAdorner:DisplayAdornerVO;
		
		public var selectedProxies:Array;
		
		public function DataSelectionProxy( System:SystemProxy, DataLookup:DataIndex, AspectModel:AspectProxy )
		{
			super( NAME );
			
			system 		= System;
			dataLookup 	= DataLookup;
			aspectModel = AspectModel;
		}
		
		public function selectData( levelOfDetail:String, lodID:int, intersection:int = 0 ):void
		{
			currentLevelOfDetail = levelOfDetail;
			currentLodID = lodID;
			currentIntersection = intersection;
			
			currentDisplayAdorner = getDisplayAdorner();
			sendNotification( NOTE_UPDATE_DISPLAY, currentDisplayAdorner );
		}
		
		public function unselectData():void
		{
			sendNotification( NOTE_UPDATE_DISPLAY );
		}
		
		private function getDisplayAdorner():DisplayAdornerVO
		{
			switch( currentLevelOfDetail )
			{
				case LevelOfDetail.INTERSECTION:
					return aspectModel.getVehicleLODDecorator( currentLodID );
					
				case LevelOfDetail.PHASE:
					return aspectModel.getPhaseLODDecorator( currentLodID, currentIntersection );
					
				case LevelOfDetail.VEHICLE:
					return aspectModel.getIntersectionLODDecorator( currentIntersection );
					
				default:
					return null;
			}
		}
	}
}