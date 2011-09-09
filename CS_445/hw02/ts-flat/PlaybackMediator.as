package com.ts.presentation.view
{
	import com.ts.presentation.model.PlaybackProxy;
	
	import flash.events.Event;
	
	import mx.binding.utils.BindingUtils;
	
	import org.puremvc.as3.patterns.mediator.Mediator;
	
	public class PlaybackMediator extends Mediator
	{
		public static const NAME:String	=	"playbackMediator";
		
		private var _playbackProxy:PlaybackProxy;
		
		public function get view():PlaybackView
		{
			return viewComponent as PlaybackView;
		}
		
		public function PlaybackMediator( viewComponent:Object=null )
		{
			super( NAME, viewComponent );
		}
		
		override public function onRegister():void
		{
			view.addEventListener( "play", 			handleEvent );
			view.addEventListener( "pause", 		handleEvent );
			view.addEventListener( "rewind", 		handleEvent );
			view.addEventListener( "decreaseSpeed", handleEvent );
			view.addEventListener( "increaseSpeed", handleEvent );
			view.addEventListener( "manualStart", handleEvent );
			view.addEventListener( "manualUpdate", handleEvent );
			view.addEventListener( "manualEnd", handleEvent );
			
			_playbackProxy = facade.retrieveProxy( PlaybackProxy.NAME ) as PlaybackProxy;
			BindingUtils.bindProperty( view, "currentTime", 	_playbackProxy, "currentSecond" );
			BindingUtils.bindProperty( view, "totalTime", 		_playbackProxy, "totalSeconds" );
			BindingUtils.bindProperty( view, "playbackSpeed", 	_playbackProxy, "playbackSpeed" );
			BindingUtils.bindProperty( view, "maxValue", 		_playbackProxy, "totalIntervals" );
			BindingUtils.bindProperty( view, "currentInterval", _playbackProxy, "currentInterval" );
			BindingUtils.bindProperty( view, "isPlaying",		_playbackProxy, "isPlaying" );
		}
		
		override public function onRemove():void
		{
			view.removeEventListener( "play", 			handleEvent );
			view.removeEventListener( "pause", 			handleEvent );
			view.removeEventListener( "rewind", 		handleEvent );
			view.removeEventListener( "decreaseSpeed", 	handleEvent );
			view.removeEventListener( "increaseSpeed", 	handleEvent );
			view.removeEventListener( "manualControl", 	handleEvent );
			
			_playbackProxy.pause();
			_playbackProxy = null;
		}
		
		private function handleEvent( event:Event ):void
		{
			trace( event.type );
			switch( event.type )
			{
				case "play":
					_playbackProxy.play();
					break;
				
				case "pause":
					_playbackProxy.pause();
					break;
				
				case "rewind":
					_playbackProxy.reset();
					break;
				
				case "decreaseSpeed":
					_playbackProxy.decrementPlaybackSpeed();
					break;
				
				case "increaseSpeed":
					_playbackProxy.incrementPlaybackSpeed();
					break;
				
				case "manualStart":
					_playbackProxy.pause();
					break;
				
				case "manualUpdate":
					if( _playbackProxy.isPlaying )
					{
						_playbackProxy.pause();
					}
					_playbackProxy.manualySetInterval( view.slider.value );
					break;
				
				case "manualEnd":
					_playbackProxy.manualySetInterval( view.slider.value );
					break;
				
				default:
					break;
			}
		}
	}
}