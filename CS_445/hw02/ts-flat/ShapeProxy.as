package com.ts.presentation.model.annotation
{
	import com.ts.presentation.view.annotation.controls.ShapeControl;
	
	import flash.geom.Point;
	
	import mx.graphics.SolidColorStroke;
	
	import spark.primitives.Path;

	public class ShapeProxy
	{	
		private var _pathData:String;
		public var shapeControl:ShapeControl;
		private var _shapeID:int;
		
		public function ShapeProxy( shapeID:int, topLeftPoint:Point = null, shape_type:String = null )
		{
			shapeControl = new ShapeControl( shapeID );
			topLeftP	= topLeftPoint ? topLeftPoint : new Point();
			shapeType 	= shape_type ? shape_type : ShapeType.LINE;
			_shapeID 	= shapeID;
			
		}
		
		private var _topLeftP:Point = new Point();
		public function get topLeftP():Point{ return _topLeftP; }
		public function set topLeftP( value:Point ):void
		{
			_topLeftP = value;
			shapeControl.x = _topLeftP.x;
			shapeControl.y = _topLeftP.y;
		}
		
		private var _bottomRightP:Point;
		public function get bottomRightP():Point{ return _bottomRightP; }
		public function set bottomRightP( value:Point ):void
		{
			_bottomRightP = value;
			refreshPathData();
		}
		
		private var _shapeType:String;
		private var _shapeFunction:Function;
		public function get shapeType():String{ return _shapeType; }
		public function set shapeType( value:String ):void
		{
			if( _shapeType == value ) return;
			_shapeType = value;
			_shapeFunction = ShapeType.getShapeFunction( value );
			
			if( _pathData )
			{
				refreshPathData();
			}
		}
		
		private var _path:Path;
		private var _stroke:SolidColorStroke;
		public function get path():Path{ return _path; }
		
		public function set color( value:uint ):void
		{
			shapeControl.color = value;
		}
		
		public function set alpha( value:Number ):void
		{
			shapeControl.alpha = value;
		}
		
		public function set weight( value:Number ):void
		{
			shapeControl.weight = value;
		}
		
		private function refreshPathData():void
		{
			var sData:String = _shapeFunction( _topLeftP, _bottomRightP );
			trace( sData );
			shapeControl.shapeData = sData;
			shapeControl.bottomRightPoint = shapeControl.parent.localToGlobal( _bottomRightP );
		}
	}
}