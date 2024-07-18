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
    public partial class Form1 : Form
    {

        string username = "KS-7";
        string password = "zoostorm2017";


        public Form1()
        {
            InitializeComponent();
        }

        private void materialRaisedButton1_Click(object sender, EventArgs e)
        {
            if(usertxt.Text == username)
            {
                if(passtxt.Text == password)
                {
                   Form2 tampil = new Form2();
            tampil.Show();
            Visible = false;
                } else
                {
                    MessageBox.Show("Username/Password Is Incorrect!");
                }
            }
        }
        }
    }

