package com.ts.presentation.controller
{
	import com.ts.aspect.model.AspectProxy;
	import com.ts.data.model.DatabaseProxy;
	import com.ts.importing.aspectClasses.AspectFactory;
	import com.ts.presentation.model.DataSelectionProxy;
	import com.ts.presentation.model.map.SystemProxy;
	import com.ts.presentation.model.perdue.PerdueProxy;
	import com.ts.presentation.model.timeSpace.TimeSpaceProxy;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	import org.puremvc.as3.utilities.statemachine.StateMachine;
	
	/**
	 * This is the final destination for both the Load and Import operations. It registers system proxies to the facade, sets up adorning layers, and
	 * gets the display model ready to present. It finishes by entering the presentation state.
	 **/
	public class FinalizeApplicationCommand extends SimpleCommand
	{
		override public function execute(notification:INotification):void
		{
			var database:DatabaseProxy = facade.retrieveProxy( DatabaseProxy.NAME ) as DatabaseProxy;
			var system:SystemProxy = notification.getBody() as SystemProxy;
			
			//-- 1. register core system proxies
			facade.registerProxy( system );
			facade.registerProxy( system.networkModel );
			facade.registerProxy( system.trafficModel );
			
			//-- 2. register display proxies that will be loaded when the aspect is built
			facade.registerProxy( new TimeSpaceProxy() );
			facade.registerProxy( new PerdueProxy() );
			
			//-- 3. build aspect proxy ( adorners for the aspect )
			var aspectProxy:AspectProxy = AspectFactory.getAspectClass( system.aspect ).buildAspectModel( database.dataIndex, system );
			
			//-- 4. build dataSelectionProxy -- need to phase this out
			var dataSelector:DataSelectionProxy = new DataSelectionProxy( system, database.dataIndex, aspectProxy );
			
			//-- 5. register display proxies
			facade.registerProxy( aspectProxy );
			facade.registerProxy( dataSelector );
			
			//-- 6. enter presentation state
			sendNotification( StateMachine.ACTION, null, ApplicationFacade.ACTION_APPLICATION_READY );
		}
	}
}