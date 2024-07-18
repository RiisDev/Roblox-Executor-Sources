namespace IceSourceUI
{
    partial class IceSourceForm
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
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.CMDS = new System.Windows.Forms.TabPage();
            this.CmdBox = new System.Windows.Forms.RichTextBox();
            this.Send = new System.Windows.Forms.Button();
            this.CmdTextBox = new System.Windows.Forms.TextBox();
            this.QuickExe = new System.Windows.Forms.TabPage();
            this.JumpPowerValue = new System.Windows.Forms.NumericUpDown();
            this.JP = new System.Windows.Forms.Button();
            this.Sit = new System.Windows.Forms.Button();
            this.Suicide = new System.Windows.Forms.Button();
            this.WalkSpeedValue = new System.Windows.Forms.NumericUpDown();
            this.Ws = new System.Windows.Forms.Button();
            this.FF = new System.Windows.Forms.Button();
            this.Btools = new System.Windows.Forms.Button();
            this.LuaC = new System.Windows.Forms.TabPage();
            this.Open = new System.Windows.Forms.Button();
            this.Clear = new System.Windows.Forms.Button();
            this.Execute = new System.Windows.Forms.Button();
            this.LuaCBox = new System.Windows.Forms.RichTextBox();
            this.Inject = new System.Windows.Forms.Button();
            this.TopCheck = new System.Windows.Forms.CheckBox();
            this.Theme = new System.Windows.Forms.Button();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.changeUIToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.materialSkinUIToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.metroModernUIToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tabControl1.SuspendLayout();
            this.CMDS.SuspendLayout();
            this.QuickExe.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.JumpPowerValue)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.WalkSpeedValue)).BeginInit();
            this.LuaC.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.CMDS);
            this.tabControl1.Controls.Add(this.QuickExe);
            this.tabControl1.Controls.Add(this.LuaC);
            this.tabControl1.Location = new System.Drawing.Point(12, 27);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(260, 237);
            this.tabControl1.TabIndex = 0;
            // 
            // CMDS
            // 
            this.CMDS.BackColor = System.Drawing.SystemColors.MenuText;
            this.CMDS.Controls.Add(this.CmdBox);
            this.CMDS.Controls.Add(this.Send);
            this.CMDS.Controls.Add(this.CmdTextBox);
            this.CMDS.Location = new System.Drawing.Point(4, 22);
            this.CMDS.Name = "CMDS";
            this.CMDS.Padding = new System.Windows.Forms.Padding(3);
            this.CMDS.Size = new System.Drawing.Size(252, 211);
            this.CMDS.TabIndex = 1;
            this.CMDS.Text = "CMDS";
            // 
            // CmdBox
            // 
            this.CmdBox.BackColor = System.Drawing.SystemColors.MenuText;
            this.CmdBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CmdBox.ForeColor = System.Drawing.SystemColors.Window;
            this.CmdBox.Location = new System.Drawing.Point(6, 6);
            this.CmdBox.Name = "CmdBox";
            this.CmdBox.ReadOnly = true;
            this.CmdBox.Size = new System.Drawing.Size(240, 156);
            this.CmdBox.TabIndex = 2;
            this.CmdBox.Text = "Welcome to Ice\nType cmds to see the commands\ncredits to see the credits\n[p] = pla" +
    "yer, [#] = number";
            this.CmdBox.TextChanged += new System.EventHandler(this.CmdBox_TextChanged);
            // 
            // Send
            // 
            this.Send.BackColor = System.Drawing.SystemColors.MenuText;
            this.Send.ForeColor = System.Drawing.SystemColors.Window;
            this.Send.Location = new System.Drawing.Point(171, 174);
            this.Send.Name = "Send";
            this.Send.Size = new System.Drawing.Size(75, 29);
            this.Send.TabIndex = 1;
            this.Send.Text = "Send";
            this.Send.UseVisualStyleBackColor = false;
            this.Send.Click += new System.EventHandler(this.Send_Click);
            // 
            // CmdTextBox
            // 
            this.CmdTextBox.BackColor = System.Drawing.SystemColors.MenuText;
            this.CmdTextBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CmdTextBox.ForeColor = System.Drawing.SystemColors.Window;
            this.CmdTextBox.Location = new System.Drawing.Point(6, 174);
            this.CmdTextBox.Name = "CmdTextBox";
            this.CmdTextBox.Size = new System.Drawing.Size(159, 29);
            this.CmdTextBox.TabIndex = 0;
            this.CmdTextBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.CmdTextBox_KeyDown);
            // 
            // QuickExe
            // 
            this.QuickExe.BackColor = System.Drawing.SystemColors.MenuText;
            this.QuickExe.Controls.Add(this.JumpPowerValue);
            this.QuickExe.Controls.Add(this.JP);
            this.QuickExe.Controls.Add(this.Sit);
            this.QuickExe.Controls.Add(this.Suicide);
            this.QuickExe.Controls.Add(this.WalkSpeedValue);
            this.QuickExe.Controls.Add(this.Ws);
            this.QuickExe.Controls.Add(this.FF);
            this.QuickExe.Controls.Add(this.Btools);
            this.QuickExe.Location = new System.Drawing.Point(4, 22);
            this.QuickExe.Name = "QuickExe";
            this.QuickExe.Size = new System.Drawing.Size(252, 211);
            this.QuickExe.TabIndex = 2;
            this.QuickExe.Text = "QuickExe";
            // 
            // JumpPowerValue
            // 
            this.JumpPowerValue.BackColor = System.Drawing.SystemColors.MenuText;
            this.JumpPowerValue.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.JumpPowerValue.ForeColor = System.Drawing.SystemColors.Window;
            this.JumpPowerValue.Location = new System.Drawing.Point(162, 122);
            this.JumpPowerValue.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
            this.JumpPowerValue.Name = "JumpPowerValue";
            this.JumpPowerValue.Size = new System.Drawing.Size(75, 20);
            this.JumpPowerValue.TabIndex = 7;
            this.JumpPowerValue.Value = new decimal(new int[] {
            50,
            0,
            0,
            0});
            // 
            // JP
            // 
            this.JP.BackColor = System.Drawing.SystemColors.MenuText;
            this.JP.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.JP.ForeColor = System.Drawing.SystemColors.Window;
            this.JP.Location = new System.Drawing.Point(162, 162);
            this.JP.Name = "JP";
            this.JP.Size = new System.Drawing.Size(75, 23);
            this.JP.TabIndex = 6;
            this.JP.Text = "Set JP";
            this.JP.UseVisualStyleBackColor = false;
            this.JP.Click += new System.EventHandler(this.JP_Click);
            // 
            // Sit
            // 
            this.Sit.BackColor = System.Drawing.SystemColors.MenuText;
            this.Sit.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Sit.ForeColor = System.Drawing.SystemColors.Window;
            this.Sit.Location = new System.Drawing.Point(12, 162);
            this.Sit.Name = "Sit";
            this.Sit.Size = new System.Drawing.Size(75, 23);
            this.Sit.TabIndex = 5;
            this.Sit.Text = "Sit";
            this.Sit.UseVisualStyleBackColor = false;
            this.Sit.Click += new System.EventHandler(this.Sit_Click);
            // 
            // Suicide
            // 
            this.Suicide.BackColor = System.Drawing.SystemColors.MenuText;
            this.Suicide.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Suicide.ForeColor = System.Drawing.SystemColors.Window;
            this.Suicide.Location = new System.Drawing.Point(12, 119);
            this.Suicide.Name = "Suicide";
            this.Suicide.Size = new System.Drawing.Size(75, 23);
            this.Suicide.TabIndex = 4;
            this.Suicide.Text = "Suicide";
            this.Suicide.UseVisualStyleBackColor = false;
            this.Suicide.Click += new System.EventHandler(this.Suicide_Click);
            // 
            // WalkSpeedValue
            // 
            this.WalkSpeedValue.BackColor = System.Drawing.SystemColors.MenuText;
            this.WalkSpeedValue.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.WalkSpeedValue.ForeColor = System.Drawing.SystemColors.Window;
            this.WalkSpeedValue.Location = new System.Drawing.Point(162, 41);
            this.WalkSpeedValue.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
            this.WalkSpeedValue.Name = "WalkSpeedValue";
            this.WalkSpeedValue.Size = new System.Drawing.Size(75, 20);
            this.WalkSpeedValue.TabIndex = 3;
            this.WalkSpeedValue.Value = new decimal(new int[] {
            16,
            0,
            0,
            0});
            // 
            // Ws
            // 
            this.Ws.BackColor = System.Drawing.SystemColors.MenuText;
            this.Ws.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Ws.ForeColor = System.Drawing.SystemColors.Window;
            this.Ws.Location = new System.Drawing.Point(162, 79);
            this.Ws.Name = "Ws";
            this.Ws.Size = new System.Drawing.Size(75, 23);
            this.Ws.TabIndex = 2;
            this.Ws.Text = "Set WS";
            this.Ws.UseVisualStyleBackColor = false;
            this.Ws.Click += new System.EventHandler(this.Ws_Click);
            // 
            // FF
            // 
            this.FF.BackColor = System.Drawing.SystemColors.MenuText;
            this.FF.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FF.ForeColor = System.Drawing.SystemColors.Window;
            this.FF.Location = new System.Drawing.Point(12, 79);
            this.FF.Name = "FF";
            this.FF.Size = new System.Drawing.Size(75, 23);
            this.FF.TabIndex = 1;
            this.FF.Text = "FF";
            this.FF.UseVisualStyleBackColor = false;
            this.FF.Click += new System.EventHandler(this.FF_Click);
            // 
            // Btools
            // 
            this.Btools.BackColor = System.Drawing.SystemColors.MenuText;
            this.Btools.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Btools.ForeColor = System.Drawing.SystemColors.Window;
            this.Btools.Location = new System.Drawing.Point(12, 41);
            this.Btools.Name = "Btools";
            this.Btools.Size = new System.Drawing.Size(75, 23);
            this.Btools.TabIndex = 0;
            this.Btools.Text = "Btools";
            this.Btools.UseVisualStyleBackColor = false;
            this.Btools.Click += new System.EventHandler(this.Btools_Click);
            // 
            // LuaC
            // 
            this.LuaC.BackColor = System.Drawing.SystemColors.MenuText;
            this.LuaC.Controls.Add(this.Open);
            this.LuaC.Controls.Add(this.Clear);
            this.LuaC.Controls.Add(this.Execute);
            this.LuaC.Controls.Add(this.LuaCBox);
            this.LuaC.ForeColor = System.Drawing.SystemColors.Window;
            this.LuaC.Location = new System.Drawing.Point(4, 22);
            this.LuaC.Name = "LuaC";
            this.LuaC.Padding = new System.Windows.Forms.Padding(3);
            this.LuaC.Size = new System.Drawing.Size(252, 211);
            this.LuaC.TabIndex = 0;
            this.LuaC.Text = "Lua C";
            // 
            // Open
            // 
            this.Open.BackColor = System.Drawing.SystemColors.MenuText;
            this.Open.ForeColor = System.Drawing.SystemColors.Window;
            this.Open.Location = new System.Drawing.Point(11, 169);
            this.Open.Name = "Open";
            this.Open.Size = new System.Drawing.Size(56, 36);
            this.Open.TabIndex = 3;
            this.Open.Text = "Open";
            this.Open.UseVisualStyleBackColor = false;
            this.Open.Click += new System.EventHandler(this.Open_Click);
            // 
            // Clear
            // 
            this.Clear.BackColor = System.Drawing.SystemColors.MenuText;
            this.Clear.ForeColor = System.Drawing.SystemColors.Window;
            this.Clear.Location = new System.Drawing.Point(165, 169);
            this.Clear.Name = "Clear";
            this.Clear.Size = new System.Drawing.Size(81, 36);
            this.Clear.TabIndex = 2;
            this.Clear.Text = "Clear";
            this.Clear.UseVisualStyleBackColor = false;
            this.Clear.Click += new System.EventHandler(this.Clear_Click);
            // 
            // Execute
            // 
            this.Execute.BackColor = System.Drawing.SystemColors.MenuText;
            this.Execute.ForeColor = System.Drawing.SystemColors.Window;
            this.Execute.Location = new System.Drawing.Point(84, 169);
            this.Execute.Name = "Execute";
            this.Execute.Size = new System.Drawing.Size(64, 36);
            this.Execute.TabIndex = 1;
            this.Execute.Text = "Execute";
            this.Execute.UseVisualStyleBackColor = false;
            this.Execute.Click += new System.EventHandler(this.Execute_Click);
            // 
            // LuaCBox
            // 
            this.LuaCBox.BackColor = System.Drawing.SystemColors.MenuText;
            this.LuaCBox.ForeColor = System.Drawing.SystemColors.Window;
            this.LuaCBox.Location = new System.Drawing.Point(6, 6);
            this.LuaCBox.Name = "LuaCBox";
            this.LuaCBox.Size = new System.Drawing.Size(240, 157);
            this.LuaCBox.TabIndex = 0;
            this.LuaCBox.Text = "";
            // 
            // Inject
            // 
            this.Inject.BackColor = System.Drawing.SystemColors.MenuText;
            this.Inject.ForeColor = System.Drawing.SystemColors.Window;
            this.Inject.Location = new System.Drawing.Point(127, 271);
            this.Inject.Name = "Inject";
            this.Inject.Size = new System.Drawing.Size(75, 23);
            this.Inject.TabIndex = 1;
            this.Inject.Text = "Inject";
            this.Inject.UseVisualStyleBackColor = false;
            this.Inject.Click += new System.EventHandler(this.Inject_Click);
            // 
            // TopCheck
            // 
            this.TopCheck.AutoSize = true;
            this.TopCheck.BackColor = System.Drawing.SystemColors.MenuText;
            this.TopCheck.Checked = true;
            this.TopCheck.CheckState = System.Windows.Forms.CheckState.Checked;
            this.TopCheck.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TopCheck.ForeColor = System.Drawing.SystemColors.Window;
            this.TopCheck.Location = new System.Drawing.Point(12, 277);
            this.TopCheck.Name = "TopCheck";
            this.TopCheck.Size = new System.Drawing.Size(109, 17);
            this.TopCheck.TabIndex = 2;
            this.TopCheck.Text = "Always on Top";
            this.TopCheck.UseVisualStyleBackColor = false;
            this.TopCheck.CheckedChanged += new System.EventHandler(this.TopCheck_CheckedChanged);
            // 
            // Theme
            // 
            this.Theme.BackColor = System.Drawing.SystemColors.MenuText;
            this.Theme.ForeColor = System.Drawing.SystemColors.Window;
            this.Theme.Location = new System.Drawing.Point(208, 271);
            this.Theme.Name = "Theme";
            this.Theme.Size = new System.Drawing.Size(75, 23);
            this.Theme.TabIndex = 3;
            this.Theme.Text = "Theme";
            this.Theme.UseVisualStyleBackColor = false;
            this.Theme.Click += new System.EventHandler(this.Theme_Click);
            // 
            // menuStrip1
            // 
            this.menuStrip1.BackColor = System.Drawing.Color.Black;
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.changeUIToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(284, 24);
            this.menuStrip1.TabIndex = 4;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // changeUIToolStripMenuItem
            // 
            this.changeUIToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.materialSkinUIToolStripMenuItem,
            this.metroModernUIToolStripMenuItem});
            this.changeUIToolStripMenuItem.ForeColor = System.Drawing.Color.Red;
            this.changeUIToolStripMenuItem.Name = "changeUIToolStripMenuItem";
            this.changeUIToolStripMenuItem.Size = new System.Drawing.Size(74, 20);
            this.changeUIToolStripMenuItem.Text = "Change UI";
            // 
            // materialSkinUIToolStripMenuItem
            // 
            this.materialSkinUIToolStripMenuItem.BackColor = System.Drawing.Color.Black;
            this.materialSkinUIToolStripMenuItem.ForeColor = System.Drawing.Color.White;
            this.materialSkinUIToolStripMenuItem.Name = "materialSkinUIToolStripMenuItem";
            this.materialSkinUIToolStripMenuItem.Size = new System.Drawing.Size(159, 22);
            this.materialSkinUIToolStripMenuItem.Text = "MaterialSkinUI";
            this.materialSkinUIToolStripMenuItem.Click += new System.EventHandler(this.materialSkinUIToolStripMenuItem_Click);
            // 
            // metroModernUIToolStripMenuItem
            // 
            this.metroModernUIToolStripMenuItem.BackColor = System.Drawing.Color.Black;
            this.metroModernUIToolStripMenuItem.ForeColor = System.Drawing.Color.White;
            this.metroModernUIToolStripMenuItem.Name = "metroModernUIToolStripMenuItem";
            this.metroModernUIToolStripMenuItem.Size = new System.Drawing.Size(159, 22);
            this.metroModernUIToolStripMenuItem.Text = "MetroModernUI";
            this.metroModernUIToolStripMenuItem.Click += new System.EventHandler(this.metroModernUIToolStripMenuItem_Click);
            // 
            // IceSourceForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.MenuText;
            this.ClientSize = new System.Drawing.Size(284, 306);
            this.Controls.Add(this.Theme);
            this.Controls.Add(this.TopCheck);
            this.Controls.Add(this.Inject);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.menuStrip1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.MainMenuStrip = this.menuStrip1;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "IceSourceForm";
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Ice Source UI Dark Theme";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.IceSourceForm_FormClosed);
            this.tabControl1.ResumeLayout(false);
            this.CMDS.ResumeLayout(false);
            this.CMDS.PerformLayout();
            this.QuickExe.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.JumpPowerValue)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.WalkSpeedValue)).EndInit();
            this.LuaC.ResumeLayout(false);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage LuaC;
        private System.Windows.Forms.TabPage CMDS;
        private System.Windows.Forms.Button Clear;
        private System.Windows.Forms.Button Execute;
        private System.Windows.Forms.RichTextBox LuaCBox;
        private System.Windows.Forms.RichTextBox CmdBox;
        private System.Windows.Forms.Button Send;
        private System.Windows.Forms.TextBox CmdTextBox;
        private System.Windows.Forms.Button Open;
        private System.Windows.Forms.Button Inject;
        private System.Windows.Forms.TabPage QuickExe;
        private System.Windows.Forms.Button Btools;
        private System.Windows.Forms.Button FF;
        private System.Windows.Forms.Button Ws;
        private System.Windows.Forms.NumericUpDown WalkSpeedValue;
        private System.Windows.Forms.Button Sit;
        private System.Windows.Forms.Button Suicide;
        private System.Windows.Forms.NumericUpDown JumpPowerValue;
        private System.Windows.Forms.Button JP;
        private System.Windows.Forms.CheckBox TopCheck;
        private System.Windows.Forms.Button Theme;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem changeUIToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem materialSkinUIToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem metroModernUIToolStripMenuItem;
    }
}

