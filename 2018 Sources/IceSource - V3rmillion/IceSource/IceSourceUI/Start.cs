using System;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;

namespace IceSourceUI
{
    public partial class Start : Form
    {
        public Start() => InitializeComponent();

        public static void DefaultForm()
        {
            if (Application.OpenForms.OfType<IceSourceForm>().Any())
            {
                //MessageBox.Show("exist lets show");
                Application.OpenForms.OfType<IceSourceForm>().First().Show();
            }
            else
            {
                //MessageBox.Show("create new");
                new IceSourceForm().Show();
            }
        }

        public static void MaterialSkinForm()
        {
            if (Application.OpenForms.OfType<IceSourceMaterialSkin>().Any())
            {
                //MessageBox.Show("exist lets show");
                Application.OpenForms.OfType<IceSourceMaterialSkin>().First().Show();
            }
            else
            {
                //MessageBox.Show("create new");
                new IceSourceMaterialSkin().Show();
            }
        }

        public static void MetroModernUIForm()
        {
            if (Application.OpenForms.OfType<IceSourceMetro>().Any())
            {
                //MessageBox.Show("exist lets show");
                Application.OpenForms.OfType<IceSourceMetro>().First().Show();
            }
            else
            {
                //MessageBox.Show("create new");
                new IceSourceMetro().Show();
            }
        }

        private void Start_Load(object sender, EventArgs e)
        {
            DefaultForm();
            Size = new Size(0, 0);
        }
    }
}
