package com.ts.importing.configuration.model.vo
{
	import com.ts.importing.configuration.model.ExternalImportProxy;

	public class ImportConfigVO
	{
		
		public var externalImportProxies:Array;
		public var externalFileExts:Array;
		public var importFilePathLookup:Object;
		
		public function ImportConfigVO( externalXML:Array, externalFileExt:Array, importPathObj:Object )
		{
			externalImportProxies = externalXML;
			externalFileExts = externalFileExt;
			importFilePathLookup = importPathObj;
		}
		
		public function getImportProxiesByExt( extension:String ):Array 
		{
			var importByExt:Array = new Array();
			for each( var sourceData:ExternalImportProxy in externalImportProxies )
			{
				if( sourceData.externalFileExtension == extension )
				{
					importByExt.push( sourceData );
				}
			}
			return importByExt;
		}
		
		public function getImportFilePathExt( ext:String ):Array
		{
			if( importFilePathLookup )
			{
				return importFilePathLookup[ ext ];
			}
			
			return null;
		}
	}
}