package com.ts.core.controller
{
	import com.ts.importing.configuration.model.ExternalImportXMLProxy;
	
	import mx.core.Application;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	import org.puremvc.as3.utilities.startupmanager.StartupManager;
	import org.puremvc.as3.utilities.statemachine.StateMachine;
	
	//-- A simple command that prepares the view for use.
	//-- Executed AFTER the modelPrep command ( after the model is prepared )
	public class PrepViewCommand extends SimpleCommand
	{
		public function PrepViewCommand()
		{
			super();
		}
		
		/** Creates the ApplicationMediator which stewards the application view component.
		 * The application sends a reference of itself as the notification body into the mediator's constructor
		 * during this STARTUP command. 
		 * The ApplicationMediator will use this reference to communicate application wide.
		 * It is the only mediator with this reference. all other mediators will be created within it.
		 **/
		override public function execute( note:INotification ):void
		{
			var app:TrafficSense = note.getBody() as TrafficSense;
			facade.registerMediator( new ApplicationMediator( app ) );
		}
	}
}