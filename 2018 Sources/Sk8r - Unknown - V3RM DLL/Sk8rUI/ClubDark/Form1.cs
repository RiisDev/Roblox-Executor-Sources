using System;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using ClubDarkAPI;
using ScintillaNET;

namespace ClubDark
{
	// Token: 0x02000002 RID: 2
	public partial class Form1 : Form
	{
		// Token: 0x06000001 RID: 1
		[DllImport("user32.dll")]
		public static extern int SendMessage(IntPtr hWnd, int Msg, int wParam, int lParam);

		// Token: 0x06000002 RID: 2
		[DllImport("user32.dll")]
		public static extern bool ReleaseCapture();

		// Token: 0x06000003 RID: 3 RVA: 0x00002050 File Offset: 0x00000250
		public Form1()
		{
			this.InitializeComponent();
			this.localplayer.Hide();
			this.Games.Hide();
		}

		// Token: 0x06000004 RID: 4 RVA: 0x000020A0 File Offset: 0x000002A0
		private void Form1_Load(object sender, EventArgs e)
		{
			this.ScriptList.DisplayMember = "FileName";
			string[] directories = Directory.GetDirectories(AppDomain.CurrentDomain.BaseDirectory, "Scripts", SearchOption.AllDirectories);
			foreach (string text in directories)
			{
				string path = text;
				string[] files = Directory.GetFiles(path, "*.txt");
				string[] files2 = Directory.GetFiles(path, "*.lua");
				foreach (string text2 in files2)
				{
					Form1.FileObject fileObject = new Form1.FileObject();
					fileObject.FileName = Path.GetFileName(text2.ToString());
					fileObject.FileContent = File.ReadAllText(text2.ToString());
					this.ScriptList.Items.Add(fileObject);
				}
				foreach (string text3 in files)
				{
					Form1.FileObject fileObject2 = new Form1.FileObject();
					fileObject2.FileName = Path.GetFileName(text3.ToString());
					fileObject2.FileContent = File.ReadAllText(text3.ToString());
					this.ScriptList.Items.Add(fileObject2);
				}
			}
			this.Input.Margins[0].Width = 16;
			string str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
			string str2 = "0123456789";
			string str3 = "ŠšŒœŸÿÀàÁáÂâÃãÄäÅåÆæÇçÈèÉéÊêËëÌìÍíÎîÏïÐðÑñÒòÓóÔôÕõÖØøÙùÚúÛûÜüÝýÞþßö";
			this.Input.StyleResetDefault();
			this.Input.Styles[32].Font = "Consolas";
			this.Input.Styles[32].Size = 10;
			this.Input.Styles[32].ForeColor = Color.FromArgb(255, 255, 255);
			this.Input.Styles[32].BackColor = Color.FromArgb(35, 35, 35);
			this.Input.StyleClearAll();
			this.Input.Styles[1].ForeColor = Color.FromArgb(87, 166, 74);
			this.Input.Styles[2].ForeColor = Color.FromArgb(87, 166, 74);
			this.Input.Styles[4].ForeColor = Color.FromArgb(181, 206, 151);
			this.Input.Styles[5].ForeColor = Color.FromArgb(77, 156, 214);
			this.Input.Styles[13].ForeColor = Color.FromArgb(77, 156, 214);
			this.Input.Styles[14].ForeColor = Color.FromArgb(77, 156, 214);
			this.Input.Styles[15].ForeColor = Color.FromArgb(77, 156, 214);
			this.Input.Styles[6].ForeColor = Color.FromArgb(214, 157, 133);
			this.Input.Styles[10].ForeColor = Color.FromArgb(255, 255, 255);
			this.Input.Styles[9].ForeColor = Color.FromArgb(127, 0, 0);
			this.Input.Lexer = Lexer.Lua;
			this.Input.WordChars = str + str2 + str3;
			this.Input.SetKeywords(0, "and break do else elseif end for function if in local nil not or repeat return then until while false true goto");
			this.Input.SetKeywords(1, "assert collectgarbage dofile error _G getmetatable ipairs loadfile next pairs pcall print rawequal rawget rawset setmetatable tonumber tostring type _VERSION xpcall string table math coroutine io os debug getfenv gcinfo load loadlib loadstring require select setfenv unpack _LOADED LUA_PATH _REQUIREDNAME package rawlen package bit32 utf8 _ENV");
			this.Input.SetKeywords(2, "string.byte string.char string.dump string.find string.format string.gsub string.len string.lower string.rep string.sub string.upper table.concat table.insert table.remove table.sort math.abs math.acos math.asin math.atan math.atan2 math.ceil math.cos math.deg math.exp math.floor math.frexp math.ldexp math.log math.max math.min math.pi math.pow math.rad math.random math.randomseed math.sin math.sqrt math.tan string.gfind string.gmatch string.match string.reverse string.pack string.packsize string.unpack table.foreach table.foreachi table.getn table.setn table.maxn table.pack table.unpack table.move math.cosh math.fmod math.huge math.log10 math.modf math.mod math.sinh math.tanh math.maxinteger math.mininteger math.tointeger math.type math.ult bit32.arshift bit32.band bit32.bnot bit32.bor bit32.btest bit32.bxor bit32.extract bit32.replace bit32.lrotate bit32.lshift bit32.rrotate bit32.rshift utf8.char utf8.charpattern utf8.codes utf8.codepoint utf8.len utf8.offset");
			this.Input.SetKeywords(3, "coroutine.create coroutine.resume coroutine.status coroutine.wrap coroutine.yield io.close io.flush io.input io.lines io.open io.output io.read io.tmpfile io.type io.write io.stdin io.stdout io.stderr os.clock os.date os.difftime os.execute os.exit os.getenv os.remove os.rename os.setlocale os.time os.tmpname coroutine.isyieldable coroutine.running io.popen module package.loaders package.seeall package.config package.searchers package.searchpath require package.cpath package.loaded package.loadlib package.path package.preload");
			this.Input.SetProperty("fold", "1");
			this.Input.SetProperty("fold.compact", "1");
			this.Input.Margins[2].Type = MarginType.Symbol;
			this.Input.Margins[2].Mask = 4261412864u;
			this.Input.Margins[2].Sensitive = true;
			this.Input.Margins[2].Width = 20;
			for (int l = 25; l <= 31; l++)
			{
				this.Input.Markers[l].SetForeColor(Color.FromArgb(35, 35, 35));
				this.Input.Markers[l].SetBackColor(Color.FromArgb(85, 85, 85));
			}
			this.Input.Markers[30].Symbol = MarkerSymbol.BoxPlus;
			this.Input.Markers[31].Symbol = MarkerSymbol.BoxMinus;
			this.Input.Markers[25].Symbol = MarkerSymbol.BoxPlusConnected;
			this.Input.Markers[27].Symbol = MarkerSymbol.TCorner;
			this.Input.Markers[26].Symbol = MarkerSymbol.BoxMinusConnected;
			this.Input.Markers[29].Symbol = MarkerSymbol.VLine;
			this.Input.Markers[28].Symbol = MarkerSymbol.LCorner;
			this.Input.AutomaticFold = (AutomaticFold.Show | AutomaticFold.Click | AutomaticFold.Change);
			this.Input.Styles[32].BackColor = Color.FromArgb(35, 35, 35);
			this.Input.SetFoldMarginHighlightColor(true, Color.FromArgb(48, 48, 48));
			this.Input.SetFoldMarginColor(true, Color.FromArgb(48, 48, 48));
			this.Input.Styles[33].BackColor = Color.FromArgb(44, 44, 44);
			this.Input.Styles[33].ForeColor = Color.FromArgb(255, 255, 255);
			this.Input.SetSelectionForeColor(true, Color.FromArgb(255, 255, 255));
			this.Input.SetSelectionBackColor(true, Color.FromArgb(70, 70, 70));
		}

		// Token: 0x06000005 RID: 5 RVA: 0x00002736 File Offset: 0x00000936
		private void label2_Click(object sender, EventArgs e)
		{
			base.Close();
		}

		// Token: 0x06000006 RID: 6 RVA: 0x00002740 File Offset: 0x00000940
		private void label1_Click(object sender, EventArgs e)
		{
			base.WindowState = FormWindowState.Minimized;
		}

		// Token: 0x06000007 RID: 7 RVA: 0x0000274C File Offset: 0x0000094C
		private void panel1_MouseDown(object sender, MouseEventArgs e)
		{
			bool flag = e.Button == MouseButtons.Left;
			if (flag)
			{
				Form1.ReleaseCapture();
				Form1.SendMessage(base.Handle, 161, 2, 0);
			}
		}

		// Token: 0x06000008 RID: 8 RVA: 0x00002788 File Offset: 0x00000988
		private void ScriptList_SelectedValueChanged(object sender, EventArgs e)
		{
			Form1.FileObject fileObject = (Form1.FileObject)this.ScriptList.SelectedItem;
			this.Input.Text = fileObject.FileContent;
		}

		// Token: 0x06000009 RID: 9 RVA: 0x000027B9 File Offset: 0x000009B9
		private void clear_Click(object sender, EventArgs e)
		{
			this.Input.Text = "";
		}

		// Token: 0x0600000A RID: 10 RVA: 0x000027CD File Offset: 0x000009CD
		private void pictureBox1_Click(object sender, EventArgs e)
		{
			this.api.LaunchExploit();
		}

		// Token: 0x0600000B RID: 11 RVA: 0x000027DC File Offset: 0x000009DC
		private void execute_Click(object sender, EventArgs e)
		{
			this.api.ExecuteScript(this.Input.Text);
		}

		// Token: 0x0600000C RID: 12 RVA: 0x000027F6 File Offset: 0x000009F6
		private void pictureBox2_Click(object sender, EventArgs e)
		{
			this.api.KillRoblox();
		}

		// Token: 0x0600000D RID: 13 RVA: 0x00002805 File Offset: 0x00000A05
		private void clubDarkToolStripMenuItem_Click(object sender, EventArgs e)
		{
			this.api.GoToUrl("https://clubdark.net");
		}

		// Token: 0x0600000E RID: 14 RVA: 0x0000281C File Offset: 0x00000A1C
		private void pictureBox3_MouseDown(object sender, MouseEventArgs e)
		{
			bool flag = e.Button == MouseButtons.Left;
			if (flag)
			{
				Form1.ReleaseCapture();
				Form1.SendMessage(base.Handle, 161, 2, 0);
			}
		}

		// Token: 0x0600000F RID: 15 RVA: 0x00002856 File Offset: 0x00000A56
		private void button1_Click(object sender, EventArgs e)
		{
			this.api.ExecuteLink("https://paste.ee/r/a5byE");
		}

		// Token: 0x06000010 RID: 16 RVA: 0x0000286A File Offset: 0x00000A6A
		private void button2_Click(object sender, EventArgs e)
		{
			this.api.ExecuteScript("noclip = false");
		}

		// Token: 0x06000011 RID: 17 RVA: 0x0000287E File Offset: 0x00000A7E
		private void button3_Click(object sender, EventArgs e)
		{
			this.api.ExecuteLink("https://paste.ee/r/G9R8q");
		}

		// Token: 0x06000012 RID: 18 RVA: 0x00002892 File Offset: 0x00000A92
		private void button4_Click(object sender, EventArgs e)
		{
			this.api.ExecuteScript("_G.FLYING = false");
		}

		// Token: 0x06000013 RID: 19 RVA: 0x000028A6 File Offset: 0x00000AA6
		private void button6_Click(object sender, EventArgs e)
		{
			this.api.ExecuteLink("https://paste.ee/r/fKkjW");
		}

		// Token: 0x06000014 RID: 20 RVA: 0x000028BA File Offset: 0x00000ABA
		private void button5_Click(object sender, EventArgs e)
		{
			this.api.ExecuteScript("clicktpon = false");
		}

		// Token: 0x06000015 RID: 21 RVA: 0x000028CE File Offset: 0x00000ACE
		private void button8_Click(object sender, EventArgs e)
		{
			this.api.ExecuteLink("https://paste.ee/r/AbFCY");
		}

		// Token: 0x06000016 RID: 22 RVA: 0x000028E2 File Offset: 0x00000AE2
		private void button7_Click(object sender, EventArgs e)
		{
			this.api.ExecuteScript("destroyon = false");
		}

		// Token: 0x06000017 RID: 23 RVA: 0x000028F6 File Offset: 0x00000AF6
		private void button10_Click(object sender, EventArgs e)
		{
			this.api.ExecuteLink("https://paste.ee/r/mtH37");
		}

		// Token: 0x06000018 RID: 24 RVA: 0x0000290A File Offset: 0x00000B0A
		private void button9_Click(object sender, EventArgs e)
		{
			this.api.ExecuteScript("clickmoveon = false");
		}

		// Token: 0x06000019 RID: 25 RVA: 0x0000291E File Offset: 0x00000B1E
		private void button16_Click(object sender, EventArgs e)
		{
			this.api.ExecuteScript("game.Players.LocalPlayer.Character.Humanoid.WalkSpeed =" + this.textBox1.Text);
		}

		// Token: 0x0600001A RID: 26 RVA: 0x00002942 File Offset: 0x00000B42
		private void button17_Click(object sender, EventArgs e)
		{
			this.api.ExecuteScript("game.Players.LocalPlayer.Character.Humanoid.JumpPower =" + this.textBox2.Text);
		}

		// Token: 0x0600001B RID: 27 RVA: 0x00002966 File Offset: 0x00000B66
		private void button18_Click(object sender, EventArgs e)
		{
			this.api.ExecuteScript("game.Players.LocalPlayer.Character.Humanoid.HipHeight =" + this.textBox3.Text);
		}

		// Token: 0x0600001C RID: 28 RVA: 0x0000298A File Offset: 0x00000B8A
		private void button11_Click(object sender, EventArgs e)
		{
			this.api.ExecuteLink("https://paste.ee/r/Cg88K");
		}

		// Token: 0x0600001D RID: 29 RVA: 0x0000299E File Offset: 0x00000B9E
		private void button12_Click(object sender, EventArgs e)
		{
			this.api.ExecuteLink("https://pastebin.com/raw/D06N9PQ7");
		}

		// Token: 0x0600001E RID: 30 RVA: 0x000029B2 File Offset: 0x00000BB2
		private void button13_Click(object sender, EventArgs e)
		{
			this.api.ExecuteScript("loadstring(game: GetObjects('rbxassetid://2649188515')[1].Source)()");
		}

		// Token: 0x0600001F RID: 31 RVA: 0x000029C6 File Offset: 0x00000BC6
		private void button14_Click(object sender, EventArgs e)
		{
			this.api.ExecuteLink("https://pastebin.com/raw/vKhgqsJy");
		}

		// Token: 0x06000020 RID: 32 RVA: 0x000029DA File Offset: 0x00000BDA
		private void button15_Click(object sender, EventArgs e)
		{
			this.api.ExecuteLink("https://paste.ee/r/iMJaN");
			MessageBox.Show("Press R to enable/disable, Q to add speed, E to decrease speed! Enjoy!", "Instruction");
		}

		// Token: 0x06000021 RID: 33 RVA: 0x000029FE File Offset: 0x00000BFE
		private void button19_Click(object sender, EventArgs e)
		{
			this.api.ExecuteLink("https://pastebin.com/raw/tzTXmYf2");
		}

		// Token: 0x06000022 RID: 34 RVA: 0x00002A14 File Offset: 0x00000C14
		private void localPlayerToolStripMenuItem_Click(object sender, EventArgs e)
		{
			bool flag = this.currentpage == 2;
			if (flag)
			{
				this.Games.Hide();
			}
			this.localplayer.Show();
			this.currentpage = 1;
		}

		// Token: 0x06000023 RID: 35 RVA: 0x00002A50 File Offset: 0x00000C50
		private void scriptEditorToolStripMenuItem_Click(object sender, EventArgs e)
		{
			bool flag = this.currentpage == 1;
			if (flag)
			{
				this.localplayer.Hide();
			}
			bool flag2 = this.currentpage == 2;
			if (flag2)
			{
				this.Games.Hide();
			}
			this.currentpage = 0;
		}

		// Token: 0x06000024 RID: 36 RVA: 0x00002A97 File Offset: 0x00000C97
		private void pictureBox4_Click(object sender, EventArgs e)
		{
			this.api.ExecuteLink("https://paste.ee/r/hrf33");
		}

		// Token: 0x06000025 RID: 37 RVA: 0x00002AAB File Offset: 0x00000CAB
		private void pictureBox5_Click(object sender, EventArgs e)
		{
			this.api.ExecuteLink("https://pastebin.com/raw/eWxjzjnM");
		}

		// Token: 0x06000026 RID: 38 RVA: 0x00002ABF File Offset: 0x00000CBF
		private void pictureBox6_Click(object sender, EventArgs e)
		{
			this.api.ExecuteLink("https://pastebin.com/raw/D9btNZUr");
		}

		// Token: 0x06000027 RID: 39 RVA: 0x00002AD4 File Offset: 0x00000CD4
		private void gamesToolStripMenuItem_Click(object sender, EventArgs e)
		{
			bool flag = this.currentpage == 1;
			if (flag)
			{
				this.localplayer.Hide();
			}
			this.Games.Show();
			this.currentpage = 2;
		}

		// Token: 0x04000001 RID: 1
		private ExploitAPI api = new ExploitAPI();

		// Token: 0x04000002 RID: 2
		public const int WM_NCLBUTTONDOWN = 161;

		// Token: 0x04000003 RID: 3
		public const int HT_CAPTION = 2;

		// Token: 0x04000004 RID: 4
		private int currentpage = 0;

		// Token: 0x02000006 RID: 6
		public class FileObject
		{
			// Token: 0x17000007 RID: 7
			// (get) Token: 0x06000035 RID: 53 RVA: 0x00005C5A File Offset: 0x00003E5A
			// (set) Token: 0x06000036 RID: 54 RVA: 0x00005C62 File Offset: 0x00003E62
			public string FileName { get; set; }

			// Token: 0x17000008 RID: 8
			// (get) Token: 0x06000037 RID: 55 RVA: 0x00005C6B File Offset: 0x00003E6B
			// (set) Token: 0x06000038 RID: 56 RVA: 0x00005C73 File Offset: 0x00003E73
			public string FileContent { get; set; }
		}
	}
}
