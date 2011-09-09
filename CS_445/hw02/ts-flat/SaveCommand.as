package com.ts.save.controller
{
	import com.ts.save.model.SaveProxy;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	import org.puremvc.as3.utilities.statemachine.StateMachine;
	
	public class SaveCommand extends SimpleCommand
	{
		override public function execute(notification:INotification):void
		{
			sendNotification( StateMachine.ACTION, null, SaveProxy.ACTION_SAVE );
		}
	}
}