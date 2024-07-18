using MetroFramework;
using MetroFramework.Forms;
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
    public partial class IceSourceMetro : MetroForm
    {
        public IceSourceMetro()
        {
            InitializeComponent();
            TopMost = true;
        }

        private void SendBtn_Click(object sender, EventArgs e)
        {
            if (CmdTextBox.Text.ToLower() == "cmds")//check if the user send cmds so we can display the commands
            {
                CmdBox.AppendText(Functions.TextToBox[0]);//Append text to the command richtextbox
                CmdTextBox.Clear();//clear the command textbox
            }
            else if (CmdTextBox.Text.ToLower() == "credits")//check if the user send credits so we can display the credits
            {
                CmdBox.AppendText(Functions.TextToBox[1]);//Append text to the command richtextbox
                CmdTextBox.Clear();//clear the command textbox
            }
            else if (CmdTextBox.Text.ToLower() == "clear")
            {
                CmdBox.Clear();
                CmdTextBox.Clear();
            }
            else
            {
                NamedPipes.CommandPipe(CmdTextBox.Text);//command pipe function to send the text in the command textbox
                CmdTextBox.Clear();//clear the command textbox
            }
        }

        private void CmdTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)//if user pressed Key Enter
            {
                SendBtn_Click(sender, e);//execute Send_click function
            }
        }

        private void CmdBox_TextChanged(object sender, EventArgs e)
        {
            CmdBox.SelectionStart = CmdBox.Text.Length;//this get all the text
            CmdBox.ScrollToCaret();//so with this we can scroll to the bottom
        }

        private void Btools_Click(object sender, EventArgs e) => NamedPipes.CommandPipe("btools me");

        private void FF_Click(object sender, EventArgs e) => NamedPipes.CommandPipe("ff me");

        private void Suicide_Click(object sender, EventArgs e) => NamedPipes.CommandPipe("kill me");

        private void Sit_Click(object sender, EventArgs e) => NamedPipes.CommandPipe("sit me");

        private void SetWS_Click(object sender, EventArgs e) => NamedPipes.CommandPipe("ws me " + WSVal.Value);

        private void SetJP_Click(object sender, EventArgs e) => NamedPipes.CommandPipe("jp me " + JPVal.Value);

        private void Open_Click(object sender, EventArgs e)
        {
            if (Functions.openfiledialog.ShowDialog() == DialogResult.OK)//check if the user clicked Ok/open
            {
                try
                {
                    LuacBox.Text = File.ReadAllText(Functions.openfiledialog.FileName);//load all the text in the lua c rightextbox

                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error: Could not read file from disk. Original error: " + ex.Message);//display if got error
                }
            }
        }

        private void Execute_Click(object sender, EventArgs e)
        {
            if (NamedPipes.NamedPipeExist(NamedPipes.scriptpipe))//check if the pipe exist
            {
                string[] array = LuacBox.Text.Split("\r\n".ToCharArray());//array to store all and split the script
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

        private void Clear_Click(object sender, EventArgs e) => LuacBox.Clear();

        private void AlwaysTop_CheckedChanged(object sender, EventArgs e)
        {
            if (AlwaysTop.Checked)
            {
                TopMost = true;
            }
            else
            {
                TopMost = false;
            }
        }

        private void Inject_Click(object sender, EventArgs e) => Functions.Inject();

        private void Theme_Click(object sender, EventArgs e)
        {
            if (metroStyleManager1.Theme == MetroThemeStyle.Dark)
            {
                Text = "Ice Source Light Theme";
            }
            else if (metroStyleManager1.Theme == MetroThemeStyle.Light)
            {
                Text = "Ice Source Dark Theme";
            }
            metroStyleManager1.Theme = metroStyleManager1.Theme == MetroThemeStyle.Light ? MetroThemeStyle.Dark : MetroThemeStyle.Light;
        }

        private void IceSourceMetro_FormClosed(object sender, FormClosedEventArgs e) => Application.Exit();

        private void ChangeUi_Click(object sender, EventArgs e)
        {
            metroContextMenu1.Show(ChangeUi, new Point(0, ChangeUi.Height));
        }

        private void DefaultMenuOption_Click(object sender, EventArgs e)
        {
            Hide();
            Start.DefaultForm();
        }

        private void MaterialMenuItem1_Click(object sender, EventArgs e)
        {
            Hide();
            Start.MaterialSkinForm();
        }
    }
}
