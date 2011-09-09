package com.ts.presentation.view
{
	import com.ts.aspect.model.graph.hGraph.HGraphProxy;
	import com.ts.presentation.model.PlaybackProxy;
	import com.ts.presentation.model.grid.ApplicationGridProxy;
	import com.ts.presentation.model.grid.GridProxy;
	import com.ts.presentation.model.map.MapProxy;
	import com.ts.presentation.model.map.SystemProxy;
	import com.ts.presentation.model.map.phase.PhaseProxy;
	import com.ts.presentation.view.annotation.AnnotationMediator;
	import com.ts.presentation.view.graphs.HistoryGraphMediator;
	import com.ts.presentation.view.map.MapMeidator;
	import com.ts.presentation.view.notification.NotificationMediator;
	import com.ts.presentation.view.perdue.PerdueMediator;
	import com.ts.presentation.view.timeSpace.TimeSpaceMediator;
	import com.ts.save.model.SaveProxy;
	
	import flash.events.Event;
	
	import mx.binding.utils.BindingUtils;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.mediator.Mediator;
	import org.puremvc.as3.utilities.statemachine.State;
	import org.puremvc.as3.utilities.statemachine.StateMachine;
	
	public class PresentationMediator extends Mediator
	{
		public static const NAME:String					= "PresentationMediator";
		
		public static const STATE_PRESENTING:String		= NAME + "/state/presenting";
		
		public static const NOTE_PRESENT:String			= NAME + "/note/present";
		public static const ACTION_PRESENT:String		= NAME + "/action/present";
		
		public static const NOTE_ENTER_PRESENTATION_MODE:String	= NAME + "/state/enterPresentationMode";
		public static const NOTE_EXIT_PRESENTATION_MODE:String	= NAME + "/state/exitPresentationMode";
		
		private var _playbackProxy:PlaybackProxy;
		private var _appGridProxy:ApplicationGridProxy;
		private var _systemProxy:SystemProxy;
		
		public function get view():PresentationView
		{
			return viewComponent as PresentationView;
		}
		
		public function PresentationMediator( viewComponent:Object=null )
		{
			super( NAME, viewComponent );
		}
		
		override public function onRegister():void
		{
			facade.registerMediator( new PlaybackMediator( view.playback ) );
			_playbackProxy 	= facade.retrieveProxy( PlaybackProxy.NAME ) as PlaybackProxy;
			_appGridProxy = facade.retrieveProxy( ApplicationGridProxy.NAME ) as ApplicationGridProxy;
			_systemProxy = facade.retrieveProxy( SystemProxy.NAME ) as SystemProxy;
			
			facade.registerMediator( new AnnotationMediator( view.annotationPane ) );
			facade.registerMediator( new NotificationMediator( view.notifications, _playbackProxy ) );
			facade.registerMediator( new TimeSpaceMediator( view.timeSpace, _playbackProxy ) );
			facade.registerMediator( new PerdueMediator( view.perdue, _playbackProxy ) );
			facade.registerMediator( new HistoryGraphMediator( view.timeTrends, _playbackProxy ) );
			facade.registerMediator( new MapMeidator( view.system, _systemProxy ) );
			
			view.notificationPane.addEventListener( "historySpanChanged", handleEvent );
			
			view.addEventListener( PresentationView.EVENT_REFRESH_NOTIFICATION, handleEvent );
			view.addEventListener( PresentationView.EVENT_REFRESH_TRENDS, 		handleEvent );
			view.addEventListener( PresentationView.EVENT_REFRESH_PERDUE, 		handleEvent );
			view.addEventListener( PresentationView.EVENT_REFRESH_TIME_SPACE, 	handleEvent );
		}
		
		override public function onRemove():void
		{
			
		}
		
		override public function listNotificationInterests():Array
		{
			return[ StateMachine.CHANGED, MapProxy.NOTE_UPDATE_PHASE_SELECTION ]
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch( notification.getName() )
			{
				case StateMachine.CHANGED:
				{
					if( State( notification.getBody() ).name == STATE_PRESENTING ||
						State( notification.getBody() ).name == SaveProxy.STATE_SAVING )
					{
						view.setCurrentState( "presenting" );
						//var graph:HGraphProxy = facade.retrieveProxy( HGraphProxy.NAME + "vehVelocity" ) as HGraphProxy;
					}
					else if( State( notification.getBody() ).name == AnnotationMediator.STATE_ANNOTATING )
					{
						view.setCurrentState( "annotating" );
					}
					else
					{
						view.setCurrentState( "hidden" );
					}
					break;
				}
				case MapProxy.NOTE_UPDATE_PHASE_SELECTION:
				{
					var phaseData:Array = notification.getBody() as Array;
					if( phaseData.length > 0 )
					{
						var phase:PhaseProxy = _systemProxy.networkModel.getPhaseAtUID( phaseData[ 0 ] );
						view.phaseLabel.text = "Phase " + String( phase.signalNumber );
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
				case "historySpanChanged":
					//_gridProxy.historySpan = view.notificationPane.historySpanBtn.x;
					break;
				
				case PresentationView.EVENT_REFRESH_NOTIFICATION:
					view.notifications.refreshMeasure( view.notificationPane.width, view.notificationSpan );
					break;
				
				case PresentationView.EVENT_REFRESH_PERDUE:
					view.perdue.refreshMeasure( view.rightTrendSpan, view.perdueSpan );
					break;
				
				case PresentationView.EVENT_REFRESH_TIME_SPACE:
					view.timeSpace.refreshMeasure( view.rightTrendSpan, view.timeSpaceSpan );
					break;
				
				case PresentationView.EVENT_REFRESH_TRENDS:
					view.timeTrends.refreshMeasure( view.leftTrendSpan, view.timeTrendSpan );
					break;
				
				default:
					break;
			}
		}
	}
}