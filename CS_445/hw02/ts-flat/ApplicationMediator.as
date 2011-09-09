package 
{
	import com.ts.core.view.IntroPaneMediator;
	import com.ts.core.view.MainMenuMediator;
	import com.ts.importing.configuration.view.ImportConfigMediator;
	import com.ts.load.view.LoadMediator;
	import com.ts.presentation.view.PresentationMediator;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.mediator.Mediator;
	import org.puremvc.as3.utilities.startupmanager.StartupManager;
	import org.puremvc.as3.utilities.startupmanager.model.StartupMonitorProxy;
	import org.puremvc.as3.utilities.statemachine.StateMachine;
	
	public class ApplicationMediator extends Mediator
	{
		public static const NAME:String = "ApplicationMediator";
		
		public function get application():TrafficSense
		{
			return viewComponent as TrafficSense;
		}
		
		public function ApplicationMediator( viewComponent:TrafficSense )
		{
			super( NAME, viewComponent );
		}
		
		override public function onRegister():void
		{
			facade.registerMediator( new IntroPaneMediator( application.introPane ) );
			facade.registerMediator( new MainMenuMediator( application.mainMenu ) );
			facade.registerMediator( new LoadMediator( application.loadPane ) );
		}
		
		override public function listNotificationInterests():Array
		{
			return [ ApplicationFacade.NOTE_ENTER_STARTUP,
					 StartupMonitorProxy.LOADING_COMPLETE
					];
		}
		
		override public function handleNotification( note:INotification ):void
		{
			switch( note.getName() )
			{
				case ApplicationFacade.NOTE_ENTER_STARTUP:
					break
				
				case StartupMonitorProxy.LOADING_COMPLETE:
					sendNotification( StateMachine.ACTION, null, ApplicationFacade.ACTION_STARTUP_COMPLETED );
					break;
					
				default:
					break;
			}
		}
	}
}