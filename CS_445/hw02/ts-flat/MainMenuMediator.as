package com.ts.core.view
{
	import com.ts.save.model.SaveProxy;
	
	import flash.events.Event;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.mediator.Mediator;
	import org.puremvc.as3.utilities.statemachine.StateMachine;
	
	public class MainMenuMediator extends Mediator
	{
		public static const NAME:String			= "MainMenuMediator";
		
		public function get view():MainMenuView
		{
			return viewComponent as MainMenuView;
		}
		
		public function MainMenuMediator( viewComponent:Object=null )
		{
			super( NAME, viewComponent );
		}
		
		override public function onRegister():void
		{
			view.fileMenu.addEventListener( "menuBtnSelected", handleEvent );
			view.viewMenu.addEventListener( "menuBtnSelected", handleEvent );
		}
		
		override public function onRemove():void
		{
			
		}
		
		override public function listNotificationInterests():Array
		{
			return[];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch( notification )
			{
				default:
					break;
			}
		}
		
		private function handleEvent( event:Event ):void
		{
			switch( event.type )
			{
				case "menuBtnSelected":
					sendNotification( view.menuBtnVO.command );
					break;
				default:
					break;
			}
		}
	}
}