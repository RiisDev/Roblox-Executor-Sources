using System;
using System.Windows.Forms;

namespace ClubDark
{
	// Token: 0x02000003 RID: 3
	internal static class Program
	{
		// Token: 0x0600002A RID: 42 RVA: 0x00005B07 File Offset: 0x00003D07
		[STAThread]
		private static void Main()
		{
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new Form1());
		}
	}
}
