package com.ts.importing.deriveSystem.command
{
	import com.ts.aspect.model.AspectProxy;
	import com.ts.aspect.model.DecorationEntry;
	import com.ts.aspect.model.DecorationPolicy;
	import com.ts.aspect.model.graph.hGraph.HGraphProxy;
	import com.ts.aspect.model.graph.plot.PlotDecorator;
	import com.ts.aspect.model.graph.plot.PlotPolicy;
	import com.ts.aspect.model.graph.plot.R1PlotProxy;
	import com.ts.aspect.model.graph.plot.vo.PlotIndex;
	import com.ts.aspect.model.graph.plot.vo.R1PlotVO;
	import com.ts.aspect.model.vo.DecorationVO;
	import com.ts.aspect.model.vo.DisplayVO;
	import com.ts.data.enumerations.LevelOfDetail;
	import com.ts.data.model.DataIndex;
	import com.ts.data.model.DataProxy;
	import com.ts.data.model.DatabaseProxy;
	import com.ts.importing.aspectClasses.RequiredAspectClass;
	import com.ts.importing.configuration.model.ImportConfigProxy;
	import com.ts.importing.configuration.model.enumerations.RequiredImportDataName;
	import com.ts.importing.deriveSystem.model.DataName;
	import com.ts.importing.deriveSystem.model.ImportDataIndex;
	import com.ts.importing.deriveSystem.vo.IntersectionAnalysisVO;
	import com.ts.importing.deriveSystem.vo.NetworkHelperDataVO;
	import com.ts.importing.deriveSystem.vo.TrafficHelperDataVO;
	import com.ts.importing.parsing.model.ImportParsingProxy;
	import com.ts.presentation.model.DataSelectionProxy;
	import com.ts.presentation.model.map.IterationProxy;
	import com.ts.presentation.model.map.NetworkProxy;
	import com.ts.presentation.model.map.SystemProxy;
	import com.ts.presentation.model.map.TrafficProxy;
	import com.ts.presentation.model.map.phase.PhaseProxy;
	import com.ts.presentation.model.map.vehicle.VehicleProxy;
	import com.ts.presentation.model.map.vo.SystemMetadataVO;
	
	import flash.geom.Point;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	
	public class DeriveSystemCommand extends SimpleCommand
	{
		override public function execute(notification:INotification):void
		{
			var importDataIndex:ImportDataIndex = notification.getBody() as ImportDataIndex;
			var importConfig:ImportConfigProxy 	= facade.retrieveProxy( ImportConfigProxy.NAME ) as ImportConfigProxy;
			var importParser:ImportParsingProxy = facade.retrieveProxy( ImportParsingProxy.NAME ) as ImportParsingProxy;
			var database:DatabaseProxy 			= facade.retrieveProxy( DatabaseProxy.NAME ) as DatabaseProxy;
			
			
			//-- 1. get intersection metadata
			var intersectionAnalysis:IntersectionAnalysisVO = analyzeIntersection( importDataIndex );
			
			
			//-- 2. assemble the system model
			var trafficModel:TrafficProxy = SystemAssembly.buildTrafficModel( importDataIndex, database.dataIndex, new TrafficHelperDataVO( intersectionAnalysis ) );
			var networkModel:NetworkProxy = SystemAssembly.buildNetworkModel( importDataIndex, database.dataIndex, new NetworkHelperDataVO( intersectionAnalysis ) );
			var metadata:SystemMetadataVO = SystemAssembly.buildMetadata( importDataIndex, networkModel, trafficModel, importConfig.aspectClass.aspectName, importParser.numIntervals, importParser.intervalsPerSecond );
			var systemProxy:SystemProxy = new SystemProxy( metadata, trafficModel, networkModel );
			SystemAssembly.FinalizeSystem( systemProxy, importDataIndex, database.dataIndex );
			
			
			//-- 3. derive all virtual data & transfer all persisting data to the database
			var importConfiguration:ImportConfigProxy =  facade.retrieveProxy( ImportConfigProxy.NAME ) as ImportConfigProxy;
			var requiredAspect:RequiredAspectClass = new RequiredAspectClass()
			requiredAspect.deriveData( importDataIndex, database.dataIndex, systemProxy );
			importConfiguration.aspectClass.deriveData( importDataIndex, database.dataIndex, systemProxy );
			
			
			//-- 4. finalize the application
			sendNotification( SystemProxy.NOTE_FINALIZE_APPLICATION, systemProxy );
			
			
			//-- 5. clean up			
			facade.removeCommand( ImportParsingProxy.NOTE_DERIVE_SYSTEM );
		}
		
		private function analyzeIntersection( importDataIndex:ImportDataIndex ):IntersectionAnalysisVO
		{
			var lanePointXData:DataProxy = importDataIndex.getDataAtName( RequiredImportDataName.IN0_LINK_POINT_X )[ 0 ];
			var lanePointYData:DataProxy = importDataIndex.getDataAtName( RequiredImportDataName.IN0_LINK_POINT_Y )[ 0 ];
			
			var maxX:Number = lanePointXData.maxValue;
			var minX:Number = lanePointXData.minValue;
			
			var maxY:Number = lanePointYData.maxValue;
			var minY:Number = lanePointYData.minValue;
			
			var sigIntersectionData:DataProxy = importDataIndex.getDataAtName( RequiredImportDataName.IN0_SIG_INTERSECTION )[ 0 ];
			var numIntersections:int = sigIntersectionData.dataVO.numUniqueValues;
			
			return new IntersectionAnalysisVO( new Point( minX, minY ), new Point( maxX, maxY ), numIntersections );
		}
	}
}