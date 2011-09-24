package com.ts.presentation.controller
{
	import com.ts.presentation.view.PresentationMediator;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	import org.puremvc.as3.utilities.statemachine.StateMachine;
	
	public class PresentCommand extends SimpleCommand
	{
		override public function execute(notification:INotification):void
		{
			sendNotification( StateMachine.ACTION, null, PresentationMediator.ACTION_PRESENT );
		}
	}
}