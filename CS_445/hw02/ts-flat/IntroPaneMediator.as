package com.ts.core.view
{
	import com.ts.importing.configuration.view.ImportConfigMediator;
	import com.ts.importing.parsing.model.ImportParsingProxy;
	import com.ts.load.model.LoadProxy;
	import com.ts.load.view.LoadMediator;
	
	import flash.events.Event;
	
	import org.puremvc.as3.interfaces.IMediator;
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.mediator.Mediator;
	import org.puremvc.as3.utilities.statemachine.State;
	import org.puremvc.as3.utilities.statemachine.StateMachine;
	
	public class IntroPaneMediator extends Mediator implements IMediator
	{
		/**
		 * The name used to request this component from the facade.
		 * <p>the "NAME" constant is common to mediators and proxys application wide.</p>
		 **/
		public static const NAME:String = "IntroPaneMediator";
		
		/**
		 * State - Module Browsing - where the user chooses to either load an existing module, or import a new one from external assets.
		 **/
		public static const STATE_MODULE_BROWSING:String		= NAME + "/state/moduleBrowsing";
		
		/**
		 * Action - Browse Modules - changes the state of the application to the "Module Browsing" state
		 **/
		public static const ACTION_BROWSE_MODULES:String		= NAME + "/action/browseModules";
		
		/**
		 * Notification - Enter Module Browse - requests the application to enter the "module browsing state"
		 **/
		public static const NOTE_ENTER_BROWSE:String 			= NAME + "/note/enterBrowse";
		
		/**
		 * Notification - Exit Module Browse - requests the application to enter the "module browsing state"
		 **/
		public static const NOTE_EXIT_BROWSE:String 			= NAME + "/note/exitBrowse";
		
		/**
		 * Read only getter, lets the mediator get its view already casted to the correct type.
		 * This getter is common in every mediator
		 **/
		public function get view():IntroPaneView
		{
			return viewComponent as IntroPaneView;
		}
		
		public function IntroPaneMediator( viewComponent:Object )
		{
			super( NAME, viewComponent );
		}
		
		/**
		 * returns an array of the notification interests of this mediator.
		 * <p>notifications are identified by a string constant, and handeled in the "handleNotification" function</p>
		 * <p>the facade inspects this list each time a notification is sent.</p>
		 * <p>this allows horizontal communication to exist within the entire application.</p>
		 **/
		override public function listNotificationInterests():Array
		{
			return [ StateMachine.CHANGED ];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch( notification.getName() )
			{
				case StateMachine.CHANGED:
					if( State( notification.getBody() ).name == STATE_MODULE_BROWSING )
					{
						view.setCurrentState( "browsing" );
						view.addEventListener( "openModule", handleEvent );
					}
					else
					{
						view.setCurrentState( "hidden" );
						view.removeEventListener( "openModule", handleEvent );
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
				case "openModule":
					sendNotification( LoadProxy.NOTE_LOAD );
					break;
				default:
					break;
			}
		}
	}
}