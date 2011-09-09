package com.ts.presentation.model.annotation
{
	import com.ts.presentation.view.annotation.controls.ShapeControl;
	
	import flash.geom.Point;
	
	import org.puremvc.as3.patterns.proxy.Proxy;
	
	import spark.components.ToggleButton;
	import spark.primitives.Ellipse;
	
	public class AnnotationProxy extends Proxy
	{
		public static const NAME:String = "annotationProxy";
	
		public static const NOTE_ANNOTATE:String	= NAME + "/note/annotate";
		
		public static const NOTE_HIGHLIGHT_SHAPE:String			= NAME + "/note/highlightShape";
		public static const NOTE_UNHIGHLIGHT_SHAPE:String		= NAME + "/note/unhighlightShape";
		public static const NOTE_UPDATE_SHAPE_SELECTION:String	= NAME + "/note/updateShapeSelection";
		
		public static const ACTION_ANNOTATE:String	= NAME + "/action/annotate";
		
		private var _shapeMap:Object;
		private var _shapeUIDs:Array;
		private var _selectedShapes:Array;
		private var _highlightedShape:int;
		
		private var _currentShapeUID:int;
		
		public function AnnotationProxy()
		{
			super( NAME );
			
			_shapeMap 			= new Object();
			_shapeUIDs			= new Array();
			_selectedShapes 	= new Array();
			_currentShapeUID	= 0;
		}
		
		public function createShape( topLeftPoint:Point = null, shapeType:String = null ):ShapeProxy
		{
			var shapeProxy:ShapeProxy = new ShapeProxy( _currentShapeUID, topLeftPoint, shapeType );
			registerShapeProxy( _currentShapeUID, shapeProxy );
			_currentShapeUID++;
			return shapeProxy;
		}
		
		public function retrieveShape( UID:int ):ShapeProxy
		{
			return _shapeMap[ UID ];
		}
		
		public function isTheOnlySelectedShape( UID:int ):Boolean
		{
			return _selectedShapes.indexOf( UID ) != -1 && _selectedShapes.length == 1;
		}
		
		public function registerShapeProxy( UID:int, proxy:ShapeProxy ):void
		{
			if( _shapeUIDs.indexOf( UID ) == -1 )
			{
				_shapeMap[ UID ] = proxy;
				_shapeUIDs.push( UID );
			}
		}
		
		public function removeShapeProxy( UID:int ):void
		{
			var index:int = _selectedShapes.indexOf( UID );
			if( index == -1 )
			{
				if( UID == _highlightedShape )
				{
					unhighlightShapeProxy();
				}
				
				removeShapeSelection( UID );
				
				_shapeUIDs.splice( index, 1 );
				
				delete( _shapeUIDs[ UID ] );
			}
		}
		
		public function removeAllShapes():void
		{
			unhighlightShapeProxy();
			unselectShapeProxys();
			_shapeUIDs = new Array();
			_shapeMap = new Object;
			_currentShapeUID = 0;
		}
		
		public function selectShapeProxy( UID:int ):void
		{
			if( _shapeUIDs.indexOf( UID ) != -1 )
			{
				for each( var SelectedUID:int in _selectedShapes )
				{
					if( UID != SelectedUID )
						_shapeMap[ SelectedUID ].selected = false;
				}
				_selectedShapes = [ UID ];
				_shapeMap[ UID ].selected = true;
				updateShapes();
			}
		}
		
		public function addShapeSelection( UID:int ):void
		{
			var index:int = _selectedShapes.indexOf( UID );
			if( index == -1 )
			{
				_selectedShapes.push( UID );
				_shapeMap[ UID ].selected = true;
				updateShapes();
			}
		}
		
		public function removeShapeSelection( UID:int ):void
		{
			var index:int = _selectedShapes.indexOf( UID );
			if( index != -1 )
			{
				_selectedShapes.splice( index, 1 );
				_shapeMap[ UID ].selected = false;
				updateShapes();
			}
		}
		
		public function unselectShapeProxys():void
		{
			for each( var SelectedUID:int in _selectedShapes )
			{
				_shapeMap[ SelectedUID ].selected = false;
			}
			_selectedShapes = [];
			updateShapes();
		}
		
		public function highlightShapeProxy( UID:int ):void
		{
			if( _shapeUIDs.indexOf( UID ) != -1 )
			{
				_highlightedShape = UID;
				sendNotification( NOTE_HIGHLIGHT_SHAPE, _highlightedShape, NAME );
			}
		}
		
		public function unhighlightShapeProxy():void
		{
			if( _highlightedShape != -1 )
			{
				sendNotification( NOTE_UNHIGHLIGHT_SHAPE, _highlightedShape, NAME );
				_highlightedShape = -1;
			}
		}
		
		private function updateShapes():void
		{
			sendNotification( NOTE_UPDATE_SHAPE_SELECTION, _selectedShapes, NAME );
		}
	}
}