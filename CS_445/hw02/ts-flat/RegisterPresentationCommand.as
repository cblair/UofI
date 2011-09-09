package com.ts.presentation.controller
{
	import com.ts.presentation.model.DataSelectionProxy;
	import com.ts.presentation.model.PlaybackProxy;
	import com.ts.presentation.model.annotation.AnnotationProxy;
	import com.ts.presentation.model.grid.ApplicationGridProxy;
	import com.ts.presentation.model.grid.GridProxy;
	import com.ts.presentation.model.map.MapProxy;
	import com.ts.presentation.view.PresentationMediator;
	import com.ts.presentation.view.PresentationView;
	import com.ts.presentation.model.map.SystemProxy;
	import com.ts.presentation.model.map.TrafficProxy;
	
	import mx.binding.utils.BindingUtils;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	
	public class RegisterPresentationCommand extends SimpleCommand
	{
		override public function execute(notification:INotification):void
		{
			if( !facade.hasMediator( PresentationMediator.NAME ) )
			{
				var presentationView:PresentationView = ApplicationMediator( facade.retrieveMediator( ApplicationMediator.NAME ) ).application.presentationPane;
							
				var systemProxy:SystemProxy = facade.retrieveProxy( SystemProxy.NAME ) as SystemProxy;
				var playbackProxy:PlaybackProxy = new PlaybackProxy( 0, systemProxy.maxIterations, systemProxy.timeResolution );
				var presentationMediator:PresentationMediator = new PresentationMediator( presentationView );
				
				facade.registerProxy( playbackProxy );
				facade.registerMediator( presentationMediator );
				facade.registerCommand( DataSelectionProxy.NOTE_REQUEST_DATA_CHANGE, ChangeDataCommand );
				facade.registerCommand( MapProxy.NOTE_UPDATE_PHASE_SELECTION, UpdatePhaseSelectionCommand );
				facade.registerCommand( AnnotationProxy.NOTE_ANNOTATE, AnnotateCommand );
				facade.registerCommand( PresentationMediator.NOTE_PRESENT, PresentCommand );
				
			}
		}
	}
}