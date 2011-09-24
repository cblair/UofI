package com.ts.presentation.model.map
{
	import com.ts.data.model.DataProxy;
	
	import org.puremvc.as3.patterns.proxy.Proxy;
	
	public class IterationProxy extends Proxy
	{
		public static const NAME:String				= "iterationProxy";
		
		private var _currentIndex:int 	= 0;
		private var _maxIndex:int 		= 1;
		private var _minIndex:int 		= 0;
		private var _changeTable:Array;
		private var _cachedChangeGroups:Object;
		private var _lodTable:Object;
		private var _changeGroupTable:Array;
		private var _changeGroups:Object;
		private var _registeredProxies:Object;
		
		public function IterationProxy( maxIndex:int, minIndex:int )
		{
			super( NAME );
			_maxIndex 			= maxIndex;
			_minIndex 			= minIndex;
			_changeTable 		= new Array();
			_changeGroupTable	= new Array();
			_changeGroups		= new Object();
			_registeredProxies	= new Object();
			_currentIndex		= 0;
			
			for( var i:int = 0; i < _maxIndex; i++ )
			{
				_changeTable.push( new Object() );
				_changeGroupTable.push( new Object () );
			}
		}
		
		public function stepForward():void
		{
			currentIndex += 1;
		}
		
		public function stepBackward():void
		{
			currentIndex -= 1;
		}
		
		[Bindable]
		public function get currentIndex():int{ return _currentIndex; }
		public function set currentIndex( value:int ):void
		{
			value = Math.max( _minIndex, Math.min( _maxIndex, value - 1 ) );
			if( value == _currentIndex + 1 )
			{
				doOptimizedValueUpdate( value );
			}
			else if( value == _currentIndex - 1 )
			{
				doOptimizedValueUpdate( value );
			}
			else
			{
				if( _cachedChangeGroups )
				{
					//doFullValueUpdate( value );
					updateCachedGroups( value );
				}
				else
				{
					doFullValueUpdate( value );
				}
			}
			_currentIndex = value;
		}
		
		private function doOptimizedValueUpdate( index:int ):void
		{
			_cachedChangeGroups = _changeGroupTable[ index ];
			var changeList:Object = _changeTable[ index ];
			for each( var dataProxy:DataProxy in changeList )
			{
				dataProxy.setChangeValueTo( index );
			}
			trace( "fast update at index " + index );
		}
		
		private function updateCachedGroups( index:int ):void
		{
			//-- update entire change group for last iteration
			var changeGroup:Object;
			for each( var groupID:String in _cachedChangeGroups )
			{
				trace( "LastGroup = " + groupID );
				changeGroup = _changeGroups[ groupID ]
				for each( var dataProxy:DataProxy in changeGroup )
				{
					dataProxy.setValueTo( index );
				}
			}
			
			//-- update entire change group for this iteration, & update groupCache.
			_cachedChangeGroups = _changeGroupTable[ index ];
			for each( groupID in _cachedChangeGroups )
			{
				trace( "NewGroup = " + groupID );
				changeGroup = _changeGroups[ groupID ]
				for each( dataProxy in changeGroup )
				{
					dataProxy.setValueTo( index );
				}
			}

			trace( "update last change list at index " + index );
		}
		
		private function doFullValueUpdate( index:int ):void
		{
			_cachedChangeGroups = _changeGroupTable[ index ];
			for each( var dataProxy:DataProxy in _registeredProxies )
			{
				dataProxy.setValueTo( index );
			}
			trace( "full Update at index " + index );
		}
		
		public function registerForIteration( dataProxy:DataProxy ):void
		{
			_registeredProxies[ dataProxy.uniqueID ] = dataProxy;
			addProxyToIterationTable( dataProxy, dataProxy.uniqueID );	
		}
		
		public function unregisterForIteration( dataProxy:DataProxy ):void
		{
			var index:int = _registeredProxies.indexOf( dataProxy )
			if( index != -1 )
			{
				_registeredProxies.splice( index, 1 );
				removeProxyFromIterationTable( dataProxy, dataProxy.uniqueID );
			}
			else
			{
				trace( "tried to unregister a missing data proxy: IterationProxy/unregisterForIteration" );
			}
		}
		
		private function addProxyToIterationTable( dataProxy:DataProxy, uniqueID:String ):void
		{
			var changeList:Array = dataProxy.dataVO.hashIndex;
			for each( var changeIndex:int in changeList )
			{
				_changeTable[ changeIndex ][ uniqueID ] = dataProxy;
				_changeGroupTable[ changeIndex ][ dataProxy.groupID ] = dataProxy.groupID;
			}
			
			if( _changeGroups[ dataProxy.groupID ] == null )
			{
				_changeGroups[ dataProxy.groupID ] = new Object();
			}
			_changeGroups[ dataProxy.groupID ][ uniqueID ] = dataProxy;
		}
		
		private function removeProxyFromIterationTable( dataProxy:DataProxy, uniqueID:String ):void
		{
			var changeList:Array = dataProxy.dataVO.hashIndex;
			
			delete _changeGroups[ dataProxy.groupID ][ uniqueID ];
			
			var len:int = 0;
			for each( var proxy:DataProxy in _changeGroups[ dataProxy.groupID ] )
			{
				len++;
			}
			
			var removeGroup:Boolean = false;
			if( len == 0 )
			{
				delete _changeGroups[ dataProxy.groupID ];
				removeGroup = true;
			}
			
			for each( var changeIndex:int in changeList )
			{
				delete _changeTable[ changeIndex ][ uniqueID ];
				if( removeGroup )
				{
					delete _changeGroupTable[ changeIndex ][ dataProxy.groupID ];
				}
			}
		}
	}
}