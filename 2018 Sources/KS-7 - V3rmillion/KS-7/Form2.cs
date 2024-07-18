using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace KS_7
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
        }

        private void flatButton6_Click(object sender, EventArgs e)
        {
            textEditorControl1.Text = "";
        }

        private void flatButton5_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog1 = new OpenFileDialog();
            openFileDialog1.Filter = "Lua File|*.lua";
            openFileDialog1.Title = "Select a Lua";
            if (openFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                this.Cursor = new Cursor(openFileDialog1.OpenFile());
        }

        private void flatButton4_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Executed Lua");
        }

        private void flatButton2_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog1 = new OpenFileDialog();
            openFileDialog1.Filter = "TXT File|*.txt";
            openFileDialog1.Title = "Select a TXT";
            if (openFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                this.Cursor = new Cursor(openFileDialog1.OpenFile());
        }

        private void flatButton3_Click(object sender, EventArgs e)
        {
            textBox1.Text = "";
        }

        private void flatButton1_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Executed Lua C");
        }

        private void materialFlatButton1_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Injected | You Can Use KS-7 Rewrite");
        }

        private void materialLabel5_Click(object sender, EventArgs e)
        {

        }
    }
}
