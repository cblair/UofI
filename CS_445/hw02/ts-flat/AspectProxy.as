package com.ts.aspect.model
{
	import com.ts.aspect.DisplayAdornerVO;
	import com.ts.data.enumerations.LevelOfDetail;
	
	import org.puremvc.as3.patterns.proxy.Proxy;
	
	public class AspectProxy extends Proxy
	{
		public static const NAME:String = "aspectProxy";
		
		public var aspectName:String;
		public var intersectionLOD:Object;
		public var phaseLOD:Object;
		public var vehicleLOD:Object;
		
		public function AspectProxy( AspectName:String, vehicleAdorners:Array, phaseAdorners:Array, intersectionAdorners:Array )
		{
			super( NAME + AspectName, data );
			aspectName = AspectName;
			intersectionLOD = new Object();
			phaseLOD = new Object();
			vehicleLOD = new Object();
			
			addLodDecorators( vehicleAdorners, LevelOfDetail.VEHICLE );
			addLodDecorators( phaseAdorners, LevelOfDetail.PHASE );
			addLodDecorators( intersectionAdorners, LevelOfDetail.INTERSECTION );
			
		}
		
		public function addLodDecorators( decorators:Array, levelOfDetail:String ):void
		{
			switch( levelOfDetail )
			{
				case LevelOfDetail.INTERSECTION:
					for each( var intersectionDecoration:IntersectionDecorationProxy in decorators )
					{
						if( intersectionLOD[ intersectionDecoration.intersectionNumber ] == null )
						{
							intersectionLOD[ intersectionDecoration.intersectionNumber ] = intersectionDecoration;
						}
						else
						{
							intersectionLOD[ intersectionDecoration.intersectionNumber ].mergeDecorationProxy( intersectionDecoration );
						}
					}
					break;
				
				case LevelOfDetail.PHASE:
					for each( var phaseDecoration:PhaseDecorationProxy in decorators )
					{
						if( phaseLOD[ phaseDecoration.intersectionNumber ] == null )
						{
							phaseLOD[ phaseDecoration.intersectionNumber ] = new Object();
						}
						if( phaseLOD[ phaseDecoration.intersectionNumber ][ phaseDecoration.phaseNumber ] == null )
						{
							phaseLOD[ phaseDecoration.intersectionNumber ][ phaseDecoration.phaseNumber ] = phaseDecoration;
						}
						else
						{
							phaseLOD[ phaseDecoration.intersectionNumber ][ phaseDecoration.phaseNumber ].mergeDecorationProxy( phaseDecoration );
						}
					}
					break;
				
				case LevelOfDetail.VEHICLE:
					for each( var vehicleDecoration:VehicleDecorationProxy in decorators )
					{
						if( vehicleLOD[ vehicleDecoration.vehicleNumber ] == null )
						{
							vehicleLOD[ vehicleDecoration.vehicleNumber ] = vehicleDecoration;
						}
						else
						{
							vehicleLOD[ vehicleDecoration.vehicleNumber ].mergeDecorationProxy( vehicleDecoration );
						}
					}
					break;
				
				default:
					break;
			}
		}
		
		public function getVehicleLODDecorator( vehicleNumber:int ):DisplayAdornerVO
		{
			if( vehicleLOD[ vehicleNumber ] )
			{
				return new DisplayAdornerVO( vehicleLOD[ vehicleNumber ].historyGraphDataProviders, 
					                         vehicleLOD[ vehicleNumber ].notificationProxies,
											 vehicleLOD[ vehicleNumber ].host );
			}
			return null;
		}
		
		public function getPhaseLODDecorator( phaseNumber:int, intersectionNumber:int ):DisplayAdornerVO
		{
			if( phaseLOD[ intersectionNumber ][ phaseNumber ] )
			{
				return new DisplayAdornerVO( phaseLOD[ intersectionNumber ][ phaseNumber ].historyGraphDataProviders,
											 phaseLOD[ intersectionNumber ][ phaseNumber ].notificationProxies,
											 phaseLOD[ intersectionNumber ][ phaseNumber ].host );
			}
			return null;
		}
		
		public function getIntersectionLODDecorator( intersectionNumber:int ):DisplayAdornerVO
		{
			if( intersectionLOD[ intersectionNumber ] )
			{
				return new DisplayAdornerVO( intersectionLOD[ intersectionNumber ].historyGraphDataProviders,
											 intersectionLOD[ intersectionNumber ].notificationProxies,
											 intersectionLOD[ intersectionNumber ].host );
			}
			return null;
		}
	}
}