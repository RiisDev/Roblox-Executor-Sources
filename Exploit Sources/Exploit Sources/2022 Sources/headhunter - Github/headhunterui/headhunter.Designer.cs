namespace headhunterui
{
    partial class headhunter
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(headhunter));
            this.titlepanel = new System.Windows.Forms.Panel();
            this.minimize = new System.Windows.Forms.Label();
            this.exit = new System.Windows.Forms.Label();
            this.title = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.button1 = new System.Windows.Forms.Button();
            this.executebtn = new System.Windows.Forms.Button();
            this.panel2 = new System.Windows.Forms.Panel();
            this.hah = new System.Windows.Forms.Panel();
            this.panel5 = new System.Windows.Forms.Panel();
            this.panel4 = new System.Windows.Forms.Panel();
            this.panel3 = new System.Windows.Forms.Panel();
            this.textbox = new FastColoredTextBoxNS.FastColoredTextBox();
            this.titlepanel.SuspendLayout();
            this.panel1.SuspendLayout();
            this.hah.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.textbox)).BeginInit();
            this.SuspendLayout();
            // 
            // titlepanel
            // 
            this.titlepanel.BackColor = System.Drawing.Color.Gray;
            this.titlepanel.Controls.Add(this.minimize);
            this.titlepanel.Controls.Add(this.exit);
            this.titlepanel.Controls.Add(this.title);
            this.titlepanel.Dock = System.Windows.Forms.DockStyle.Top;
            this.titlepanel.Location = new System.Drawing.Point(0, 0);
            this.titlepanel.Name = "titlepanel";
            this.titlepanel.Size = new System.Drawing.Size(799, 56);
            this.titlepanel.TabIndex = 0;
            this.titlepanel.Paint += new System.Windows.Forms.PaintEventHandler(this.titlepanel_Paint);
            this.titlepanel.MouseDown += new System.Windows.Forms.MouseEventHandler(this.titlepanel_MouseDown);
            this.titlepanel.MouseMove += new System.Windows.Forms.MouseEventHandler(this.titlepanel_MouseMove);
            this.titlepanel.MouseUp += new System.Windows.Forms.MouseEventHandler(this.titlepanel_MouseUp);
            // 
            // minimize
            // 
            this.minimize.AutoSize = true;
            this.minimize.Font = new System.Drawing.Font("Azonix", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.minimize.Location = new System.Drawing.Point(711, 9);
            this.minimize.Name = "minimize";
            this.minimize.Size = new System.Drawing.Size(32, 32);
            this.minimize.TabIndex = 2;
            this.minimize.Text = "-";
            this.minimize.Click += new System.EventHandler(this.minimize_Click);
            // 
            // exit
            // 
            this.exit.AutoSize = true;
            this.exit.Font = new System.Drawing.Font("Azonix", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.exit.Location = new System.Drawing.Point(755, 9);
            this.exit.Name = "exit";
            this.exit.Size = new System.Drawing.Size(38, 32);
            this.exit.TabIndex = 1;
            this.exit.Text = "X";
            this.exit.Click += new System.EventHandler(this.exit_Click);
            // 
            // title
            // 
            this.title.AutoSize = true;
            this.title.Font = new System.Drawing.Font("Azonix", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.title.Location = new System.Drawing.Point(11, 12);
            this.title.Name = "title";
            this.title.Size = new System.Drawing.Size(427, 32);
            this.title.TabIndex = 0;
            this.title.Text = "headhunter - fishy";
            this.title.MouseDown += new System.Windows.Forms.MouseEventHandler(this.titlepanel_MouseDown);
            this.title.MouseMove += new System.Windows.Forms.MouseEventHandler(this.titlepanel_MouseMove);
            this.title.MouseUp += new System.Windows.Forms.MouseEventHandler(this.titlepanel_MouseUp);
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.IndianRed;
            this.panel1.Controls.Add(this.button1);
            this.panel1.Controls.Add(this.executebtn);
            this.panel1.Controls.Add(this.panel2);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel1.Location = new System.Drawing.Point(0, 359);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(799, 47);
            this.panel1.TabIndex = 1;
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(128)))), ((int)(((byte)(255)))), ((int)(((byte)(128)))));
            this.button1.FlatAppearance.BorderColor = System.Drawing.Color.IndianRed;
            this.button1.FlatAppearance.BorderSize = 3;
            this.button1.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.button1.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button1.Font = new System.Drawing.Font("Azonix", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button1.Location = new System.Drawing.Point(162, 11);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(124, 33);
            this.button1.TabIndex = 3;
            this.button1.Text = "Clear";
            this.button1.UseVisualStyleBackColor = false;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // executebtn
            // 
            this.executebtn.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(128)))), ((int)(((byte)(255)))), ((int)(((byte)(128)))));
            this.executebtn.FlatAppearance.BorderColor = System.Drawing.Color.IndianRed;
            this.executebtn.FlatAppearance.BorderSize = 3;
            this.executebtn.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.executebtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.executebtn.Font = new System.Drawing.Font("Azonix", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.executebtn.Location = new System.Drawing.Point(19, 11);
            this.executebtn.Name = "executebtn";
            this.executebtn.Size = new System.Drawing.Size(124, 33);
            this.executebtn.TabIndex = 2;
            this.executebtn.Text = "Execute";
            this.executebtn.UseVisualStyleBackColor = false;
            this.executebtn.Click += new System.EventHandler(this.executebtn_Click);
            // 
            // panel2
            // 
            this.panel2.BackColor = System.Drawing.Color.Gray;
            this.panel2.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel2.Location = new System.Drawing.Point(0, 26);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(799, 21);
            this.panel2.TabIndex = 2;
            // 
            // hah
            // 
            this.hah.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.hah.Controls.Add(this.textbox);
            this.hah.Controls.Add(this.panel5);
            this.hah.Controls.Add(this.panel4);
            this.hah.Controls.Add(this.panel3);
            this.hah.Location = new System.Drawing.Point(0, 47);
            this.hah.Name = "hah";
            this.hah.Size = new System.Drawing.Size(799, 318);
            this.hah.TabIndex = 3;
            // 
            // panel5
            // 
            this.panel5.BackColor = System.Drawing.Color.IndianRed;
            this.panel5.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel5.Location = new System.Drawing.Point(0, 0);
            this.panel5.Name = "panel5";
            this.panel5.Size = new System.Drawing.Size(799, 22);
            this.panel5.TabIndex = 6;
            // 
            // panel4
            // 
            this.panel4.BackColor = System.Drawing.Color.IndianRed;
            this.panel4.Location = new System.Drawing.Point(780, 0);
            this.panel4.Name = "panel4";
            this.panel4.Size = new System.Drawing.Size(19, 332);
            this.panel4.TabIndex = 5;
            // 
            // panel3
            // 
            this.panel3.BackColor = System.Drawing.Color.IndianRed;
            this.panel3.Location = new System.Drawing.Point(0, 0);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(19, 332);
            this.panel3.TabIndex = 4;
            // 
            // textbox
            // 
            this.textbox.AutoCompleteBracketsList = new char[] {
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
            this.textbox.AutoIndentCharsPatterns = "\r\n^\\s*[\\w\\.]+(\\s\\w+)?\\s*(?<range>=)\\s*(?<range>.+)\r\n";
            this.textbox.AutoScrollMinSize = new System.Drawing.Size(603, 14);
            this.textbox.BackBrush = null;
            this.textbox.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.textbox.BracketsHighlightStrategy = FastColoredTextBoxNS.BracketsHighlightStrategy.Strategy2;
            this.textbox.CharHeight = 14;
            this.textbox.CharWidth = 8;
            this.textbox.CommentPrefix = "--";
            this.textbox.Cursor = System.Windows.Forms.Cursors.IBeam;
            this.textbox.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(180)))), ((int)(((byte)(180)))), ((int)(((byte)(180)))));
            this.textbox.Font = new System.Drawing.Font("Courier New", 9.75F);
            this.textbox.ForeColor = System.Drawing.Color.White;
            this.textbox.IndentBackColor = System.Drawing.Color.IndianRed;
            this.textbox.IsReplaceMode = false;
            this.textbox.Language = FastColoredTextBoxNS.Language.Lua;
            this.textbox.LeftBracket = '(';
            this.textbox.LeftBracket2 = '{';
            this.textbox.LineNumberColor = System.Drawing.Color.Black;
            this.textbox.Location = new System.Drawing.Point(18, 22);
            this.textbox.Name = "textbox";
            this.textbox.Paddings = new System.Windows.Forms.Padding(0);
            this.textbox.RightBracket = ')';
            this.textbox.RightBracket2 = '}';
            this.textbox.SelectionColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(128)))));
            this.textbox.ServiceColors = ((FastColoredTextBoxNS.ServiceColors)(resources.GetObject("textbox.ServiceColors")));
            this.textbox.ServiceLinesColor = System.Drawing.Color.Black;
            this.textbox.Size = new System.Drawing.Size(769, 296);
            this.textbox.TabIndex = 7;
            this.textbox.Text = "-- Welcome to headhunter! A simple exploit written in C++ made by fishy.";
            this.textbox.Zoom = 100;
            // 
            // headhunter
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Silver;
            this.ClientSize = new System.Drawing.Size(799, 406);
            this.Controls.Add(this.hah);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.titlepanel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "headhunter";
            this.Text = "headhunter";
            this.TopMost = true;
            this.titlepanel.ResumeLayout(false);
            this.titlepanel.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.hah.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.textbox)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel titlepanel;
        private System.Windows.Forms.Label minimize;
        private System.Windows.Forms.Label exit;
        private System.Windows.Forms.Label title;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Button executebtn;
        private System.Windows.Forms.Panel hah;
        private System.Windows.Forms.Panel panel4;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Panel panel5;
        private System.Windows.Forms.Button button1;
        private FastColoredTextBoxNS.FastColoredTextBox textbox;
    }
}

