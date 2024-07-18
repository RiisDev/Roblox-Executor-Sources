using System;
using System.Windows.Forms;

namespace Empathy
{
	// Token: 0x02000006 RID: 6
	internal static class Program
	{
		// Token: 0x06000015 RID: 21 RVA: 0x00003AF9 File Offset: 0x00001CF9
		[STAThread]
		private static void Main()
		{
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new Form1());
		}
	}
}
