package com.ts.importing.configuration.view
{
	import com.ts.importing.RegisterImportCommand;
	import com.ts.importing.RemoveImportCommand;
	import com.ts.importing.configuration.model.ImportConfigProxy;
	import com.ts.importing.configuration.model.TemplateXMLProxy;
	import com.ts.importing.parsing.model.ImportParsingProxy;
	
	import flash.events.Event;
	
	import mx.collections.ArrayCollection;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.mediator.Mediator;
	import org.puremvc.as3.utilities.statemachine.State;
	import org.puremvc.as3.utilities.statemachine.StateMachine;
	
	/**
	 * Import Pane Mediator is responsible for keeping the import veiw in a current state during an import process.
	 * <p>The import process is as follows:</p>
	 * <p>1. Select Template and Emphasis Objectives</p>
	 * <p>2. Validate Import Files ( check that they exist )</p>
	 * <p>3. Import Raw data files</p>
	 * <p>4. Validate Raw data files</p>
	 * <p>5. Parse Raw data into data sets</p>
	 * <p>6. Generate any additional "virtual" data sets needed for the module to run</p>
	 * <p>8. Trash unneeded data</p>
	 * <p>9. Package needed data</p>
	 * <p>10. Pass the data to the application for unpacking</p>
	 **/
	public class ImportConfigMediator extends Mediator
	{
		/**
		 * The name used to request this component from the facade.
		 * <p>the "NAME" constant is common to mediators and proxys application wide.</p>
		 **/
		public static const NAME:String = "importPaneMediator";
		
		private var _parserProxy:ImportParsingProxy;
		private var _importConfig:ImportConfigProxy;
		private var _templateProxy:TemplateXMLProxy;
		
		/**
		 * Read only getter, lets the mediator get its view already casted to the correct type.
		 * <p>This getter is common in every mediator</p>
		 **/
		public function get view():ImportConfigView
		{
			return viewComponent as ImportConfigView;
		}
		
		public function ImportConfigMediator( viewComponent:Object, parserProxy:ImportParsingProxy, importConfig:ImportConfigProxy, templateProxy:TemplateXMLProxy )
		{
			super( NAME, viewComponent );
			_parserProxy = parserProxy;
			_importConfig = importConfig;
			_templateProxy = templateProxy;
		}
		
		override public function onRegister():void
		{
			facade.registerCommand( ImportConfigProxy.NOTE_ENTER_IMPORT_MODE, RegisterImportCommand );
			facade.registerCommand( ImportConfigProxy.NOTE_EXIT_IMPORT_MODE, RemoveImportCommand );
		}
		
		override public function onRemove():void
		{
			facade.removeCommand( ImportConfigProxy.NOTE_ENTER_IMPORT_MODE );
			facade.removeCommand( ImportConfigProxy.NOTE_EXIT_IMPORT_MODE );
		}
		
		/**
		 * returns an array of the notification interests of this mediator.
		 * <p>notifications are identified by a string constant, and handeled in the "handleNotification" function</p>
		 * <p>the facade inspects this list each time a notification is sent.</p>
		 * <p>this allows horizontal communication to exist within the entire application.</p>
		 **/
		override public function listNotificationInterests():Array
		{
			return [ StateMachine.CHANGED,
					 ImportParsingProxy.NOTE_LOADING_DATA,
					 ImportParsingProxy.NOTE_MISSING_DATA_FAIL,
					 ImportConfigProxy.NOTE_IMPORT_CONFIG_READY ]
		}
		
		/**
		 * handles notification interests for this mediator
		 **/
		override public function handleNotification(notification:INotification):void
		{
			switch( notification.getName() )
			{
				case StateMachine.CHANGED:
				{
					if( State( notification.getBody() ).name == ImportConfigProxy.STATE_IMPORTING_MODULE ||
						State( notification.getBody() ).name == ImportConfigProxy.STATE_APP_READY_IMPORTING )
					{
						view.gotoOpeningState();
						view.importList.externalImportProxies = _importConfig.externalImportProxies;
						view.templateMenu.templateVOCollection = new ArrayCollection( _templateProxy.templateXMLVO.templates );
						view.addEventListener( "saveImportList", handleEvent );
						view.addEventListener( "beginImport", handleEvent );
						view.addEventListener( "cancelImport", handleEvent );
						view.addEventListener( "aspectSelected", handleEvent );
						view.addEventListener( "aspectUnselected", handleEvent );
					}
					else
					{
						view.gotoInactiveState();
						view.importList.deconstruct();
						view.removeEventListener( "saveImportList", handleEvent );
						view.removeEventListener( "beginImport", handleEvent );
						view.removeEventListener( "cancelImport", handleEvent );
						view.removeEventListener( "aspectUnselected", handleEvent );
					}
				}
				break;
				
				case ImportParsingProxy.NOTE_LOADING_DATA:
					//parserProxy = facade.retrieveProxy( ParseProxy.NAME ) as ParseProxy;
					//view.currentExt.text = "." + parserProxy.workingExt;
					//view.progress.text = String( parserProxy.currentLoadProgress );
					break;
				
				case ImportParsingProxy.NOTE_MISSING_DATA_FAIL:
					//parserProxy = facade.retrieveProxy( ImportParsingProxy.NAME ) as ImportParsingProxy;
					//view.importList.missingDataSets = parserProxy.missingDataSets;
					break;
				
				case ImportConfigProxy.NOTE_IMPORT_CONFIG_READY:
					view.importList.externalImportProxies = _importConfig.externalImportProxies;
					break;
				
				default:	break;
			}
		}
		
		private function handleEvent( event:Event ):void
		{
			switch( event.type )
			{
				case "saveImportList":
					sendNotification( ImportConfigProxy.NOTE_SAVE_IMPORT_INSTRUCTIONS );
					break;
				
				case "beginImport":
					sendNotification( ImportConfigProxy.NOTE_BROWSE_FOR_IMPORT );
					break;

				case "cancelImport":
					sendNotification( ImportConfigProxy.NOTE_CANCEL_IMPORT );
					break;

				case "aspectSelected":
					view.importList.externalImportProxies = _importConfig.selectAspect( view.aspectMenu.currentAspect.aspectClass );
					break;
				
				case "aspectUnselected":
					view.importList.externalImportProxies = _importConfig.selectAspect( null );
					break;
				
				default:	break;
			}
		}
	}
}