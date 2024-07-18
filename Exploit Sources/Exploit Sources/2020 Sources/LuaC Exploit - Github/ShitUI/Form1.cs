using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ShitUI
{
    public partial class Form1 : Form
    {
        public const int WM_NCLBUTTONDOWN = 0xA1;
        public const int HT_CAPTION = 0x2;

        [DllImportAttribute("user32.dll")]
        public static extern int SendMessage(IntPtr hWnd, int Msg, int wParam, int lParam);
        [DllImportAttribute("user32.dll")]
        public static extern bool ReleaseCapture();
        public Form1()
        {
            InitializeComponent();
        }

        private void Button2_Click(object sender, EventArgs e)
        {
            if (NamedPipes.NamedPipeExist(NamedPipes.luacpipename))
            {
                NamedPipes.LuaCPipe(fastColoredTextBox1.Text);
            }
            else
            {
                MessageBox.Show("Your Not Injected!");
            }
        }

        private void Button1_Click(object sender, EventArgs e)
        {
            if (textBox1.Text == "cmds")
            {
                richTextBox1.Text = "ws [P] [V] \n" + "jp [P] [V] \n" + "print [Text]" + "kill [P]";
            }
            if (NamedPipes.NamedPipeExist(NamedPipes.cmdpipename))
            {
                NamedPipes.CommandPipe(textBox1.Text);
            }
            else
            {
                MessageBox.Show("Your Not Injected!");
            }

        }

        private void Button3_Click(object sender, EventArgs e)
        {
            fastColoredTextBox1.Text = "";
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void Button5_Click(object sender, EventArgs e)
        {
            richTextBox1.Text = "\n";
        }
       
        private void Button6_Click(object sender, EventArgs e)
        {
            Process.Start(@"Injector.exe");
        }

        private void Button7_Click(object sender, EventArgs e)
        {
            Process[] workers = Process.GetProcessesByName("RobloxPlayerBeta");
            foreach (Process worker in workers)
            {
                worker.Kill();
                worker.WaitForExit();
                worker.Dispose();
            }
        }

        private void Panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void Panel1_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                ReleaseCapture();
                SendMessage(Handle, WM_NCLBUTTONDOWN, HT_CAPTION, 0);
            }
        }

        private void Button8_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void Button9_Click(object sender, EventArgs e)
        {
            this.MinimizeBox = true;
        }
    }
}
