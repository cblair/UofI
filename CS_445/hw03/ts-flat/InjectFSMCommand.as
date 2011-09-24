package com.ts.core.controller
{
	import com.ts.core.view.IntroPaneMediator;
	import com.ts.importing.configuration.model.ImportConfigProxy;
	import com.ts.load.model.LoadProxy;
	import com.ts.presentation.model.annotation.AnnotationProxy;
	import com.ts.presentation.view.PresentationMediator;
	import com.ts.presentation.view.annotation.AnnotationMediator;
	import com.ts.save.model.SaveProxy;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	import org.puremvc.as3.utilities.statemachine.FSMInjector;
	
	/**
	 * Inject the Finite State Machine.
	 * <p>Though the XML could come from anywhere, even loaded remotely,
	 * we are simply creating it here in this command since it is 
	 * easy to refefence the StopWatch ACTION and STATE constants,
	 * so they will match up with the events acutally being sent from
	 * the stopwatch view.</p>
	 * 
	 * <p>
	 * Then we use the <code>FSMInjector</code> to create the configured 
	 * <code>StateMachine</code> from the XML FSM description and &apos;inject&apos; 
	 * it into the PureMVC apparatus.</p>
	 *
	 * <p> 
	 * The <code>StateMachine</code> is registered as an <code>IMediator</code>, 
	 * interested in <code>StateMachine.ACTION</code> Notifications. The 
	 * <code>type</code> parameter of those Notifications must be a valid 
	 * registered <code>State</code> in the FSM.</p>
	 **/
	public class InjectFSMCommand extends SimpleCommand
	{
		public function InjectFSMCommand()
		{
			super();
		}
		
		override public function execute(notification:INotification):void
		{
			facade.registerCommand( ApplicationFacade.NOTE_ENTER_STARTUP, StartupResourcesCommand );
			facade.removeCommand( ApplicationFacade.STARTUP );
			//-- Create the FSM XML Definition
			var fsm:XML =
			<fsm initial={ApplicationFacade.STATE_STARTING_UP}>
				<state name={ApplicationFacade.STATE_STARTING_UP} entering={ApplicationFacade.NOTE_ENTER_STARTUP}>
					<transition action={ApplicationFacade.ACTION_STARTUP_COMPLETED} target={IntroPaneMediator.STATE_MODULE_BROWSING}/>
				</state>
				<state name={IntroPaneMediator.STATE_MODULE_BROWSING} entering={IntroPaneMediator.NOTE_ENTER_BROWSE} exiting={IntroPaneMediator.NOTE_EXIT_BROWSE}>
					<transition action={LoadProxy.ACTION_LOAD} target={LoadProxy.STATE_LOADING}/>
					<transition action={ImportConfigProxy.ACTION_IMPORT} target={ImportConfigProxy.STATE_IMPORTING_MODULE}/>
				</state>
				<state name={LoadProxy.STATE_LOADING} entering={LoadProxy.NOTE_ENTER_LOAD} exiting={LoadProxy.NOTE_EXIT_LOAD}>
					<transition action={LoadProxy.ACTION_CANCEL_LOAD} target={IntroPaneMediator.STATE_MODULE_BROWSING}/>
					<transition action={ApplicationFacade.ACTION_APPLICATION_READY} target={PresentationMediator.STATE_PRESENTING}/>
				</state>
				<state name={SaveProxy.STATE_SAVING} entering={SaveProxy.NOTE_ENTER_SAVE} exiting={SaveProxy.NOTE_EXIT_SAVE}>
					<transition action={SaveProxy.ACTION_CANCEL_SAVE} target={PresentationMediator.STATE_PRESENTING}/>
					<transition action={SaveProxy.ACTION_SAVE_COMPLETE} target={PresentationMediator.STATE_PRESENTING}/>
				</state>
				<state name={ImportConfigProxy.STATE_IMPORTING_MODULE} entering={ImportConfigProxy.NOTE_ENTER_IMPORT_MODE} exiting={ImportConfigProxy.NOTE_EXIT_IMPORT_MODE}>
					<transition action={ImportConfigProxy.ACTION_IMPORT_CANCELLED} target={IntroPaneMediator.STATE_MODULE_BROWSING}/>
					<transition action={ApplicationFacade.ACTION_APPLICATION_READY} target={PresentationMediator.STATE_PRESENTING}/>
				</state>
				<state name={PresentationMediator.STATE_PRESENTING} entering={PresentationMediator.NOTE_ENTER_PRESENTATION_MODE} exiting={PresentationMediator.NOTE_EXIT_PRESENTATION_MODE}>
					<transition action={ImportConfigProxy.ACTION_IMPORT_CANCELLED} target={IntroPaneMediator.STATE_MODULE_BROWSING}/>
					<transition action={SaveProxy.ACTION_SAVE} target={SaveProxy.STATE_SAVING}/>
					<transition action={LoadProxy.ACTION_LOAD} target={LoadProxy.STATE_APP_READY_LOADING}/>
					<transition action={ImportConfigProxy.ACTION_IMPORT} target={ImportConfigProxy.STATE_APP_READY_IMPORTING}/>
					<transition action={AnnotationProxy.ACTION_ANNOTATE} target={AnnotationMediator.STATE_ANNOTATING}/>
				</state>
				<state name={AnnotationMediator.STATE_ANNOTATING} entering={AnnotationMediator.NOTE_ENTER_ANNOTATION_MODE} exiting={AnnotationMediator.NOTE_EXIT_ANNOTATION_MODE}>
					<transition action={PresentationMediator.ACTION_PRESENT} target={PresentationMediator.STATE_PRESENTING}/>
				</state>
				<state name={LoadProxy.STATE_APP_READY_LOADING} entering={LoadProxy.NOTE_ENTER_LOAD} exiting={LoadProxy.NOTE_EXIT_LOAD}>
					<transition action={LoadProxy.ACTION_CANCEL_LOAD} target={PresentationMediator.STATE_PRESENTING}/>
					<transition action={ApplicationFacade.ACTION_APPLICATION_READY} target={PresentationMediator.STATE_PRESENTING}/>
				</state>
				<state name={ImportConfigProxy.STATE_APP_READY_IMPORTING} entering={ImportConfigProxy.NOTE_ENTER_IMPORT_MODE} exiting={ImportConfigProxy.NOTE_EXIT_IMPORT_MODE}>
					<transition action={ImportConfigProxy.ACTION_IMPORT_CANCELLED} target={PresentationMediator.STATE_PRESENTING}/>
					<transition action={ApplicationFacade.ACTION_APPLICATION_READY} target={PresentationMediator.STATE_PRESENTING}/>
				</state>
			</fsm>;
			
			//-- Create and inject the stateMachine
			var injector:FSMInjector = new FSMInjector( fsm );
			injector.inject();
				
				
		}
	}
}