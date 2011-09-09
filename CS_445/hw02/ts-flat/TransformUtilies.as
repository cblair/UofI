package com.ts.utilities.drawing 
{
	import flash.geom.Matrix;
	import flash.geom.Point;
	import flash.geom.Transform;
	
	import spark.components.Group;
	import spark.primitives.supportClasses.GraphicElement;

	public class TransformUtilies
	{
		public function TransformUtilies( )
		{
		}
		
		public static function rotateSprite( rect:Group, rotation:Number, point:Point ):Group
		{
			rotation = rotation * ( Math.PI / 180 );
			
			var rotationMatrix:Matrix = rect.transform.matrix;

			rotationMatrix.translate( -point.x, -point.y );
			rotationMatrix.rotate( rotation );
			rotationMatrix.translate( point.x, point.y );
			
			rect.transform.matrix = rotationMatrix;
			
			return rect;
		}
		
		public static function zoomSprite( rect:Group, zoom:Number, point:Point ):Group
		{
			var rotationMatrix:Matrix = rect.transform.matrix;

			rotationMatrix.translate( -point.x, -point.y );
			rotationMatrix.scale( zoom, zoom );
			rotationMatrix.translate( point.x, point.y );
			
			rect.transform.matrix = rotationMatrix;
			
			return rect;
		}
		
		public static function translateSprite( rect:Group, translateX:Number, translateY:Number ):Group
		{
			var translationMatrix:Matrix = rect.transform.matrix;
			
			translationMatrix.translate( translateX, translateY );
			
			rect.transform.matrix = translationMatrix;
			
			return rect;
		}
	}
}