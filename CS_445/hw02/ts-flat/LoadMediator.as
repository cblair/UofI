package com.ts.load.view
{
	import com.ts.importing.configuration.view.ImportConfigMediator;
	import com.ts.load.model.LoadProxy;
	
	import flash.events.Event;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.mediator.Mediator;
	import org.puremvc.as3.utilities.statemachine.State;
	import org.puremvc.as3.utilities.statemachine.StateMachine;
	
	public class LoadMediator extends Mediator
	{
		public static const NAME:String						= "LoadMediator";
		
		public function get view():LoadView
		{
			return viewComponent as LoadView;
		}
		
		public function LoadMediator( viewComponent:Object=null )
		{
			super( NAME, viewComponent );
		}
		
		override public function listNotificationInterests():Array
		{
			return[ StateMachine.CHANGED ];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch( notification.getName() )
			{
				case StateMachine.CHANGED:
					if( State( notification.getBody() ).name == LoadProxy.STATE_LOADING ||
						State( notification.getBody() ).name == LoadProxy.STATE_APP_READY_LOADING )
					{
						view.setCurrentState( "loading" );
						view.addEventListener( "cancelLoad", handleEvent );
					}
					else
					{
						view.setCurrentState( "hidden" );
						view.removeEventListener( "cancelLoad", handleEvent );
					}
					break;
				
				default:
					break;
			}
		}
		
		private function handleEvent( event:Event ):void
		{
			switch( event.type )
			{
				case "cancelLoad":
					sendNotification( LoadProxy.NOTE_CANCEL_LOAD );
					break;
				default:
					break;
			}
		}
	}
}