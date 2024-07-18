using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.IO.Pipes;
using System.Threading;

// dragger code wasn't made by me, I skidded that from stackoverflow, you can probably find it easily lmao

namespace headhunterui
{
    public partial class headhunter : Form
    {
        private bool dragging = false;
        private Point dragCursorPoint;
        private Point dragFormPoint;
        private Pipes pipesystem = new Pipes();

        public headhunter()
        {
            InitializeComponent();
        }

        private void exit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void minimize_Click(object sender, EventArgs e)
        {
            this.WindowState = FormWindowState.Minimized;
        }

        private void titlepanel_Paint(object sender, PaintEventArgs e)
        {

        }

        private void titlepanel_MouseDown(object sender, MouseEventArgs e)
        {
            dragging = true;
            dragCursorPoint = Cursor.Position;
            dragFormPoint = this.Location;
        }

        private void titlepanel_MouseMove(object sender, MouseEventArgs e)
        {
            if (dragging)
            {
                Point dif = Point.Subtract(Cursor.Position, new Size(dragCursorPoint));
                this.Location = Point.Add(dragFormPoint, new Size(dif));
            }
        }

        private void titlepanel_MouseUp(object sender, MouseEventArgs e)
        {
            dragging = false;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            textbox.Clear();
        }

        private void executebtn_Click(object sender, EventArgs e)
        {
            pipesystem.write_pipe(textbox.Text);
        }
    }

    public class Pipes
    {
        public void write_pipe(string text)
        {
            new Thread(() =>
            {
                using (NamedPipeClientStream NPCS = new NamedPipeClientStream(".", "headhunter", PipeDirection.Out))
                {
                    NPCS.Connect();
                    using (StreamWriter sw = new StreamWriter(NPCS))
                    {
                        sw.Write(text);
                    }
                }
            }).Start();
        }
    }
}