package com.ts.core.controller
{
	import flash.display.DisplayObject;
	import flash.events.TimerEvent;
	import flash.utils.Timer;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	
	public class WaitForDisplayCommand extends SimpleCommand
	{
		private var _waitingCall:Function;
		
		override public function execute(notification:INotification):void
		{
			_waitingCall = notification.getBody() as Function;
			var timer:Timer = new Timer( 60, 0 );
			timer.addEventListener( TimerEvent.TIMER, handleEvent );
			timer.start();
		}
		
		private function handleEvent( event:TimerEvent ):void
		{
			Timer( event.target ).removeEventListener( TimerEvent.TIMER, handleEvent );
			_waitingCall();
		}
	}
}