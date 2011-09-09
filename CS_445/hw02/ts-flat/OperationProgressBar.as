package com.ts.global.view.components
{
	import com.ts.importing.configuration.model.ExternalImportProxy;
	import com.ts.importing.configuration.view.ImportProgressBarSkin;
	
	import spark.components.Group;
	import spark.components.Label;
	import spark.components.supportClasses.SkinnableComponent;

	[ SkinState( "inactive" ) ]
	[ SkinState( "active" ) ]
	[ SkinState( "complete" ) ]
	[ SkinState( "failed" ) ]
	
	public class OperationProgressBar extends SkinnableComponent
	{		
		[ SkinPart( required="true" ) ]
		public var progressBar:Group;
		
		[ SkinPart( required="true" ) ]
		public var label:Label;
		
		private var _componentState:String = "inactive";
		private var _progress:Number = 0;
		private var _labelText:String = "";
		[Bindable] public var statusText:String = "";
		
		public function OperationProgressBar()
		{
			super();
			this.setStyle( "skinClass", com.ts.importing.configuration.view.ImportProgressBarSkin );
		}
		
		public function set progress( value:Number ):void
		{
			if( _componentState != "inactive" )
			{
				_progress = value;
				validateNow();
			}
		}
		
		public function set isValid( value:Boolean ):void
		{
			if( value )
			{
				active();
			}
			else
			{
				failed();
			}
		}
		
		[Bindable]
		public function get progress():Number
		{
			return _progress;
		}
		
		public function set labelText( value:String ):void
		{
			_labelText = value;
		}
		
		[Bindable]
		public function get labelText():String
		{
			return _labelText;
		}
		
		public function inactive():void
		{
			setCurrentSkinState( "inactive" );
			progress = 0;
		}
		
		public function active():void
		{
			setCurrentSkinState( "active" );
		}
		
		public function complete():void
		{
			setCurrentSkinState( "complete" );
		}
		
		public function failed():void
		{
			setCurrentSkinState( "failed" );
		}
		
		override protected function partAdded( partName:String, instance:Object ):void
		{
			super.partAdded( partName, instance );
		}
		
		override protected function partRemoved( partName:String, instance:Object ):void
		{
			super.partRemoved( partName, instance );
		}
		
		override protected function getCurrentSkinState():String
		{
			return _componentState;
		}
		
		public function setCurrentSkinState( value:String ):void
		{
			_componentState = value;
			invalidateSkinState();
		}
	}
}