package com.ts.load.controller
{
	import com.ts.load.model.LoadProxy;
	import com.ts.load.view.LoadMediator;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	import org.puremvc.as3.utilities.statemachine.StateMachine;
	
	public class LoadCommand extends SimpleCommand
	{
		override public function execute(notification:INotification):void
		{
			sendNotification( StateMachine.ACTION, null, LoadProxy.ACTION_LOAD );
		}
	}
}