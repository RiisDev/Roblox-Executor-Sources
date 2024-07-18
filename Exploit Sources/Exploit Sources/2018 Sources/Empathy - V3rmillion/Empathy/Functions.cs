using System;
using System.Threading;
using System.Windows.Forms;

namespace Empathy
{
	// Token: 0x02000003 RID: 3
	internal class Functions
	{
		// Token: 0x0600000C RID: 12 RVA: 0x000038D0 File Offset: 0x00001AD0
		public static void Inject()
		{
			bool flag = NamedPipes.NamedPipeExist(NamedPipes.luapipename);
			if (flag)
			{
				MessageBox.Show("Already injected!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
			}
			else
			{
				bool flag2 = !NamedPipes.NamedPipeExist(NamedPipes.luapipename);
				if (flag2)
				{
					switch (Injector.DllInjector.GetInstance.Inject("RobloxPlayerBeta", AppDomain.CurrentDomain.BaseDirectory + Functions.exploitdllname))
					{
					case Injector.DllInjectionResult.DllNotFound:
						MessageBox.Show("Couldn't find " + Functions.exploitdllname, "Dll was not found!", MessageBoxButtons.OK, MessageBoxIcon.Hand);
						break;
					case Injector.DllInjectionResult.GameProcessNotFound:
						MessageBox.Show("Couldn't find RobloxPlayerBeta.exe!", "Target process was not found!", MessageBoxButtons.OK, MessageBoxIcon.Hand);
						break;
					case Injector.DllInjectionResult.InjectionFailed:
						MessageBox.Show("Injection Failed!", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Hand);
						break;
					default:
					{
						Thread.Sleep(3000);
						bool flag3 = !NamedPipes.NamedPipeExist(NamedPipes.luapipename);
						if (flag3)
						{
						}
						break;
					}
					}
				}
			}
		}

		// Token: 0x04000015 RID: 21
		public static string exploitdllname = "Empathy.dll";

		// Token: 0x04000016 RID: 22
		public static OpenFileDialog openfiledialog = new OpenFileDialog
		{
			Filter = "Lua Script Txt (*.txt)|*.txt|All files (*.*)|*.*",
			FilterIndex = 1,
			RestoreDirectory = true,
			Title = "Empathy Lua Open Script"
		};
	}
}
