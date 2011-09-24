package com.ts.importing.adornerClasses
{
	import com.ts.aspect.model.AspectProxy;
	import com.ts.data.enumerations.LevelOfDetail;
	import com.ts.data.model.DataIndex;
	import com.ts.presentation.model.map.SystemProxy;
	
	import org.puremvc.as3.patterns.proxy.Proxy;

	public class AbstractAdorner extends Proxy
	{
		public static const NAME:String	= "abstractAdorner";
		public var aspectName:String;
		
		public function AbstractAdorner( AspectName:String )
		{
			aspectName = AspectName;
			super( NAME );
		}
		
		public function buildAdornerModel( dataIndex:DataIndex, system:SystemProxy ):AspectProxy
		{
			var requiredAdorner:RequiredAdorner = new RequiredAdorner();
			var aspectProxy:AspectProxy = new AspectProxy( aspectName, 
														   requiredAdorner.buildRequiredVehicleDecoration( dataIndex, system ),
														   requiredAdorner.buildRequiredPhaseDecoration( dataIndex, system ),
														   requiredAdorner.buildRequiredIntersectionDecoration( dataIndex, system ) );
			
			aspectProxy.addLodDecorators( buildVehicleDecoration( dataIndex, system ), LevelOfDetail.VEHICLE );
			aspectProxy.addLodDecorators( buildPhaseDecoration( dataIndex, system ), LevelOfDetail.PHASE );
			aspectProxy.addLodDecorators( buildIntersectionDecoration( dataIndex, system ), LevelOfDetail.INTERSECTION );
			
			return aspectProxy;
		}
		
		protected function buildVehicleDecoration( dataIndex:DataIndex, system:SystemProxy ):Array
		{
			//-- override me!!!
			return [];
		}
		
		protected function buildPhaseDecoration( dataIndex:DataIndex, system:SystemProxy ):Array
		{
			//-- override me!!!
			return [];
		}
		
		protected function buildIntersectionDecoration( dataIndex:DataIndex, system:SystemProxy ):Array
		{
			//-- override me!!!
			return [];
		}
	}
}