
namespace Constipated_X
{
    partial class ConstipatedX
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ConstipatedX));
            this.panel1 = new System.Windows.Forms.Panel();
            this.ClearText = new System.Windows.Forms.Button();
            this.SaveFile = new System.Windows.Forms.Button();
            this.OpenFile = new System.Windows.Forms.Button();
            this.Inject = new System.Windows.Forms.Button();
            this.Execute = new System.Windows.Forms.Button();
            this.ScriptTextBox = new FastColoredTextBoxNS.FastColoredTextBox();
            this.panel2 = new System.Windows.Forms.Panel();
            this.ConstipatedXLabel = new System.Windows.Forms.Label();
            this.CXLabel = new System.Windows.Forms.Label();
            this.Minimize = new System.Windows.Forms.Label();
            this.Close = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ScriptTextBox)).BeginInit();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.panel1.Controls.Add(this.ClearText);
            this.panel1.Controls.Add(this.SaveFile);
            this.panel1.Controls.Add(this.OpenFile);
            this.panel1.Controls.Add(this.Inject);
            this.panel1.Controls.Add(this.Execute);
            this.panel1.Controls.Add(this.ScriptTextBox);
            this.panel1.Controls.Add(this.panel2);
            this.panel1.Location = new System.Drawing.Point(-4, -4);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(730, 360);
            this.panel1.TabIndex = 0;
            // 
            // ClearText
            // 
            this.ClearText.BackColor = System.Drawing.Color.BurlyWood;
            this.ClearText.FlatAppearance.BorderSize = 0;
            this.ClearText.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ActiveBorder;
            this.ClearText.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ClearText.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ClearText.Location = new System.Drawing.Point(414, 308);
            this.ClearText.Name = "ClearText";
            this.ClearText.Size = new System.Drawing.Size(99, 32);
            this.ClearText.TabIndex = 6;
            this.ClearText.Text = "Clear";
            this.ClearText.UseVisualStyleBackColor = false;
            this.ClearText.Click += new System.EventHandler(this.ClearText_Click);
            // 
            // SaveFile
            // 
            this.SaveFile.BackColor = System.Drawing.Color.BurlyWood;
            this.SaveFile.FlatAppearance.BorderSize = 0;
            this.SaveFile.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ActiveBorder;
            this.SaveFile.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.SaveFile.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.SaveFile.Location = new System.Drawing.Point(286, 308);
            this.SaveFile.Name = "SaveFile";
            this.SaveFile.Size = new System.Drawing.Size(99, 32);
            this.SaveFile.TabIndex = 5;
            this.SaveFile.Text = "Save";
            this.SaveFile.UseVisualStyleBackColor = false;
            this.SaveFile.Click += new System.EventHandler(this.SaveFile_Click);
            // 
            // OpenFile
            // 
            this.OpenFile.BackColor = System.Drawing.Color.BurlyWood;
            this.OpenFile.FlatAppearance.BorderSize = 0;
            this.OpenFile.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ActiveBorder;
            this.OpenFile.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.OpenFile.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.OpenFile.Location = new System.Drawing.Point(157, 310);
            this.OpenFile.Name = "OpenFile";
            this.OpenFile.Size = new System.Drawing.Size(99, 32);
            this.OpenFile.TabIndex = 4;
            this.OpenFile.Text = "Open";
            this.OpenFile.UseVisualStyleBackColor = false;
            this.OpenFile.Click += new System.EventHandler(this.OpenFile_Click);
            // 
            // Inject
            // 
            this.Inject.BackColor = System.Drawing.Color.BurlyWood;
            this.Inject.FlatAppearance.BorderSize = 0;
            this.Inject.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ActiveBorder;
            this.Inject.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.Inject.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Inject.Location = new System.Drawing.Point(611, 308);
            this.Inject.Name = "Inject";
            this.Inject.Size = new System.Drawing.Size(99, 32);
            this.Inject.TabIndex = 3;
            this.Inject.Text = "Inject";
            this.Inject.UseVisualStyleBackColor = false;
            this.Inject.Click += new System.EventHandler(this.Inject_Click);
            // 
            // Execute
            // 
            this.Execute.BackColor = System.Drawing.Color.BurlyWood;
            this.Execute.FlatAppearance.BorderSize = 0;
            this.Execute.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ActiveBorder;
            this.Execute.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.Execute.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Execute.Location = new System.Drawing.Point(22, 310);
            this.Execute.Name = "Execute";
            this.Execute.Size = new System.Drawing.Size(99, 32);
            this.Execute.TabIndex = 2;
            this.Execute.Text = "Execute";
            this.Execute.UseVisualStyleBackColor = false;
            this.Execute.Click += new System.EventHandler(this.Execute_Click);
            // 
            // ScriptTextBox
            // 
            this.ScriptTextBox.AutoCompleteBracketsList = new char[] {
        '(',
        ')',
        '{',
        '}',
        '[',
        ']',
        '\"',
        '\"',
        '\'',
        '\''};
            this.ScriptTextBox.AutoIndentCharsPatterns = "^\\s*[\\w\\.]+(\\s\\w+)?\\s*(?<range>=)\\s*(?<range>[^;=]+);\r\n^\\s*(case|default)\\s*[^:]*" +
    "(?<range>:)\\s*(?<range>[^;]+);";
            this.ScriptTextBox.AutoScrollMinSize = new System.Drawing.Size(251, 42);
            this.ScriptTextBox.BackBrush = null;
            this.ScriptTextBox.BackColor = System.Drawing.SystemColors.Desktop;
            this.ScriptTextBox.CharHeight = 14;
            this.ScriptTextBox.CharWidth = 8;
            this.ScriptTextBox.Cursor = System.Windows.Forms.Cursors.IBeam;
            this.ScriptTextBox.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(180)))), ((int)(((byte)(180)))), ((int)(((byte)(180)))));
            this.ScriptTextBox.IndentBackColor = System.Drawing.Color.BurlyWood;
            this.ScriptTextBox.IsReplaceMode = false;
            this.ScriptTextBox.LineNumberColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.ScriptTextBox.Location = new System.Drawing.Point(51, 62);
            this.ScriptTextBox.Name = "ScriptTextBox";
            this.ScriptTextBox.Paddings = new System.Windows.Forms.Padding(0);
            this.ScriptTextBox.SelectionColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(255)))));
            this.ScriptTextBox.ServiceColors = ((FastColoredTextBoxNS.ServiceColors)(resources.GetObject("ScriptTextBox.ServiceColors")));
            this.ScriptTextBox.Size = new System.Drawing.Size(610, 240);
            this.ScriptTextBox.TabIndex = 1;
            this.ScriptTextBox.Text = "-- CryonGG --\r\n\r\n-- Lua Executor / Injector--";
            this.ScriptTextBox.Zoom = 100;
            // 
            // panel2
            // 
            this.panel2.BackColor = System.Drawing.Color.SeaShell;
            this.panel2.Controls.Add(this.ConstipatedXLabel);
            this.panel2.Controls.Add(this.CXLabel);
            this.panel2.Controls.Add(this.Minimize);
            this.panel2.Controls.Add(this.Close);
            this.panel2.Location = new System.Drawing.Point(3, 0);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(721, 56);
            this.panel2.TabIndex = 0;
            this.panel2.MouseDown += new System.Windows.Forms.MouseEventHandler(this.FormMoveable_MouseDown);
            // 
            // ConstipatedXLabel
            // 
            this.ConstipatedXLabel.AutoSize = true;
            this.ConstipatedXLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 21.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ConstipatedXLabel.ForeColor = System.Drawing.SystemColors.ActiveCaption;
            this.ConstipatedXLabel.Location = new System.Drawing.Point(299, 13);
            this.ConstipatedXLabel.Name = "ConstipatedXLabel";
            this.ConstipatedXLabel.Size = new System.Drawing.Size(146, 33);
            this.ConstipatedXLabel.TabIndex = 3;
            this.ConstipatedXLabel.Text = "CryonGG";
            // 
            // CXLabel
            // 
            this.CXLabel.AutoSize = true;
            this.CXLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 21.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CXLabel.ForeColor = System.Drawing.SystemColors.ActiveCaption;
            this.CXLabel.Location = new System.Drawing.Point(40, 12);
            this.CXLabel.Name = "CXLabel";
            this.CXLabel.Size = new System.Drawing.Size(61, 33);
            this.CXLabel.TabIndex = 2;
            this.CXLabel.Text = "CG";
            // 
            // Minimize
            // 
            this.Minimize.AutoSize = true;
            this.Minimize.BackColor = System.Drawing.Color.Transparent;
            this.Minimize.Font = new System.Drawing.Font("Microsoft Sans Serif", 24F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Minimize.ForeColor = System.Drawing.SystemColors.ActiveCaption;
            this.Minimize.Location = new System.Drawing.Point(635, 8);
            this.Minimize.Name = "Minimize";
            this.Minimize.Size = new System.Drawing.Size(28, 37);
            this.Minimize.TabIndex = 1;
            this.Minimize.Text = "-";
            this.Minimize.Click += new System.EventHandler(this.Minimize_Click);
            // 
            // Close
            // 
            this.Close.AutoSize = true;
            this.Close.BackColor = System.Drawing.Color.Transparent;
            this.Close.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Close.ForeColor = System.Drawing.SystemColors.ActiveCaption;
            this.Close.Location = new System.Drawing.Point(676, 13);
            this.Close.Name = "Close";
            this.Close.Size = new System.Drawing.Size(31, 29);
            this.Close.TabIndex = 0;
            this.Close.Text = "X";
            this.Close.Click += new System.EventHandler(this.Close_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.Color.Transparent;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 24F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(635, 8);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(28, 37);
            this.label1.TabIndex = 1;
            this.label1.Text = "-";
            this.label1.Click += new System.EventHandler(this.Minimize_Click);
            // 
            // ConstipatedX
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(718, 348);
            this.Controls.Add(this.panel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "ConstipatedX";
            this.Text = "Constipated X";
            this.panel1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.ScriptTextBox)).EndInit();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Label Minimize;
        private System.Windows.Forms.Label Close;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label ConstipatedXLabel;
        private System.Windows.Forms.Label CXLabel;
        private FastColoredTextBoxNS.FastColoredTextBox ScriptTextBox;
        private System.Windows.Forms.Button Inject;
        private System.Windows.Forms.Button Execute;
        private System.Windows.Forms.Button ClearText;
        private System.Windows.Forms.Button SaveFile;
        private System.Windows.Forms.Button OpenFile;
    }
}

