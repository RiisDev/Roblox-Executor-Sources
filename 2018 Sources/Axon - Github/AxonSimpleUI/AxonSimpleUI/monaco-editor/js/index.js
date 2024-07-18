
var Monacoeditor = monaco.editor.create(document.getElementById('container'), {
	value: [].join('\n'),
	language: 'lua',
	theme: 'vs-dark',
	smoothScrolling: true,
	scrollbar: {
		verticalHasArrows: true,
		horizontalHasArrows: true
	},
	scrollBeyondLastLine: false
});

function GetMonacoEditorText() {
	return Monacoeditor.getValue();
}

function SetMonacoEditorText(text) {
	Monacoeditor.setValue(text);
}

$(document).ready(function() {
	require(['vs/editor/editor.main'], function () {
		$(".theme-picker").change(function() {
			changeTheme(this.selectedIndex);
		});
	});
});

function changeTheme(theme) {
	var newTheme = (theme === 0 ? 'vs-dark' : ( theme === 1 ? 'vs' : 'hc-black' ));
	monaco.editor.setTheme(newTheme);
}
