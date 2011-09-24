package com.ts.presentation.view.map
{
	import com.ts.data.enumerations.LevelOfDetail;
	import com.ts.presentation.model.DataSelectionProxy;
	import com.ts.presentation.model.map.MapProxy;
	import com.ts.presentation.model.vo.LodSelectionRequestVO;
	import com.ts.presentation.view.map.controls.PhaseControl;
	import com.ts.presentation.model.map.SystemProxy;
	
	import flash.events.Event;
	
	import org.puremvc.as3.patterns.mediator.Mediator;
	
	public class MapMeidator extends Mediator
	{
		public static const NAME:String = "mapMediator";
		
		private var _system:SystemProxy;
		private var _mapProxy:MapProxy;
		
		public function get view():MapView
		{
			return viewComponent as MapView;
		}
		
		public function MapMeidator( viewComponent:MapView, System:SystemProxy )
		{
			super( NAME, viewComponent );
			_system = System;
			
			_mapProxy = new MapProxy();
			facade.registerProxy( _mapProxy );
			
			facade.registerMediator( new TrafficMediator( view.trafficView ) );
			facade.registerMediator( new NetworkMediator( view.networkView ) );
		}
		
		private function handleEvent( event:Event ):void
		{
			switch( event.type )
			{
				default:
					break;
			}
		}
	}
}