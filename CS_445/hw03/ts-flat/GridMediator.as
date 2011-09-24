package com.ts.presentation.view.grid
{
	import com.ts.presentation.model.PlaybackProxy;
	import com.ts.presentation.model.grid.GridProxy;
	
	import flash.events.Event;
	
	import mx.binding.utils.BindingUtils;
	
	import org.puremvc.as3.patterns.mediator.Mediator;
	
	public class GridMediator extends Mediator
	{
		public static const NAME:String = "playbackGridMediator";
		
		protected var _gridProxy:GridProxy;
		protected var _playbackProxy:PlaybackProxy;
		
		private var _propOrigin:Number;
		
		public function get view():GridView
		{
			return viewComponent as GridView
		}
		
		public function GridMediator( viewComponent:GridView, playbackProxy:PlaybackProxy, gridProxy:GridProxy, name:String = "" )
		{
			super( NAME + name, viewComponent );
			
			_playbackProxy = playbackProxy;
			_gridProxy = gridProxy;
			
			BindingUtils.bindProperty( view, "currentSecond", 	_gridProxy, "currentSecond" );
			BindingUtils.bindProperty( view, "hLines",			_gridProxy,	"hLines" );
			BindingUtils.bindProperty( view, "timeLine",		_gridProxy,	"timeLine" );
			BindingUtils.bindProperty( view, "hOffset",			_gridProxy,	"hOffset" );
			BindingUtils.bindProperty( view, "hScrollP",		_gridProxy,	"hScrollP" );
		}
	}
}