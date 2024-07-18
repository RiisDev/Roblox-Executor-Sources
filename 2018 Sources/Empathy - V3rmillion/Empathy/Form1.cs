using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Windows.Forms;
using Bunifu.Framework.UI;
using FastColoredTextBoxNS;

namespace Empathy
{
	// Token: 0x02000002 RID: 2
	public partial class Form1 : Form
	{
		// Token: 0x06000001 RID: 1 RVA: 0x00002050 File Offset: 0x00000250
		public Form1()
		{
			this.InitializeComponent();
		}

		// Token: 0x06000002 RID: 2 RVA: 0x00002068 File Offset: 0x00000268
		private void bunifuImageButton1_Click(object sender, EventArgs e)
		{
			Application.Exit();
		}

		// Token: 0x06000003 RID: 3 RVA: 0x00002071 File Offset: 0x00000271
		private void bunifuImageButton2_Click(object sender, EventArgs e)
		{
			base.WindowState = FormWindowState.Minimized;
		}

		// Token: 0x06000004 RID: 4 RVA: 0x0000207C File Offset: 0x0000027C
		private void bunifuFlatButton1_Click(object sender, EventArgs e)
		{
			MessageBox.Show("Please allow Empathy 5 seconds to inject.", "Empathy Notification");
			Functions.Inject();
		}

		// Token: 0x06000005 RID: 5 RVA: 0x00002098 File Offset: 0x00000298
		private void bunifuFlatButton3_Click(object sender, EventArgs e)
		{
			bool flag = Functions.openfiledialog.ShowDialog() == DialogResult.OK;
			if (flag)
			{
				try
				{
					this.fastColoredTextBox1.Text = File.ReadAllText(Functions.openfiledialog.FileName);
				}
				catch (Exception ex)
				{
					MessageBox.Show("Error: Could not read file from disk. Original error: " + ex.Message);
				}
			}
		}

		// Token: 0x06000006 RID: 6 RVA: 0x00002104 File Offset: 0x00000304
		private void bunifuFlatButton4_Click(object sender, EventArgs e)
		{
			this.fastColoredTextBox1.Clear();
		}

		// Token: 0x06000007 RID: 7 RVA: 0x00002114 File Offset: 0x00000314
		private void bunifuFlatButton2_Click(object sender, EventArgs e)
		{
			bool flag = NamedPipes.NamedPipeExist(NamedPipes.luapipename);
			if (flag)
			{
				NamedPipes.LuaPipe("spawn (function()\r\n" + this.fastColoredTextBox1.Text + "\r\nend)");
			}
		}

		// Token: 0x06000008 RID: 8 RVA: 0x00002152 File Offset: 0x00000352
		private void bunifuFlatButton5_Click(object sender, EventArgs e)
		{
			MessageBox.Show("Empathy mainly developed by xRoss#7283.", "Empathy Notification");
		}

		// Token: 0x06000009 RID: 9 RVA: 0x00002165 File Offset: 0x00000365
		private void bunifuFlatButton6_Click(object sender, EventArgs e)
		{
			Process.Start("https://discord.gg/n5ShBQp");
		}
	}
}
