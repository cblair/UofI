package 
{
	import com.ts.core.controller.StartupCommand;
	import com.ts.core.controller.StartupResourcesCommand;
	import com.ts.core.controller.WaitForDisplayCommand;
	import com.ts.importing.configuration.model.ExternalImportXMLProxy;
	
	import org.puremvc.as3.interfaces.IFacade;
	import org.puremvc.as3.patterns.facade.Facade;
	import org.puremvc.as3.utilities.startupmanager.StartupManager;
	import org.puremvc.as3.utilities.startupmanager.controller.StartupResourceFailedCommand;
	import org.puremvc.as3.utilities.startupmanager.controller.StartupResourceLoadedCommand;
	
	public class ApplicationFacade extends Facade implements IFacade
	{
		public static const STARTUP:String					= "startup";
		
		public static const NOTE_ENTER_STARTUP:String 		= "/note/enterStartup";
		public static const NOTE_WAIT_FOR_DISPLAY:String	= "/note/waitForDisplay";
		
		public static const ACTION_APPLICATION_READY:String	= "/action/applicationReady";
		public static const ACTION_STARTUP_COMPLETED:String	= "/action/startupCompleted";
		
		public static const STATE_STARTING_UP:String		= "/state/startingUp";
	
		public function ApplicationFacade()
		{
			super();
		}
		
		/**
		 * Singleton ApplicationFacade Factory Method
		 **/
		public static function getInstance():ApplicationFacade
		{
			if( instance == null ) instance = new ApplicationFacade();
			return instance as ApplicationFacade;
		}
		
		// Register Commands with the Controller
		override protected function initializeController():void
		{
			super.initializeController();
			registerCommand( STARTUP, StartupCommand );
			registerCommand( NOTE_WAIT_FOR_DISPLAY, WaitForDisplayCommand );
		}
		
		/**
		 * Startup the PureMVC apparatus, passing in a reference to the application
		 **/
		public function startup( app:TrafficSense ):void
		{
			sendNotification( STARTUP, app );
		}
	}
}