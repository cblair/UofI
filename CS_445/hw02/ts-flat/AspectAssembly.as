package com.ts.importing.deriveSystem.command
{
	import com.ts.aspect.model.AbstractPlotDecorator;
	import com.ts.aspect.model.AspectProxy;
	import com.ts.aspect.model.DecorationEntry;
	import com.ts.aspect.model.HistoryGraphProxy;
	import com.ts.aspect.model.NullHistoryGraphProxy;
	import com.ts.aspect.model.PhaseDecorationProxy;
	import com.ts.aspect.model.StateMapPlotDecorator;
	import com.ts.aspect.model.graph.plot.PlotPolicy;
	import com.ts.aspect.model.vo.DecorationVO;
	import com.ts.data.enumerations.LevelOfDetail;
	import com.ts.data.model.DataIndex;
	import com.ts.data.model.DataProxy;
	import com.ts.importing.configuration.model.enumerations.ExternalImportDataName;
	import com.ts.importing.configuration.model.enumerations.RequiredImportDataName;
	import com.ts.presentation.model.map.SystemProxy;
	import com.ts.presentation.model.map.intersection.IntersectionProxy;
	import com.ts.presentation.model.map.phase.PhaseProxy;

	public class AspectAssembly
	{
		public static function buildAspectModel( dataIndex:DataIndex, system:SystemProxy ):AspectProxy
		{	
			var redDecoration:DecorationVO = new DecorationVO( 0x000000, 0, 0xF16850, 1, 0x000000, 0, false, false, "Red Signal Display", "www.reach101.net" );
			var yellowDecoration:DecorationVO = new DecorationVO( 0x000000, 0, 0xFCD621, 1, 0x000000, 0, false, false, "Yellow Signal Display", "www.reach101.net" );
			var greenDecoration:DecorationVO = new DecorationVO( 0x000000, 0, 0x7AC943, 1, 0x000000, 0, false, false, "Green Signal Display", "www.reach101.net" );
			
			var rEntry:DecorationEntry = new DecorationEntry( 2, redDecoration );
			var yEntry:DecorationEntry = new DecorationEntry( 1, yellowDecoration );
			var gEntry:DecorationEntry = new DecorationEntry( 0, greenDecoration );
			
			
			var onDetectorDecoration:DecorationVO = new DecorationVO( 0x000000, 0, 0x3F73F5, 1, 0x000000, 0, false, false, "Vehicle Detected", "www.reach101.net" );
			var offDetectorDecoration:DecorationVO = new DecorationVO( 0x000000, 0, 0x000000, 0, 0x000000, 0, false, false, "No Vehicle", "www.reach101.net" );
			
			var onDEntry:DecorationEntry = new DecorationEntry( 1, onDetectorDecoration );
			var offDEntry:DecorationEntry = new DecorationEntry( 0, offDetectorDecoration );
			
			var phaseDecorationProxys:Array = new Array();
			for each( var intersection:IntersectionProxy in system.networkModel.intersectionIndex.intersections )
			{
				for each( var phase:PhaseProxy in intersection.phaseIndex.phases )
				{
					var hGraphs:Array = new Array();
					
					var detectorData:DataProxy = dataIndex.getDataAtUniqueID( ExternalImportDataName.LDP_DETECTOR, phase.signalNumber, intersection.intersectionNumber );
					var signalData:DataProxy = dataIndex.getDataAtUniqueID( ExternalImportDataName.LDP_SIG_DISPLAY, phase.signalNumber, intersection.intersectionNumber );
					
					if( detectorData )
					{
						hGraphs.push( new HistoryGraphProxy( detectorData.uniqueID, detectorData, new StateMapPlotDecorator( [ offDEntry, onDEntry ], PlotPolicy.STATE_REPEATING ) ) );
					}
					else
					{
						hGraphs.push( new NullHistoryGraphProxy() );
					}
					
					if( signalData )
					{
						hGraphs.push( new HistoryGraphProxy( signalData.uniqueID, signalData, new StateMapPlotDecorator( [ gEntry, yEntry, rEntry ], PlotPolicy.STATE_STACKED ) ) );
					}
					else
					{
						hGraphs.push( new NullHistoryGraphProxy() );
					}
					
					phaseDecorationProxys.push( new PhaseDecorationProxy( phase.signalNumber, intersection.intersectionNumber, hGraphs ) );				
				}
			}
			
			return new AspectProxy( "TEST", [], phaseDecorationProxys, [] );
		}
	}
}