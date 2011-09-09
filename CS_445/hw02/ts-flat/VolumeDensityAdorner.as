package com.ts.importing.adornerClasses
{
	import com.fManagement.utilities.StringUtilities;
	import com.ts.aspect.development.DecorationEntry;
	import com.ts.aspect.development.DecorationInterpolator;
	import com.ts.aspect.development.HistoryGraphProxy;
	import com.ts.aspect.development.HistoryLabel;
	import com.ts.aspect.development.HistoryPlotLayout;
	import com.ts.aspect.development.HistoryPlotMask;
	import com.ts.aspect.development.HistoryPlotPolicy;
	import com.ts.aspect.development.HistoryPlotProxy;
	import com.ts.aspect.development.InfoEntry;
	import com.ts.aspect.development.InfoInterpolator;
	import com.ts.aspect.development.NullHistoryGraphProxy;
	import com.ts.aspect.model.PhaseDecorationProxy;
	import com.ts.data.model.DataIndex;
	import com.ts.data.model.DataProxy;
	import com.ts.data.model.DiscreteDataProxy;
	import com.ts.importing.configuration.model.enumerations.ExternalImportDataName;
	import com.ts.importing.configuration.model.enumerations.RequiredImportDataName;
	import com.ts.importing.deriveSystem.model.DataName;
	import com.ts.importing.deriveSystem.model.StandardTimingDataName;
	import com.ts.importing.deriveSystem.model.VolumeDensityDataName;
	import com.ts.presentation.model.map.SystemProxy;
	import com.ts.presentation.model.map.intersection.IntersectionProxy;
	import com.ts.presentation.model.map.phase.PhaseProxy;
	import com.ts.presentation.model.notification.NotificationDecorationEntry;
	import com.ts.presentation.model.notification.NotificationPlotProxy;
	import com.ts.presentation.model.notification.NotificationProxy;
	import com.ts.presentation.model.timeSpace.TimeSpaceProxy;
	import com.ts.presentation.view.decorationAssets.converters.DataToColorConverter;
	import com.ts.presentation.view.decorationAssets.interpolators.StateColorInterpolator;
	import com.ts.utilities.StringFormatters;

	public class VolumeDensityAdorner extends AbstractAdorner
	{
		public function VolumeDensityAdorner()
		{
			super( "Volumne Density" );
		}
		
		override protected function buildPhaseDecoration(dataIndex:DataIndex, system:SystemProxy):Array
		{
			var phaseDecorationProxys:Array = new Array();
			var timeSpaceProxy:TimeSpaceProxy = facade.retrieveProxy( TimeSpaceProxy.NAME ) as TimeSpaceProxy;
			
			for each( var intersection:IntersectionProxy in system.networkModel.intersectionIndex.intersections )
			{
				for each( var phase:PhaseProxy in intersection.phaseIndex.phases )
				{
					var historyGraphProxies:Array = new Array();
					
					
					
					
					
					//-- build all history graph adorners
					
					//-- detector history graph -----------------------------------------------------------------------------------------
					
					var detectorData:DataProxy = dataIndex.getDataAtUniqueID( DataName.DETECTOR, phase.signalNumber, intersection.intersectionNumber );
					var detectorLabel:HistoryLabel = new HistoryLabel( "Detector", [ "true", "false" ], "" );
					
					//-- if this phaseNum + intersectionNum has detector data
					if( detectorData )
					{
						var detectorDecoration:DecorationInterpolator = new DecorationInterpolator( detectorData.uncompressedData, 
							[ new DecorationEntry( 1, 0x3F73F5, 1 ),
								new DecorationEntry( 0, 0x3F73F5, 0 ) ] );
						
						var detectorInfo:InfoInterpolator = new InfoInterpolator( detectorData.uncompressedData,
							[ new InfoEntry( 1, "Vehicle is Detected!", "http://www.signaltiming.com/The_Signal_Timing_Manual_08082008.pdf#page=110" ),
								new InfoEntry( 0, "No vehicle is detected.", "http://www.signaltiming.com/The_Signal_Timing_Manual_08082008.pdf#page=110" ) ] );
						
						var detectorMask:HistoryPlotMask = new HistoryPlotMask( detectorData, HistoryPlotPolicy.HORIZONTAL_BAR );
						
						var detectorPlot:HistoryPlotProxy = new HistoryPlotProxy( detectorMask.buildHistoryPlotMask(), 
							detectorInfo.buildInfoPlotParts(), 
							detectorDecoration.buildDecorationPlotParts() );
						
						historyGraphProxies.push( new HistoryGraphProxy( detectorLabel, [ detectorPlot ], HistoryPlotLayout.OVERLAP ) );
					}
					else
					{
						historyGraphProxies.push( new NullHistoryGraphProxy( detectorLabel ) );
					}
					
					
					
					var gapInEffectData:DataProxy 	= dataIndex.getDataAtUniqueID( ExternalImportDataName.LDP_EFFECTIVE_GAP, phase.signalNumber, intersection.intersectionNumber );
					var gapStateData:DataProxy 		= dataIndex.getDataAtUniqueID( VolumeDensityDataName.GAP_IN_EFFECT_STATE, phase.signalNumber, intersection.intersectionNumber );
					
					//-- if this phaseNum + intersectionNum has gapTimer data
					if( gapInEffectData && !isNaN( gapInEffectData.maxValue ) && !isNaN( gapInEffectData.minValue ) && gapStateData )
					{
						var gapInEffectLabel:HistoryLabel = new HistoryLabel( "Gap In Effect Timer", [ StringFormatters.getDecimalPercision( String( gapInEffectData.maxValue ), 0 ),
							StringFormatters.getDecimalPercision( String( gapInEffectData.maxValue / 2 ), 0 ), "0"], "seconds" );
						
						var gapInEffectDecoration:DecorationInterpolator = new DecorationInterpolator( gapStateData.uncompressedData, 
							[ new DecorationEntry( 1, 0x7d3ea1, 1 ),
								new DecorationEntry( 2, 0xa380b7, 1 ), 
								new DecorationEntry( 3, 0xCCCCCC, 1 ) ] );
						
						var gapInEffectInfo:InfoInterpolator = new InfoInterpolator( gapStateData.uncompressedData,
							[ new InfoEntry( 1, "Time before reduction", "http://www.signaltiming.com/The_Signal_Timing_Manual_08082008.pdf#page=110" ),
								new InfoEntry( 2, "Time to reduce", "http://www.signaltiming.com/The_Signal_Timing_Manual_08082008.pdf#page=110" ),
								new InfoEntry( 3, "After reduction", "http://www.signaltiming.com/The_Signal_Timing_Manual_08082008.pdf#page=110" ),
								new InfoEntry( NaN, "Gap out.", "http://www.signaltiming.com/The_Signal_Timing_Manual_08082008.pdf#page=110" )] );
						
						var gapInEffectMask:HistoryPlotMask = new HistoryPlotMask( gapInEffectData, HistoryPlotPolicy.LINEAR_FILL );
						
						var gapInEffectPlot:HistoryPlotProxy = new HistoryPlotProxy( gapInEffectMask.buildHistoryPlotMask(), 
							gapInEffectInfo.buildInfoPlotParts(), 
							gapInEffectDecoration.buildDecorationPlotParts() );
						
						historyGraphProxies.push( new HistoryGraphProxy( gapInEffectLabel, [ gapInEffectPlot ], HistoryPlotLayout.OVERLAP ) );
					}
					else
					{
						historyGraphProxies.push( new NullHistoryGraphProxy( new HistoryLabel( "Gap In Effect", [ "", "", "" ], "" ) ) );
					}
					
					
					//-- minGreen history graph -----------------------------------------------------------------------------------------
					
					var minGreenData:DataProxy = dataIndex.getDataAtUniqueID( DataName.MIN_GREEN, phase.signalNumber, intersection.intersectionNumber );
					
					//-- if this phaseNum + intersectionNum has minGreen data
					if( minGreenData && !isNaN( minGreenData.maxValue ) && !isNaN( minGreenData.minValue ) )
					{
						//-- build minGreendataset
						var minGreenLabel:HistoryLabel = new HistoryLabel( "Min Green", [ StringFormatters.getDecimalPercision( String( minGreenData.maxValue ), 0 ),
							StringFormatters.getDecimalPercision( String( minGreenData.maxValue / 2 ), 0 ), "0"], "seconds" );
						
						var minGreenDecoration:DecorationInterpolator = new DecorationInterpolator( minGreenData.uncompressedData, [ new DecorationEntry( 0, 0x7d3ea1, 1  ) ], true );
						
						var minGreenInfo:InfoInterpolator = new InfoInterpolator( minGreenData.uncompressedData,
							[ new InfoEntry( 0, "Min green active!", "http://www.signaltiming.com/The_Signal_Timing_Manual_08082008.pdf#page=110" ),
								new InfoEntry( NaN, "Min out.", "http://www.signaltiming.com/The_Signal_Timing_Manual_08082008.pdf#page=110" )], true );
						
						var minGreenMask:HistoryPlotMask = new HistoryPlotMask( minGreenData, HistoryPlotPolicy.LINEAR_FILL );
						
						var minGreenPlot:HistoryPlotProxy = new HistoryPlotProxy( minGreenMask.buildHistoryPlotMask(), 
							minGreenInfo.buildInfoPlotParts(), 
							minGreenDecoration.buildDecorationPlotParts() );
						
						historyGraphProxies.push( new HistoryGraphProxy( minGreenLabel, [ minGreenPlot ], HistoryPlotLayout.OVERLAP ) );
					}
					else
					{
						historyGraphProxies.push( new NullHistoryGraphProxy( new HistoryLabel( "Min Green", [ "", "", "" ], "" ) ) );
					}
					
					//-- maxGreen history graph -----------------------------------------------------------------------------------------
					
					var maxGreenData:DataProxy = dataIndex.getDataAtUniqueID( DataName.MAX_GREEN, phase.signalNumber, intersection.intersectionNumber );
					
					//-- if this phaseNum + intersectionNum has maxGreen data
					if( maxGreenData && !isNaN( maxGreenData.maxValue ) && !isNaN( maxGreenData.minValue ) )
					{
						var maxGreenLabel:HistoryLabel = new HistoryLabel( "Max Green", [ StringFormatters.getDecimalPercision( String( maxGreenData.maxValue ), 0 ),
							StringFormatters.getDecimalPercision( String( maxGreenData.maxValue / 2 ), 0 ), "0"], "seconds" );
						
						var maxGreenDecoration:DecorationInterpolator = new DecorationInterpolator( maxGreenData.uncompressedData, [ new DecorationEntry( 0, 0x7d3ea1, 1  ) ], true );
						
						var maxGreenInfo:InfoInterpolator = new InfoInterpolator( maxGreenData.uncompressedData,
							[ new InfoEntry( 0, "Max green active!", "http://www.signaltiming.com/The_Signal_Timing_Manual_08082008.pdf#page=110" ),
								new InfoEntry( NaN, "Max green inactive...", "http://www.signaltiming.com/The_Signal_Timing_Manual_08082008.pdf#page=110" )], true );
						
						var maxGreenMask:HistoryPlotMask = new HistoryPlotMask( maxGreenData, HistoryPlotPolicy.LINEAR_FILL );
						
						var maxGreenPlot:HistoryPlotProxy = new HistoryPlotProxy( maxGreenMask.buildHistoryPlotMask(), 
							maxGreenInfo.buildInfoPlotParts(), 
							maxGreenDecoration.buildDecorationPlotParts() );
						
						historyGraphProxies.push( new HistoryGraphProxy( maxGreenLabel, [ maxGreenPlot ], HistoryPlotLayout.OVERLAP ) );
					}
					else
					{
						historyGraphProxies.push( new NullHistoryGraphProxy( new HistoryLabel( "Max Green", [ "", "", "" ], "" ) ) );
					}
					
					
					
					//-- signal display history graph -----------------------------------------------------------------------------------------
					
					var signalData:DataProxy = dataIndex.getDataAtUniqueID( DataName.SIG_DISPLAY, phase.signalNumber, intersection.intersectionNumber );
					var signalLabel:HistoryLabel = new HistoryLabel( "Signal Display", [ "red", "yellow", "green" ], "" );
					
					//-- if this phaseNum + intersectionNum has signal data
					if( signalData )
					{
						var signalDecoration:DecorationInterpolator = new DecorationInterpolator( signalData.uncompressedData, 
							[ new DecorationEntry( 3, 0xF16850, 1 ),
								new DecorationEntry( 2, 0xFDE67A, 1 ),
								new DecorationEntry( 1, 0x7AC943, 1 ) ] );
						
						var signalInfo:InfoInterpolator = new InfoInterpolator( signalData.uncompressedData,
							[ new InfoEntry( 3, "Red Signal", "http://www.signaltiming.com/The_Signal_Timing_Manual_08082008.pdf#page=110" ),
								new InfoEntry( 2, "Yellow Signal", "http://www.signaltiming.com/The_Signal_Timing_Manual_08082008.pdf#page=110" ),
								new InfoEntry( 1, "Green Signal.", "http://www.signaltiming.com/The_Signal_Timing_Manual_08082008.pdf#page=110" ) ] );
						
						var signalMask:HistoryPlotMask = new HistoryPlotMask( signalData, HistoryPlotPolicy.STACKED_BAR, [ 1, 2, 3 ] );
						
						var signalPlot:HistoryPlotProxy = new HistoryPlotProxy( signalMask.buildHistoryPlotMask(), 
							signalInfo.buildInfoPlotParts(), 
							signalDecoration.buildDecorationPlotParts() );
						
						historyGraphProxies.push( new HistoryGraphProxy( signalLabel, [ signalPlot ], HistoryPlotLayout.OVERLAP ) );
					}
					else
					{
						historyGraphProxies.push( new NullHistoryGraphProxy( signalLabel ) );
					}
					
					
					
					
					
					//-- create all notification adorners
					
					var notificationProxies:Array = new Array();
					
					
					if( signalData )
					{
						//-- Loading Notification -----------------
						var signalNPlotProxy:NotificationPlotProxy = new NotificationPlotProxy( [], [ new NotificationDecorationEntry( 3, 0xF16850, 1 ),
							new NotificationDecorationEntry( 2, 0xFDE67A, 1 ),
							new NotificationDecorationEntry( 1, 0x7AC943, 1 ) ] );
						
						notificationProxies.push( new NotificationProxy( signalData as DiscreteDataProxy, true, signalNPlotProxy ) );
						
						
						//-- Loading Time space -------------------
						timeSpaceProxy.addPhasePlot( phase.uniqueID, phase.phaseVehIndex, phase.linkIndex.length, phase.signalIndex.signals[ 0 ].at,
							system.maxIterations, signalData.uncompressedData, 
							new DataToColorConverter( new StateColorInterpolator( [ 1, 2, 3 ], [ 0x7AC943, 0xFDE67A, 0xF16850 ] ) ) );
					}
					else
					{
						notificationProxies.push( new NotificationProxy( null, false, null ) );
					}
					if( gapStateData )
					{
						var stateNPlotProxy:NotificationPlotProxy = new NotificationPlotProxy( [], [ new NotificationDecorationEntry( 1, 0x7d3ea1, 1 ),
																									 new NotificationDecorationEntry( 2, 0xa380b7, 1 ),
																									 new NotificationDecorationEntry( 3, 0xCCCCCC, 1 ) ] );
						
						notificationProxies.push( new NotificationProxy( gapStateData as DiscreteDataProxy, true, stateNPlotProxy ) );
					}
					else
					{
						notificationProxies.push( new NotificationProxy( null, false, stateNPlotProxy ) );
					}
					
					
					//-- currently only supporting hGraph proxies && notifications, will support others soon.
					phaseDecorationProxys.push( new PhaseDecorationProxy( phase.signalNumber, intersection.intersectionNumber, historyGraphProxies, notificationProxies, phase ) );
				}
				
			}
			
			return phaseDecorationProxys;
		}
	}
}