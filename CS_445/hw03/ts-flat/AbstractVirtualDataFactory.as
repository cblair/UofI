package com.ts.importing.virtualData
{
	import com.ts.data.model.DataIndex;
	import com.ts.importing.deriveSystem.model.ImportDataIndex;
	import com.ts.presentation.model.map.SystemProxy;
	
	import org.puremvc.as3.patterns.proxy.Proxy;

	public class AbstractVirtualDataFactory extends Proxy
	{
		public var buildOrder:int;
		public var buildList:Array; 
		public var requirements:Array;
		public var name:String;
		
		public function getBuildList():Array
		{
			return getBuildRequirements( new Array() );
		}
		
		public function getBuildRequirements( buildRequirements:Array ):Array
		{
			for each( var requirement:Object in requirements )
			{
				if( requirement is AbstractVirtualDataFactory )
				{
					buildRequirements = AbstractVirtualDataFactory( requirement ).getBuildRequirements( buildRequirements );
				}
				buildRequirements.push( requirement ); //-- Push requirement whether it's a string or a factory.
			}
			return buildRequirements //-- return recursive list.
		}
		
		public function discoverBuildOrder( order:int ):int
		{
			var maxDepth:int = order;
			if( order > 10 )
			{
				trace( "Circular Reference" );
				return order;
			}
			for each( var requirement:Object in requirements )
			{
				if( requirement is AbstractVirtualDataFactory )
				{
					maxDepth = Math.max( AbstractVirtualDataFactory( requirement ).discoverBuildOrder( order + 1 ), maxDepth );
				}
			}
			buildOrder = maxDepth - order;
			return maxDepth;
		}
		
		public function AbstractVirtualDataFactory( Name:String, Requirements:Array )
		{
			super( Name + "Proxy" );
			name = Name;
			requirements = new Array();
			for each( var requirement:Object in Requirements )
			{
				if( requirement is Class )
				{
					var dataFactory:AbstractVirtualDataFactory = new requirement() as AbstractVirtualDataFactory;
					requirements.push( dataFactory );
				}
				else
				{
					requirements.push( requirement );
				}
			}
		}
		
		public function prepBuild():Array
		{
			buildOrder = discoverBuildOrder( 0 );
			buildList = getBuildRequirements( [ this ] );
			
			return buildList;
		}
		
		public function deriveData( dataIndex:DataIndex, importIndex:ImportDataIndex, system:SystemProxy ):void
		{
		}
	}
}