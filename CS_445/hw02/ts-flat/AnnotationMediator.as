package com.ts.presentation.view.annotation
{	
	import com.ts.global.assets.ColorAssets;
	import com.ts.presentation.model.annotation.AnnotationProxy;
	import com.ts.presentation.model.annotation.ShapeProxy;
	import com.ts.presentation.model.annotation.ShapeType;
	import com.ts.presentation.view.PresentationMediator;
	import com.ts.presentation.view.annotation.controls.ShapeControl;
	
	import flash.events.Event;
	import flash.geom.Point;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.mediator.Mediator;
	import org.puremvc.as3.utilities.statemachine.State;
	import org.puremvc.as3.utilities.statemachine.StateMachine;
	
	public class AnnotationMediator extends Mediator
	{
		public static const NAME:String					= "AnnotationMediator";
		
		public static const NOTE_ENTER_ANNOTATION_MODE:String	= NAME + "/note/enterAnnotationMode";
		public static const NOTE_EXIT_ANNOTATION_MODE:String	= NAME + "/note/exitAnnotationMode";
		
		public static const STATE_ANNOTATING:String				= NAME + "/state/annotating";
		
		private var _annotationProxy:AnnotationProxy;
		
		public function get view():AnnotationView
		{
			return viewComponent as AnnotationView;
		}
		
		public function AnnotationMediator( viewComponent:Object=null )
		{
			super( NAME, viewComponent );
		}
		
		override public function onRegister():void
		{
			_annotationProxy = new AnnotationProxy();
			facade.registerProxy( _annotationProxy );
		}
		
		override public function onRemove():void
		{
		}
		
		private function addListeners():void
		{
			view.addEventListener( AnnotationView.EVENT_START_SHAPE, handleEvent );
			view.addEventListener( AnnotationView.EVENT_UPDATE_SHAPE, handleEvent );
			view.addEventListener( AnnotationView.EVENT_FINISH_UPDATE, handleEvent );
			view.addEventListener( AnnotationView.EVENT_MOVE_SHAPE, handleEvent );
			view.addEventListener( AnnotationView.EVENT_FINISH_MOVE, handleEvent );
			view.addEventListener( AnnotationView.EVENT_DELETE_SHAPE, handleEvent );
			view.addEventListener( AnnotationView.EVENT_DELETE_ALL_SHAPES, handleEvent );
		}
		
		private function removeListeners():void
		{
			view.removeEventListener( AnnotationView.EVENT_START_SHAPE, handleEvent );
			view.removeEventListener( AnnotationView.EVENT_UPDATE_SHAPE, handleEvent );
			view.removeEventListener( AnnotationView.EVENT_FINISH_UPDATE, handleEvent );
			view.removeEventListener( AnnotationView.EVENT_MOVE_SHAPE, handleEvent );
			view.removeEventListener( AnnotationView.EVENT_FINISH_MOVE, handleEvent );
			view.removeEventListener( AnnotationView.EVENT_DELETE_SHAPE, handleEvent );
			view.removeEventListener( AnnotationView.EVENT_DELETE_ALL_SHAPES, handleEvent );
		}
		
		override public function listNotificationInterests():Array
		{
			return[ StateMachine.CHANGED ]
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch( notification.getName() )
			{
				case StateMachine.CHANGED:
				{
					if( State( notification.getBody() ).name == STATE_ANNOTATING )
					{
						addListeners();
						view.setCurrentState( "annotating" );
					}
					else if( State( notification.getBody() ).name == PresentationMediator.STATE_PRESENTING )
					{
						removeListeners();
						view.setCurrentState( "static" );
					}
					else
					{
						if( view.currentState != "hidden" )
						{
							removeListeners();
						}
						view.setCurrentState( "hidden" );
					}
				}
				default:
					break;
			}
		}
		
		private function handleEvent( event:Event ):void
		{
			switch( event.type )
			{
				case AnnotationView.EVENT_START_SHAPE:
					var shape:ShapeProxy = _annotationProxy.createShape( view.currentStartPoint, view.currentShapeType );
					shape.color = ColorAssets.MAIN_EMPHASIS;
					shape.weight = 2;
					shape.alpha = .8;
					view.addNewShape( shape.shapeControl );
					break;
				
				case AnnotationView.EVENT_UPDATE_SHAPE:
					_annotationProxy.retrieveShape( view.currentShape.shapeID ).bottomRightP = new Point( view.mouseX, view.mouseY );
					break;
				
				case AnnotationView.EVENT_FINISH_UPDATE:
					_annotationProxy.retrieveShape( view.currentShape.shapeID ).bottomRightP = new Point( view.mouseX, view.mouseY );
					view.finishShape( view.currentShape );
					break;
				
				case AnnotationView.EVENT_MOVE_SHAPE:
					_annotationProxy.retrieveShape( view.currentShape.shapeID ).topLeftP = new Point( view.moveOrigin.x + view.mouseX - view.currentStartPoint.x, view.moveOrigin.y + view.mouseY - view.currentStartPoint.y );
					break;
				
				case AnnotationView.EVENT_FINISH_MOVE:
					_annotationProxy.retrieveShape( view.currentShape.shapeID ).topLeftP = new Point( view.moveOrigin.x + view.mouseX - view.currentStartPoint.x, view.moveOrigin.y + view.mouseY - view.currentStartPoint.y );
					view.finishMove( view.currentShape );
					break;
				
				case AnnotationView.EVENT_DELETE_SHAPE:
					_annotationProxy.removeShapeProxy( view.currentOverShape.shapeID );
					view.deleteShape( view.currentOverShape );
					break;
				
				case AnnotationView.EVENT_DELETE_ALL_SHAPES:
					_annotationProxy.removeAllShapes();
					view.deleteAllShapes();
					break;
					
				
				
				default:
					break;
			}
		}
	}
}