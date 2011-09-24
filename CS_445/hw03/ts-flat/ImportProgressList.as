package com.ts.importing.configuration.view
{
	
	import com.ts.global.view.components.OperationProgressBar;
	import com.ts.importing.configuration.model.ExternalImportProxy;
	
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import mx.binding.utils.BindingUtils;
	
	import spark.components.HGroup;
	import spark.components.VGroup;
	import spark.components.VScrollBar;
	
	public class ImportProgressList extends HGroup
	{
		private var _vGroup:VGroup;
		private var _pBars:Array;
		private var _vScroll:VScrollBar;
		private var _pBarMap:Object;
		
		public function ImportProgressList()
		{
		}
	
		private var _externalImportProxies:Array = new Array();
		private var _externalImportProxiesChanged:Boolean = false;
		
		public function set externalImportProxies( value:Array ):void
		{
			_externalImportProxies = value;
			_externalImportProxiesChanged = true;
			invalidateProperties();
		}
		
		public function set missingDataSets( value:Array ):void
		{
			if( value )
			{
				for each( var missingDataSet:ExternalImportProxy in value )
				{
					OperationProgressBar( _pBarMap[ missingDataSet.dataName ] as OperationProgressBar ).setCurrentSkinState( "dataMissing" );
				}
			}
		}
		
		public function deconstruct():void
		{
			if( _vGroup )
			{
				removeElement( _vGroup );
				_vGroup = null;
			}
		}
		
		public function construct():void
		{
			if( _vGroup )
			{
				deconstruct();
			}
			
			_vGroup = new VGroup();
			addElementAt( _vGroup, 0 );
			_vGroup.left = 0;
			_vGroup.percentWidth = 100;
			_vGroup.gap = -1;
			_vGroup.percentHeight = 100;
		}
		
		override protected function createChildren():void
		{
			super.createChildren();
			
			_vGroup = new VGroup();
			addElement( _vGroup );
			_vGroup.left = 0;
			_vGroup.right = 16;
			_vGroup.percentHeight = 100;
			_vGroup.gap = -1
			
			_vScroll = new VScrollBar();
			addElement( _vScroll );
			_vScroll.percentHeight = 100;
			
			invalidateProperties();
		}
		
		private function handleEvent( event:Event ):void
		{
		}
		
		override protected function commitProperties():void
		{
			super.commitProperties();
			if( _externalImportProxiesChanged )
			{
				construct();
				
				_pBars = new Array();
				_pBarMap = new Object();
				
				for each( var importProxy:ExternalImportProxy in _externalImportProxies )
				{
					var pBar:OperationProgressBar = new OperationProgressBar();
					_vGroup.addElement( pBar );
					pBar.percentWidth = 100;
					pBar.height = 20;
					pBar.labelText = importProxy.externalFileExtension + " > " + importProxy.longName;
					pBar.id = importProxy.dataName;
					_pBars.push( pBar );
					_pBarMap[ pBar.id ] = pBar;
					BindingUtils.bindProperty( pBar, "progress", importProxy, "importProgress" );
					BindingUtils.bindProperty( pBar, "statusText", importProxy, "statusMessage" );
					BindingUtils.bindProperty( pBar, "isValid", importProxy, "isValid" );
					//pBar.statusText = importProxy.statusMessage;
					//pBar.isValid = importProxy.isValid;
					pBar.toolTip = importProxy.toolTip + "\n\nVISSIM > "+ importProxy.externalFileLocation + " > " + importProxy.externalParamName;
					pBar.addEventListener( MouseEvent.ROLL_OUT, handleEvent );
					pBar.addEventListener( MouseEvent.ROLL_OVER, handleEvent );
				}
				
				_vScroll.viewport = _vGroup;
				
				_externalImportProxiesChanged = false;
			}
		}
	}
}