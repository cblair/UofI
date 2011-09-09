package com.ts.presentation.controller
{
	import com.ts.presentation.model.annotation.AnnotationProxy;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	import org.puremvc.as3.utilities.statemachine.StateMachine;
	
	public class AnnotateCommand extends SimpleCommand
	{
		override public function execute(notification:INotification):void
		{
			sendNotification( StateMachine.ACTION, null, AnnotationProxy.ACTION_ANNOTATE );
		}
	}
}