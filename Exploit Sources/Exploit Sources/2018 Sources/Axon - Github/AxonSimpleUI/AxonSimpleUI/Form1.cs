using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace AxonSimpleUI
{
    public partial class AxonSimpleUIForm : Form
    {
        public AxonSimpleUIForm()
        {
            InitializeComponent();
            MonacoEditor.Url = new Uri(string.Format("file:///{0}/monaco-editor/index.html", Directory.GetCurrentDirectory()));
            MonacoEditor.Document.BackColor = System.Drawing.SystemColors.ControlDark;
        }

        public const int WM_NCLBUTTONDOWN = 0xA1;
        public const int HT_CAPTION = 0x2;

        [DllImportAttribute("user32.dll")]
        public static extern int SendMessage(IntPtr hWnd,
                         int Msg, int wParam, int lParam);
        [DllImportAttribute("user32.dll")]
        public static extern bool ReleaseCapture();

        private void TitleDraggable_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                ReleaseCapture();
                SendMessage(Handle, WM_NCLBUTTONDOWN, HT_CAPTION, 0);
            }
        }

        private void CloseButton_Click(object sender, EventArgs e) => Environment.Exit(0);

        private void MinimizeButton_Click(object sender, EventArgs e) => WindowState = FormWindowState.Minimized;

        private void Inject_Click(object sender, EventArgs e) => Functions.Inject();

        private void ExecuteButton_Click(object sender, EventArgs e)
        {
            if (NamedPipes.NamedPipeExist(NamedPipes.luapipename))//check if the pipe exist
            {
                NamedPipes.LuaPipe(MonacoEditor.Document.InvokeScript("GetMonacoEditorText").ToString());//lua pipe function to send the script
            }
            else
            {
                MessageBox.Show($"Inject {Functions.exploitdllname} before Using this!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);//if the pipe can't be found a messagebox will appear
                return;
            }
        }

        private void ClearButton_Click(object sender, EventArgs e) => MonacoEditor.Document.InvokeScript("SetMonacoEditorText", new object[] { "" });//Clear the MonacoEditor

        private void OpenButton_Click(object sender, EventArgs e)
        {
            if (Functions.openfiledialog.ShowDialog() == DialogResult.OK)//check if the user clicked Ok/open
            {
                try
                {
                    MonacoEditor.Document.InvokeScript("SetMonacoEditorText", new object[]
                    {
                        File.ReadAllText(Functions.openfiledialog.FileName)
                    });//load all the text in the MonacoEditor

                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Error: Could not read file from disk. Original error: {ex.Message}");//display if got error
                }
            }
        }

        private void StatusTimer_Tick(object sender, EventArgs e)
        {
            if (NamedPipes.NamedPipeExist(NamedPipes.luapipename))
            {
                StatusLabel.Text = "Injected";
                StatusLabel.ForeColor = System.Drawing.Color.Green;
            }
            else
            {
                StatusLabel.Text = "Not Injected";
                StatusLabel.ForeColor = System.Drawing.Color.Red;
            }
        }
    }
}
