package com.ts.importing.virtualData
{
	public class DerivationCollection
	{
		public var deriverList:Array;
		public var importList:Array;
		public var deriverOrderMap:Object;
		
		public function DerivationCollection()
		{
			deriverList = new Array();
		}
		
		public function addDeriver( deriver:Class ):void
		{
			deriverList.push( new deriver() as AbstractVirtualDataFactory );
		}
		
		public function createDerivationTree():void
		{
			deriverOrderMap = new Object();
			var deriverReqs:Array = new Array();
			importList = new Array();
			
			for each( var deriverReq:AbstractVirtualDataFactory in deriverList )
			{
				deriverReq.prepBuild();
				deriverReqs = deriverReqs.concat( deriverReq.buildList );
			}
			
			for each( var item:* in deriverReqs )
			{
				if( item is String )
				{
					if( importList.indexOf( item ) == -1 )
					{
						importList.push( item );
					}
				}
				else
				{
					var buildOrder:int = AbstractVirtualDataFactory( item ).buildOrder;
					if( !deriverOrderMap[ buildOrder ] )
					{
						deriverOrderMap[ buildOrder ] = new Array();
					}
					
					if( deriverOrderMap[ buildOrder ].indexOf( item ) == -1 )
					{
						deriverOrderMap[ buildOrder ].push( item );
					}
				}
			}
		}
	}
}