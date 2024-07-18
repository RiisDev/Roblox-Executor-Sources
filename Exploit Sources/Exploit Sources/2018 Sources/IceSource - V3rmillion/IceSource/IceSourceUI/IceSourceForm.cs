using System;
using System.Drawing;
using System.IO;
using System.IO.Pipes;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows.Forms;

namespace IceSourceUI
{
    public partial class IceSourceForm : Form
    {
        public IceSourceForm()
        {
            InitializeComponent();
            //TopMost Enabled by default
            TopMost = true;
        }
        //clear button click event
        private void Clear_Click(object sender, EventArgs e) => LuaCBox.Clear();//this clear all the text in luac richtextbox
        //execute button click event
        private void Execute_Click(object sender, EventArgs e)
        {
            if (NamedPipes.NamedPipeExist(NamedPipes.scriptpipe))//check if the pipe exist
            {
                string[] array = LuaCBox.Text.Split("\r\n".ToCharArray());//array to store all and split the script
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
        //send button click event
        private void Send_Click(object sender, EventArgs e)
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
        //command richtextbox textchanged event
        private void CmdBox_TextChanged(object sender, EventArgs e)
        {
            CmdBox.SelectionStart = CmdBox.Text.Length;//this get all the text
            CmdBox.ScrollToCaret();//so with this we can scroll to the bottom
        }
        //command textbox keydown event
        private void CmdTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)//if user pressed Key Enter
            {
                Send_Click(sender, e);//execute Send_click function
            }
        }
        //open button click event
        private void Open_Click(object sender, EventArgs e)
        {
            if (Functions.openfiledialog.ShowDialog() == DialogResult.OK)//check if the user clicked Ok/open
            {
                try
                {
                    LuaCBox.Text = File.ReadAllText(Functions.openfiledialog.FileName);//load all the text in the lua c rightextbox

                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error: Could not read file from disk. Original error: " + ex.Message);//display if got error
                }
            }
        }
        //inject button click event
        public void Inject_Click(object sender, EventArgs e) => Functions.Inject();

        private void Btools_Click(object sender, EventArgs e) => NamedPipes.CommandPipe("btools me");

        private void FF_Click(object sender, EventArgs e) => NamedPipes.CommandPipe("ff me");

        private void Ws_Click(object sender, EventArgs e) => NamedPipes.CommandPipe("ws me " + WalkSpeedValue.Value);

        private void Suicide_Click(object sender, EventArgs e) => NamedPipes.CommandPipe("kill me");

        private void Sit_Click(object sender, EventArgs e) => NamedPipes.CommandPipe("sit me");

        private void JP_Click(object sender, EventArgs e) => NamedPipes.CommandPipe("jp me " + JumpPowerValue.Value);

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

        private int thm;

        public void UpdateColorControls(Control theui)
        {
            Color Black = Color.FromName("Black");
            Color White = Color.FromName("White");
            
            switch (thm)
            {
                case 1:
                    theui.BackColor = Black;
                    theui.ForeColor = White;
                    this.BackColor = Black;
                    this.Text = "Ice Source UI Dark Theme";
                    break;
                case 0:
                    theui.BackColor = White;
                    theui.ForeColor = Black;
                    this.BackColor = White;
                    this.Text = "Ice Source UI Light Theme";
                    break;
            }

            foreach (Control subC in theui.Controls)
            {
                UpdateColorControls(subC);
            }
        }

        private void Theme_Click(object sender, EventArgs e)
        {
            foreach (Control c in this.Controls)
            {
                UpdateColorControls(c);
            }
            thm++;
            if (thm > 1) thm = 0;
        }

        private void materialSkinUIToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Hide();
            Start.MaterialSkinForm();
        }

        private void IceSourceForm_FormClosed(object sender, FormClosedEventArgs e) => Application.Exit();

        private void metroModernUIToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Hide();
            Start.MetroModernUIForm();
        }
    }
}
