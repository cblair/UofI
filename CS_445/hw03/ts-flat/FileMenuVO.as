package com.ts.core.model.vo
{
	import com.ts.importing.configuration.model.ImportConfigProxy;
	import com.ts.load.model.LoadProxy;
	import com.ts.save.model.SaveProxy;

	public class FileMenuVO extends MenuBtnCollectionVO
	{
		public function FileMenuVO()
		{
			super();
			addMenuBtn( "Save", SaveProxy.NOTE_SAVE );
			addMenuBtn( "Open", LoadProxy.NOTE_LOAD );
			addMenuBtn( "Import", ImportConfigProxy.NOTE_IMPORT );
		}
	}
}