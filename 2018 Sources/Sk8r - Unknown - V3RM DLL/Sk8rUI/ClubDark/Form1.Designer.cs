namespace ClubDark
{
	// Token: 0x02000002 RID: 2
	public partial class Form1 : global::System.Windows.Forms.Form
	{
		// Token: 0x06000028 RID: 40 RVA: 0x00002B10 File Offset: 0x00000D10
		protected override void Dispose(bool disposing)
		{
			bool flag = disposing && this.components != null;
			if (flag)
			{
				this.components.Dispose();
			}
			base.Dispose(disposing);
		}

		// Token: 0x06000029 RID: 41 RVA: 0x00002B48 File Offset: 0x00000D48
		private void InitializeComponent()
		{
			global::System.ComponentModel.ComponentResourceManager componentResourceManager = new global::System.ComponentModel.ComponentResourceManager(typeof(global::ClubDark.Form1));
			this.ScriptList = new global::System.Windows.Forms.ListBox();
			this.label1 = new global::System.Windows.Forms.Label();
			this.label2 = new global::System.Windows.Forms.Label();
			this.execute = new global::System.Windows.Forms.Button();
			this.clear = new global::System.Windows.Forms.Button();
			this.panel1 = new global::System.Windows.Forms.Panel();
			this.pictureBox3 = new global::System.Windows.Forms.PictureBox();
			this.panel2 = new global::System.Windows.Forms.Panel();
			this.menuStrip2 = new global::System.Windows.Forms.MenuStrip();
			this.scriptEditorToolStripMenuItem = new global::System.Windows.Forms.ToolStripMenuItem();
			this.openToolStripMenuItem = new global::System.Windows.Forms.ToolStripMenuItem();
			this.saveAsToolStripMenuItem = new global::System.Windows.Forms.ToolStripMenuItem();
			this.localPlayerToolStripMenuItem = new global::System.Windows.Forms.ToolStripMenuItem();
			this.gamesToolStripMenuItem = new global::System.Windows.Forms.ToolStripMenuItem();
			this.aboutToolStripMenuItem = new global::System.Windows.Forms.ToolStripMenuItem();
			this.aboutToolStripMenuItem1 = new global::System.Windows.Forms.ToolStripMenuItem();
			this.clubDarkToolStripMenuItem = new global::System.Windows.Forms.ToolStripMenuItem();
			this.pictureBox2 = new global::System.Windows.Forms.PictureBox();
			this.pictureBox1 = new global::System.Windows.Forms.PictureBox();
			this.menuStrip1 = new global::System.Windows.Forms.MenuStrip();
			this.Input = new global::ScintillaNET.Scintilla();
			this.localplayer = new global::System.Windows.Forms.Panel();
			this.label11 = new global::System.Windows.Forms.Label();
			this.button15 = new global::System.Windows.Forms.Button();
			this.button14 = new global::System.Windows.Forms.Button();
			this.button13 = new global::System.Windows.Forms.Button();
			this.button12 = new global::System.Windows.Forms.Button();
			this.button11 = new global::System.Windows.Forms.Button();
			this.textBox3 = new global::System.Windows.Forms.TextBox();
			this.textBox2 = new global::System.Windows.Forms.TextBox();
			this.textBox1 = new global::System.Windows.Forms.TextBox();
			this.label10 = new global::System.Windows.Forms.Label();
			this.label9 = new global::System.Windows.Forms.Label();
			this.label8 = new global::System.Windows.Forms.Label();
			this.label7 = new global::System.Windows.Forms.Label();
			this.label6 = new global::System.Windows.Forms.Label();
			this.button9 = new global::System.Windows.Forms.Button();
			this.button10 = new global::System.Windows.Forms.Button();
			this.button7 = new global::System.Windows.Forms.Button();
			this.button8 = new global::System.Windows.Forms.Button();
			this.button5 = new global::System.Windows.Forms.Button();
			this.button6 = new global::System.Windows.Forms.Button();
			this.label5 = new global::System.Windows.Forms.Label();
			this.button4 = new global::System.Windows.Forms.Button();
			this.button3 = new global::System.Windows.Forms.Button();
			this.label4 = new global::System.Windows.Forms.Label();
			this.button2 = new global::System.Windows.Forms.Button();
			this.label3 = new global::System.Windows.Forms.Label();
			this.button1 = new global::System.Windows.Forms.Button();
			this.button16 = new global::System.Windows.Forms.Button();
			this.button17 = new global::System.Windows.Forms.Button();
			this.button18 = new global::System.Windows.Forms.Button();
			this.button19 = new global::System.Windows.Forms.Button();
			this.Games = new global::System.Windows.Forms.Panel();
			this.pictureBox4 = new global::System.Windows.Forms.PictureBox();
			this.pictureBox5 = new global::System.Windows.Forms.PictureBox();
			this.pictureBox6 = new global::System.Windows.Forms.PictureBox();
			this.panel1.SuspendLayout();
			((global::System.ComponentModel.ISupportInitialize)this.pictureBox3).BeginInit();
			this.panel2.SuspendLayout();
			this.menuStrip2.SuspendLayout();
			((global::System.ComponentModel.ISupportInitialize)this.pictureBox2).BeginInit();
			((global::System.ComponentModel.ISupportInitialize)this.pictureBox1).BeginInit();
			this.localplayer.SuspendLayout();
			this.Games.SuspendLayout();
			((global::System.ComponentModel.ISupportInitialize)this.pictureBox4).BeginInit();
			((global::System.ComponentModel.ISupportInitialize)this.pictureBox5).BeginInit();
			((global::System.ComponentModel.ISupportInitialize)this.pictureBox6).BeginInit();
			base.SuspendLayout();
			this.ScriptList.BackColor = global::System.Drawing.Color.FromArgb(25, 25, 25);
			this.ScriptList.BorderStyle = global::System.Windows.Forms.BorderStyle.None;
			this.ScriptList.ForeColor = global::System.Drawing.SystemColors.Info;
			this.ScriptList.FormattingEnabled = true;
			this.ScriptList.Location = new global::System.Drawing.Point(517, 63);
			this.ScriptList.Name = "ScriptList";
			this.ScriptList.Size = new global::System.Drawing.Size(120, 325);
			this.ScriptList.TabIndex = 2;
			this.ScriptList.SelectedValueChanged += new global::System.EventHandler(this.ScriptList_SelectedValueChanged);
			this.label1.AutoSize = true;
			this.label1.Cursor = global::System.Windows.Forms.Cursors.Hand;
			this.label1.FlatStyle = global::System.Windows.Forms.FlatStyle.Popup;
			this.label1.Location = new global::System.Drawing.Point(595, 4);
			this.label1.Name = "label1";
			this.label1.Size = new global::System.Drawing.Size(13, 13);
			this.label1.TabIndex = 5;
			this.label1.Text = "_";
			this.label1.Click += new global::System.EventHandler(this.label1_Click);
			this.label2.AutoSize = true;
			this.label2.Cursor = global::System.Windows.Forms.Cursors.Hand;
			this.label2.FlatStyle = global::System.Windows.Forms.FlatStyle.Popup;
			this.label2.Font = new global::System.Drawing.Font("Microsoft Sans Serif", 12f, global::System.Drawing.FontStyle.Regular, global::System.Drawing.GraphicsUnit.Point, 0);
			this.label2.Location = new global::System.Drawing.Point(614, 4);
			this.label2.Name = "label2";
			this.label2.Size = new global::System.Drawing.Size(16, 20);
			this.label2.TabIndex = 6;
			this.label2.Text = "x";
			this.label2.Click += new global::System.EventHandler(this.label2_Click);
			this.execute.BackColor = global::System.Drawing.Color.FromArgb(255, 128, 0);
			this.execute.FlatAppearance.BorderSize = 0;
			this.execute.FlatStyle = global::System.Windows.Forms.FlatStyle.Flat;
			this.execute.Font = new global::System.Drawing.Font("Microsoft Sans Serif", 11.25f, global::System.Drawing.FontStyle.Regular, global::System.Drawing.GraphicsUnit.Point, 0);
			this.execute.ForeColor = global::System.Drawing.Color.White;
			this.execute.Location = new global::System.Drawing.Point(0, 363);
			this.execute.Name = "execute";
			this.execute.Size = new global::System.Drawing.Size(249, 25);
			this.execute.TabIndex = 7;
			this.execute.Text = "EXECUTE";
			this.execute.UseVisualStyleBackColor = false;
			this.execute.Click += new global::System.EventHandler(this.execute_Click);
			this.clear.BackColor = global::System.Drawing.Color.OrangeRed;
			this.clear.FlatAppearance.BorderSize = 0;
			this.clear.FlatStyle = global::System.Windows.Forms.FlatStyle.Flat;
			this.clear.Font = new global::System.Drawing.Font("Microsoft Sans Serif", 11.25f, global::System.Drawing.FontStyle.Regular, global::System.Drawing.GraphicsUnit.Point, 0);
			this.clear.ForeColor = global::System.Drawing.Color.White;
			this.clear.Location = new global::System.Drawing.Point(255, 363);
			this.clear.Name = "clear";
			this.clear.Size = new global::System.Drawing.Size(256, 25);
			this.clear.TabIndex = 8;
			this.clear.Text = "CLEAR";
			this.clear.UseVisualStyleBackColor = false;
			this.clear.Click += new global::System.EventHandler(this.clear_Click);
			this.panel1.BackColor = global::System.Drawing.Color.FromArgb(25, 25, 25);
			this.panel1.Controls.Add(this.pictureBox3);
			this.panel1.Controls.Add(this.panel2);
			this.panel1.Controls.Add(this.pictureBox2);
			this.panel1.Controls.Add(this.pictureBox1);
			this.panel1.Controls.Add(this.label1);
			this.panel1.Controls.Add(this.label2);
			this.panel1.Location = new global::System.Drawing.Point(-1, 0);
			this.panel1.Name = "panel1";
			this.panel1.Size = new global::System.Drawing.Size(639, 57);
			this.panel1.TabIndex = 9;
			this.panel1.MouseDown += new global::System.Windows.Forms.MouseEventHandler(this.panel1_MouseDown);
			this.pictureBox3.Image = (global::System.Drawing.Image)componentResourceManager.GetObject("pictureBox3.Image");
			this.pictureBox3.Location = new global::System.Drawing.Point(0, 0);
			this.pictureBox3.Name = "pictureBox3";
			this.pictureBox3.Size = new global::System.Drawing.Size(124, 60);
			this.pictureBox3.SizeMode = global::System.Windows.Forms.PictureBoxSizeMode.StretchImage;
			this.pictureBox3.TabIndex = 14;
			this.pictureBox3.TabStop = false;
			this.pictureBox3.MouseDown += new global::System.Windows.Forms.MouseEventHandler(this.pictureBox3_MouseDown);
			this.panel2.BackColor = global::System.Drawing.Color.Transparent;
			this.panel2.Controls.Add(this.menuStrip2);
			this.panel2.Location = new global::System.Drawing.Point(121, 36);
			this.panel2.Name = "panel2";
			this.panel2.Size = new global::System.Drawing.Size(328, 73);
			this.panel2.TabIndex = 10;
			this.menuStrip2.BackColor = global::System.Drawing.Color.Transparent;
			this.menuStrip2.Items.AddRange(new global::System.Windows.Forms.ToolStripItem[]
			{
				this.scriptEditorToolStripMenuItem,
				this.localPlayerToolStripMenuItem,
				this.gamesToolStripMenuItem,
				this.aboutToolStripMenuItem,
				this.aboutToolStripMenuItem1
			});
			this.menuStrip2.Location = new global::System.Drawing.Point(0, 0);
			this.menuStrip2.Name = "menuStrip2";
			this.menuStrip2.Size = new global::System.Drawing.Size(328, 24);
			this.menuStrip2.TabIndex = 0;
			this.menuStrip2.Text = "menuStrip2";
			this.scriptEditorToolStripMenuItem.BackColor = global::System.Drawing.Color.Transparent;
			this.scriptEditorToolStripMenuItem.DropDownItems.AddRange(new global::System.Windows.Forms.ToolStripItem[]
			{
				this.openToolStripMenuItem,
				this.saveAsToolStripMenuItem
			});
			this.scriptEditorToolStripMenuItem.ForeColor = global::System.Drawing.Color.White;
			this.scriptEditorToolStripMenuItem.Name = "scriptEditorToolStripMenuItem";
			this.scriptEditorToolStripMenuItem.Size = new global::System.Drawing.Size(64, 20);
			this.scriptEditorToolStripMenuItem.Text = "Executor";
			this.scriptEditorToolStripMenuItem.Click += new global::System.EventHandler(this.scriptEditorToolStripMenuItem_Click);
			this.openToolStripMenuItem.Name = "openToolStripMenuItem";
			this.openToolStripMenuItem.Size = new global::System.Drawing.Size(180, 22);
			this.openToolStripMenuItem.Text = "Open";
			this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
			this.saveAsToolStripMenuItem.Size = new global::System.Drawing.Size(180, 22);
			this.saveAsToolStripMenuItem.Text = "Save As";
			this.localPlayerToolStripMenuItem.BackColor = global::System.Drawing.Color.Transparent;
			this.localPlayerToolStripMenuItem.ForeColor = global::System.Drawing.Color.White;
			this.localPlayerToolStripMenuItem.Name = "localPlayerToolStripMenuItem";
			this.localPlayerToolStripMenuItem.Size = new global::System.Drawing.Size(82, 20);
			this.localPlayerToolStripMenuItem.Text = "Local Player";
			this.localPlayerToolStripMenuItem.Click += new global::System.EventHandler(this.localPlayerToolStripMenuItem_Click);
			this.gamesToolStripMenuItem.BackColor = global::System.Drawing.Color.Transparent;
			this.gamesToolStripMenuItem.ForeColor = global::System.Drawing.Color.White;
			this.gamesToolStripMenuItem.Name = "gamesToolStripMenuItem";
			this.gamesToolStripMenuItem.Size = new global::System.Drawing.Size(55, 20);
			this.gamesToolStripMenuItem.Text = "Games";
			this.gamesToolStripMenuItem.Click += new global::System.EventHandler(this.gamesToolStripMenuItem_Click);
			this.aboutToolStripMenuItem.BackColor = global::System.Drawing.Color.Transparent;
			this.aboutToolStripMenuItem.ForeColor = global::System.Drawing.Color.White;
			this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
			this.aboutToolStripMenuItem.Size = new global::System.Drawing.Size(61, 20);
			this.aboutToolStripMenuItem.Text = "Settings";
			this.aboutToolStripMenuItem1.BackColor = global::System.Drawing.Color.Transparent;
			this.aboutToolStripMenuItem1.DropDownItems.AddRange(new global::System.Windows.Forms.ToolStripItem[]
			{
				this.clubDarkToolStripMenuItem
			});
			this.aboutToolStripMenuItem1.ForeColor = global::System.Drawing.Color.White;
			this.aboutToolStripMenuItem1.Name = "aboutToolStripMenuItem1";
			this.aboutToolStripMenuItem1.Size = new global::System.Drawing.Size(52, 20);
			this.aboutToolStripMenuItem1.Text = "About";
			this.clubDarkToolStripMenuItem.Name = "clubDarkToolStripMenuItem";
			this.clubDarkToolStripMenuItem.Size = new global::System.Drawing.Size(126, 22);
			this.clubDarkToolStripMenuItem.Text = "Club Dark";
			this.clubDarkToolStripMenuItem.Click += new global::System.EventHandler(this.clubDarkToolStripMenuItem_Click);
			this.pictureBox2.Cursor = global::System.Windows.Forms.Cursors.Hand;
			this.pictureBox2.Image = (global::System.Drawing.Image)componentResourceManager.GetObject("pictureBox2.Image");
			this.pictureBox2.Location = new global::System.Drawing.Point(160, 6);
			this.pictureBox2.Name = "pictureBox2";
			this.pictureBox2.Size = new global::System.Drawing.Size(24, 24);
			this.pictureBox2.SizeMode = global::System.Windows.Forms.PictureBoxSizeMode.AutoSize;
			this.pictureBox2.TabIndex = 13;
			this.pictureBox2.TabStop = false;
			this.pictureBox2.Click += new global::System.EventHandler(this.pictureBox2_Click);
			this.pictureBox1.Cursor = global::System.Windows.Forms.Cursors.Hand;
			this.pictureBox1.Image = (global::System.Drawing.Image)componentResourceManager.GetObject("pictureBox1.Image");
			this.pictureBox1.Location = new global::System.Drawing.Point(130, 6);
			this.pictureBox1.Name = "pictureBox1";
			this.pictureBox1.Size = new global::System.Drawing.Size(24, 24);
			this.pictureBox1.SizeMode = global::System.Windows.Forms.PictureBoxSizeMode.AutoSize;
			this.pictureBox1.TabIndex = 12;
			this.pictureBox1.TabStop = false;
			this.pictureBox1.Click += new global::System.EventHandler(this.pictureBox1_Click);
			this.menuStrip1.Location = new global::System.Drawing.Point(0, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new global::System.Drawing.Size(638, 24);
			this.menuStrip1.TabIndex = 10;
			this.menuStrip1.Text = "menuStrip1";
			this.Input.BorderStyle = global::System.Windows.Forms.BorderStyle.None;
			this.Input.EdgeColor = global::System.Drawing.Color.SteelBlue;
			this.Input.Location = new global::System.Drawing.Point(2, 60);
			this.Input.Name = "Input";
			this.Input.Size = new global::System.Drawing.Size(509, 295);
			this.Input.TabIndex = 11;
			this.Input.Text = "print(\"Sk8r works!\")";
			this.localplayer.Controls.Add(this.button19);
			this.localplayer.Controls.Add(this.button18);
			this.localplayer.Controls.Add(this.button17);
			this.localplayer.Controls.Add(this.button16);
			this.localplayer.Controls.Add(this.label11);
			this.localplayer.Controls.Add(this.button15);
			this.localplayer.Controls.Add(this.button14);
			this.localplayer.Controls.Add(this.button13);
			this.localplayer.Controls.Add(this.button12);
			this.localplayer.Controls.Add(this.button11);
			this.localplayer.Controls.Add(this.textBox3);
			this.localplayer.Controls.Add(this.textBox2);
			this.localplayer.Controls.Add(this.textBox1);
			this.localplayer.Controls.Add(this.label10);
			this.localplayer.Controls.Add(this.label9);
			this.localplayer.Controls.Add(this.label8);
			this.localplayer.Controls.Add(this.label7);
			this.localplayer.Controls.Add(this.label6);
			this.localplayer.Controls.Add(this.button9);
			this.localplayer.Controls.Add(this.button10);
			this.localplayer.Controls.Add(this.button7);
			this.localplayer.Controls.Add(this.button8);
			this.localplayer.Controls.Add(this.button5);
			this.localplayer.Controls.Add(this.button6);
			this.localplayer.Controls.Add(this.label5);
			this.localplayer.Controls.Add(this.button4);
			this.localplayer.Controls.Add(this.button3);
			this.localplayer.Controls.Add(this.label4);
			this.localplayer.Controls.Add(this.button2);
			this.localplayer.Controls.Add(this.label3);
			this.localplayer.Controls.Add(this.button1);
			this.localplayer.Location = new global::System.Drawing.Point(0, 58);
			this.localplayer.Name = "localplayer";
			this.localplayer.Size = new global::System.Drawing.Size(638, 342);
			this.localplayer.TabIndex = 12;
			this.label11.AutoSize = true;
			this.label11.Font = new global::System.Drawing.Font("Microsoft Sans Serif", 12f, global::System.Drawing.FontStyle.Regular, global::System.Drawing.GraphicsUnit.Point, 0);
			this.label11.Location = new global::System.Drawing.Point(143, 310);
			this.label11.Name = "label11";
			this.label11.Size = new global::System.Drawing.Size(323, 20);
			this.label11.TabIndex = 28;
			this.label11.Text = "Have better script? Contact me in Club Dark!";
			this.button15.BackColor = global::System.Drawing.Color.Transparent;
			this.button15.FlatAppearance.BorderColor = global::System.Drawing.Color.FromArgb(255, 128, 0);
			this.button15.FlatAppearance.BorderSize = 2;
			this.button15.FlatStyle = global::System.Windows.Forms.FlatStyle.Flat;
			this.button15.Font = new global::System.Drawing.Font("Microsoft Sans Serif", 12f, global::System.Drawing.FontStyle.Regular, global::System.Drawing.GraphicsUnit.Point, 0);
			this.button15.ForeColor = global::System.Drawing.SystemColors.HighlightText;
			this.button15.Location = new global::System.Drawing.Point(408, 171);
			this.button15.Name = "button15";
			this.button15.Size = new global::System.Drawing.Size(185, 32);
			this.button15.TabIndex = 27;
			this.button15.Text = "CAM FLY";
			this.button15.UseVisualStyleBackColor = false;
			this.button15.Click += new global::System.EventHandler(this.button15_Click);
			this.button14.BackColor = global::System.Drawing.Color.Transparent;
			this.button14.FlatAppearance.BorderColor = global::System.Drawing.Color.FromArgb(255, 128, 0);
			this.button14.FlatAppearance.BorderSize = 2;
			this.button14.FlatStyle = global::System.Windows.Forms.FlatStyle.Flat;
			this.button14.Font = new global::System.Drawing.Font("Microsoft Sans Serif", 12f, global::System.Drawing.FontStyle.Regular, global::System.Drawing.GraphicsUnit.Point, 0);
			this.button14.ForeColor = global::System.Drawing.SystemColors.HighlightText;
			this.button14.Location = new global::System.Drawing.Point(408, 133);
			this.button14.Name = "button14";
			this.button14.Size = new global::System.Drawing.Size(185, 32);
			this.button14.TabIndex = 26;
			this.button14.Text = "ESP";
			this.button14.UseVisualStyleBackColor = false;
			this.button14.Click += new global::System.EventHandler(this.button14_Click);
			this.button13.BackColor = global::System.Drawing.Color.Transparent;
			this.button13.FlatAppearance.BorderColor = global::System.Drawing.Color.FromArgb(255, 128, 0);
			this.button13.FlatAppearance.BorderSize = 2;
			this.button13.FlatStyle = global::System.Windows.Forms.FlatStyle.Flat;
			this.button13.Font = new global::System.Drawing.Font("Microsoft Sans Serif", 12f, global::System.Drawing.FontStyle.Regular, global::System.Drawing.GraphicsUnit.Point, 0);
			this.button13.ForeColor = global::System.Drawing.SystemColors.HighlightText;
			this.button13.Location = new global::System.Drawing.Point(408, 95);
			this.button13.Name = "button13";
			this.button13.Size = new global::System.Drawing.Size(185, 32);
			this.button13.TabIndex = 25;
			this.button13.Text = "Sk8r UI";
			this.button13.UseVisualStyleBackColor = false;
			this.button13.Click += new global::System.EventHandler(this.button13_Click);
			this.button12.BackColor = global::System.Drawing.Color.Transparent;
			this.button12.FlatAppearance.BorderColor = global::System.Drawing.Color.FromArgb(255, 128, 0);
			this.button12.FlatAppearance.BorderSize = 2;
			this.button12.FlatStyle = global::System.Windows.Forms.FlatStyle.Flat;
			this.button12.Font = new global::System.Drawing.Font("Microsoft Sans Serif", 12f, global::System.Drawing.FontStyle.Regular, global::System.Drawing.GraphicsUnit.Point, 0);
			this.button12.ForeColor = global::System.Drawing.SystemColors.HighlightText;
			this.button12.Location = new global::System.Drawing.Point(408, 57);
			this.button12.Name = "button12";
			this.button12.Size = new global::System.Drawing.Size(185, 32);
			this.button12.TabIndex = 24;
			this.button12.Text = "Explorer";
			this.button12.UseVisualStyleBackColor = false;
			this.button12.Click += new global::System.EventHandler(this.button12_Click);
			this.button11.BackColor = global::System.Drawing.Color.Transparent;
			this.button11.FlatAppearance.BorderColor = global::System.Drawing.Color.FromArgb(255, 128, 0);
			this.button11.FlatAppearance.BorderSize = 2;
			this.button11.FlatStyle = global::System.Windows.Forms.FlatStyle.Flat;
			this.button11.Font = new global::System.Drawing.Font("Microsoft Sans Serif", 12f, global::System.Drawing.FontStyle.Regular, global::System.Drawing.GraphicsUnit.Point, 0);
			this.button11.ForeColor = global::System.Drawing.SystemColors.HighlightText;
			this.button11.Location = new global::System.Drawing.Point(408, 19);
			this.button11.Name = "button11";
			this.button11.Size = new global::System.Drawing.Size(185, 32);
			this.button11.TabIndex = 23;
			this.button11.Text = "Reviz Admin";
			this.button11.UseVisualStyleBackColor = false;
			this.button11.Click += new global::System.EventHandler(this.button11_Click);
			this.textBox3.BackColor = global::System.Drawing.Color.FromArgb(25, 25, 25);
			this.textBox3.ForeColor = global::System.Drawing.SystemColors.Window;
			this.textBox3.Location = new global::System.Drawing.Point(166, 272);
			this.textBox3.Name = "textBox3";
			this.textBox3.Size = new global::System.Drawing.Size(100, 20);
			this.textBox3.TabIndex = 22;
			this.textBox3.Text = "0";
			this.textBox3.TextAlign = global::System.Windows.Forms.HorizontalAlignment.Center;
			this.textBox2.BackColor = global::System.Drawing.Color.FromArgb(25, 25, 25);
			this.textBox2.ForeColor = global::System.Drawing.SystemColors.Window;
			this.textBox2.Location = new global::System.Drawing.Point(166, 241);
			this.textBox2.Name = "textBox2";
			this.textBox2.Size = new global::System.Drawing.Size(100, 20);
			this.textBox2.TabIndex = 21;
			this.textBox2.Text = "50";
			this.textBox2.TextAlign = global::System.Windows.Forms.HorizontalAlignment.Center;
			this.textBox1.BackColor = global::System.Drawing.Color.FromArgb(25, 25, 25);
			this.textBox1.ForeColor = global::System.Drawing.SystemColors.Window;
			this.textBox1.Location = new global::System.Drawing.Point(166, 209);
			this.textBox1.Name = "textBox1";
			this.textBox1.Size = new global::System.Drawing.Size(100, 20);
			this.textBox1.TabIndex = 20;
			this.textBox1.Text = "16";
			this.textBox1.TextAlign = global::System.Windows.Forms.HorizontalAlignment.Center;
			this.label10.AutoSize = true;
			this.label10.BackColor = global::System.Drawing.Color.Transparent;
			this.label10.Font = new global::System.Drawing.Font("Consolas", 20.25f, global::System.Drawing.FontStyle.Regular, global::System.Drawing.GraphicsUnit.Point, 0);
			this.label10.ForeColor = global::System.Drawing.Color.White;
			this.label10.Location = new global::System.Drawing.Point(3, 265);
			this.label10.Name = "label10";
			this.label10.Size = new global::System.Drawing.Size(135, 32);
			this.label10.TabIndex = 19;
			this.label10.Text = "Levitate";
			this.label9.AutoSize = true;
			this.label9.BackColor = global::System.Drawing.Color.Transparent;
			this.label9.Font = new global::System.Drawing.Font("Consolas", 20.25f, global::System.Drawing.FontStyle.Regular, global::System.Drawing.GraphicsUnit.Point, 0);
			this.label9.ForeColor = global::System.Drawing.Color.White;
			this.label9.Location = new global::System.Drawing.Point(3, 234);
			this.label9.Name = "label9";
			this.label9.Size = new global::System.Drawing.Size(150, 32);
			this.label9.TabIndex = 18;
			this.label9.Text = "JumpPower";
			this.label8.AutoSize = true;
			this.label8.BackColor = global::System.Drawing.Color.Transparent;
			this.label8.Font = new global::System.Drawing.Font("Consolas", 20.25f, global::System.Drawing.FontStyle.Regular, global::System.Drawing.GraphicsUnit.Point, 0);
			this.label8.ForeColor = global::System.Drawing.Color.White;
			this.label8.Location = new global::System.Drawing.Point(3, 203);
			this.label8.Name = "label8";
			this.label8.Size = new global::System.Drawing.Size(150, 32);
			this.label8.TabIndex = 17;
			this.label8.Text = "WalkSpeed";
			this.label7.AutoSize = true;
			this.label7.BackColor = global::System.Drawing.Color.Transparent;
			this.label7.Font = new global::System.Drawing.Font("Consolas", 20.25f, global::System.Drawing.FontStyle.Regular, global::System.Drawing.GraphicsUnit.Point, 0);
			this.label7.ForeColor = global::System.Drawing.Color.White;
			this.label7.Location = new global::System.Drawing.Point(3, 148);
			this.label7.Name = "label7";
			this.label7.Size = new global::System.Drawing.Size(165, 32);
			this.label7.TabIndex = 16;
			this.label7.Text = "Click Drag";
			this.label6.AutoSize = true;
			this.label6.BackColor = global::System.Drawing.Color.Transparent;
			this.label6.Font = new global::System.Drawing.Font("Consolas", 20.25f, global::System.Drawing.FontStyle.Regular, global::System.Drawing.GraphicsUnit.Point, 0);
			this.label6.ForeColor = global::System.Drawing.Color.White;
			this.label6.Location = new global::System.Drawing.Point(3, 111);
			this.label6.Name = "label6";
			this.label6.Size = new global::System.Drawing.Size(150, 32);
			this.label6.TabIndex = 15;
			this.label6.Text = "QToDelete";
			this.button9.BackColor = global::System.Drawing.Color.Transparent;
			this.button9.FlatAppearance.BorderColor = global::System.Drawing.Color.FromArgb(255, 128, 0);
			this.button9.FlatAppearance.BorderSize = 2;
			this.button9.FlatStyle = global::System.Windows.Forms.FlatStyle.Flat;
			this.button9.ForeColor = global::System.Drawing.SystemColors.HighlightText;
			this.button9.Location = new global::System.Drawing.Point(255, 157);
			this.button9.Name = "button9";
			this.button9.Size = new global::System.Drawing.Size(75, 23);
			this.button9.TabIndex = 14;
			this.button9.Text = "OFF";
			this.button9.UseVisualStyleBackColor = false;
			this.button9.Click += new global::System.EventHandler(this.button9_Click);
			this.button10.BackColor = global::System.Drawing.Color.Transparent;
			this.button10.FlatAppearance.BorderColor = global::System.Drawing.Color.FromArgb(255, 128, 0);
			this.button10.FlatAppearance.BorderSize = 2;
			this.button10.FlatStyle = global::System.Windows.Forms.FlatStyle.Flat;
			this.button10.ForeColor = global::System.Drawing.SystemColors.HighlightText;
			this.button10.Location = new global::System.Drawing.Point(174, 157);
			this.button10.Name = "button10";
			this.button10.Size = new global::System.Drawing.Size(75, 23);
			this.button10.TabIndex = 13;
			this.button10.Text = "ON";
			this.button10.UseVisualStyleBackColor = false;
			this.button10.Click += new global::System.EventHandler(this.button10_Click);
			this.button7.BackColor = global::System.Drawing.Color.Transparent;
			this.button7.FlatAppearance.BorderColor = global::System.Drawing.Color.FromArgb(255, 128, 0);
			this.button7.FlatAppearance.BorderSize = 2;
			this.button7.FlatStyle = global::System.Windows.Forms.FlatStyle.Flat;
			this.button7.ForeColor = global::System.Drawing.SystemColors.HighlightText;
			this.button7.Location = new global::System.Drawing.Point(255, 114);
			this.button7.Name = "button7";
			this.button7.Size = new global::System.Drawing.Size(75, 23);
			this.button7.TabIndex = 12;
			this.button7.Text = "OFF";
			this.button7.UseVisualStyleBackColor = false;
			this.button7.Click += new global::System.EventHandler(this.button7_Click);
			this.button8.BackColor = global::System.Drawing.Color.Transparent;
			this.button8.FlatAppearance.BorderColor = global::System.Drawing.Color.FromArgb(255, 128, 0);
			this.button8.FlatAppearance.BorderSize = 2;
			this.button8.FlatStyle = global::System.Windows.Forms.FlatStyle.Flat;
			this.button8.ForeColor = global::System.Drawing.SystemColors.HighlightText;
			this.button8.Location = new global::System.Drawing.Point(174, 114);
			this.button8.Name = "button8";
			this.button8.Size = new global::System.Drawing.Size(75, 23);
			this.button8.TabIndex = 11;
			this.button8.Text = "ON";
			this.button8.UseVisualStyleBackColor = false;
			this.button8.Click += new global::System.EventHandler(this.button8_Click);
			this.button5.BackColor = global::System.Drawing.Color.Transparent;
			this.button5.FlatAppearance.BorderColor = global::System.Drawing.Color.FromArgb(255, 128, 0);
			this.button5.FlatAppearance.BorderSize = 2;
			this.button5.FlatStyle = global::System.Windows.Forms.FlatStyle.Flat;
			this.button5.ForeColor = global::System.Drawing.SystemColors.HighlightText;
			this.button5.Location = new global::System.Drawing.Point(189, 77);
			this.button5.Name = "button5";
			this.button5.Size = new global::System.Drawing.Size(75, 23);
			this.button5.TabIndex = 10;
			this.button5.Text = "OFF";
			this.button5.UseVisualStyleBackColor = false;
			this.button5.Click += new global::System.EventHandler(this.button5_Click);
			this.button6.BackColor = global::System.Drawing.Color.Transparent;
			this.button6.FlatAppearance.BorderColor = global::System.Drawing.Color.FromArgb(255, 128, 0);
			this.button6.FlatAppearance.BorderSize = 2;
			this.button6.FlatStyle = global::System.Windows.Forms.FlatStyle.Flat;
			this.button6.ForeColor = global::System.Drawing.SystemColors.HighlightText;
			this.button6.Location = new global::System.Drawing.Point(108, 77);
			this.button6.Name = "button6";
			this.button6.Size = new global::System.Drawing.Size(75, 23);
			this.button6.TabIndex = 9;
			this.button6.Text = "ON";
			this.button6.UseVisualStyleBackColor = false;
			this.button6.Click += new global::System.EventHandler(this.button6_Click);
			this.label5.AutoSize = true;
			this.label5.BackColor = global::System.Drawing.Color.Transparent;
			this.label5.Font = new global::System.Drawing.Font("Consolas", 20.25f, global::System.Drawing.FontStyle.Regular, global::System.Drawing.GraphicsUnit.Point, 0);
			this.label5.ForeColor = global::System.Drawing.Color.White;
			this.label5.Location = new global::System.Drawing.Point(3, 72);
			this.label5.Name = "label5";
			this.label5.Size = new global::System.Drawing.Size(105, 32);
			this.label5.TabIndex = 8;
			this.label5.Text = "TP (Q)";
			this.button4.BackColor = global::System.Drawing.Color.Transparent;
			this.button4.FlatAppearance.BorderColor = global::System.Drawing.Color.FromArgb(255, 128, 0);
			this.button4.FlatAppearance.BorderSize = 2;
			this.button4.FlatStyle = global::System.Windows.Forms.FlatStyle.Flat;
			this.button4.ForeColor = global::System.Drawing.SystemColors.HighlightText;
			this.button4.Location = new global::System.Drawing.Point(189, 45);
			this.button4.Name = "button4";
			this.button4.Size = new global::System.Drawing.Size(75, 23);
			this.button4.TabIndex = 7;
			this.button4.Text = "OFF";
			this.button4.UseVisualStyleBackColor = false;
			this.button4.Click += new global::System.EventHandler(this.button4_Click);
			this.button3.BackColor = global::System.Drawing.Color.Transparent;
			this.button3.FlatAppearance.BorderColor = global::System.Drawing.Color.FromArgb(255, 128, 0);
			this.button3.FlatAppearance.BorderSize = 2;
			this.button3.FlatStyle = global::System.Windows.Forms.FlatStyle.Flat;
			this.button3.ForeColor = global::System.Drawing.SystemColors.HighlightText;
			this.button3.Location = new global::System.Drawing.Point(108, 45);
			this.button3.Name = "button3";
			this.button3.Size = new global::System.Drawing.Size(75, 23);
			this.button3.TabIndex = 6;
			this.button3.Text = "ON";
			this.button3.UseVisualStyleBackColor = false;
			this.button3.Click += new global::System.EventHandler(this.button3_Click);
			this.label4.AutoSize = true;
			this.label4.BackColor = global::System.Drawing.Color.Transparent;
			this.label4.Font = new global::System.Drawing.Font("Consolas", 20.25f, global::System.Drawing.FontStyle.Regular, global::System.Drawing.GraphicsUnit.Point, 0);
			this.label4.ForeColor = global::System.Drawing.Color.White;
			this.label4.Location = new global::System.Drawing.Point(3, 41);
			this.label4.Name = "label4";
			this.label4.Size = new global::System.Drawing.Size(60, 32);
			this.label4.TabIndex = 5;
			this.label4.Text = "Fly";
			this.button2.BackColor = global::System.Drawing.Color.Transparent;
			this.button2.FlatAppearance.BorderColor = global::System.Drawing.Color.FromArgb(255, 128, 0);
			this.button2.FlatAppearance.BorderSize = 2;
			this.button2.FlatStyle = global::System.Windows.Forms.FlatStyle.Flat;
			this.button2.ForeColor = global::System.Drawing.SystemColors.HighlightText;
			this.button2.Location = new global::System.Drawing.Point(189, 13);
			this.button2.Name = "button2";
			this.button2.Size = new global::System.Drawing.Size(75, 23);
			this.button2.TabIndex = 4;
			this.button2.Text = "OFF";
			this.button2.UseVisualStyleBackColor = false;
			this.button2.Click += new global::System.EventHandler(this.button2_Click);
			this.label3.AutoSize = true;
			this.label3.BackColor = global::System.Drawing.Color.Transparent;
			this.label3.Font = new global::System.Drawing.Font("Consolas", 20.25f, global::System.Drawing.FontStyle.Regular, global::System.Drawing.GraphicsUnit.Point, 0);
			this.label3.ForeColor = global::System.Drawing.Color.White;
			this.label3.Location = new global::System.Drawing.Point(3, 10);
			this.label3.Name = "label3";
			this.label3.Size = new global::System.Drawing.Size(105, 32);
			this.label3.TabIndex = 3;
			this.label3.Text = "NoClip";
			this.button1.BackColor = global::System.Drawing.Color.Transparent;
			this.button1.FlatAppearance.BorderColor = global::System.Drawing.Color.FromArgb(255, 128, 0);
			this.button1.FlatAppearance.BorderSize = 2;
			this.button1.FlatStyle = global::System.Windows.Forms.FlatStyle.Flat;
			this.button1.ForeColor = global::System.Drawing.SystemColors.HighlightText;
			this.button1.Location = new global::System.Drawing.Point(108, 13);
			this.button1.Name = "button1";
			this.button1.Size = new global::System.Drawing.Size(75, 23);
			this.button1.TabIndex = 1;
			this.button1.Text = "ON";
			this.button1.UseVisualStyleBackColor = false;
			this.button1.Click += new global::System.EventHandler(this.button1_Click);
			this.button16.BackColor = global::System.Drawing.Color.Transparent;
			this.button16.FlatAppearance.BorderColor = global::System.Drawing.Color.FromArgb(255, 128, 0);
			this.button16.FlatAppearance.BorderSize = 2;
			this.button16.FlatStyle = global::System.Windows.Forms.FlatStyle.Flat;
			this.button16.ForeColor = global::System.Drawing.SystemColors.HighlightText;
			this.button16.Location = new global::System.Drawing.Point(272, 207);
			this.button16.Name = "button16";
			this.button16.Size = new global::System.Drawing.Size(75, 23);
			this.button16.TabIndex = 29;
			this.button16.Text = "CHANGE";
			this.button16.UseVisualStyleBackColor = false;
			this.button16.Click += new global::System.EventHandler(this.button16_Click);
			this.button17.BackColor = global::System.Drawing.Color.Transparent;
			this.button17.FlatAppearance.BorderColor = global::System.Drawing.Color.FromArgb(255, 128, 0);
			this.button17.FlatAppearance.BorderSize = 2;
			this.button17.FlatStyle = global::System.Windows.Forms.FlatStyle.Flat;
			this.button17.ForeColor = global::System.Drawing.SystemColors.HighlightText;
			this.button17.Location = new global::System.Drawing.Point(272, 238);
			this.button17.Name = "button17";
			this.button17.Size = new global::System.Drawing.Size(75, 23);
			this.button17.TabIndex = 30;
			this.button17.Text = "CHANGE";
			this.button17.UseVisualStyleBackColor = false;
			this.button17.Click += new global::System.EventHandler(this.button17_Click);
			this.button18.BackColor = global::System.Drawing.Color.Transparent;
			this.button18.FlatAppearance.BorderColor = global::System.Drawing.Color.FromArgb(255, 128, 0);
			this.button18.FlatAppearance.BorderSize = 2;
			this.button18.FlatStyle = global::System.Windows.Forms.FlatStyle.Flat;
			this.button18.ForeColor = global::System.Drawing.SystemColors.HighlightText;
			this.button18.Location = new global::System.Drawing.Point(272, 270);
			this.button18.Name = "button18";
			this.button18.Size = new global::System.Drawing.Size(75, 23);
			this.button18.TabIndex = 31;
			this.button18.Text = "CHANGE";
			this.button18.UseVisualStyleBackColor = false;
			this.button18.Click += new global::System.EventHandler(this.button18_Click);
			this.button19.BackColor = global::System.Drawing.Color.Transparent;
			this.button19.FlatAppearance.BorderColor = global::System.Drawing.Color.FromArgb(255, 128, 0);
			this.button19.FlatAppearance.BorderSize = 2;
			this.button19.FlatStyle = global::System.Windows.Forms.FlatStyle.Flat;
			this.button19.Font = new global::System.Drawing.Font("Microsoft Sans Serif", 12f, global::System.Drawing.FontStyle.Regular, global::System.Drawing.GraphicsUnit.Point, 0);
			this.button19.ForeColor = global::System.Drawing.SystemColors.HighlightText;
			this.button19.Location = new global::System.Drawing.Point(408, 209);
			this.button19.Name = "button19";
			this.button19.Size = new global::System.Drawing.Size(185, 32);
			this.button19.TabIndex = 32;
			this.button19.Text = "ROSE HUB";
			this.button19.UseVisualStyleBackColor = false;
			this.button19.Click += new global::System.EventHandler(this.button19_Click);
			this.Games.Controls.Add(this.pictureBox6);
			this.Games.Controls.Add(this.pictureBox5);
			this.Games.Controls.Add(this.pictureBox4);
			this.Games.Location = new global::System.Drawing.Point(-1, 58);
			this.Games.Name = "Games";
			this.Games.Size = new global::System.Drawing.Size(639, 342);
			this.Games.TabIndex = 13;
			this.pictureBox4.Image = (global::System.Drawing.Image)componentResourceManager.GetObject("pictureBox4.Image");
			this.pictureBox4.Location = new global::System.Drawing.Point(12, 19);
			this.pictureBox4.Name = "pictureBox4";
			this.pictureBox4.Size = new global::System.Drawing.Size(150, 150);
			this.pictureBox4.SizeMode = global::System.Windows.Forms.PictureBoxSizeMode.AutoSize;
			this.pictureBox4.TabIndex = 0;
			this.pictureBox4.TabStop = false;
			this.pictureBox4.Click += new global::System.EventHandler(this.pictureBox4_Click);
			this.pictureBox5.Image = (global::System.Drawing.Image)componentResourceManager.GetObject("pictureBox5.Image");
			this.pictureBox5.Location = new global::System.Drawing.Point(174, 19);
			this.pictureBox5.Name = "pictureBox5";
			this.pictureBox5.Size = new global::System.Drawing.Size(150, 150);
			this.pictureBox5.SizeMode = global::System.Windows.Forms.PictureBoxSizeMode.AutoSize;
			this.pictureBox5.TabIndex = 1;
			this.pictureBox5.TabStop = false;
			this.pictureBox5.Click += new global::System.EventHandler(this.pictureBox5_Click);
			this.pictureBox6.Image = (global::System.Drawing.Image)componentResourceManager.GetObject("pictureBox6.Image");
			this.pictureBox6.Location = new global::System.Drawing.Point(336, 19);
			this.pictureBox6.Name = "pictureBox6";
			this.pictureBox6.Size = new global::System.Drawing.Size(150, 150);
			this.pictureBox6.SizeMode = global::System.Windows.Forms.PictureBoxSizeMode.AutoSize;
			this.pictureBox6.TabIndex = 2;
			this.pictureBox6.TabStop = false;
			this.pictureBox6.Click += new global::System.EventHandler(this.pictureBox6_Click);
			base.AutoScaleDimensions = new global::System.Drawing.SizeF(6f, 13f);
			base.AutoScaleMode = global::System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = global::System.Drawing.Color.FromArgb(25, 25, 25);
			base.ClientSize = new global::System.Drawing.Size(638, 397);
			base.Controls.Add(this.Games);
			base.Controls.Add(this.localplayer);
			base.Controls.Add(this.Input);
			base.Controls.Add(this.panel1);
			base.Controls.Add(this.clear);
			base.Controls.Add(this.execute);
			base.Controls.Add(this.ScriptList);
			base.Controls.Add(this.menuStrip1);
			this.ForeColor = global::System.Drawing.SystemColors.HighlightText;
			base.FormBorderStyle = global::System.Windows.Forms.FormBorderStyle.None;
			base.Icon = (global::System.Drawing.Icon)componentResourceManager.GetObject("$this.Icon");
			base.Name = "Form1";
			this.Text = "Club Dark";
			base.Load += new global::System.EventHandler(this.Form1_Load);
			this.panel1.ResumeLayout(false);
			this.panel1.PerformLayout();
			((global::System.ComponentModel.ISupportInitialize)this.pictureBox3).EndInit();
			this.panel2.ResumeLayout(false);
			this.panel2.PerformLayout();
			this.menuStrip2.ResumeLayout(false);
			this.menuStrip2.PerformLayout();
			((global::System.ComponentModel.ISupportInitialize)this.pictureBox2).EndInit();
			((global::System.ComponentModel.ISupportInitialize)this.pictureBox1).EndInit();
			this.localplayer.ResumeLayout(false);
			this.localplayer.PerformLayout();
			this.Games.ResumeLayout(false);
			this.Games.PerformLayout();
			((global::System.ComponentModel.ISupportInitialize)this.pictureBox4).EndInit();
			((global::System.ComponentModel.ISupportInitialize)this.pictureBox5).EndInit();
			((global::System.ComponentModel.ISupportInitialize)this.pictureBox6).EndInit();
			base.ResumeLayout(false);
			base.PerformLayout();
		}

		// Token: 0x04000005 RID: 5
		private global::System.ComponentModel.IContainer components = null;

		// Token: 0x04000006 RID: 6
		private global::System.Windows.Forms.ListBox ScriptList;

		// Token: 0x04000007 RID: 7
		private global::System.Windows.Forms.Label label1;

		// Token: 0x04000008 RID: 8
		private global::System.Windows.Forms.Label label2;

		// Token: 0x04000009 RID: 9
		private global::System.Windows.Forms.Button execute;

		// Token: 0x0400000A RID: 10
		private global::System.Windows.Forms.Button clear;

		// Token: 0x0400000B RID: 11
		private global::System.Windows.Forms.Panel panel1;

		// Token: 0x0400000C RID: 12
		private global::System.Windows.Forms.Panel panel2;

		// Token: 0x0400000D RID: 13
		private global::System.Windows.Forms.MenuStrip menuStrip2;

		// Token: 0x0400000E RID: 14
		private global::System.Windows.Forms.ToolStripMenuItem scriptEditorToolStripMenuItem;

		// Token: 0x0400000F RID: 15
		private global::System.Windows.Forms.ToolStripMenuItem localPlayerToolStripMenuItem;

		// Token: 0x04000010 RID: 16
		private global::System.Windows.Forms.MenuStrip menuStrip1;

		// Token: 0x04000011 RID: 17
		private global::System.Windows.Forms.ToolStripMenuItem gamesToolStripMenuItem;

		// Token: 0x04000012 RID: 18
		private global::System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;

		// Token: 0x04000013 RID: 19
		private global::System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem1;

		// Token: 0x04000014 RID: 20
		private global::System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;

		// Token: 0x04000015 RID: 21
		private global::System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;

		// Token: 0x04000016 RID: 22
		private global::ScintillaNET.Scintilla Input;

		// Token: 0x04000017 RID: 23
		private global::System.Windows.Forms.PictureBox pictureBox2;

		// Token: 0x04000018 RID: 24
		private global::System.Windows.Forms.PictureBox pictureBox1;

		// Token: 0x04000019 RID: 25
		private global::System.Windows.Forms.ToolStripMenuItem clubDarkToolStripMenuItem;

		// Token: 0x0400001A RID: 26
		private global::System.Windows.Forms.PictureBox pictureBox3;

		// Token: 0x0400001B RID: 27
		private global::System.Windows.Forms.Panel localplayer;

		// Token: 0x0400001C RID: 28
		private global::System.Windows.Forms.Button button1;

		// Token: 0x0400001D RID: 29
		private global::System.Windows.Forms.TextBox textBox3;

		// Token: 0x0400001E RID: 30
		private global::System.Windows.Forms.TextBox textBox2;

		// Token: 0x0400001F RID: 31
		private global::System.Windows.Forms.TextBox textBox1;

		// Token: 0x04000020 RID: 32
		private global::System.Windows.Forms.Label label10;

		// Token: 0x04000021 RID: 33
		private global::System.Windows.Forms.Label label9;

		// Token: 0x04000022 RID: 34
		private global::System.Windows.Forms.Label label8;

		// Token: 0x04000023 RID: 35
		private global::System.Windows.Forms.Label label7;

		// Token: 0x04000024 RID: 36
		private global::System.Windows.Forms.Label label6;

		// Token: 0x04000025 RID: 37
		private global::System.Windows.Forms.Button button9;

		// Token: 0x04000026 RID: 38
		private global::System.Windows.Forms.Button button10;

		// Token: 0x04000027 RID: 39
		private global::System.Windows.Forms.Button button7;

		// Token: 0x04000028 RID: 40
		private global::System.Windows.Forms.Button button8;

		// Token: 0x04000029 RID: 41
		private global::System.Windows.Forms.Button button5;

		// Token: 0x0400002A RID: 42
		private global::System.Windows.Forms.Button button6;

		// Token: 0x0400002B RID: 43
		private global::System.Windows.Forms.Label label5;

		// Token: 0x0400002C RID: 44
		private global::System.Windows.Forms.Button button4;

		// Token: 0x0400002D RID: 45
		private global::System.Windows.Forms.Button button3;

		// Token: 0x0400002E RID: 46
		private global::System.Windows.Forms.Label label4;

		// Token: 0x0400002F RID: 47
		private global::System.Windows.Forms.Button button2;

		// Token: 0x04000030 RID: 48
		private global::System.Windows.Forms.Label label3;

		// Token: 0x04000031 RID: 49
		private global::System.Windows.Forms.Button button13;

		// Token: 0x04000032 RID: 50
		private global::System.Windows.Forms.Button button12;

		// Token: 0x04000033 RID: 51
		private global::System.Windows.Forms.Button button11;

		// Token: 0x04000034 RID: 52
		private global::System.Windows.Forms.Label label11;

		// Token: 0x04000035 RID: 53
		private global::System.Windows.Forms.Button button15;

		// Token: 0x04000036 RID: 54
		private global::System.Windows.Forms.Button button14;

		// Token: 0x04000037 RID: 55
		private global::System.Windows.Forms.Button button18;

		// Token: 0x04000038 RID: 56
		private global::System.Windows.Forms.Button button17;

		// Token: 0x04000039 RID: 57
		private global::System.Windows.Forms.Button button16;

		// Token: 0x0400003A RID: 58
		private global::System.Windows.Forms.Button button19;

		// Token: 0x0400003B RID: 59
		private global::System.Windows.Forms.Panel Games;

		// Token: 0x0400003C RID: 60
		private global::System.Windows.Forms.PictureBox pictureBox6;

		// Token: 0x0400003D RID: 61
		private global::System.Windows.Forms.PictureBox pictureBox5;

		// Token: 0x0400003E RID: 62
		private global::System.Windows.Forms.PictureBox pictureBox4;
	}
}
