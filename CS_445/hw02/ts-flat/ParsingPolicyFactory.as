package com.ts.importing.parsing.model
{

	public class ParsingPolicyFactory
	{
		public function ParsingPolicyFactory()
		{
		}
		
		public static function getParsingPolicy( ext:String, externalImportProxies:Array, rawData:String, dataRange:int, dataResolution:int ):AbstractParsingPolicy
		{
			switch ( ext )
			{
				case "fzp":		return new FZPParsingPolicy( externalImportProxies, rawData, dataRange, dataResolution );
				case "LDP":		return new LDPParsingPolicy( externalImportProxies, rawData, dataRange, dataResolution );
				case "str":		return new STRParsingPolicy( externalImportProxies, rawData, dataRange, dataResolution );
				case "lsa":		return new LSAParsingPolicy( externalImportProxies, rawData, dataRange, dataResolution );
				case "in0":		return new IN0ParsingPolicy( externalImportProxies, rawData, dataRange, dataResolution );
				case "mer":		return new MERParsingPolicy( externalImportProxies, rawData, dataRange, dataResolution );
				default:		return null;
			}
		}
	}
}