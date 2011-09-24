package com.ts.presentation.controller
{
	import com.ts.presentation.model.DataSelectionProxy;
	import com.ts.presentation.model.vo.LodSelectionRequestVO;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	
	public class ChangeDataCommand extends SimpleCommand
	{
		override public function execute(notification:INotification):void
		{
			var dataSelector:DataSelectionProxy = facade.retrieveProxy( DataSelectionProxy.NAME ) as DataSelectionProxy;
			var selectVars:LodSelectionRequestVO = notification.getBody() as LodSelectionRequestVO;
			
			if( selectVars )
			{
				dataSelector.selectData( selectVars.levelOfDetail, selectVars.lodID, selectVars.intersectionNumber );
			}
			else
			{
				dataSelector.unselectData();
			}
		}
	}
}