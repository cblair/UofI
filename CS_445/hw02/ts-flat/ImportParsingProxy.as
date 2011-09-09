package com.ts.importing.parsing.model
{
	import com.fManagement.fileLoader.FileEvent;
	import com.fManagement.fileLoader.FileLoader;
	import com.fManagement.fileTypes.FileTypes;
	import com.ts.data.model.DataProxy;
	import com.ts.importing.configuration.model.ExternalImportProxy;
	import com.ts.importing.configuration.model.enumerations.ExternalImportDataName;
	import com.ts.importing.configuration.model.vo.ImportConfigVO;
	import com.ts.importing.deriveSystem.command.DeriveSystemCommand;
	import com.ts.importing.deriveSystem.model.ImportDataIndex;
	import com.ts.importing.parsing.controller.ParsingCompleteCommand;
	import com.ts.importing.parsing.controller.ValidationCompleteCommand;
	
	import flash.events.Event;
	
	import org.puremvc.as3.patterns.proxy.Proxy;
	
	public class ImportParsingProxy extends Proxy
	{
		
		public static const NAME:String = 'importParsingProxy';
		public static const NOTE_LOADING_DATA:String			= NAME + "/note/loadingData";
		public static const NOTE_LOAD_COMPLETE:String			= NAME + "/note/loadComplete";
		public static const NOTE_LOAD_FAILED:String				= NAME + "/note/loadFailed";
		public static const NOTE_VALIDATING_DATA:String			= NAME + "/note/validatingData";
		public static const NOTE_MISSING_DATA_FAIL:String		= NAME + "/note/missingDataFail";
		public static const NOTE_ALL_DATA_FOUND:String			= NAME + "/note/allDataFound";
		public static const NOTE_PARSING_DATA:String			= NAME + "/note/parsingData";
		public static const NOTE_DERIVE_SYSTEM:String			= NAME + "/note/deriveSystem";
		
		public static const NOTE_VALIDATION_COMPLETE:String		= NAME + "/note/validationComplete";
		public static const NOTE_PARSING_COMPLETE:String		= NAME + "/note/parsingComplete";
		
		private var _importConfigVO:ImportConfigVO;
		private var _workingImportProxies:Array;
		private var _cyclePointer:int = -1;
		private var _workingFile:String;
		private var _workingParser:AbstractParsingPolicy;
		private var _parseQueue:Array;
		private var _mergedDataProxies:Array
		private var _validImport:Boolean;
		
		private var _dataRange:int = 0;
		private var _intervalsPerSecond:Number = 1;
		private var _in0Parsed:Boolean = false;
		
		public var missingImportProxies:Array;
		public var workingExt:String;
		public var currentLoadProgress:Number = 0;
		
		public function get numIntervals()		:int{ return _dataRange; }
		public function get intervalsPerSecond():Number{ return _intervalsPerSecond; }
		
		/**
		 * Load Resources
		 * Get Parsers
		 * Validate Resources
		 * Parse Resources
		 * Combine Resources
		 * **/
		public function ImportParsingProxy()
		{
			super( NAME );
			_parseQueue = new Array();
			_mergedDataProxies = new Array();
			missingImportProxies = new Array();
		}
		
		public function importAssetsForParsing( importConfigVO:ImportConfigVO ):void
		{
			_importConfigVO = importConfigVO;
			sendNotification( NOTE_LOADING_DATA );
			for each( var importProxy:ExternalImportProxy in _importConfigVO.externalImportProxies )
			{
				importProxy.statusMessage = "Loading source file...";
				importProxy.importProgress = .05;
			}
			startNextCycle();
		}
		
		private var _loadQueue:Array = new Array();
		private function startNextCycle():void
		{
			//-- need to get some global parse variables that are in the in0
			if( !_in0Parsed )
			{
				workingExt = "in0";
				_loadQueue = _importConfigVO.getImportFilePathExt( workingExt );
				_workingImportProxies = _importConfigVO.getImportProxiesByExt( workingExt );
				loadResource();
			}
			else
			{
				if( _loadQueue.length == 0 )
				{
					_cyclePointer ++;
					//-- skip over in0 because its already loaded
					if( _cyclePointer < _importConfigVO.externalFileExts.length )
					{
						if( _importConfigVO.externalFileExts[ _cyclePointer ] == "in0" )
						{
							_cyclePointer++;
						}
					}
					
					if( _cyclePointer < _importConfigVO.externalFileExts.length )
					{
						workingExt = _importConfigVO.externalFileExts[ _cyclePointer ];
						_loadQueue = _importConfigVO.getImportFilePathExt( workingExt );
						_workingImportProxies = _importConfigVO.getImportProxiesByExt( workingExt );
					}
					else
					{
						//** WAIT TO UPDATE DISPLAY **//
						sendNotification( ApplicationFacade.NOTE_WAIT_FOR_DISPLAY, validateImport );
						//validateDataSets()
					}
				}
				
				if( _cyclePointer < _importConfigVO.externalFileExts.length )
				{
					loadResource();
				}
			}
		}
		
		private function loadResource():void
		{
			var fileLoader:FileLoader = new FileLoader();
			fileLoader.addEventListener( FileEvent.FILE_OPEN, onResourceLoad );
			fileLoader.addEventListener( FileEvent.FILE_OPEN_FAILURE, onOpenFailed );
			fileLoader.addEventListener( FileEvent.LOAD_PROGRESS_UPDATE, onResourceUpdate );
				
			trace( _loadQueue[0] );
			fileLoader.loadFile( _loadQueue.shift(), FileTypes.TXT );
			sendNotification( NOTE_LOADING_DATA, workingExt );
		}
		
		private function onOpenFailed( event:FileEvent ):void
		{
			event.target.removeEventListener( FileEvent.FILE_OPEN, onResourceLoad );
			event.target.removeEventListener( FileEvent.FILE_OPEN_FAILURE, onOpenFailed );
			event.target.removeEventListener( FileEvent.LOAD_PROGRESS_UPDATE, onResourceUpdate );
			sendNotification( NOTE_LOAD_FAILED, workingExt );
			
			for each( var importProxy:ExternalImportProxy in _workingImportProxies )
			{
				importProxy.statusMessage = "Source file failed to load";
				importProxy.importProgress = 0;
				importProxy.isValid = false;
			}
		}
		
		private function onResourceLoad( event:FileEvent ):void
		{
			event.target.removeEventListener( FileEvent.FILE_OPEN, onResourceLoad );
			event.target.removeEventListener( FileEvent.FILE_OPEN_FAILURE, onOpenFailed );
			event.target.removeEventListener( FileEvent.LOAD_PROGRESS_UPDATE, onResourceUpdate );
			sendNotification( NOTE_LOAD_COMPLETE, workingExt );
			
			for each( var importProxy:ExternalImportProxy in _workingImportProxies )
			{
				importProxy.statusMessage = "Source file loaded successfully";
				importProxy.importProgress = .25;
			}
			
			_workingFile = String(event.file.fileData);
			buildParser();
		}
		
		private function onResourceUpdate( event:FileEvent ):void
		{
			trace( event.progress );
			for each( var importProxy:ExternalImportProxy in _workingImportProxies )
			{
				importProxy.statusMessage = "Source file loading...";
				importProxy.importProgress = event.progress * .2 + .05;
			}
			currentLoadProgress = event.progress;
		}
		
		private function buildParser():void
		{
			_workingImportProxies = _importConfigVO.getImportProxiesByExt( workingExt );
			var abstractParsingPolicy:AbstractParsingPolicy = ParsingPolicyFactory.getParsingPolicy( workingExt, _workingImportProxies, _workingFile, _dataRange, _intervalsPerSecond );
			_parseQueue.push( abstractParsingPolicy );
			facade.registerProxy( abstractParsingPolicy );
			
			if( !_in0Parsed )
			{
				_intervalsPerSecond = IN0ParsingPolicy( abstractParsingPolicy ).intervalsPerSecond;
				_dataRange = IN0ParsingPolicy( abstractParsingPolicy ).dataRange * _intervalsPerSecond;
				_in0Parsed = true;
			}
			
			//** WAIT TO UPDATE DISPLAY **//
			sendNotification( ApplicationFacade.NOTE_WAIT_FOR_DISPLAY, startNextCycle );
			//startNextCycle();
		}
		
		public function validateImport():void
		{
			sendNotification( NOTE_VALIDATING_DATA );
			
			for each( var importProxy:ExternalImportProxy in _importConfigVO.externalImportProxies )
			{
				importProxy.statusMessage = "preparing source for validation...";
			}
			
			var isValid:Boolean = true;
			_cyclePointer = -1;
			_validImport = true;
			facade.registerCommand( NOTE_VALIDATION_COMPLETE, ValidationCompleteCommand );
			_workingParser = null;
			validateNextImport();
		}
		
		public function validateNextImport():void
		{
			_cyclePointer++;
			
			if( _workingParser )
			{
				if( !_workingParser.isValid )
				{
					_validImport = false;
				}
			}
			
			if( _cyclePointer < _parseQueue.length )
			{
				_workingParser = AbstractParsingPolicy( _parseQueue[ _cyclePointer ] ) as AbstractParsingPolicy;
				
				_workingParser.validateImport();
			}
			else
			{
				facade.removeCommand( NOTE_VALIDATION_COMPLETE );
				
				if( _validImport )
				{
					sendNotification( NOTE_ALL_DATA_FOUND );
					sendNotification( NOTE_PARSING_DATA );
					parseImport();
				}
				else
				{
					sendNotification( NOTE_MISSING_DATA_FAIL );
					parseImport();
					//-- pass out _workingParser.invalidDataSetNames()
					//-- alert application that the import is invalid
					//-- continue validating, but don't parse 
				}
			}
		}
		
		public function parseImport():void
		{
			for each( var importProxy:ExternalImportProxy in _importConfigVO.externalImportProxies )
			{
				importProxy.statusMessage = "parsing import data...";
			}
			_cyclePointer = -1;
			_workingParser = null;
			facade.registerCommand( NOTE_PARSING_COMPLETE, ParsingCompleteCommand );
			facade.registerCommand( NOTE_DERIVE_SYSTEM, DeriveSystemCommand );
			parseNextImport();
			
		}
		
		public function parseNextImport():void
		{
			if( _workingParser )
			{
				mergeImport( _workingParser.parsedDataProxies );
			}
			
			_cyclePointer++;
			if( _cyclePointer < _parseQueue.length )
			{
				_workingParser = AbstractParsingPolicy( _parseQueue[ _cyclePointer ] ) as AbstractParsingPolicy;
				_workingParser.parseImport();
			}
			else
			{
				facade.removeCommand( NOTE_PARSING_COMPLETE );
				
				var importDataIndex:ImportDataIndex = new ImportDataIndex( _mergedDataProxies );
				sendNotification( NOTE_DERIVE_SYSTEM, importDataIndex );
				closeParser();
			}
		}
		
		private function mergeImport( dataProxies:Array ):void
		{
			if( _mergedDataProxies.length == 0 )
			{
				_mergedDataProxies = dataProxies;
			}
			else
			{
				_mergedDataProxies = _mergedDataProxies.concat( dataProxies );
			}
		}
		
		public function closeParser():void
		{
			_importConfigVO = null;
			_workingImportProxies = null;
			_workingFile = null;
			_workingParser = null;
			_parseQueue = null;
			_mergedDataProxies = null;
			missingImportProxies = null;
			workingExt = null;
		}
	}
}