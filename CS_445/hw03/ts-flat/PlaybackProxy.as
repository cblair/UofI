package com.ts.presentation.model
{
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.TimerEvent;
	import flash.utils.Timer;
	
	import org.puremvc.as3.patterns.proxy.Proxy;
	
	public class PlaybackProxy extends Proxy
	{
		public static const NAME:String						= "playbackProxy";		
		public static const NOTE_PLAYBACK_CHANGED:String	= NAME + "/note/playbackChanged";
		
		public static const UPDATE_INTERVAL:String			= "updateInterval";
		
		[Bindable] public var totalSeconds:Number = 0;
		[Bindable] public var currentSecond:Number = 0;
		[Bindable] public var totalIntervals:int = 1;
		
		public var eventDispatcher:EventDispatcher;
		
		private var _timer:Timer;
		[Bindable] public var isPlaying:Boolean = false;
		
		private var _speedMultiplier:int = 2;
		private var _playbackSpeed:Number = 1;
		[Bindable]
		public function get playbackSpeed():Number{ return _playbackSpeed; }
		public function set playbackSpeed( value:Number ):void
		{ 
			_playbackSpeed = value; 
			updateTimer();
		}//-- Do nothing
		
		public function manualySetInterval( value:int ):void
		{
			if( isPlaying )
			{
				pause();
			}
			currentInterval = Math.max( 0, Math.min( totalIntervals, value ) );
		}
		
		public function incrementPlaybackSpeed():void
		{
			playbackSpeed = playbackSpeed < 16 ? playbackSpeed * _speedMultiplier : 16;
		}
		
		public function decrementPlaybackSpeed():void
		{
			playbackSpeed = playbackSpeed > 1/8 ? playbackSpeed / _speedMultiplier : 1/8;
		}
		
		private var _resolutionInSeconds:Number = .1;
		[Bindable]
		public function get resolutionInSeconds():Number{ return _resolutionInSeconds; }
		public function set resolutionInSeconds( value:Number ):void
		{
			_resolutionInSeconds = value;
			updateModel();
		}
		
		private var _intervalVO:Object = new Object;
		private var _currentInterval:int = 0;
		[Bindable]
		public function get currentInterval():int{ return _currentInterval; }
		public function set currentInterval( value:int ):void
		{
			_intervalVO.last = _currentInterval;
			_intervalVO.current = value;
			_currentInterval = value;
			currentSecond = _currentInterval * _resolutionInSeconds;
			//sendNotification( NOTE_PLAYBACK_CHANGED, _intervalVO );
			eventDispatcher.dispatchEvent( new Event( UPDATE_INTERVAL ) );
		}
		
		private var _endInterval:int = 1;
		[Bindable]
		public function get endInterval():int{ return _endInterval; }
		public function set endInterval( value:int ):void
		{
			value = Math.max( value, _startInterval + 1 );
			_endInterval = value;
			updateModel();
		}
		
		private var _startInterval:int = 0;
		[Bindable]
		public function get startInterval():int{ return _startInterval; }
		public function set startInterval( value:int ):void
		{
			value = Math.min( value, _endInterval - 1 );
			_startInterval = value;
			updateModel();
		}
		
		public function PlaybackProxy( StartInterval:int = 0, EndInterval:int = 1, ResolutionInSeconds:Number = .1 )
		{
			super( NAME );
			endInterval			= EndInterval;
			startInterval 		= StartInterval;
			resolutionInSeconds	= ResolutionInSeconds;
			
			updateTimer();
			isPlaying = false;
			eventDispatcher =  new EventDispatcher();
		}
		
		public function play():void
		{
			_timer.addEventListener( TimerEvent.TIMER, onNextInterval );
			_timer.addEventListener( TimerEvent.TIMER_COMPLETE, onResetTimer );
			isPlaying = true;
			_timer.start();
		}
		
		public function pause():void
		{
			_timer.stop();
			_timer.removeEventListener( TimerEvent.TIMER, onNextInterval );
			_timer.removeEventListener( TimerEvent.TIMER_COMPLETE, onResetTimer );
			isPlaying = false;
		}
		
		public function reset():void
		{
			_timer.stop();
			_timer.removeEventListener( TimerEvent.TIMER, onNextInterval );
			_timer.removeEventListener( TimerEvent.TIMER_COMPLETE, onResetTimer );
			isPlaying = false;
			currentInterval = startInterval;
			updateTimer();
		}
		
		private function updateModel():void
		{
			totalSeconds 	= ( _endInterval - _startInterval ) * _resolutionInSeconds;
			totalIntervals 	= _endInterval - _startInterval;
			currentInterval = ( Math.min( _endInterval, Math.max( _startInterval, currentInterval ) ) );
			updateTimer();
			//-- setting current interval will cascade to set the currentSecond.
			//-- want to bound the currentInterval incase max or min changed.
		}
		
		private function updateTimer():void
		{
			if( isPlaying )
			{
				_timer.stop();
				_timer.removeEventListener( TimerEvent.TIMER, onNextInterval );
				_timer.removeEventListener( TimerEvent.TIMER_COMPLETE, onResetTimer );
			}
			
			_timer = new Timer( _resolutionInSeconds * 1000 / _playbackSpeed, totalIntervals );
			
			if( isPlaying )
			{
				_timer.addEventListener( TimerEvent.TIMER, onNextInterval );
				_timer.addEventListener( TimerEvent.TIMER_COMPLETE, onResetTimer );
				_timer.start();
			}
		}
		
		private function onNextInterval( event:TimerEvent ):void
		{
			if( currentInterval == endInterval )
			{
				reset();
			}
			else
			{
				currentInterval++;
			}
		}
		
		private function onResetTimer( event:TimerEvent ):void
		{
			updateTimer();
		}
	}
}