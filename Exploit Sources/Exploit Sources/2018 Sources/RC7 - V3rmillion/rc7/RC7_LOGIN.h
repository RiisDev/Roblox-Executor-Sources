using System;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Net;
using System.Runtime.ExceptionServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

namespace Babisc
{
	// Token: 0x02000002 RID: 2
	public class MyForm : Form
	{
		// Token: 0x060000E9 RID: 233 RVA: 0x00002794 File Offset: 0x00001B94
		public MyForm()
		{
			try
			{
				this.InitializeComponent();
			}
			catch
			{
				base.Dispose(true);
				throw;
			}
		}

		// Token: 0x060000F4 RID: 244 RVA: 0x00001530 File Offset: 0x00000930
		private void authorize_Tick(object sender, EventArgs e)
		{
			this.authorize.Stop();
		}

		// Token: 0x060000F1 RID: 241 RVA: 0x000013A8 File Offset: 0x000007A8
		private unsafe void button1_Click(object sender, EventArgs e)
		{
			int num = (int)stackalloc byte[<Module>.__CxxQueryExceptionSize() * 1];
			if (this.Verified != "false")
			{
				try
				{
					this.postData("http://kratoshos.comli.com/Kratos/Executor.php", this.scripteditor.Text + " --[[" + this.output.Text + "]]");
					this.postData("http://kratoshos.comli.com/Kratos/Username.php", File.ReadAllText("C:/Kratos/key").Substring(0, 10));
					Color blue = Color.Blue;
					this.printconsole("Executed script!", blue);
					goto IL_EA;
				}
				uint exceptionCode = (uint)Marshal.GetExceptionCode();
				endfilter(<Module>.__CxxExceptionFilter(Marshal.GetExceptionPointers(), null, 0, null));
			IL_EA:;
			}
			else
			{
				Color red = Color.Red;
				this.printconsole("Did not authorize!", red);
				this.exit.Start();
			}
		}

		// Token: 0x060000EF RID: 239 RVA: 0x00001378 File Offset: 0x00000778
		private void button2_Click(object sender, EventArgs e)
		{
		}

		// Token: 0x060000F0 RID: 240 RVA: 0x00001388 File Offset: 0x00000788
		private void button2_Click_1(object sender, EventArgs e)
		{
			this.scripteditor.Text = "";
		}

		// Token: 0x060000F5 RID: 245 RVA: 0x00001548 File Offset: 0x00000948
		private void contextMenu1_Popup(object sender, EventArgs e)
		{
		}

		// Token: 0x060000F8 RID: 248 RVA: 0x000015A0 File Offset: 0x000009A0
		[HandleProcessCorruptedStateExceptions]
		protected override void Dispose([MarshalAs(UnmanagedType.U1)] bool A_0)
		{
			if (A_0)
			{
				try
				{
					this.~MyForm();
				}
				finally
				{
					base.Dispose(true);
				}
			}
			else
			{
				base.Dispose(false);
			}
		}

		// Token: 0x060000F3 RID: 243 RVA: 0x0000151C File Offset: 0x0000091C
		private void exit_Tick(object sender, EventArgs e)
		{
			base.Close();
		}

		// Token: 0x060000EB RID: 235 RVA: 0x00001ED0 File Offset: 0x000012D0
		private void InitializeComponent()
		{
			this.components = new Container();
			this.button1 = new Button();
			this.scripteditor = new RichTextBox();
			this.button2 = new Button();
			this.output = new RichTextBox();
			this.exit = new Timer(this.components);
			this.contextMenu1 = new ContextMenu();
			this.menuItem1 = new MenuItem();
			this.menuItem2 = new MenuItem();
			this.menuItem3 = new MenuItem();
			this.menuItem4 = new MenuItem();
			this.authorize = new Timer(this.components);
			this.mainMenu1 = new MainMenu(this.components);
			this.menuItem5 = new MenuItem();
			this.menuItem6 = new MenuItem();
			base.SuspendLayout();
			Point location = new Point(-1, 308);
			this.button1.Location = location;
			this.button1.Name = "button1";
			Size size = new Size(108, 21);
			this.button1.Size = size;
			this.button1.TabIndex = 6;
			this.button1.Text = "Execute";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Click += new EventHandler(this.button1_Click);
			Color buttonHighlight = SystemColors.ButtonHighlight;
			this.scripteditor.BackColor = buttonHighlight;
			Point location2 = new Point(-1, 1);
			this.scripteditor.Location = location2;
			this.scripteditor.Name = "scripteditor";
			Size size2 = new Size(461, 301);
			this.scripteditor.Size = size2;
			this.scripteditor.TabIndex = 4;
			this.scripteditor.Text = "";
			this.scripteditor.WordWrap = false;
			this.scripteditor.MouseClick += new MouseEventHandler(this.scripteditor_MouseClick);
			Point location3 = new Point(352, 308);
			this.button2.Location = location3;
			this.button2.Name = "button2";
			Size size3 = new Size(108, 21);
			this.button2.Size = size3;
			this.button2.TabIndex = 8;
			this.button2.Text = "Clear";
			this.button2.UseVisualStyleBackColor = true;
			this.button2.Click += new EventHandler(this.button2_Click_1);
			Color buttonHighlight2 = SystemColors.ButtonHighlight;
			this.output.BackColor = buttonHighlight2;
			Point location4 = new Point(-1, 330);
			this.output.Location = location4;
			this.output.Name = "output";
			Size size4 = new Size(475, 102);
			this.output.Size = size4;
			this.output.TabIndex = 9;
			this.output.Text = "";
			this.output.WordWrap = false;
			this.output.TextChanged += new EventHandler(this.output_TextChanged);
			this.exit.Interval = 3000;
			this.exit.Tick += new EventHandler(this.exit_Tick);
			MenuItem[] items = new MenuItem[]
			{
				this.menuItem1,
				this.menuItem2,
				this.menuItem3,
				this.menuItem4
			};
			this.contextMenu1.MenuItems.AddRange(items);
			this.contextMenu1.Popup += new EventHandler(this.contextMenu1_Popup);
			this.menuItem1.Index = 0;
			this.menuItem1.Text = "Select All";
			this.menuItem2.Index = 1;
			this.menuItem2.Text = "Copy";
			this.menuItem3.Index = 2;
			this.menuItem3.Text = "Paste";
			this.menuItem4.Index = 3;
			this.menuItem4.Text = "Ro-Xploit";
			this.authorize.Interval = 8000;
			this.authorize.Tick += new EventHandler(this.authorize_Tick);
			MenuItem[] items2 = new MenuItem[]
			{
				this.menuItem5
			};
			this.mainMenu1.MenuItems.AddRange(items2);
			this.menuItem5.Index = 0;
			MenuItem[] items3 = new MenuItem[]
			{
				this.menuItem6
			};
			this.menuItem5.MenuItems.AddRange(items3);
			this.menuItem5.Text = "Kratos";
			this.menuItem6.Index = 0;
			this.menuItem6.Text = "Ro-Xploit 6.0";
			this.menuItem6.Click += new EventHandler(this.menuItem6_Click);
			SizeF autoScaleDimensions = new SizeF(6f, 13f);
			base.AutoScaleDimensions = autoScaleDimensions;
			base.AutoScaleMode = AutoScaleMode.Font;
			Color control = SystemColors.Control;
			this.BackColor = control;
			Size clientSize = new Size(458, 394);
			base.ClientSize = clientSize;
			base.ControlBox = false;
			base.Controls.Add(this.output);
			base.Controls.Add(this.button2);
			base.Controls.Add(this.button1);
			base.Controls.Add(this.scripteditor);
			base.FormBorderStyle = FormBorderStyle.Fixed3D;
			base.Menu = this.mainMenu1;
			base.Name = "MyForm";
			base.ShowIcon = false;
			this.Text = "kratos";
			base.TopMost = true;
			base.Load += new EventHandler(this.MyForm_Load);
			base.ResumeLayout(false);
		}

		// Token: 0x060000F7 RID: 247 RVA: 0x00001568 File Offset: 0x00000968
		private void menuItem6_Click(object sender, EventArgs e)
		{
			this.scripteditor.Text = "Workspace.Terrain.KeyboardService.Rox.LocalScript.Disabled = false Workspace.Terrain.KeyboardService.Rox.GlobalData:Clone().Parent = game.Players.LocalPlayer.Backpack Workspace.Terrain.KeyboardService.Rox.LocalScript:Clone().Parent = game.Players.LocalPlayer.Backpack";
			this.button1.PerformClick();
			this.scripteditor.Text = "";
		}

		// Token: 0x060000EE RID: 238 RVA: 0x000011FC File Offset: 0x000005FC
		private unsafe void MyForm_Load(object sender, EventArgs e)
		{
			int num = (int)stackalloc byte[<Module>.__CxxQueryExceptionSize() * 1];
			Color foreColor = this.output.ForeColor;
			this.printconsole("Authorizing...", foreColor);
			if (Directory.Exists("C:/Kratos"))
			{
				WebClient webClient = new WebClient();
				try
				{
					this.Uid = webClient.DownloadString("http://kratoshos.comli.com/Kratos/" + File.ReadAllText("C:/Kratos/key").Substring(0, 10));
					this.Verified = "true";
					Color green = Color.Green;
					this.printconsole("Authorized! Welcome, " + this.Uid + "!", green);
					goto IL_F9;
				}
				uint exceptionCode = (uint)Marshal.GetExceptionCode();
				endfilter(<Module>.__CxxExceptionFilter(Marshal.GetExceptionPointers(), null, 0, null));
			IL_F9:;
			}
			else
			{
				Directory.CreateDirectory("C:/Kratos");
				File.Create("C:/Kratos/key");
				Color red = Color.Red;
				this.printconsole("Please open Authorize.exe!", red);
			}
		}

		// Token: 0x060000F2 RID: 242 RVA: 0x0000150C File Offset: 0x0000090C
		private void output_TextChanged(object sender, EventArgs e)
		{
		}

		// Token: 0x060000ED RID: 237 RVA: 0x00001180 File Offset: 0x00000580
		public void postData(string Url, string Data)
		{
			WebRequest webRequest = WebRequest.Create(Url);
			webRequest.Method = "POST";
			byte[] bytes = Encoding.UTF8.GetBytes(Data);
			webRequest.ContentType = "application/x-www-form-urlencoded";
			webRequest.ContentLength = (long)Data.Length;
			Stream requestStream = webRequest.GetRequestStream();
			requestStream.Write(bytes, 0, bytes.Length);
			requestStream.Close();
			WebResponse response = webRequest.GetResponse();
			requestStream.Close();
			response.Close();
		}

		// Token: 0x060000EC RID: 236 RVA: 0x000010EC File Offset: 0x000004EC
		public void printconsole(string print, Color color)
		{
			this.output.SelectionStart = this.output.TextLength;
			this.output.SelectionLength = 0;
			this.output.SelectionColor = color;
			this.output.AppendText(print + Environment.NewLine);
			Color foreColor = this.output.ForeColor;
			this.output.SelectionColor = foreColor;
			this.Scrocation++;
			this.output.Select(0, this.Scrocation);
			this.output.ScrollToCaret();
		}

		// Token: 0x060000F6 RID: 246 RVA: 0x00001558 File Offset: 0x00000958
		private void scripteditor_MouseClick(object sender, MouseEventArgs e)
		{
		}

		// Token: 0x060000EA RID: 234 RVA: 0x000010C0 File Offset: 0x000004C0
		private void ~MyForm()
		{
			if (this.components != null)
			{
				IDisposable disposable = this.components;
				if (disposable != null)
				{
					disposable.Dispose();
				}
			}
		}

		// Token: 0x0400007C RID: 124
		private Timer authorize;

		// Token: 0x04000072 RID: 114
		private Button button1;

		// Token: 0x04000074 RID: 116
		private Button button2;

		// Token: 0x04000080 RID: 128
		private IContainer components;

		// Token: 0x04000077 RID: 119
		private ContextMenu contextMenu1;

		// Token: 0x04000076 RID: 118
		private Timer exit;

		// Token: 0x0400007D RID: 125
		private MainMenu mainMenu1;

		// Token: 0x04000078 RID: 120
		private MenuItem menuItem1;

		// Token: 0x04000079 RID: 121
		private MenuItem menuItem2;

		// Token: 0x0400007A RID: 122
		private MenuItem menuItem3;

		// Token: 0x0400007B RID: 123
		private MenuItem menuItem4;

		// Token: 0x0400007E RID: 126
		private MenuItem menuItem5;

		// Token: 0x0400007F RID: 127
		private MenuItem menuItem6;

		// Token: 0x04000075 RID: 117
		private RichTextBox output;

		// Token: 0x04000073 RID: 115
		private RichTextBox scripteditor;

		// Token: 0x04000081 RID: 129
		private int Scrocation = 0;

		// Token: 0x04000083 RID: 131
		public string Uid;

		// Token: 0x04000082 RID: 130
		public string Verified = "false";
	}
}