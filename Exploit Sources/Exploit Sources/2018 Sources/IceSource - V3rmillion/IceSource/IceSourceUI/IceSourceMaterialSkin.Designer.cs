namespace IceSourceUI
{
    partial class IceSourceMaterialSkin
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.TabSelector1 = new MaterialSkin.Controls.MaterialTabSelector();
            this.TabControl1 = new MaterialSkin.Controls.MaterialTabControl();
            this.Commands = new System.Windows.Forms.TabPage();
            this.SendBTN = new MaterialSkin.Controls.MaterialRaisedButton();
            this.CommandTextBox = new System.Windows.Forms.TextBox();
            this.CommandBox = new System.Windows.Forms.RichTextBox();
            this.QuickExe = new System.Windows.Forms.TabPage();
            this.JPValue = new System.Windows.Forms.NumericUpDown();
            this.WSValue = new System.Windows.Forms.NumericUpDown();
            this.SetJPBTN = new MaterialSkin.Controls.MaterialRaisedButton();
            this.SetWSBTN = new MaterialSkin.Controls.MaterialRaisedButton();
            this.SitBTN = new MaterialSkin.Controls.MaterialRaisedButton();
            this.SuicideBTN = new MaterialSkin.Controls.MaterialRaisedButton();
            this.FFBTN = new MaterialSkin.Controls.MaterialRaisedButton();
            this.BtoolsBTN = new MaterialSkin.Controls.MaterialRaisedButton();
            this.LuaC = new System.Windows.Forms.TabPage();
            this.ClearBTN = new MaterialSkin.Controls.MaterialRaisedButton();
            this.ExecuteBTN = new MaterialSkin.Controls.MaterialRaisedButton();
            this.OpenBTN = new MaterialSkin.Controls.MaterialRaisedButton();
            this.ScriptBox = new System.Windows.Forms.RichTextBox();
            this.InjectBTN = new MaterialSkin.Controls.MaterialRaisedButton();
            this.TopCheck = new MaterialSkin.Controls.MaterialCheckBox();
            this.ThemeBTN = new MaterialSkin.Controls.MaterialRaisedButton();
            this.toolStripContainer1 = new System.Windows.Forms.ToolStripContainer();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.changeUIToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.defaultToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.metroModernUIToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.TabControl1.SuspendLayout();
            this.Commands.SuspendLayout();
            this.QuickExe.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.JPValue)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.WSValue)).BeginInit();
            this.LuaC.SuspendLayout();
            this.toolStripContainer1.TopToolStripPanel.SuspendLayout();
            this.toolStripContainer1.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // TabSelector1
            // 
            this.TabSelector1.BaseTabControl = this.TabControl1;
            this.TabSelector1.Depth = 0;
            this.TabSelector1.Location = new System.Drawing.Point(-1, 63);
            this.TabSelector1.MouseState = MaterialSkin.MouseState.HOVER;
            this.TabSelector1.Name = "TabSelector1";
            this.TabSelector1.Size = new System.Drawing.Size(342, 23);
            this.TabSelector1.TabIndex = 0;
            this.TabSelector1.Text = "TabSelector";
            // 
            // TabControl1
            // 
            this.TabControl1.Controls.Add(this.Commands);
            this.TabControl1.Controls.Add(this.QuickExe);
            this.TabControl1.Controls.Add(this.LuaC);
            this.TabControl1.Depth = 0;
            this.TabControl1.Location = new System.Drawing.Point(-1, 92);
            this.TabControl1.MouseState = MaterialSkin.MouseState.HOVER;
            this.TabControl1.Name = "TabControl1";
            this.TabControl1.SelectedIndex = 0;
            this.TabControl1.Size = new System.Drawing.Size(342, 182);
            this.TabControl1.TabIndex = 1;
            // 
            // Commands
            // 
            this.Commands.Controls.Add(this.SendBTN);
            this.Commands.Controls.Add(this.CommandTextBox);
            this.Commands.Controls.Add(this.CommandBox);
            this.Commands.Location = new System.Drawing.Point(4, 22);
            this.Commands.Name = "Commands";
            this.Commands.Padding = new System.Windows.Forms.Padding(3);
            this.Commands.Size = new System.Drawing.Size(334, 156);
            this.Commands.TabIndex = 0;
            this.Commands.Text = "Commands";
            this.Commands.UseVisualStyleBackColor = true;
            // 
            // SendBTN
            // 
            this.SendBTN.Depth = 0;
            this.SendBTN.Location = new System.Drawing.Point(259, 125);
            this.SendBTN.MouseState = MaterialSkin.MouseState.HOVER;
            this.SendBTN.Name = "SendBTN";
            this.SendBTN.Primary = true;
            this.SendBTN.Size = new System.Drawing.Size(75, 25);
            this.SendBTN.TabIndex = 2;
            this.SendBTN.Text = "Send";
            this.SendBTN.UseVisualStyleBackColor = true;
            this.SendBTN.Click += new System.EventHandler(this.SendBTN_Click);
            // 
            // CommandTextBox
            // 
            this.CommandTextBox.BackColor = System.Drawing.SystemColors.Control;
            this.CommandTextBox.Font = new System.Drawing.Font("Lucida Sans Unicode", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CommandTextBox.Location = new System.Drawing.Point(6, 125);
            this.CommandTextBox.Name = "CommandTextBox";
            this.CommandTextBox.Size = new System.Drawing.Size(238, 24);
            this.CommandTextBox.TabIndex = 1;
            this.CommandTextBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.CommandTextBox_KeyDown);
            // 
            // CommandBox
            // 
            this.CommandBox.BackColor = System.Drawing.SystemColors.Control;
            this.CommandBox.Font = new System.Drawing.Font("Lucida Sans Unicode", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CommandBox.Location = new System.Drawing.Point(0, 6);
            this.CommandBox.Name = "CommandBox";
            this.CommandBox.ReadOnly = true;
            this.CommandBox.Size = new System.Drawing.Size(334, 113);
            this.CommandBox.TabIndex = 0;
            this.CommandBox.Text = "Welcome to Ice\nType cmds to see the commands\ncredits to see the credits\n[p] = pla" +
    "yer, [#] = number\n";
            this.CommandBox.TextChanged += new System.EventHandler(this.CommandBox_TextChanged);
            // 
            // QuickExe
            // 
            this.QuickExe.Controls.Add(this.JPValue);
            this.QuickExe.Controls.Add(this.WSValue);
            this.QuickExe.Controls.Add(this.SetJPBTN);
            this.QuickExe.Controls.Add(this.SetWSBTN);
            this.QuickExe.Controls.Add(this.SitBTN);
            this.QuickExe.Controls.Add(this.SuicideBTN);
            this.QuickExe.Controls.Add(this.FFBTN);
            this.QuickExe.Controls.Add(this.BtoolsBTN);
            this.QuickExe.Location = new System.Drawing.Point(4, 22);
            this.QuickExe.Name = "QuickExe";
            this.QuickExe.Padding = new System.Windows.Forms.Padding(3);
            this.QuickExe.Size = new System.Drawing.Size(334, 156);
            this.QuickExe.TabIndex = 1;
            this.QuickExe.Text = "Quick Exe";
            this.QuickExe.UseVisualStyleBackColor = true;
            // 
            // JPValue
            // 
            this.JPValue.Location = new System.Drawing.Point(189, 80);
            this.JPValue.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
            this.JPValue.Name = "JPValue";
            this.JPValue.Size = new System.Drawing.Size(75, 20);
            this.JPValue.TabIndex = 7;
            this.JPValue.Value = new decimal(new int[] {
            50,
            0,
            0,
            0});
            // 
            // WSValue
            // 
            this.WSValue.Location = new System.Drawing.Point(189, 22);
            this.WSValue.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
            this.WSValue.Name = "WSValue";
            this.WSValue.Size = new System.Drawing.Size(75, 20);
            this.WSValue.TabIndex = 6;
            this.WSValue.Value = new decimal(new int[] {
            16,
            0,
            0,
            0});
            // 
            // SetJPBTN
            // 
            this.SetJPBTN.Depth = 0;
            this.SetJPBTN.Location = new System.Drawing.Point(189, 106);
            this.SetJPBTN.MouseState = MaterialSkin.MouseState.HOVER;
            this.SetJPBTN.Name = "SetJPBTN";
            this.SetJPBTN.Primary = true;
            this.SetJPBTN.Size = new System.Drawing.Size(75, 23);
            this.SetJPBTN.TabIndex = 5;
            this.SetJPBTN.Text = "Set JP";
            this.SetJPBTN.UseVisualStyleBackColor = true;
            this.SetJPBTN.Click += new System.EventHandler(this.SetJPBTN_Click);
            // 
            // SetWSBTN
            // 
            this.SetWSBTN.Depth = 0;
            this.SetWSBTN.Location = new System.Drawing.Point(189, 48);
            this.SetWSBTN.MouseState = MaterialSkin.MouseState.HOVER;
            this.SetWSBTN.Name = "SetWSBTN";
            this.SetWSBTN.Primary = true;
            this.SetWSBTN.Size = new System.Drawing.Size(75, 23);
            this.SetWSBTN.TabIndex = 4;
            this.SetWSBTN.Text = "Set WS";
            this.SetWSBTN.UseVisualStyleBackColor = true;
            this.SetWSBTN.Click += new System.EventHandler(this.SetWSBTN_Click);
            // 
            // SitBTN
            // 
            this.SitBTN.Depth = 0;
            this.SitBTN.Location = new System.Drawing.Point(18, 106);
            this.SitBTN.MouseState = MaterialSkin.MouseState.HOVER;
            this.SitBTN.Name = "SitBTN";
            this.SitBTN.Primary = true;
            this.SitBTN.Size = new System.Drawing.Size(75, 23);
            this.SitBTN.TabIndex = 3;
            this.SitBTN.Text = "Sit";
            this.SitBTN.UseVisualStyleBackColor = true;
            this.SitBTN.Click += new System.EventHandler(this.SitBTN_Click);
            // 
            // SuicideBTN
            // 
            this.SuicideBTN.Depth = 0;
            this.SuicideBTN.Location = new System.Drawing.Point(18, 77);
            this.SuicideBTN.MouseState = MaterialSkin.MouseState.HOVER;
            this.SuicideBTN.Name = "SuicideBTN";
            this.SuicideBTN.Primary = true;
            this.SuicideBTN.Size = new System.Drawing.Size(75, 23);
            this.SuicideBTN.TabIndex = 2;
            this.SuicideBTN.Text = "Suicide";
            this.SuicideBTN.UseVisualStyleBackColor = true;
            this.SuicideBTN.Click += new System.EventHandler(this.SuicideBTN_Click);
            // 
            // FFBTN
            // 
            this.FFBTN.Depth = 0;
            this.FFBTN.Location = new System.Drawing.Point(18, 48);
            this.FFBTN.MouseState = MaterialSkin.MouseState.HOVER;
            this.FFBTN.Name = "FFBTN";
            this.FFBTN.Primary = true;
            this.FFBTN.Size = new System.Drawing.Size(75, 23);
            this.FFBTN.TabIndex = 1;
            this.FFBTN.Text = "FF";
            this.FFBTN.UseVisualStyleBackColor = true;
            this.FFBTN.Click += new System.EventHandler(this.FFBTN_Click);
            // 
            // BtoolsBTN
            // 
            this.BtoolsBTN.Depth = 0;
            this.BtoolsBTN.Location = new System.Drawing.Point(18, 19);
            this.BtoolsBTN.MouseState = MaterialSkin.MouseState.HOVER;
            this.BtoolsBTN.Name = "BtoolsBTN";
            this.BtoolsBTN.Primary = true;
            this.BtoolsBTN.Size = new System.Drawing.Size(75, 23);
            this.BtoolsBTN.TabIndex = 0;
            this.BtoolsBTN.Text = "Btools";
            this.BtoolsBTN.UseVisualStyleBackColor = true;
            this.BtoolsBTN.Click += new System.EventHandler(this.BtoolsBTN_Click);
            // 
            // LuaC
            // 
            this.LuaC.Controls.Add(this.ClearBTN);
            this.LuaC.Controls.Add(this.ExecuteBTN);
            this.LuaC.Controls.Add(this.OpenBTN);
            this.LuaC.Controls.Add(this.ScriptBox);
            this.LuaC.Location = new System.Drawing.Point(4, 22);
            this.LuaC.Name = "LuaC";
            this.LuaC.Size = new System.Drawing.Size(334, 156);
            this.LuaC.TabIndex = 2;
            this.LuaC.Text = "Lua C";
            this.LuaC.UseVisualStyleBackColor = true;
            // 
            // ClearBTN
            // 
            this.ClearBTN.Depth = 0;
            this.ClearBTN.Location = new System.Drawing.Point(225, 127);
            this.ClearBTN.MouseState = MaterialSkin.MouseState.HOVER;
            this.ClearBTN.Name = "ClearBTN";
            this.ClearBTN.Primary = true;
            this.ClearBTN.Size = new System.Drawing.Size(75, 23);
            this.ClearBTN.TabIndex = 3;
            this.ClearBTN.Text = "Clear";
            this.ClearBTN.UseVisualStyleBackColor = true;
            this.ClearBTN.Click += new System.EventHandler(this.ClearBTN_Click);
            // 
            // ExecuteBTN
            // 
            this.ExecuteBTN.Depth = 0;
            this.ExecuteBTN.Location = new System.Drawing.Point(117, 127);
            this.ExecuteBTN.MouseState = MaterialSkin.MouseState.HOVER;
            this.ExecuteBTN.Name = "ExecuteBTN";
            this.ExecuteBTN.Primary = true;
            this.ExecuteBTN.Size = new System.Drawing.Size(75, 23);
            this.ExecuteBTN.TabIndex = 2;
            this.ExecuteBTN.Text = "Execute";
            this.ExecuteBTN.UseVisualStyleBackColor = true;
            this.ExecuteBTN.Click += new System.EventHandler(this.ExecuteBTN_Click);
            // 
            // OpenBTN
            // 
            this.OpenBTN.Depth = 0;
            this.OpenBTN.Location = new System.Drawing.Point(9, 127);
            this.OpenBTN.MouseState = MaterialSkin.MouseState.HOVER;
            this.OpenBTN.Name = "OpenBTN";
            this.OpenBTN.Primary = true;
            this.OpenBTN.Size = new System.Drawing.Size(75, 23);
            this.OpenBTN.TabIndex = 1;
            this.OpenBTN.Text = "Open";
            this.OpenBTN.UseVisualStyleBackColor = true;
            this.OpenBTN.Click += new System.EventHandler(this.OpenBTN_Click);
            // 
            // ScriptBox
            // 
            this.ScriptBox.Location = new System.Drawing.Point(0, 3);
            this.ScriptBox.Name = "ScriptBox";
            this.ScriptBox.Size = new System.Drawing.Size(338, 118);
            this.ScriptBox.TabIndex = 0;
            this.ScriptBox.Text = "";
            // 
            // InjectBTN
            // 
            this.InjectBTN.Depth = 0;
            this.InjectBTN.Location = new System.Drawing.Point(140, 272);
            this.InjectBTN.MouseState = MaterialSkin.MouseState.HOVER;
            this.InjectBTN.Name = "InjectBTN";
            this.InjectBTN.Primary = true;
            this.InjectBTN.Size = new System.Drawing.Size(75, 23);
            this.InjectBTN.TabIndex = 2;
            this.InjectBTN.Text = "Inject";
            this.InjectBTN.UseVisualStyleBackColor = true;
            this.InjectBTN.Click += new System.EventHandler(this.InjectBTN_Click);
            // 
            // TopCheck
            // 
            this.TopCheck.AutoSize = true;
            this.TopCheck.Checked = true;
            this.TopCheck.CheckState = System.Windows.Forms.CheckState.Checked;
            this.TopCheck.Depth = 0;
            this.TopCheck.Font = new System.Drawing.Font("Roboto", 10F);
            this.TopCheck.Location = new System.Drawing.Point(3, 269);
            this.TopCheck.Margin = new System.Windows.Forms.Padding(0);
            this.TopCheck.MouseLocation = new System.Drawing.Point(-1, -1);
            this.TopCheck.MouseState = MaterialSkin.MouseState.HOVER;
            this.TopCheck.Name = "TopCheck";
            this.TopCheck.Ripple = true;
            this.TopCheck.Size = new System.Drawing.Size(120, 30);
            this.TopCheck.TabIndex = 4;
            this.TopCheck.Text = "Always on Top";
            this.TopCheck.UseVisualStyleBackColor = true;
            this.TopCheck.CheckedChanged += new System.EventHandler(this.TopCheck_CheckedChanged);
            // 
            // ThemeBTN
            // 
            this.ThemeBTN.Depth = 0;
            this.ThemeBTN.Location = new System.Drawing.Point(244, 272);
            this.ThemeBTN.MouseState = MaterialSkin.MouseState.HOVER;
            this.ThemeBTN.Name = "ThemeBTN";
            this.ThemeBTN.Primary = true;
            this.ThemeBTN.Size = new System.Drawing.Size(75, 23);
            this.ThemeBTN.TabIndex = 5;
            this.ThemeBTN.Text = "Theme";
            this.ThemeBTN.UseVisualStyleBackColor = true;
            this.ThemeBTN.Click += new System.EventHandler(this.ThemeBTN_Click);
            // 
            // toolStripContainer1
            // 
            this.toolStripContainer1.BottomToolStripPanelVisible = false;
            // 
            // toolStripContainer1.ContentPanel
            // 
            this.toolStripContainer1.ContentPanel.Size = new System.Drawing.Size(82, 3);
            this.toolStripContainer1.LeftToolStripPanelVisible = false;
            this.toolStripContainer1.Location = new System.Drawing.Point(255, 30);
            this.toolStripContainer1.Name = "toolStripContainer1";
            this.toolStripContainer1.RightToolStripPanelVisible = false;
            this.toolStripContainer1.Size = new System.Drawing.Size(82, 27);
            this.toolStripContainer1.TabIndex = 6;
            this.toolStripContainer1.Text = "toolStripContainer1";
            // 
            // toolStripContainer1.TopToolStripPanel
            // 
            this.toolStripContainer1.TopToolStripPanel.Controls.Add(this.menuStrip1);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Dock = System.Windows.Forms.DockStyle.None;
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.changeUIToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(82, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // changeUIToolStripMenuItem
            // 
            this.changeUIToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.defaultToolStripMenuItem,
            this.metroModernUIToolStripMenuItem});
            this.changeUIToolStripMenuItem.Name = "changeUIToolStripMenuItem";
            this.changeUIToolStripMenuItem.Size = new System.Drawing.Size(74, 20);
            this.changeUIToolStripMenuItem.Text = "Change UI";
            // 
            // defaultToolStripMenuItem
            // 
            this.defaultToolStripMenuItem.Name = "defaultToolStripMenuItem";
            this.defaultToolStripMenuItem.Size = new System.Drawing.Size(159, 22);
            this.defaultToolStripMenuItem.Text = "Default";
            this.defaultToolStripMenuItem.Click += new System.EventHandler(this.defaultToolStripMenuItem_Click);
            // 
            // metroModernUIToolStripMenuItem
            // 
            this.metroModernUIToolStripMenuItem.Name = "metroModernUIToolStripMenuItem";
            this.metroModernUIToolStripMenuItem.Size = new System.Drawing.Size(159, 22);
            this.metroModernUIToolStripMenuItem.Text = "MetroModernUI";
            this.metroModernUIToolStripMenuItem.Click += new System.EventHandler(this.metroModernUIToolStripMenuItem_Click);
            // 
            // IceSourceMaterialSkin
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(340, 300);
            this.Controls.Add(this.toolStripContainer1);
            this.Controls.Add(this.ThemeBTN);
            this.Controls.Add(this.TopCheck);
            this.Controls.Add(this.InjectBTN);
            this.Controls.Add(this.TabControl1);
            this.Controls.Add(this.TabSelector1);
            this.MainMenuStrip = this.menuStrip1;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "IceSourceMaterialSkin";
            this.ShowIcon = false;
            this.Sizable = false;
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Ice Source Light Theme";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.IceSourceMaterialSkin_FormClosed);
            this.TabControl1.ResumeLayout(false);
            this.Commands.ResumeLayout(false);
            this.Commands.PerformLayout();
            this.QuickExe.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.JPValue)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.WSValue)).EndInit();
            this.LuaC.ResumeLayout(false);
            this.toolStripContainer1.TopToolStripPanel.ResumeLayout(false);
            this.toolStripContainer1.TopToolStripPanel.PerformLayout();
            this.toolStripContainer1.ResumeLayout(false);
            this.toolStripContainer1.PerformLayout();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private MaterialSkin.Controls.MaterialTabSelector TabSelector1;
        private MaterialSkin.Controls.MaterialTabControl TabControl1;
        private System.Windows.Forms.TabPage Commands;
        private System.Windows.Forms.TabPage QuickExe;
        private System.Windows.Forms.TabPage LuaC;
        private MaterialSkin.Controls.MaterialRaisedButton InjectBTN;
        private MaterialSkin.Controls.MaterialRaisedButton SendBTN;
        private System.Windows.Forms.TextBox CommandTextBox;
        private System.Windows.Forms.RichTextBox CommandBox;
        private MaterialSkin.Controls.MaterialCheckBox TopCheck;
        private MaterialSkin.Controls.MaterialRaisedButton ThemeBTN;
        private System.Windows.Forms.ToolStripContainer toolStripContainer1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem changeUIToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem defaultToolStripMenuItem;
        private MaterialSkin.Controls.MaterialRaisedButton FFBTN;
        private MaterialSkin.Controls.MaterialRaisedButton BtoolsBTN;
        private MaterialSkin.Controls.MaterialRaisedButton SitBTN;
        private MaterialSkin.Controls.MaterialRaisedButton SuicideBTN;
        private MaterialSkin.Controls.MaterialRaisedButton SetJPBTN;
        private MaterialSkin.Controls.MaterialRaisedButton SetWSBTN;
        private System.Windows.Forms.NumericUpDown JPValue;
        private System.Windows.Forms.NumericUpDown WSValue;
        private MaterialSkin.Controls.MaterialRaisedButton ClearBTN;
        private MaterialSkin.Controls.MaterialRaisedButton ExecuteBTN;
        private MaterialSkin.Controls.MaterialRaisedButton OpenBTN;
        private System.Windows.Forms.RichTextBox ScriptBox;
        private System.Windows.Forms.ToolStripMenuItem metroModernUIToolStripMenuItem;
    }
}