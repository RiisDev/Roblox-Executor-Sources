namespace Empathy
{
	// Token: 0x02000002 RID: 2
	public partial class Form1 : global::System.Windows.Forms.Form
	{
		// Token: 0x0600000A RID: 10 RVA: 0x00002174 File Offset: 0x00000374
		protected override void Dispose(bool disposing)
		{
			bool flag = disposing && this.components != null;
			if (flag)
			{
				this.components.Dispose();
			}
			base.Dispose(disposing);
		}

		// Token: 0x0600000B RID: 11 RVA: 0x000021AC File Offset: 0x000003AC
		private void InitializeComponent()
		{
			this.components = new global::System.ComponentModel.Container();
			global::System.ComponentModel.ComponentResourceManager componentResourceManager = new global::System.ComponentModel.ComponentResourceManager(typeof(global::Empathy.Form1));
			this.bunifuDragControl1 = new global::Bunifu.Framework.UI.BunifuDragControl(this.components);
			this.BottomToolStripPanel = new global::System.Windows.Forms.ToolStripPanel();
			this.TopToolStripPanel = new global::System.Windows.Forms.ToolStripPanel();
			this.RightToolStripPanel = new global::System.Windows.Forms.ToolStripPanel();
			this.LeftToolStripPanel = new global::System.Windows.Forms.ToolStripPanel();
			this.ContentPanel = new global::System.Windows.Forms.ToolStripContentPanel();
			this.panel1 = new global::System.Windows.Forms.Panel();
			this.panel2 = new global::System.Windows.Forms.Panel();
			this.panel3 = new global::System.Windows.Forms.Panel();
			this.fastColoredTextBox1 = new global::FastColoredTextBoxNS.FastColoredTextBox();
			this.bunifuFlatButton1 = new global::Bunifu.Framework.UI.BunifuFlatButton();
			this.bunifuFlatButton2 = new global::Bunifu.Framework.UI.BunifuFlatButton();
			this.bunifuFlatButton3 = new global::Bunifu.Framework.UI.BunifuFlatButton();
			this.bunifuFlatButton4 = new global::Bunifu.Framework.UI.BunifuFlatButton();
			this.bunifuFlatButton5 = new global::Bunifu.Framework.UI.BunifuFlatButton();
			this.bunifuFlatButton6 = new global::Bunifu.Framework.UI.BunifuFlatButton();
			this.pictureBox1 = new global::System.Windows.Forms.PictureBox();
			this.bunifuImageButton1 = new global::Bunifu.Framework.UI.BunifuImageButton();
			this.bunifuImageButton2 = new global::Bunifu.Framework.UI.BunifuImageButton();
			this.panel1.SuspendLayout();
			this.panel2.SuspendLayout();
			this.panel3.SuspendLayout();
			((global::System.ComponentModel.ISupportInitialize)this.fastColoredTextBox1).BeginInit();
			((global::System.ComponentModel.ISupportInitialize)this.pictureBox1).BeginInit();
			((global::System.ComponentModel.ISupportInitialize)this.bunifuImageButton1).BeginInit();
			((global::System.ComponentModel.ISupportInitialize)this.bunifuImageButton2).BeginInit();
			base.SuspendLayout();
			this.bunifuDragControl1.Fixed = true;
			this.bunifuDragControl1.Horizontal = true;
			this.bunifuDragControl1.TargetControl = this.panel1;
			this.bunifuDragControl1.Vertical = true;
			this.BottomToolStripPanel.Location = new global::System.Drawing.Point(0, 0);
			this.BottomToolStripPanel.Name = "BottomToolStripPanel";
			this.BottomToolStripPanel.Orientation = global::System.Windows.Forms.Orientation.Horizontal;
			this.BottomToolStripPanel.RowMargin = new global::System.Windows.Forms.Padding(3, 0, 0, 0);
			this.BottomToolStripPanel.Size = new global::System.Drawing.Size(0, 0);
			this.TopToolStripPanel.Location = new global::System.Drawing.Point(0, 0);
			this.TopToolStripPanel.Name = "TopToolStripPanel";
			this.TopToolStripPanel.Orientation = global::System.Windows.Forms.Orientation.Horizontal;
			this.TopToolStripPanel.RowMargin = new global::System.Windows.Forms.Padding(3, 0, 0, 0);
			this.TopToolStripPanel.Size = new global::System.Drawing.Size(0, 0);
			this.RightToolStripPanel.Location = new global::System.Drawing.Point(0, 0);
			this.RightToolStripPanel.Name = "RightToolStripPanel";
			this.RightToolStripPanel.Orientation = global::System.Windows.Forms.Orientation.Horizontal;
			this.RightToolStripPanel.RowMargin = new global::System.Windows.Forms.Padding(3, 0, 0, 0);
			this.RightToolStripPanel.Size = new global::System.Drawing.Size(0, 0);
			this.LeftToolStripPanel.Location = new global::System.Drawing.Point(0, 0);
			this.LeftToolStripPanel.Name = "LeftToolStripPanel";
			this.LeftToolStripPanel.Orientation = global::System.Windows.Forms.Orientation.Horizontal;
			this.LeftToolStripPanel.RowMargin = new global::System.Windows.Forms.Padding(3, 0, 0, 0);
			this.LeftToolStripPanel.Size = new global::System.Drawing.Size(0, 0);
			this.ContentPanel.AutoScroll = true;
			this.ContentPanel.Size = new global::System.Drawing.Size(591, 350);
			this.panel1.Controls.Add(this.bunifuImageButton2);
			this.panel1.Controls.Add(this.bunifuImageButton1);
			this.panel1.Controls.Add(this.pictureBox1);
			this.panel1.Location = new global::System.Drawing.Point(0, 0);
			this.panel1.Name = "panel1";
			this.panel1.Size = new global::System.Drawing.Size(591, 40);
			this.panel1.TabIndex = 0;
			this.panel2.Controls.Add(this.bunifuFlatButton6);
			this.panel2.Controls.Add(this.bunifuFlatButton5);
			this.panel2.Controls.Add(this.bunifuFlatButton4);
			this.panel2.Controls.Add(this.bunifuFlatButton3);
			this.panel2.Controls.Add(this.bunifuFlatButton2);
			this.panel2.Controls.Add(this.bunifuFlatButton1);
			this.panel2.Location = new global::System.Drawing.Point(0, 41);
			this.panel2.Name = "panel2";
			this.panel2.Size = new global::System.Drawing.Size(138, 308);
			this.panel2.TabIndex = 1;
			this.panel3.Controls.Add(this.fastColoredTextBox1);
			this.panel3.Location = new global::System.Drawing.Point(140, 41);
			this.panel3.Name = "panel3";
			this.panel3.Size = new global::System.Drawing.Size(451, 308);
			this.panel3.TabIndex = 2;
			this.fastColoredTextBox1.AutoCompleteBracketsList = new char[]
			{
				'(',
				')',
				'{',
				'}',
				'[',
				']',
				'"',
				'"',
				'\'',
				'\''
			};
			this.fastColoredTextBox1.AutoScrollMinSize = new global::System.Drawing.Size(27, 14);
			this.fastColoredTextBox1.BackBrush = null;
			this.fastColoredTextBox1.BackColor = global::System.Drawing.Color.FromArgb(45, 45, 45);
			this.fastColoredTextBox1.CharHeight = 14;
			this.fastColoredTextBox1.CharWidth = 8;
			this.fastColoredTextBox1.Cursor = global::System.Windows.Forms.Cursors.IBeam;
			this.fastColoredTextBox1.DisabledColor = global::System.Drawing.Color.FromArgb(100, 180, 180, 180);
			this.fastColoredTextBox1.ForeColor = global::System.Drawing.Color.White;
			this.fastColoredTextBox1.IndentBackColor = global::System.Drawing.Color.FromArgb(35, 35, 35);
			this.fastColoredTextBox1.IsReplaceMode = false;
			this.fastColoredTextBox1.Language = global::FastColoredTextBoxNS.Language.Lua;
			this.fastColoredTextBox1.LineNumberColor = global::System.Drawing.Color.White;
			this.fastColoredTextBox1.Location = new global::System.Drawing.Point(4, 1);
			this.fastColoredTextBox1.Name = "fastColoredTextBox1";
			this.fastColoredTextBox1.Paddings = new global::System.Windows.Forms.Padding(0);
			this.fastColoredTextBox1.SelectionColor = global::System.Drawing.Color.FromArgb(60, 0, 0, 255);
			this.fastColoredTextBox1.ServiceColors = (global::FastColoredTextBoxNS.ServiceColors)componentResourceManager.GetObject("fastColoredTextBox1.ServiceColors");
			this.fastColoredTextBox1.ServiceLinesColor = global::System.Drawing.Color.FromArgb(35, 35, 35);
			this.fastColoredTextBox1.Size = new global::System.Drawing.Size(444, 306);
			this.fastColoredTextBox1.TabIndex = 0;
			this.fastColoredTextBox1.Zoom = 100;
			this.bunifuFlatButton1.Activecolor = global::System.Drawing.Color.FromArgb(50, 50, 50);
			this.bunifuFlatButton1.BackColor = global::System.Drawing.Color.FromArgb(50, 50, 50);
			this.bunifuFlatButton1.BackgroundImageLayout = global::System.Windows.Forms.ImageLayout.Stretch;
			this.bunifuFlatButton1.BorderRadius = 0;
			this.bunifuFlatButton1.ButtonText = "Inject";
			this.bunifuFlatButton1.Cursor = global::System.Windows.Forms.Cursors.Hand;
			this.bunifuFlatButton1.DisabledColor = global::System.Drawing.Color.Gray;
			this.bunifuFlatButton1.Iconcolor = global::System.Drawing.Color.Transparent;
			this.bunifuFlatButton1.Iconimage = (global::System.Drawing.Image)componentResourceManager.GetObject("bunifuFlatButton1.Iconimage");
			this.bunifuFlatButton1.Iconimage_right = null;
			this.bunifuFlatButton1.Iconimage_right_Selected = null;
			this.bunifuFlatButton1.Iconimage_Selected = null;
			this.bunifuFlatButton1.IconMarginLeft = 0;
			this.bunifuFlatButton1.IconMarginRight = 0;
			this.bunifuFlatButton1.IconRightVisible = true;
			this.bunifuFlatButton1.IconRightZoom = 0.0;
			this.bunifuFlatButton1.IconVisible = false;
			this.bunifuFlatButton1.IconZoom = 90.0;
			this.bunifuFlatButton1.IsTab = false;
			this.bunifuFlatButton1.Location = new global::System.Drawing.Point(3, 2);
			this.bunifuFlatButton1.Name = "bunifuFlatButton1";
			this.bunifuFlatButton1.Normalcolor = global::System.Drawing.Color.FromArgb(50, 50, 50);
			this.bunifuFlatButton1.OnHovercolor = global::System.Drawing.Color.FromArgb(40, 40, 40);
			this.bunifuFlatButton1.OnHoverTextColor = global::System.Drawing.Color.White;
			this.bunifuFlatButton1.selected = false;
			this.bunifuFlatButton1.Size = new global::System.Drawing.Size(135, 42);
			this.bunifuFlatButton1.TabIndex = 0;
			this.bunifuFlatButton1.Text = "Inject";
			this.bunifuFlatButton1.TextAlign = global::System.Drawing.ContentAlignment.MiddleCenter;
			this.bunifuFlatButton1.Textcolor = global::System.Drawing.Color.White;
			this.bunifuFlatButton1.TextFont = new global::System.Drawing.Font("Microsoft Sans Serif", 9.75f, global::System.Drawing.FontStyle.Regular, global::System.Drawing.GraphicsUnit.Point, 0);
			this.bunifuFlatButton1.Click += new global::System.EventHandler(this.bunifuFlatButton1_Click);
			this.bunifuFlatButton2.Activecolor = global::System.Drawing.Color.FromArgb(50, 50, 50);
			this.bunifuFlatButton2.BackColor = global::System.Drawing.Color.FromArgb(50, 50, 50);
			this.bunifuFlatButton2.BackgroundImageLayout = global::System.Windows.Forms.ImageLayout.Stretch;
			this.bunifuFlatButton2.BorderRadius = 0;
			this.bunifuFlatButton2.ButtonText = "Execute";
			this.bunifuFlatButton2.Cursor = global::System.Windows.Forms.Cursors.Hand;
			this.bunifuFlatButton2.DisabledColor = global::System.Drawing.Color.Gray;
			this.bunifuFlatButton2.Iconcolor = global::System.Drawing.Color.Transparent;
			this.bunifuFlatButton2.Iconimage = (global::System.Drawing.Image)componentResourceManager.GetObject("bunifuFlatButton2.Iconimage");
			this.bunifuFlatButton2.Iconimage_right = null;
			this.bunifuFlatButton2.Iconimage_right_Selected = null;
			this.bunifuFlatButton2.Iconimage_Selected = null;
			this.bunifuFlatButton2.IconMarginLeft = 0;
			this.bunifuFlatButton2.IconMarginRight = 0;
			this.bunifuFlatButton2.IconRightVisible = true;
			this.bunifuFlatButton2.IconRightZoom = 0.0;
			this.bunifuFlatButton2.IconVisible = false;
			this.bunifuFlatButton2.IconZoom = 90.0;
			this.bunifuFlatButton2.IsTab = false;
			this.bunifuFlatButton2.Location = new global::System.Drawing.Point(3, 50);
			this.bunifuFlatButton2.Name = "bunifuFlatButton2";
			this.bunifuFlatButton2.Normalcolor = global::System.Drawing.Color.FromArgb(50, 50, 50);
			this.bunifuFlatButton2.OnHovercolor = global::System.Drawing.Color.FromArgb(40, 40, 40);
			this.bunifuFlatButton2.OnHoverTextColor = global::System.Drawing.Color.White;
			this.bunifuFlatButton2.selected = false;
			this.bunifuFlatButton2.Size = new global::System.Drawing.Size(135, 42);
			this.bunifuFlatButton2.TabIndex = 1;
			this.bunifuFlatButton2.Text = "Execute";
			this.bunifuFlatButton2.TextAlign = global::System.Drawing.ContentAlignment.MiddleCenter;
			this.bunifuFlatButton2.Textcolor = global::System.Drawing.Color.White;
			this.bunifuFlatButton2.TextFont = new global::System.Drawing.Font("Microsoft Sans Serif", 9.75f, global::System.Drawing.FontStyle.Regular, global::System.Drawing.GraphicsUnit.Point, 0);
			this.bunifuFlatButton2.Click += new global::System.EventHandler(this.bunifuFlatButton2_Click);
			this.bunifuFlatButton3.Activecolor = global::System.Drawing.Color.FromArgb(50, 50, 50);
			this.bunifuFlatButton3.BackColor = global::System.Drawing.Color.FromArgb(50, 50, 50);
			this.bunifuFlatButton3.BackgroundImageLayout = global::System.Windows.Forms.ImageLayout.Stretch;
			this.bunifuFlatButton3.BorderRadius = 0;
			this.bunifuFlatButton3.ButtonText = "Open";
			this.bunifuFlatButton3.Cursor = global::System.Windows.Forms.Cursors.Hand;
			this.bunifuFlatButton3.DisabledColor = global::System.Drawing.Color.Gray;
			this.bunifuFlatButton3.Iconcolor = global::System.Drawing.Color.Transparent;
			this.bunifuFlatButton3.Iconimage = (global::System.Drawing.Image)componentResourceManager.GetObject("bunifuFlatButton3.Iconimage");
			this.bunifuFlatButton3.Iconimage_right = null;
			this.bunifuFlatButton3.Iconimage_right_Selected = null;
			this.bunifuFlatButton3.Iconimage_Selected = null;
			this.bunifuFlatButton3.IconMarginLeft = 0;
			this.bunifuFlatButton3.IconMarginRight = 0;
			this.bunifuFlatButton3.IconRightVisible = true;
			this.bunifuFlatButton3.IconRightZoom = 0.0;
			this.bunifuFlatButton3.IconVisible = false;
			this.bunifuFlatButton3.IconZoom = 90.0;
			this.bunifuFlatButton3.IsTab = false;
			this.bunifuFlatButton3.Location = new global::System.Drawing.Point(3, 98);
			this.bunifuFlatButton3.Name = "bunifuFlatButton3";
			this.bunifuFlatButton3.Normalcolor = global::System.Drawing.Color.FromArgb(50, 50, 50);
			this.bunifuFlatButton3.OnHovercolor = global::System.Drawing.Color.FromArgb(40, 40, 40);
			this.bunifuFlatButton3.OnHoverTextColor = global::System.Drawing.Color.White;
			this.bunifuFlatButton3.selected = false;
			this.bunifuFlatButton3.Size = new global::System.Drawing.Size(135, 42);
			this.bunifuFlatButton3.TabIndex = 2;
			this.bunifuFlatButton3.Text = "Open";
			this.bunifuFlatButton3.TextAlign = global::System.Drawing.ContentAlignment.MiddleCenter;
			this.bunifuFlatButton3.Textcolor = global::System.Drawing.Color.White;
			this.bunifuFlatButton3.TextFont = new global::System.Drawing.Font("Microsoft Sans Serif", 9.75f, global::System.Drawing.FontStyle.Regular, global::System.Drawing.GraphicsUnit.Point, 0);
			this.bunifuFlatButton3.Click += new global::System.EventHandler(this.bunifuFlatButton3_Click);
			this.bunifuFlatButton4.Activecolor = global::System.Drawing.Color.FromArgb(50, 50, 50);
			this.bunifuFlatButton4.BackColor = global::System.Drawing.Color.FromArgb(50, 50, 50);
			this.bunifuFlatButton4.BackgroundImageLayout = global::System.Windows.Forms.ImageLayout.Stretch;
			this.bunifuFlatButton4.BorderRadius = 0;
			this.bunifuFlatButton4.ButtonText = "Clear";
			this.bunifuFlatButton4.Cursor = global::System.Windows.Forms.Cursors.Hand;
			this.bunifuFlatButton4.DisabledColor = global::System.Drawing.Color.Gray;
			this.bunifuFlatButton4.Iconcolor = global::System.Drawing.Color.Transparent;
			this.bunifuFlatButton4.Iconimage = (global::System.Drawing.Image)componentResourceManager.GetObject("bunifuFlatButton4.Iconimage");
			this.bunifuFlatButton4.Iconimage_right = null;
			this.bunifuFlatButton4.Iconimage_right_Selected = null;
			this.bunifuFlatButton4.Iconimage_Selected = null;
			this.bunifuFlatButton4.IconMarginLeft = 0;
			this.bunifuFlatButton4.IconMarginRight = 0;
			this.bunifuFlatButton4.IconRightVisible = true;
			this.bunifuFlatButton4.IconRightZoom = 0.0;
			this.bunifuFlatButton4.IconVisible = false;
			this.bunifuFlatButton4.IconZoom = 90.0;
			this.bunifuFlatButton4.IsTab = false;
			this.bunifuFlatButton4.Location = new global::System.Drawing.Point(3, 146);
			this.bunifuFlatButton4.Name = "bunifuFlatButton4";
			this.bunifuFlatButton4.Normalcolor = global::System.Drawing.Color.FromArgb(50, 50, 50);
			this.bunifuFlatButton4.OnHovercolor = global::System.Drawing.Color.FromArgb(40, 40, 40);
			this.bunifuFlatButton4.OnHoverTextColor = global::System.Drawing.Color.White;
			this.bunifuFlatButton4.selected = false;
			this.bunifuFlatButton4.Size = new global::System.Drawing.Size(135, 42);
			this.bunifuFlatButton4.TabIndex = 3;
			this.bunifuFlatButton4.Text = "Clear";
			this.bunifuFlatButton4.TextAlign = global::System.Drawing.ContentAlignment.MiddleCenter;
			this.bunifuFlatButton4.Textcolor = global::System.Drawing.Color.White;
			this.bunifuFlatButton4.TextFont = new global::System.Drawing.Font("Microsoft Sans Serif", 9.75f, global::System.Drawing.FontStyle.Regular, global::System.Drawing.GraphicsUnit.Point, 0);
			this.bunifuFlatButton4.Click += new global::System.EventHandler(this.bunifuFlatButton4_Click);
			this.bunifuFlatButton5.Activecolor = global::System.Drawing.Color.FromArgb(50, 50, 50);
			this.bunifuFlatButton5.BackColor = global::System.Drawing.Color.FromArgb(50, 50, 50);
			this.bunifuFlatButton5.BackgroundImageLayout = global::System.Windows.Forms.ImageLayout.Stretch;
			this.bunifuFlatButton5.BorderRadius = 0;
			this.bunifuFlatButton5.ButtonText = "Credits";
			this.bunifuFlatButton5.Cursor = global::System.Windows.Forms.Cursors.Hand;
			this.bunifuFlatButton5.DisabledColor = global::System.Drawing.Color.Gray;
			this.bunifuFlatButton5.Iconcolor = global::System.Drawing.Color.Transparent;
			this.bunifuFlatButton5.Iconimage = (global::System.Drawing.Image)componentResourceManager.GetObject("bunifuFlatButton5.Iconimage");
			this.bunifuFlatButton5.Iconimage_right = null;
			this.bunifuFlatButton5.Iconimage_right_Selected = null;
			this.bunifuFlatButton5.Iconimage_Selected = null;
			this.bunifuFlatButton5.IconMarginLeft = 0;
			this.bunifuFlatButton5.IconMarginRight = 0;
			this.bunifuFlatButton5.IconRightVisible = true;
			this.bunifuFlatButton5.IconRightZoom = 0.0;
			this.bunifuFlatButton5.IconVisible = false;
			this.bunifuFlatButton5.IconZoom = 90.0;
			this.bunifuFlatButton5.IsTab = false;
			this.bunifuFlatButton5.Location = new global::System.Drawing.Point(3, 215);
			this.bunifuFlatButton5.Name = "bunifuFlatButton5";
			this.bunifuFlatButton5.Normalcolor = global::System.Drawing.Color.FromArgb(50, 50, 50);
			this.bunifuFlatButton5.OnHovercolor = global::System.Drawing.Color.FromArgb(40, 40, 40);
			this.bunifuFlatButton5.OnHoverTextColor = global::System.Drawing.Color.White;
			this.bunifuFlatButton5.selected = false;
			this.bunifuFlatButton5.Size = new global::System.Drawing.Size(135, 42);
			this.bunifuFlatButton5.TabIndex = 4;
			this.bunifuFlatButton5.Text = "Credits";
			this.bunifuFlatButton5.TextAlign = global::System.Drawing.ContentAlignment.MiddleCenter;
			this.bunifuFlatButton5.Textcolor = global::System.Drawing.Color.White;
			this.bunifuFlatButton5.TextFont = new global::System.Drawing.Font("Microsoft Sans Serif", 9.75f, global::System.Drawing.FontStyle.Regular, global::System.Drawing.GraphicsUnit.Point, 0);
			this.bunifuFlatButton5.Click += new global::System.EventHandler(this.bunifuFlatButton5_Click);
			this.bunifuFlatButton6.Activecolor = global::System.Drawing.Color.FromArgb(50, 50, 50);
			this.bunifuFlatButton6.BackColor = global::System.Drawing.Color.FromArgb(50, 50, 50);
			this.bunifuFlatButton6.BackgroundImageLayout = global::System.Windows.Forms.ImageLayout.Stretch;
			this.bunifuFlatButton6.BorderRadius = 0;
			this.bunifuFlatButton6.ButtonText = "Discord Server";
			this.bunifuFlatButton6.Cursor = global::System.Windows.Forms.Cursors.Hand;
			this.bunifuFlatButton6.DisabledColor = global::System.Drawing.Color.Gray;
			this.bunifuFlatButton6.Iconcolor = global::System.Drawing.Color.Transparent;
			this.bunifuFlatButton6.Iconimage = (global::System.Drawing.Image)componentResourceManager.GetObject("bunifuFlatButton6.Iconimage");
			this.bunifuFlatButton6.Iconimage_right = null;
			this.bunifuFlatButton6.Iconimage_right_Selected = null;
			this.bunifuFlatButton6.Iconimage_Selected = null;
			this.bunifuFlatButton6.IconMarginLeft = 0;
			this.bunifuFlatButton6.IconMarginRight = 0;
			this.bunifuFlatButton6.IconRightVisible = true;
			this.bunifuFlatButton6.IconRightZoom = 0.0;
			this.bunifuFlatButton6.IconVisible = false;
			this.bunifuFlatButton6.IconZoom = 90.0;
			this.bunifuFlatButton6.IsTab = false;
			this.bunifuFlatButton6.Location = new global::System.Drawing.Point(3, 263);
			this.bunifuFlatButton6.Name = "bunifuFlatButton6";
			this.bunifuFlatButton6.Normalcolor = global::System.Drawing.Color.FromArgb(50, 50, 50);
			this.bunifuFlatButton6.OnHovercolor = global::System.Drawing.Color.FromArgb(40, 40, 40);
			this.bunifuFlatButton6.OnHoverTextColor = global::System.Drawing.Color.White;
			this.bunifuFlatButton6.selected = false;
			this.bunifuFlatButton6.Size = new global::System.Drawing.Size(135, 42);
			this.bunifuFlatButton6.TabIndex = 5;
			this.bunifuFlatButton6.Text = "Discord Server";
			this.bunifuFlatButton6.TextAlign = global::System.Drawing.ContentAlignment.MiddleCenter;
			this.bunifuFlatButton6.Textcolor = global::System.Drawing.Color.White;
			this.bunifuFlatButton6.TextFont = new global::System.Drawing.Font("Microsoft Sans Serif", 9.75f, global::System.Drawing.FontStyle.Regular, global::System.Drawing.GraphicsUnit.Point, 0);
			this.bunifuFlatButton6.Click += new global::System.EventHandler(this.bunifuFlatButton6_Click);
			this.pictureBox1.Image = (global::System.Drawing.Image)componentResourceManager.GetObject("pictureBox1.Image");
			this.pictureBox1.Location = new global::System.Drawing.Point(0, 0);
			this.pictureBox1.Name = "pictureBox1";
			this.pictureBox1.Size = new global::System.Drawing.Size(114, 40);
			this.pictureBox1.SizeMode = global::System.Windows.Forms.PictureBoxSizeMode.Zoom;
			this.pictureBox1.TabIndex = 0;
			this.pictureBox1.TabStop = false;
			this.bunifuImageButton1.BackColor = global::System.Drawing.Color.FromArgb(50, 50, 50);
			this.bunifuImageButton1.Image = (global::System.Drawing.Image)componentResourceManager.GetObject("bunifuImageButton1.Image");
			this.bunifuImageButton1.ImageActive = null;
			this.bunifuImageButton1.Location = new global::System.Drawing.Point(547, 0);
			this.bunifuImageButton1.Name = "bunifuImageButton1";
			this.bunifuImageButton1.Size = new global::System.Drawing.Size(43, 40);
			this.bunifuImageButton1.SizeMode = global::System.Windows.Forms.PictureBoxSizeMode.Zoom;
			this.bunifuImageButton1.TabIndex = 1;
			this.bunifuImageButton1.TabStop = false;
			this.bunifuImageButton1.Zoom = 10;
			this.bunifuImageButton1.Click += new global::System.EventHandler(this.bunifuImageButton1_Click);
			this.bunifuImageButton2.BackColor = global::System.Drawing.Color.FromArgb(50, 50, 50);
			this.bunifuImageButton2.Image = (global::System.Drawing.Image)componentResourceManager.GetObject("bunifuImageButton2.Image");
			this.bunifuImageButton2.ImageActive = null;
			this.bunifuImageButton2.Location = new global::System.Drawing.Point(503, 0);
			this.bunifuImageButton2.Name = "bunifuImageButton2";
			this.bunifuImageButton2.Size = new global::System.Drawing.Size(43, 40);
			this.bunifuImageButton2.SizeMode = global::System.Windows.Forms.PictureBoxSizeMode.Zoom;
			this.bunifuImageButton2.TabIndex = 2;
			this.bunifuImageButton2.TabStop = false;
			this.bunifuImageButton2.Zoom = 10;
			this.bunifuImageButton2.Click += new global::System.EventHandler(this.bunifuImageButton2_Click);
			base.AutoScaleDimensions = new global::System.Drawing.SizeF(6f, 13f);
			base.AutoScaleMode = global::System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = global::System.Drawing.Color.FromArgb(50, 50, 50);
			base.ClientSize = new global::System.Drawing.Size(591, 350);
			base.Controls.Add(this.panel3);
			base.Controls.Add(this.panel2);
			base.Controls.Add(this.panel1);
			base.FormBorderStyle = global::System.Windows.Forms.FormBorderStyle.None;
			base.Icon = (global::System.Drawing.Icon)componentResourceManager.GetObject("$this.Icon");
			base.MaximizeBox = false;
			base.Name = "Form1";
			base.StartPosition = global::System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "EmpathyUI";
			base.TopMost = true;
			this.panel1.ResumeLayout(false);
			this.panel2.ResumeLayout(false);
			this.panel3.ResumeLayout(false);
			((global::System.ComponentModel.ISupportInitialize)this.fastColoredTextBox1).EndInit();
			((global::System.ComponentModel.ISupportInitialize)this.pictureBox1).EndInit();
			((global::System.ComponentModel.ISupportInitialize)this.bunifuImageButton1).EndInit();
			((global::System.ComponentModel.ISupportInitialize)this.bunifuImageButton2).EndInit();
			base.ResumeLayout(false);
		}

		// Token: 0x04000001 RID: 1
		private global::System.ComponentModel.IContainer components = null;

		// Token: 0x04000002 RID: 2
		private global::Bunifu.Framework.UI.BunifuDragControl bunifuDragControl1;

		// Token: 0x04000003 RID: 3
		private global::System.Windows.Forms.Panel panel1;

		// Token: 0x04000004 RID: 4
		private global::System.Windows.Forms.ToolStripPanel BottomToolStripPanel;

		// Token: 0x04000005 RID: 5
		private global::System.Windows.Forms.ToolStripPanel TopToolStripPanel;

		// Token: 0x04000006 RID: 6
		private global::System.Windows.Forms.ToolStripPanel RightToolStripPanel;

		// Token: 0x04000007 RID: 7
		private global::System.Windows.Forms.ToolStripPanel LeftToolStripPanel;

		// Token: 0x04000008 RID: 8
		private global::System.Windows.Forms.ToolStripContentPanel ContentPanel;

		// Token: 0x04000009 RID: 9
		private global::System.Windows.Forms.Panel panel2;

		// Token: 0x0400000A RID: 10
		private global::Bunifu.Framework.UI.BunifuFlatButton bunifuFlatButton6;

		// Token: 0x0400000B RID: 11
		private global::Bunifu.Framework.UI.BunifuFlatButton bunifuFlatButton5;

		// Token: 0x0400000C RID: 12
		private global::Bunifu.Framework.UI.BunifuFlatButton bunifuFlatButton4;

		// Token: 0x0400000D RID: 13
		private global::Bunifu.Framework.UI.BunifuFlatButton bunifuFlatButton3;

		// Token: 0x0400000E RID: 14
		private global::Bunifu.Framework.UI.BunifuFlatButton bunifuFlatButton2;

		// Token: 0x0400000F RID: 15
		private global::Bunifu.Framework.UI.BunifuFlatButton bunifuFlatButton1;

		// Token: 0x04000010 RID: 16
		private global::System.Windows.Forms.Panel panel3;

		// Token: 0x04000011 RID: 17
		private global::FastColoredTextBoxNS.FastColoredTextBox fastColoredTextBox1;

		// Token: 0x04000012 RID: 18
		private global::Bunifu.Framework.UI.BunifuImageButton bunifuImageButton2;

		// Token: 0x04000013 RID: 19
		private global::Bunifu.Framework.UI.BunifuImageButton bunifuImageButton1;

		// Token: 0x04000014 RID: 20
		private global::System.Windows.Forms.PictureBox pictureBox1;
	}
}
