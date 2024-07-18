using HaxonUI;
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
using AxonSimpleUI;


namespace Hacks
{
    public partial class HaxonWindow : Form
    {
        public HaxonWindow()
        {
            InitializeComponent();
        }

        private void button2_Click(object sender, EventArgs e)
        {

        }

        private void siticoneButton1_Click(object sender, EventArgs e)
        {
           
            Functions.Inject();
        }

        private void ClearButton_Click(object sender, EventArgs e)
        {
            ScriptBox.Clear();
        }

        private void siticoneButton2_Click(object sender, EventArgs e)
        {
            if (Functions.openfiledialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    ScriptBox.Text = File.ReadAllText(Functions.openfiledialog.FileName);

                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error: Could not read file from disk. Original error: " + ex.Message);
                }

            }
        }

        private void siticoneButton4_Click(object sender, EventArgs e)
        {

        }

        private void siticoneButton5_Click(object sender, EventArgs e)
        {

        }

        private void siticoneButton1_Click_1(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void siticoneButton3_Click(object sender, EventArgs e)
        {
            this.WindowState = FormWindowState.Minimized;
        }

        private void siticoneButton4_Click_1(object sender, EventArgs e)
        {
            About about = new About();
            about.Show();
        }

        private void ExecuteButton_Click(object sender, EventArgs e)
        {
            if (NamedPipes.NamedPipeExist(NamedPipes.luapipename))
            {
                NamedPipes.LuaPipe(ScriptBox.Text);
            }
            else
            {
                MessageBox.Show("Inject " + Functions.exploitdllname + " before Using this!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation); return;
            }
        }

        private void ScriptBox_TextChanged(object sender, EventArgs e)
        {

        }
      
        private void HaxonWindow_Load(object sender, EventArgs e)
        {
            Functions.PopulateListBox(listBox1, "./Scripts", "*.txt");
            Functions.PopulateListBox(listBox1, "./Scripts", "*.lua");
        }

        private void siticoneButton5_Click_1(object sender, EventArgs e)
        {
            listBox1.Items.Clear();//Clear Items in the LuaScriptList
            Functions.PopulateListBox(listBox1, "./Scripts", "*.txt");
            Functions.PopulateListBox(listBox1, "./Scripts", "*.lua");
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            ScriptBox.Text = File.ReadAllText($"./Scripts/{listBox1.SelectedItem}");
        }
    }
}
