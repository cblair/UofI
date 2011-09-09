package com.ts.importing.configuration.controller
{
	import com.ts.importing.configuration.model.ExternalImportProxy;
	import com.ts.importing.configuration.model.ExternalImportXMLProxy;
	import com.ts.importing.configuration.model.ImportConfigProxy;
	import com.ts.importing.configuration.model.TemplateXMLProxy;
	import com.ts.importing.configuration.model.enumerations.RequiredImportDataName;
	import com.ts.importing.configuration.view.ImportConfigMediator;
	import com.ts.importing.parsing.model.ImportParsingProxy;
	import com.ts.importing.virtualData.AbstractVirtualDataFactory;
	import com.ts.load.controller.RemoveLoadCommand;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;

	
	/**
	 * Prepares the importConfiguration model.
	 * 
	 * <p> The first step in importing a new module is determining how to prepare the data model. </p>
	 * <p> ConfigureImportCommand prepares the diffent configuration options the user can choose to model. </p>
	 * <p> Once the user has decided on a configuration, the import process can begin. ( see ImportModuleCommand ) </p>
	 */
	public class ConfigureImportCommand extends SimpleCommand
	{
		override public function execute(notification:INotification):void
		{			
			if( !facade.hasProxy( ImportConfigProxy.NAME ) )
			{
				var externalXMLProxy:ExternalImportXMLProxy = facade.retrieveProxy( ExternalImportXMLProxy.NAME ) as ExternalImportXMLProxy;
				
				var importConfigurationProxy:ImportConfigProxy = new ImportConfigProxy( externalXMLProxy );
				var importParser:ImportParsingProxy = new ImportParsingProxy();
				var templates:TemplateXMLProxy = facade.retrieveProxy( TemplateXMLProxy.NAME ) as TemplateXMLProxy;
				
				facade.registerProxy( importConfigurationProxy );
				facade.registerProxy( importParser );
				
				facade.registerMediator( new ImportConfigMediator( ApplicationMediator( facade.retrieveMediator( ApplicationMediator.NAME ) as ApplicationMediator ).application.importPane,
																   importParser, importConfigurationProxy, templates ) );
				
				sendNotification( ImportConfigProxy.NOTE_IMPORT_CONFIG_READY );
			}
		}
	}
}