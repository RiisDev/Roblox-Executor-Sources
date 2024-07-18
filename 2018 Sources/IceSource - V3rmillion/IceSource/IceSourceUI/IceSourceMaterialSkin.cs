using MaterialSkin;
using MaterialSkin.Controls;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace IceSourceUI
{
    public partial class IceSourceMaterialSkin : MaterialForm
    {
        private readonly MaterialSkinManager materialSkinManager;
        public IceSourceMaterialSkin()
        {
            InitializeComponent();
            TopMost = true;
            materialSkinManager = MaterialSkinManager.Instance;
            materialSkinManager.AddFormToManage(this);
            materialSkinManager.Theme = MaterialSkinManager.Themes.LIGHT;
            materialSkinManager.ColorScheme = new ColorScheme(Primary.BlueGrey800, Primary.BlueGrey900, Primary.BlueGrey500, Accent.LightBlue200, TextShade.WHITE);
        }

        private void InjectBTN_Click(object sender, EventArgs e) => Functions.Inject();

        private void IceSourceMaterialSkin_FormClosed(object sender, FormClosedEventArgs e) => Application.Exit();

        private void SendBTN_Click(object sender, EventArgs e)
        {
            if (CommandTextBox.Text.ToLower() == "cmds")//check if the user send cmds so we can display the commands
            {
                CommandBox.AppendText(Functions.TextToBox[0]);//Append text to the command richtextbox
                CommandTextBox.Clear();//clear the command textbox
            }
            else if (CommandTextBox.Text.ToLower() == "credits")//check if the user send credits so we can display the credits
            {
                CommandBox.AppendText(Functions.TextToBox[1]);//Append text to the command richtextbox
                CommandTextBox.Clear();//clear the command textbox
            }
            else if (CommandTextBox.Text.ToLower() == "clear")
            {
                CommandBox.Clear();
                CommandTextBox.Clear();
            }
            else
            {
                NamedPipes.CommandPipe(CommandTextBox.Text);//command pipe function to send the text in the command textbox
                CommandTextBox.Clear();//clear the command textbox
            }
        }

        private void CommandBox_TextChanged(object sender, EventArgs e)
        {
            CommandBox.SelectionStart = CommandBox.Text.Length;//this get all the text
            CommandBox.ScrollToCaret();//so with this we can scroll to the bottom
        }

        private void CommandTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)//if user pressed Key Enter
            {
                SendBTN_Click(sender, e);//execute Send_click function
            }
        }

        private void TopCheck_CheckedChanged(object sender, EventArgs e)
        {
            if (TopCheck.Checked)
            {
                TopMost = true;
            }
            else
            {
                TopMost = false;
            }
        }

        private void ThemeBTN_Click(object sender, EventArgs e)
        {
            Color Black = Color.FromName("Black");
            Color White = Color.FromName("White");
            materialSkinManager.Theme = materialSkinManager.Theme == MaterialSkinManager.Themes.DARK ? MaterialSkinManager.Themes.LIGHT : MaterialSkinManager.Themes.DARK;
            switch (materialSkinManager.Theme)
            {
                case MaterialSkinManager.Themes.DARK:
                    Text = "Ice Source Dark Theme";
                CommandBox.BackColor = Black;
                CommandBox.ForeColor = White;
                CommandTextBox.BackColor = Black;
                CommandTextBox.ForeColor = White;
                WSValue.BackColor = Black;
                WSValue.ForeColor = White;
                JPValue.BackColor = Black;
                JPValue.ForeColor = White;
                ScriptBox.BackColor = Black;
                ScriptBox.ForeColor = White;
                changeUIToolStripMenuItem.BackColor = Black;
                changeUIToolStripMenuItem.ForeColor = White;
                menuStrip1.BackColor = Black;
                menuStrip1.ForeColor = White;
                defaultToolStripMenuItem.BackColor = Black;
                defaultToolStripMenuItem.ForeColor = White;
                metroModernUIToolStripMenuItem.BackColor = Black;
                metroModernUIToolStripMenuItem.ForeColor = White;
                    break;
                case MaterialSkinManager.Themes.LIGHT:
                    Text = "Ice Source Light Theme";
                CommandBox.BackColor = White;
                CommandBox.ForeColor = Black;
                CommandTextBox.BackColor = White;
                CommandTextBox.ForeColor = Black;
                WSValue.BackColor = White;
                WSValue.ForeColor = Black;
                JPValue.BackColor = White;
                JPValue.ForeColor = Black;
                ScriptBox.BackColor = White;
                ScriptBox.ForeColor = Black;
                changeUIToolStripMenuItem.BackColor = White;
                changeUIToolStripMenuItem.ForeColor = Black;
                defaultToolStripMenuItem.BackColor = White;
                defaultToolStripMenuItem.ForeColor = Black;
                menuStrip1.BackColor = White;
                menuStrip1.ForeColor = Black;
                metroModernUIToolStripMenuItem.BackColor = White;
                metroModernUIToolStripMenuItem.ForeColor = Black;
                    break;
            }
        }

        private void defaultToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Hide();
            Start.DefaultForm();
        }

        private void BtoolsBTN_Click(object sender, EventArgs e) => NamedPipes.CommandPipe("btools me");

        private void FFBTN_Click(object sender, EventArgs e) => NamedPipes.CommandPipe("ff me");

        private void SuicideBTN_Click(object sender, EventArgs e) => NamedPipes.CommandPipe("kill me");

        private void SitBTN_Click(object sender, EventArgs e) => NamedPipes.CommandPipe("sit me");

        private void SetWSBTN_Click(object sender, EventArgs e) => NamedPipes.CommandPipe("ws me " + WSValue.Value);

        private void SetJPBTN_Click(object sender, EventArgs e) => NamedPipes.CommandPipe("jp me " + JPValue.Value);

        private void OpenBTN_Click(object sender, EventArgs e)
        {
            if (Functions.openfiledialog.ShowDialog() == DialogResult.OK)//check if the user clicked Ok/open
            {
                try
                {
                    ScriptBox.Text = File.ReadAllText(Functions.openfiledialog.FileName);//load all the text in the lua c rightextbox

                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error: Could not read file from disk. Original error: " + ex.Message);//display if got error
                }
            }
        }

        private void ClearBTN_Click(object sender, EventArgs e) => ScriptBox.Clear();

        private void ExecuteBTN_Click(object sender, EventArgs e)
        {
            if (NamedPipes.NamedPipeExist(NamedPipes.scriptpipe))//check if the pipe exist
            {
                string[] array = ScriptBox.Text.Split("\r\n".ToCharArray());//array to store all and split the script
                for (int i = 0; i < array.Length; i++)//for loop to send all the lines
                {
                    string script = array[i];
                    try
                    {
                        NamedPipes.LuaCPipe(script);//lua c pipe function to send the array
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message.ToString());//if there any error a messagebox will pop up with the error
                    }
                }
            }
            else
            {
                MessageBox.Show("Inject " + Functions.exploitdll + " before Using this!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);//if the pipe can't be found a messagebox will appear
                return;
            }
        }

        private void metroModernUIToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Hide();
            Start.MetroModernUIForm();
        }
    }
}
