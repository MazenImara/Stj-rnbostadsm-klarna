CKEDITOR.on( 'dialogDefinition', function( ev )
{
	// Take the dialog name and its definition from the event
	// data.
	var dialogName = ev.data.name;
	var dialogDefinition = ev.data.definition;
	if ( dialogName == 'image' ) { //Tilldelar bilden en klass beroende på alignment
		var infoTab = dialogDefinition.getContents( 'info' );
		//Tar bort lite skit som ändå inte funkar pga filter
		infoTab.remove('txtBorder');
		infoTab.remove('txtHSpace');
    infoTab.remove('txtVSpace');
    //-------//

		var alignField = infoTab.get( 'cmbAlign' );
		alignField.onChange = function()
		{
			var dialog = this.getDialog();
			var cssField = dialog.getContentElement( 'advanced', 'txtGenClass' );
			cssField.setValue(this.getValue());
		};
	}
	else if ( dialogName == 'link' ) { //Tilldelar länken en klass beroende på filändelse
		var infoTab = dialogDefinition.getContents( 'info' );
		var protocolField = infoTab.get('protocol');
		protocolField.onChange = function() {
			if(!this.getValue()) { //Valt <annan>
				var dialog = this.getDialog();
				var urlField = dialog.getContentElement('info', 'url');
				var extension = urlField.getValue().split('.').pop();
				var valid_extensions = new Array("zip", "gzip", "rar", "txt", "doc", "docx", "odt", "xls", "xlsx", "ods", "ppt", "pptx", "odp", "pdf");
				if(valid_extensions.indexOf(extension) != -1) { //Hittat en valid filändelse
					var cssField = dialog.getContentElement( 'advanced', 'advCSSClasses' );
					cssField.setValue('filelink filelink-'+extension);
				}
			}
		};
		var urlField = infoTab.get('url');
		urlField.onChange = function() {
			if(this.getValue().substr(0, 6) === 'sites/') {
				var dialog = this.getDialog();
				var protocolField = dialog.getContentElement('info', 'protocol');
				protocolField.setValue('');
			}
		};
	}
});
CKEDITOR.on('instanceReady', function( ev ) { //Snyggar till HTML så att den blir mer lättläst
  var blockTags = ['div','h1','h2','h3','h4','h5','h6','p','pre','li','blockquote','ul','ol',
  'table','thead','tbody','tfoot','tr', 'td','th',];

  for (var i = 0; i < blockTags.length; i++)
  {
     ev.editor.dataProcessor.writer.setRules( blockTags[i], {
        indent : false,
        breakBeforeOpen : true,
        breakAfterOpen : false,
        breakBeforeClose : false,
        breakAfterClose : true
     });
  }
});