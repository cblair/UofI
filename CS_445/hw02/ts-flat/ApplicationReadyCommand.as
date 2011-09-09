package com.ts.core.controller
{
	import com.ts.importing.configuration.model.RequiredXMLProxy;
	import com.ts.importing.configuration.model.ExternalImportXMLProxy;
	import com.ts.importing.configuration.model.TemplateXMLProxy;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	import org.puremvc.as3.utilities.startupmanager.StartupManager;
	import org.puremvc.as3.utilities.startupmanager.model.StartupMonitorProxy;
	
	public class ApplicationReadyCommand extends SimpleCommand
	{
		override public function execute(notification:INotification):void
		{
			facade.removeCommand( ExternalImportXMLProxy.NOTE_SOURCE_XML_LOADED );
			facade.removeCommand( ExternalImportXMLProxy.NOTE_SOURCE_XML_FAILED );
			facade.removeCommand( TemplateXMLProxy.NOTE_TEMPLATE_XML_LOADED );
			facade.removeCommand( TemplateXMLProxy.NOTE_TEMPLATE_XML_FAILED );
			facade.removeCommand( RequiredXMLProxy.NOTE_CORE_XML_LOADED );
			facade.removeCommand( RequiredXMLProxy.NOTE_CORE_XML_FAILED );
			facade.removeCommand( StartupMonitorProxy.LOADING_COMPLETE );
		}
	}
}