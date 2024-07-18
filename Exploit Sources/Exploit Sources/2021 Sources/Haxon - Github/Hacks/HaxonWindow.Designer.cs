namespace Hacks
{
    partial class HaxonWindow
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(HaxonWindow));
            this.panel1 = new System.Windows.Forms.Panel();
            this.siticoneButton3 = new Siticone.UI.WinForms.SiticoneButton();
            this.siticoneButton1 = new Siticone.UI.WinForms.SiticoneButton();
            this.label1 = new System.Windows.Forms.Label();
            this.siticoneDragControl1 = new Siticone.UI.WinForms.SiticoneDragControl(this.components);
            this.siticoneDragControl2 = new Siticone.UI.WinForms.SiticoneDragControl(this.components);
            this.siticoneDragControl3 = new Siticone.UI.WinForms.SiticoneDragControl(this.components);
            this.listBox1 = new System.Windows.Forms.ListBox();
            this.Inject = new Siticone.UI.WinForms.SiticoneButton();
            this.siticoneButton2 = new Siticone.UI.WinForms.SiticoneButton();
            this.ClearButton = new Siticone.UI.WinForms.SiticoneButton();
            this.ExecuteButton = new Siticone.UI.WinForms.SiticoneButton();
            this.ScriptBox = new System.Windows.Forms.RichTextBox();
            this.siticoneButton4 = new Siticone.UI.WinForms.SiticoneButton();
            this.siticoneButton5 = new Siticone.UI.WinForms.SiticoneButton();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.siticoneButton3);
            this.panel1.Controls.Add(this.siticoneButton1);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Location = new System.Drawing.Point(-6, -3);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(742, 44);
            this.panel1.TabIndex = 8;
            // 
            // siticoneButton3
            // 
            this.siticoneButton3.CheckedState.Parent = this.siticoneButton3;
            this.siticoneButton3.CustomImages.Parent = this.siticoneButton3;
            this.siticoneButton3.Font = new System.Drawing.Font("Segoe UI", 9F);
            this.siticoneButton3.ForeColor = System.Drawing.Color.White;
            this.siticoneButton3.HoveredState.Parent = this.siticoneButton3;
            this.siticoneButton3.Location = new System.Drawing.Point(658, 6);
            this.siticoneButton3.Name = "siticoneButton3";
            this.siticoneButton3.ShadowDecoration.Parent = this.siticoneButton3;
            this.siticoneButton3.Size = new System.Drawing.Size(39, 28);
            this.siticoneButton3.TabIndex = 19;
            this.siticoneButton3.Text = "_";
            this.siticoneButton3.Click += new System.EventHandler(this.siticoneButton3_Click);
            // 
            // siticoneButton1
            // 
            this.siticoneButton1.CheckedState.Parent = this.siticoneButton1;
            this.siticoneButton1.CustomImages.Parent = this.siticoneButton1;
            this.siticoneButton1.FillColor = System.Drawing.Color.Red;
            this.siticoneButton1.Font = new System.Drawing.Font("Segoe UI", 9F);
            this.siticoneButton1.ForeColor = System.Drawing.Color.White;
            this.siticoneButton1.HoveredState.Parent = this.siticoneButton1;
            this.siticoneButton1.Location = new System.Drawing.Point(703, 2);
            this.siticoneButton1.Name = "siticoneButton1";
            this.siticoneButton1.ShadowDecoration.Parent = this.siticoneButton1;
            this.siticoneButton1.Size = new System.Drawing.Size(39, 32);
            this.siticoneButton1.TabIndex = 18;
            this.siticoneButton1.Text = "X";
            this.siticoneButton1.Click += new System.EventHandler(this.siticoneButton1_Click_1);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Century Gothic", 15.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.Color.White;
            this.label1.Location = new System.Drawing.Point(301, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(132, 25);
            this.label1.TabIndex = 9;
            this.label1.Text = "Haxon 1.0.0";
            // 
            // siticoneDragControl1
            // 
            this.siticoneDragControl1.TargetControl = this.panel1;
            // 
            // siticoneDragControl2
            // 
            this.siticoneDragControl2.TargetControl = this;
            // 
            // siticoneDragControl3
            // 
            this.siticoneDragControl3.TargetControl = this.label1;
            // 
            // listBox1
            // 
            this.listBox1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(95)))), ((int)(((byte)(149)))), ((int)(((byte)(254)))));
            this.listBox1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.listBox1.FormattingEnabled = true;
            this.listBox1.Location = new System.Drawing.Point(560, 45);
            this.listBox1.Name = "listBox1";
            this.listBox1.Size = new System.Drawing.Size(164, 301);
            this.listBox1.TabIndex = 9;
            this.listBox1.SelectedIndexChanged += new System.EventHandler(this.listBox1_SelectedIndexChanged);
            // 
            // Inject
            // 
            this.Inject.CheckedState.Parent = this.Inject;
            this.Inject.CustomImages.Parent = this.Inject;
            this.Inject.Font = new System.Drawing.Font("Segoe UI", 9F);
            this.Inject.ForeColor = System.Drawing.Color.White;
            this.Inject.HoveredState.Parent = this.Inject;
            this.Inject.Location = new System.Drawing.Point(115, 352);
            this.Inject.Name = "Inject";
            this.Inject.ShadowDecoration.Parent = this.Inject;
            this.Inject.Size = new System.Drawing.Size(125, 33);
            this.Inject.TabIndex = 11;
            this.Inject.Text = "Inject";
            this.Inject.Click += new System.EventHandler(this.siticoneButton1_Click);
            // 
            // siticoneButton2
            // 
            this.siticoneButton2.CheckedState.Parent = this.siticoneButton2;
            this.siticoneButton2.CustomImages.Parent = this.siticoneButton2;
            this.siticoneButton2.Font = new System.Drawing.Font("Segoe UI", 9F);
            this.siticoneButton2.ForeColor = System.Drawing.Color.White;
            this.siticoneButton2.HoveredState.Parent = this.siticoneButton2;
            this.siticoneButton2.Location = new System.Drawing.Point(246, 352);
            this.siticoneButton2.Name = "siticoneButton2";
            this.siticoneButton2.ShadowDecoration.Parent = this.siticoneButton2;
            this.siticoneButton2.Size = new System.Drawing.Size(125, 33);
            this.siticoneButton2.TabIndex = 12;
            this.siticoneButton2.Text = "Open File";
            this.siticoneButton2.Click += new System.EventHandler(this.siticoneButton2_Click);
            // 
            // ClearButton
            // 
            this.ClearButton.CheckedState.Parent = this.ClearButton;
            this.ClearButton.CustomImages.Parent = this.ClearButton;
            this.ClearButton.Font = new System.Drawing.Font("Segoe UI", 9F);
            this.ClearButton.ForeColor = System.Drawing.Color.White;
            this.ClearButton.HoveredState.Parent = this.ClearButton;
            this.ClearButton.Location = new System.Drawing.Point(402, 352);
            this.ClearButton.Name = "ClearButton";
            this.ClearButton.ShadowDecoration.Parent = this.ClearButton;
            this.ClearButton.Size = new System.Drawing.Size(125, 33);
            this.ClearButton.TabIndex = 13;
            this.ClearButton.Text = "Clear";
            this.ClearButton.Click += new System.EventHandler(this.ClearButton_Click);
            // 
            // ExecuteButton
            // 
            this.ExecuteButton.CheckedState.Parent = this.ExecuteButton;
            this.ExecuteButton.CustomImages.Parent = this.ExecuteButton;
            this.ExecuteButton.Font = new System.Drawing.Font("Segoe UI", 9F);
            this.ExecuteButton.ForeColor = System.Drawing.Color.White;
            this.ExecuteButton.HoveredState.Parent = this.ExecuteButton;
            this.ExecuteButton.Location = new System.Drawing.Point(6, 352);
            this.ExecuteButton.Name = "ExecuteButton";
            this.ExecuteButton.ShadowDecoration.Parent = this.ExecuteButton;
            this.ExecuteButton.Size = new System.Drawing.Size(103, 33);
            this.ExecuteButton.TabIndex = 16;
            this.ExecuteButton.Text = "Execute";
            this.ExecuteButton.Click += new System.EventHandler(this.ExecuteButton_Click);
            // 
            // ScriptBox
            // 
            this.ScriptBox.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(95)))), ((int)(((byte)(149)))), ((int)(((byte)(254)))));
            this.ScriptBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.ScriptBox.Cursor = System.Windows.Forms.Cursors.IBeam;
            this.ScriptBox.ForeColor = System.Drawing.Color.White;
            this.ScriptBox.Location = new System.Drawing.Point(13, 47);
            this.ScriptBox.Name = "ScriptBox";
            this.ScriptBox.Size = new System.Drawing.Size(541, 299);
            this.ScriptBox.TabIndex = 17;
            this.ScriptBox.Text = "//Erase this text and enter your own!\n";
            this.ScriptBox.TextChanged += new System.EventHandler(this.ScriptBox_TextChanged);
            // 
            // siticoneButton4
            // 
            this.siticoneButton4.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(95)))), ((int)(((byte)(149)))), ((int)(((byte)(255)))));
            this.siticoneButton4.CheckedState.Parent = this.siticoneButton4;
            this.siticoneButton4.CustomImages.Parent = this.siticoneButton4;
            this.siticoneButton4.Font = new System.Drawing.Font("Segoe UI", 9F);
            this.siticoneButton4.ForeColor = System.Drawing.Color.White;
            this.siticoneButton4.HoveredState.Parent = this.siticoneButton4;
            this.siticoneButton4.Location = new System.Drawing.Point(509, 352);
            this.siticoneButton4.Name = "siticoneButton4";
            this.siticoneButton4.ShadowDecoration.Parent = this.siticoneButton4;
            this.siticoneButton4.Size = new System.Drawing.Size(125, 33);
            this.siticoneButton4.TabIndex = 18;
            this.siticoneButton4.Text = "About";
            this.siticoneButton4.Click += new System.EventHandler(this.siticoneButton4_Click_1);
            // 
            // siticoneButton5
            // 
            this.siticoneButton5.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(95)))), ((int)(((byte)(149)))), ((int)(((byte)(255)))));
            this.siticoneButton5.CheckedState.Parent = this.siticoneButton5;
            this.siticoneButton5.CustomImages.Parent = this.siticoneButton5;
            this.siticoneButton5.Font = new System.Drawing.Font("Segoe UI", 9F);
            this.siticoneButton5.ForeColor = System.Drawing.Color.White;
            this.siticoneButton5.HoveredState.Parent = this.siticoneButton5;
            this.siticoneButton5.Location = new System.Drawing.Point(611, 352);
            this.siticoneButton5.Name = "siticoneButton5";
            this.siticoneButton5.ShadowDecoration.Parent = this.siticoneButton5;
            this.siticoneButton5.Size = new System.Drawing.Size(125, 33);
            this.siticoneButton5.TabIndex = 19;
            this.siticoneButton5.Text = "Refresh";
            this.siticoneButton5.Click += new System.EventHandler(this.siticoneButton5_Click_1);
            // 
            // HaxonWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(95)))), ((int)(((byte)(149)))), ((int)(((byte)(254)))));
            this.ClientSize = new System.Drawing.Size(736, 387);
            this.Controls.Add(this.siticoneButton5);
            this.Controls.Add(this.siticoneButton4);
            this.Controls.Add(this.ScriptBox);
            this.Controls.Add(this.ExecuteButton);
            this.Controls.Add(this.ClearButton);
            this.Controls.Add(this.siticoneButton2);
            this.Controls.Add(this.Inject);
            this.Controls.Add(this.listBox1);
            this.Controls.Add(this.panel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "HaxonWindow";
            this.Text = "Haxon";
            this.Load += new System.EventHandler(this.HaxonWindow_Load);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label1;
        private Siticone.UI.WinForms.SiticoneDragControl siticoneDragControl1;
        private Siticone.UI.WinForms.SiticoneDragControl siticoneDragControl2;
        private Siticone.UI.WinForms.SiticoneDragControl siticoneDragControl3;
        private Siticone.UI.WinForms.SiticoneButton ExecuteButton;
        private Siticone.UI.WinForms.SiticoneButton ClearButton;
        private Siticone.UI.WinForms.SiticoneButton siticoneButton2;
        private Siticone.UI.WinForms.SiticoneButton Inject;
        private System.Windows.Forms.ListBox listBox1;
        private System.Windows.Forms.RichTextBox ScriptBox;
        private Siticone.UI.WinForms.SiticoneButton siticoneButton1;
        private Siticone.UI.WinForms.SiticoneButton siticoneButton3;
        private Siticone.UI.WinForms.SiticoneButton siticoneButton4;
        private Siticone.UI.WinForms.SiticoneButton siticoneButton5;
    }
}