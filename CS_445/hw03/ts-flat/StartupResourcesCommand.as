package com.ts.core.controller
{
	import com.ts.importing.configuration.model.ExternalImportXMLProxy;
	import com.ts.importing.configuration.model.RequiredXMLProxy;
	import com.ts.importing.configuration.model.TemplateXMLProxy;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	import org.puremvc.as3.utilities.startupmanager.StartupManager;
	import org.puremvc.as3.utilities.startupmanager.controller.StartupResourceFailedCommand;
	import org.puremvc.as3.utilities.startupmanager.controller.StartupResourceLoadedCommand;
	import org.puremvc.as3.utilities.startupmanager.interfaces.IStartupProxy;
	import org.puremvc.as3.utilities.startupmanager.model.RetryParameters;
	import org.puremvc.as3.utilities.startupmanager.model.RetryPolicy;
	import org.puremvc.as3.utilities.startupmanager.model.StartupMonitorProxy;
	import org.puremvc.as3.utilities.startupmanager.model.StartupResourceProxy;
	
	public class StartupResourcesCommand extends SimpleCommand
	{
		private var _monitor:StartupMonitorProxy;
		
		override public function execute(notification:INotification):void
		{		
			facade.registerProxy( new StartupMonitorProxy () );
			facade.removeCommand( ApplicationFacade.NOTE_ENTER_STARTUP );
			facade.registerCommand( ExternalImportXMLProxy.NOTE_SOURCE_XML_LOADED, StartupResourceLoadedCommand );
			facade.registerCommand( ExternalImportXMLProxy.NOTE_SOURCE_XML_FAILED, StartupResourceFailedCommand );
			facade.registerCommand( TemplateXMLProxy.NOTE_TEMPLATE_XML_LOADED, StartupResourceLoadedCommand );
			facade.registerCommand( TemplateXMLProxy.NOTE_TEMPLATE_XML_FAILED, StartupResourceFailedCommand );
			facade.registerCommand( RequiredXMLProxy.NOTE_CORE_XML_LOADED, StartupResourceLoadedCommand );
			facade.registerCommand( RequiredXMLProxy.NOTE_CORE_XML_FAILED, StartupResourceFailedCommand );
			facade.registerCommand( StartupManager.LOADING_COMPLETE, ApplicationReadyCommand );
			
			_monitor = facade.retrieveProxy( StartupMonitorProxy.NAME ) as StartupMonitorProxy;
			_monitor.defaultRetryPolicy = new RetryPolicy( new RetryParameters( 0, 0, 30 ) );
			
			var sourceXMLProxy:IStartupProxy = new ExternalImportXMLProxy();
			var templateXMLProxy:IStartupProxy = new TemplateXMLProxy();
			var coreXMLProxy:IStartupProxy = new RequiredXMLProxy();
			
			facade.registerProxy( sourceXMLProxy );
			facade.registerProxy( templateXMLProxy );
			facade.registerProxy( coreXMLProxy );
			
			var rSourceXMLProxy:StartupResourceProxy = makeAndRegisterStartupResource( ExternalImportXMLProxy.SRNAME, sourceXMLProxy );
			var rTemplateXMLProxy:StartupResourceProxy = makeAndRegisterStartupResource( TemplateXMLProxy.SRNAME, templateXMLProxy )
			var rCoreXMLProxy:StartupResourceProxy = makeAndRegisterStartupResource( RequiredXMLProxy.SRNAME, coreXMLProxy )
			
			_monitor.loadResources();
		}
		
		private function makeAndRegisterStartupResource( srName:String, srProxy:IStartupProxy ):StartupResourceProxy
		{
			var resourceProxy:StartupResourceProxy = new StartupResourceProxy( srName, srProxy );
			facade.registerProxy( resourceProxy );
			_monitor.addResource( resourceProxy );
			
			return resourceProxy;
		}
	}
}