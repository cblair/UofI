package com.ts.importing.parsing.model
{
	import com.ts.data.model.DataProxy;
	import com.ts.importing.configuration.model.ExternalImportProxy;
	
	import org.puremvc.as3.patterns.proxy.Proxy;

	public class AbstractParsingPolicy extends Proxy
	{
		protected var _externalImportProxies:Array;
		protected var _rawData:String;
		protected var _invalidImportProxies:Array;
		protected var _iterator:int;
		protected var _workingImportProxy:ExternalImportProxy;
		protected var _isValid:Boolean;
		protected var _newDataProxies:Array;
		protected var _dataRange:int;
		protected var _dataResolution:int;
		
		public function AbstractParsingPolicy( externalImportProxies:Array, rawData:String, name:String, dataRange:int, dataResolution:int )
		{
			super( name );
			
			_externalImportProxies = externalImportProxies;
			_rawData = rawData;
			_invalidImportProxies = new Array();
			_iterator = -1;
			_dataRange = dataRange;
			_dataResolution = dataResolution;
		}
		
		public function get isValid():Boolean
		{
			return _isValid;
		}
		
		public function get invalidDataSetNames():Array
		{
			return _invalidImportProxies;
		}
		
		public function validateImport():void
		{
			var isValid:Boolean = true;
			_iterator = -1;
			
			validateNextProxy();
		}
		
		private function validateNextProxy():void
		{
			_iterator++;
			if( _iterator < _externalImportProxies.length )
			{
				_workingImportProxy = _externalImportProxies[ _iterator ] as ExternalImportProxy;
				
				if( validateWorkingImportProxy() )
				{
					_workingImportProxy.statusMessage = "Data found, preparing source for parsing";
					_workingImportProxy.importProgress = .26;
				}
				else
				{
					_isValid = false;
					_workingImportProxy.statusMessage = "Data missing, closing import. please check ." + _workingImportProxy.externalFileExtension + " export file";
					_workingImportProxy.importProgress = 0;
					_workingImportProxy.isValid = false;
				}
				
				//** WAIT TO UPDATE DISPLAY **//
				sendNotification( ApplicationFacade.NOTE_WAIT_FOR_DISPLAY, validateNextProxy );
			}
			else
			{
				sendNotification( ImportParsingProxy.NOTE_VALIDATION_COMPLETE, isValid );
			}
		}
		
		protected function validateWorkingImportProxy():Boolean
		{
			//-- OVERRIDE ME!! --//
			return false;
		}
		
		public function get parsedDataProxies():Array
		{
			return _newDataProxies;
		}
		
		public function parseImport():Array
		{
			_iterator = -1;
			_newDataProxies = new Array();
			initializeParser();
			parseNextProxy();
			
			return null;
		}
		
		private function parseNextProxy():void
		{
			_iterator++;
			if( _iterator < _externalImportProxies.length )
			{
				_workingImportProxy = _externalImportProxies[ _iterator ] as ExternalImportProxy;
				_workingImportProxy.statusMessage = "Parsing & refactoring into data proxy...";
				_workingImportProxy.importProgress = .3;
				parseWorkingProxy();
			}
			else
			{
				sendNotification( ImportParsingProxy.NOTE_PARSING_COMPLETE );
			}
		}
		
		protected function initializeParser():void
		{
			//-- OVERRIDE ME
		}
		
		protected function parseWorkingProxy():void
		{
			//-- OVERRIDE ME, BUT CALL ME IN THE END!! --//
			_workingImportProxy.statusMessage = "Import Complete";
			_workingImportProxy.importProgress = 1;
			sendNotification( ApplicationFacade.NOTE_WAIT_FOR_DISPLAY, parseNextProxy );
			//packageWorkingProxy();
		}
		
		/*protected function packageWorkingProxy():void
		{
			//-- OVERRIDE ME!! --//
			_workingImportProxy.statusMessage = "Import Complete";
			_workingImportProxy.importProgress = 1;
			

			sendNotification( ApplicationFacade.NOTE_WAIT_FOR_DISPLAY, parseNextProxy );
		}*/
	}
}