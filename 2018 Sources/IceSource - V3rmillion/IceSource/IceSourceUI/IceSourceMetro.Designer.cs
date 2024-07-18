namespace IceSourceUI
{
    partial class IceSourceMetro
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
            this.components = new System.ComponentModel.Container();
            this.metroTabControl1 = new MetroFramework.Controls.MetroTabControl();
            this.Commands = new System.Windows.Forms.TabPage();
            this.SendBtn = new MetroFramework.Controls.MetroButton();
            this.CmdTextBox = new MetroFramework.Controls.MetroTextBox();
            this.CmdBox = new System.Windows.Forms.RichTextBox();
            this.QuickExe = new System.Windows.Forms.TabPage();
            this.JPVal = new System.Windows.Forms.NumericUpDown();
            this.WSVal = new System.Windows.Forms.NumericUpDown();
            this.SetJP = new MetroFramework.Controls.MetroButton();
            this.SetWS = new MetroFramework.Controls.MetroButton();
            this.Sit = new MetroFramework.Controls.MetroButton();
            this.Suicide = new MetroFramework.Controls.MetroButton();
            this.FF = new MetroFramework.Controls.MetroButton();
            this.Btools = new MetroFramework.Controls.MetroButton();
            this.LuaC = new System.Windows.Forms.TabPage();
            this.Clear = new MetroFramework.Controls.MetroButton();
            this.Execute = new MetroFramework.Controls.MetroButton();
            this.Open = new MetroFramework.Controls.MetroButton();
            this.LuacBox = new System.Windows.Forms.RichTextBox();
            this.AlwaysTop = new MetroFramework.Controls.MetroCheckBox();
            this.Inject = new MetroFramework.Controls.MetroButton();
            this.Theme = new MetroFramework.Controls.MetroButton();
            this.metroStyleManager1 = new MetroFramework.Components.MetroStyleManager(this.components);
            this.metroStyleExtender1 = new MetroFramework.Components.MetroStyleExtender(this.components);
            this.ChangeUi = new MetroFramework.Controls.MetroButton();
            this.metroContextMenu1 = new MetroFramework.Controls.MetroContextMenu(this.components);
            this.metroToolTip1 = new MetroFramework.Components.MetroToolTip();
            this.DefaultMenuOption = new System.Windows.Forms.ToolStripMenuItem();
            this.MaterialMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.metroTabControl1.SuspendLayout();
            this.Commands.SuspendLayout();
            this.QuickExe.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.JPVal)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.WSVal)).BeginInit();
            this.LuaC.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.metroStyleManager1)).BeginInit();
            this.metroContextMenu1.SuspendLayout();
            this.SuspendLayout();
            // 
            // metroTabControl1
            // 
            this.metroTabControl1.Controls.Add(this.Commands);
            this.metroTabControl1.Controls.Add(this.QuickExe);
            this.metroTabControl1.Controls.Add(this.LuaC);
            this.metroTabControl1.Location = new System.Drawing.Point(0, 63);
            this.metroTabControl1.Name = "metroTabControl1";
            this.metroTabControl1.SelectedIndex = 0;
            this.metroTabControl1.Size = new System.Drawing.Size(301, 275);
            this.metroTabControl1.SizeMode = System.Windows.Forms.TabSizeMode.Fixed;
            this.metroTabControl1.TabIndex = 0;
            this.metroTabControl1.UseSelectable = true;
            // 
            // Commands
            // 
            this.metroStyleExtender1.SetApplyMetroTheme(this.Commands, true);
            this.Commands.Controls.Add(this.SendBtn);
            this.Commands.Controls.Add(this.CmdTextBox);
            this.Commands.Controls.Add(this.CmdBox);
            this.Commands.Location = new System.Drawing.Point(4, 38);
            this.Commands.Name = "Commands";
            this.Commands.Size = new System.Drawing.Size(293, 233);
            this.Commands.TabIndex = 0;
            this.Commands.Text = "Commands";
            // 
            // SendBtn
            // 
            this.SendBtn.Location = new System.Drawing.Point(205, 197);
            this.SendBtn.Name = "SendBtn";
            this.SendBtn.Size = new System.Drawing.Size(74, 24);
            this.SendBtn.TabIndex = 2;
            this.SendBtn.Text = "Send";
            this.SendBtn.UseSelectable = true;
            this.SendBtn.Click += new System.EventHandler(this.SendBtn_Click);
            // 
            // CmdTextBox
            // 
            // 
            // 
            // 
            this.CmdTextBox.CustomButton.Image = null;
            this.CmdTextBox.CustomButton.Location = new System.Drawing.Point(171, 2);
            this.CmdTextBox.CustomButton.Name = "";
            this.CmdTextBox.CustomButton.Size = new System.Drawing.Size(19, 19);
            this.CmdTextBox.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.CmdTextBox.CustomButton.TabIndex = 1;
            this.CmdTextBox.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.CmdTextBox.CustomButton.UseSelectable = true;
            this.CmdTextBox.CustomButton.Visible = false;
            this.CmdTextBox.FontSize = MetroFramework.MetroTextBoxSize.Medium;
            this.CmdTextBox.FontWeight = MetroFramework.MetroTextBoxWeight.Bold;
            this.CmdTextBox.Lines = new string[0];
            this.CmdTextBox.Location = new System.Drawing.Point(4, 198);
            this.CmdTextBox.MaxLength = 32767;
            this.CmdTextBox.Name = "CmdTextBox";
            this.CmdTextBox.PasswordChar = '\0';
            this.CmdTextBox.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.CmdTextBox.SelectedText = "";
            this.CmdTextBox.SelectionLength = 0;
            this.CmdTextBox.SelectionStart = 0;
            this.CmdTextBox.ShortcutsEnabled = true;
            this.CmdTextBox.Size = new System.Drawing.Size(193, 24);
            this.CmdTextBox.TabIndex = 1;
            this.CmdTextBox.UseSelectable = true;
            this.CmdTextBox.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.CmdTextBox.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            this.CmdTextBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.CmdTextBox_KeyDown);
            // 
            // CmdBox
            // 
            this.metroStyleExtender1.SetApplyMetroTheme(this.CmdBox, true);
            this.CmdBox.Font = new System.Drawing.Font("Lucida Sans Unicode", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CmdBox.Location = new System.Drawing.Point(3, 14);
            this.CmdBox.Name = "CmdBox";
            this.CmdBox.ReadOnly = true;
            this.CmdBox.Size = new System.Drawing.Size(287, 173);
            this.CmdBox.TabIndex = 0;
            this.CmdBox.Text = "Welcome to Ice\nType cmds to see the commands\ncredits to see the credits\n[p] = pla" +
    "yer, [#] = number";
            this.CmdBox.TextChanged += new System.EventHandler(this.CmdBox_TextChanged);
            // 
            // QuickExe
            // 
            this.metroStyleExtender1.SetApplyMetroTheme(this.QuickExe, true);
            this.QuickExe.Controls.Add(this.JPVal);
            this.QuickExe.Controls.Add(this.WSVal);
            this.QuickExe.Controls.Add(this.SetJP);
            this.QuickExe.Controls.Add(this.SetWS);
            this.QuickExe.Controls.Add(this.Sit);
            this.QuickExe.Controls.Add(this.Suicide);
            this.QuickExe.Controls.Add(this.FF);
            this.QuickExe.Controls.Add(this.Btools);
            this.QuickExe.Location = new System.Drawing.Point(4, 38);
            this.QuickExe.Name = "QuickExe";
            this.QuickExe.Size = new System.Drawing.Size(293, 233);
            this.QuickExe.TabIndex = 1;
            this.QuickExe.Text = "Quick Exe";
            // 
            // JPVal
            // 
            this.metroStyleExtender1.SetApplyMetroTheme(this.JPVal, true);
            this.JPVal.Font = new System.Drawing.Font("Lucida Sans Unicode", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.JPVal.Location = new System.Drawing.Point(165, 135);
            this.JPVal.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
            this.JPVal.Name = "JPVal";
            this.JPVal.Size = new System.Drawing.Size(80, 24);
            this.JPVal.TabIndex = 7;
            this.JPVal.Value = new decimal(new int[] {
            50,
            0,
            0,
            0});
            // 
            // WSVal
            // 
            this.metroStyleExtender1.SetApplyMetroTheme(this.WSVal, true);
            this.WSVal.Font = new System.Drawing.Font("Lucida Sans Unicode", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.WSVal.Location = new System.Drawing.Point(171, 35);
            this.WSVal.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
            this.WSVal.Name = "WSVal";
            this.WSVal.Size = new System.Drawing.Size(80, 24);
            this.WSVal.TabIndex = 6;
            this.WSVal.Value = new decimal(new int[] {
            16,
            0,
            0,
            0});
            // 
            // SetJP
            // 
            this.SetJP.Location = new System.Drawing.Point(164, 175);
            this.SetJP.Name = "SetJP";
            this.SetJP.Size = new System.Drawing.Size(90, 28);
            this.SetJP.TabIndex = 5;
            this.SetJP.Text = "Set JP";
            this.SetJP.UseSelectable = true;
            this.SetJP.Click += new System.EventHandler(this.SetJP_Click);
            // 
            // SetWS
            // 
            this.SetWS.Location = new System.Drawing.Point(166, 75);
            this.SetWS.Name = "SetWS";
            this.SetWS.Size = new System.Drawing.Size(90, 28);
            this.SetWS.TabIndex = 4;
            this.SetWS.Text = "Set WS";
            this.SetWS.UseSelectable = true;
            this.SetWS.Click += new System.EventHandler(this.SetWS_Click);
            // 
            // Sit
            // 
            this.Sit.Location = new System.Drawing.Point(20, 175);
            this.Sit.Name = "Sit";
            this.Sit.Size = new System.Drawing.Size(90, 28);
            this.Sit.TabIndex = 3;
            this.Sit.Text = "Sit";
            this.Sit.UseSelectable = true;
            this.Sit.Click += new System.EventHandler(this.Sit_Click);
            // 
            // Suicide
            // 
            this.Suicide.Location = new System.Drawing.Point(20, 125);
            this.Suicide.Name = "Suicide";
            this.Suicide.Size = new System.Drawing.Size(90, 28);
            this.Suicide.TabIndex = 2;
            this.Suicide.Text = "Suicide";
            this.Suicide.UseSelectable = true;
            this.Suicide.Click += new System.EventHandler(this.Suicide_Click);
            // 
            // FF
            // 
            this.FF.Location = new System.Drawing.Point(20, 75);
            this.FF.Name = "FF";
            this.FF.Size = new System.Drawing.Size(90, 28);
            this.FF.TabIndex = 1;
            this.FF.Text = "FF";
            this.FF.UseSelectable = true;
            this.FF.Click += new System.EventHandler(this.FF_Click);
            // 
            // Btools
            // 
            this.Btools.Location = new System.Drawing.Point(20, 25);
            this.Btools.Name = "Btools";
            this.Btools.Size = new System.Drawing.Size(90, 28);
            this.Btools.TabIndex = 0;
            this.Btools.Text = "Btools";
            this.Btools.UseSelectable = true;
            this.Btools.Click += new System.EventHandler(this.Btools_Click);
            // 
            // LuaC
            // 
            this.metroStyleExtender1.SetApplyMetroTheme(this.LuaC, true);
            this.LuaC.Controls.Add(this.Clear);
            this.LuaC.Controls.Add(this.Execute);
            this.LuaC.Controls.Add(this.Open);
            this.LuaC.Controls.Add(this.LuacBox);
            this.LuaC.Location = new System.Drawing.Point(4, 38);
            this.LuaC.Name = "LuaC";
            this.LuaC.Size = new System.Drawing.Size(293, 233);
            this.LuaC.TabIndex = 2;
            this.LuaC.Text = "Lua C";
            // 
            // Clear
            // 
            this.Clear.Location = new System.Drawing.Point(202, 190);
            this.Clear.Name = "Clear";
            this.Clear.Size = new System.Drawing.Size(80, 30);
            this.Clear.TabIndex = 3;
            this.Clear.Text = "Clear";
            this.Clear.UseSelectable = true;
            this.Clear.Click += new System.EventHandler(this.Clear_Click);
            // 
            // Execute
            // 
            this.Execute.Location = new System.Drawing.Point(99, 190);
            this.Execute.Name = "Execute";
            this.Execute.Size = new System.Drawing.Size(80, 30);
            this.Execute.TabIndex = 2;
            this.Execute.Text = "Execute";
            this.Execute.UseSelectable = true;
            this.Execute.Click += new System.EventHandler(this.Execute_Click);
            // 
            // Open
            // 
            this.Open.Location = new System.Drawing.Point(12, 190);
            this.Open.Name = "Open";
            this.Open.Size = new System.Drawing.Size(80, 30);
            this.Open.TabIndex = 1;
            this.Open.Text = "Open";
            this.Open.UseSelectable = true;
            this.Open.Click += new System.EventHandler(this.Open_Click);
            // 
            // LuacBox
            // 
            this.metroStyleExtender1.SetApplyMetroTheme(this.LuacBox, true);
            this.LuacBox.Location = new System.Drawing.Point(5, 7);
            this.LuacBox.Name = "LuacBox";
            this.LuacBox.Size = new System.Drawing.Size(287, 178);
            this.LuacBox.TabIndex = 0;
            this.LuacBox.Text = "";
            // 
            // AlwaysTop
            // 
            this.AlwaysTop.AutoSize = true;
            this.AlwaysTop.Checked = true;
            this.AlwaysTop.CheckState = System.Windows.Forms.CheckState.Checked;
            this.AlwaysTop.Location = new System.Drawing.Point(7, 344);
            this.AlwaysTop.Name = "AlwaysTop";
            this.AlwaysTop.Size = new System.Drawing.Size(100, 15);
            this.AlwaysTop.TabIndex = 1;
            this.AlwaysTop.Text = "Always on Top";
            this.AlwaysTop.UseSelectable = true;
            this.AlwaysTop.CheckedChanged += new System.EventHandler(this.AlwaysTop_CheckedChanged);
            // 
            // Inject
            // 
            this.Inject.Location = new System.Drawing.Point(118, 342);
            this.Inject.Name = "Inject";
            this.Inject.Size = new System.Drawing.Size(90, 28);
            this.Inject.TabIndex = 2;
            this.Inject.Text = "Inject";
            this.Inject.UseSelectable = true;
            this.Inject.Click += new System.EventHandler(this.Inject_Click);
            // 
            // Theme
            // 
            this.Theme.Location = new System.Drawing.Point(216, 344);
            this.Theme.Name = "Theme";
            this.Theme.Size = new System.Drawing.Size(78, 28);
            this.Theme.TabIndex = 3;
            this.Theme.Text = "Theme";
            this.Theme.UseSelectable = true;
            this.Theme.Click += new System.EventHandler(this.Theme_Click);
            // 
            // metroStyleManager1
            // 
            this.metroStyleManager1.Owner = this;
            // 
            // ChangeUi
            // 
            this.ChangeUi.Location = new System.Drawing.Point(0, 4);
            this.ChangeUi.Name = "ChangeUi";
            this.ChangeUi.Size = new System.Drawing.Size(75, 23);
            this.ChangeUi.TabIndex = 4;
            this.ChangeUi.Text = "Change UI";
            this.ChangeUi.UseSelectable = true;
            this.ChangeUi.Click += new System.EventHandler(this.ChangeUi_Click);
            // 
            // metroContextMenu1
            // 
            this.metroContextMenu1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.DefaultMenuOption,
            this.MaterialMenuItem1});
            this.metroContextMenu1.Name = "metroContextMenu1";
            this.metroContextMenu1.Size = new System.Drawing.Size(153, 70);
            // 
            // metroToolTip1
            // 
            this.metroToolTip1.Style = MetroFramework.MetroColorStyle.Blue;
            this.metroToolTip1.StyleManager = null;
            this.metroToolTip1.Theme = MetroFramework.MetroThemeStyle.Light;
            // 
            // DefaultMenuOption
            // 
            this.DefaultMenuOption.Name = "DefaultMenuOption";
            this.DefaultMenuOption.Size = new System.Drawing.Size(152, 22);
            this.DefaultMenuOption.Text = "Default";
            this.DefaultMenuOption.Click += new System.EventHandler(this.DefaultMenuOption_Click);
            // 
            // MaterialMenuItem1
            // 
            this.MaterialMenuItem1.Name = "MaterialMenuItem1";
            this.MaterialMenuItem1.Size = new System.Drawing.Size(152, 22);
            this.MaterialMenuItem1.Text = "MaterialSkin";
            this.MaterialMenuItem1.Click += new System.EventHandler(this.MaterialMenuItem1_Click);
            // 
            // IceSourceMetro
            // 
            this.ApplyImageInvert = true;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(300, 381);
            this.Controls.Add(this.ChangeUi);
            this.Controls.Add(this.Theme);
            this.Controls.Add(this.Inject);
            this.Controls.Add(this.AlwaysTop);
            this.Controls.Add(this.metroTabControl1);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "IceSourceMetro";
            this.Resizable = false;
            this.ShowIcon = false;
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.StyleManager = this.metroStyleManager1;
            this.Text = "Ice Source Light Theme";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.IceSourceMetro_FormClosed);
            this.metroTabControl1.ResumeLayout(false);
            this.Commands.ResumeLayout(false);
            this.QuickExe.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.JPVal)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.WSVal)).EndInit();
            this.LuaC.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.metroStyleManager1)).EndInit();
            this.metroContextMenu1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private MetroFramework.Controls.MetroTabControl metroTabControl1;
        private System.Windows.Forms.TabPage Commands;
        private System.Windows.Forms.TabPage LuaC;
        private MetroFramework.Controls.MetroButton SendBtn;
        private MetroFramework.Controls.MetroTextBox CmdTextBox;
        private System.Windows.Forms.RichTextBox CmdBox;
        private System.Windows.Forms.TabPage QuickExe;
        private System.Windows.Forms.NumericUpDown JPVal;
        private System.Windows.Forms.NumericUpDown WSVal;
        private MetroFramework.Controls.MetroButton SetJP;
        private MetroFramework.Controls.MetroButton SetWS;
        private MetroFramework.Controls.MetroButton Sit;
        private MetroFramework.Controls.MetroButton Suicide;
        private MetroFramework.Controls.MetroButton FF;
        private MetroFramework.Controls.MetroButton Btools;
        private MetroFramework.Controls.MetroButton Clear;
        private MetroFramework.Controls.MetroButton Execute;
        private MetroFramework.Controls.MetroButton Open;
        private System.Windows.Forms.RichTextBox LuacBox;
        private MetroFramework.Controls.MetroCheckBox AlwaysTop;
        private MetroFramework.Controls.MetroButton Inject;
        private MetroFramework.Controls.MetroButton Theme;
        private MetroFramework.Components.MetroStyleManager metroStyleManager1;
        private MetroFramework.Components.MetroStyleExtender metroStyleExtender1;
        private MetroFramework.Controls.MetroButton ChangeUi;
        private MetroFramework.Controls.MetroContextMenu metroContextMenu1;
        private System.Windows.Forms.ToolStripMenuItem DefaultMenuOption;
        private System.Windows.Forms.ToolStripMenuItem MaterialMenuItem1;
        private MetroFramework.Components.MetroToolTip metroToolTip1;
    }
}