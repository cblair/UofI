package com.ts.presentation.model.annotation
{
	import com.ts.utilities.drawing.PathUtilities;

	public class ShapeType
	{
		public static const LINE:String			= "line";
		public static const ELLIPSE:String 		= "ellipse";
		public static const RECTANGLE:String	= "rectangle";
		public static const ARROW:String 		= "arrow";
		
		public static function getShapeFunction( shapeType:String ):Function
		{
			switch( shapeType )
			{
				case LINE:
					return PathUtilities.lineFromBounds;
				
				case ELLIPSE:
					return PathUtilities.ellipseFromBounds;
					
				case RECTANGLE:
					return PathUtilities.rectangleFromBounds;
					
				case ARROW:
					return PathUtilities.arrowFromBounds;
					
				default:
					return PathUtilities.lineFromBounds;
			}
		}
	}
}