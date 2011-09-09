package com.ts.importing.parsing.controller
{
	import com.ts.importing.configuration.model.vo.ImportConfigVO;
	import com.ts.importing.parsing.model.ImportParsingProxy;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	
	public class ParseImportCommand extends SimpleCommand
	{
		override public function execute(notification:INotification):void
		{
			var importPrepVO:ImportConfigVO = notification.getBody() as ImportConfigVO;
			var parseProxy:ImportParsingProxy = facade.retrieveProxy( ImportParsingProxy.NAME ) as ImportParsingProxy
			
			parseProxy.importAssetsForParsing( importPrepVO );
		}
	}
}