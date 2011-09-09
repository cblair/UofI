package com.ts.importing.aspectClasses
{
	import com.ts.aspect.model.AspectProxy;
	import com.ts.data.model.DataIndex;
	import com.ts.importing.adornerClasses.AbstractAdorner;
	import com.ts.importing.deriveSystem.model.ImportDataIndex;
	import com.ts.importing.virtualData.AbstractVirtualDataFactory;
	import com.ts.importing.virtualData.DerivationCollection;
	import com.ts.presentation.model.map.SystemProxy;

	public class AbstractAspectClass
	{
		protected var derivationCollection:DerivationCollection;
		protected var adorner:AbstractAdorner;
		private var _aspectName:String;
		
		public function get aspectName():String{ return _aspectName; }
		
		public function AbstractAspectClass( Derivers:DerivationCollection, Adorner:AbstractAdorner, AspectName:String )
		{
			derivationCollection 	= Derivers;
			adorner 				= Adorner;
			_aspectName 			= AspectName;
			derivationCollection.createDerivationTree();
		}
		
		public function get derivationRequirements():Array
		{
			return derivationCollection.importList;
		}
		
		public function get derivationMap():Object
		{
			return derivationCollection.deriverOrderMap;
		}
		
		public function deriveData( importIndex:ImportDataIndex, dataIndex:DataIndex, system:SystemProxy ):DataIndex
		{
			var i:int = 0;
			while( derivationCollection.deriverOrderMap[ i ] )
			{
				for each( var deriver:AbstractVirtualDataFactory in derivationCollection.deriverOrderMap[ i ] )
				{
					deriver.deriveData( dataIndex, importIndex, system );
				}
				i++;
			}
			return dataIndex;
		}
		
		public function buildAspectModel( dataIndex:DataIndex, system:SystemProxy ):AspectProxy
		{
			if( adorner )
			{
				return adorner.buildAdornerModel( dataIndex, system );
			}
			else
			{
				return null;
			}
		}
	}
}