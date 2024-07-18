using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

namespace Empathy
{
	// Token: 0x02000004 RID: 4
	internal class Injector
	{
		// Token: 0x0200000A RID: 10
		public enum DllInjectionResult
		{
			// Token: 0x0400001D RID: 29
			DllNotFound,
			// Token: 0x0400001E RID: 30
			GameProcessNotFound,
			// Token: 0x0400001F RID: 31
			InjectionFailed,
			// Token: 0x04000020 RID: 32
			Success
		}

		// Token: 0x0200000B RID: 11
		public sealed class DllInjector
		{
			// Token: 0x0600001D RID: 29
			[DllImport("kernel32.dll", SetLastError = true)]
			private static extern IntPtr OpenProcess(uint dwDesiredAccess, int bInheritHandle, uint dwProcessId);

			// Token: 0x0600001E RID: 30
			[DllImport("kernel32.dll", SetLastError = true)]
			private static extern int CloseHandle(IntPtr hObject);

			// Token: 0x0600001F RID: 31
			[DllImport("kernel32.dll", SetLastError = true)]
			private static extern IntPtr GetProcAddress(IntPtr hModule, string lpProcName);

			// Token: 0x06000020 RID: 32
			[DllImport("kernel32.dll", SetLastError = true)]
			private static extern IntPtr GetModuleHandle(string lpModuleName);

			// Token: 0x06000021 RID: 33
			[DllImport("kernel32.dll", SetLastError = true)]
			private static extern IntPtr VirtualAllocEx(IntPtr hProcess, IntPtr lpAddress, IntPtr dwSize, uint flAllocationType, uint flProtect);

			// Token: 0x06000022 RID: 34
			[DllImport("kernel32.dll", SetLastError = true)]
			private static extern int WriteProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] buffer, uint size, int lpNumberOfBytesWritten);

			// Token: 0x06000023 RID: 35
			[DllImport("kernel32.dll", SetLastError = true)]
			private static extern IntPtr CreateRemoteThread(IntPtr hProcess, IntPtr lpThreadAttribute, IntPtr dwStackSize, IntPtr lpStartAddress, IntPtr lpParameter, uint dwCreationFlags, IntPtr lpThreadId);

			// Token: 0x17000004 RID: 4
			// (get) Token: 0x06000024 RID: 36 RVA: 0x00003BC0 File Offset: 0x00001DC0
			public static Injector.DllInjector GetInstance
			{
				get
				{
					bool flag = Injector.DllInjector._instance == null;
					if (flag)
					{
						Injector.DllInjector._instance = new Injector.DllInjector();
					}
					return Injector.DllInjector._instance;
				}
			}

			// Token: 0x06000025 RID: 37 RVA: 0x00003B14 File Offset: 0x00001D14
			private DllInjector()
			{
			}

			// Token: 0x06000026 RID: 38 RVA: 0x00003BF0 File Offset: 0x00001DF0
			public Injector.DllInjectionResult Inject(string sProcName, string sDllPath)
			{
				bool flag = !File.Exists(sDllPath);
				Injector.DllInjectionResult result;
				if (flag)
				{
					result = Injector.DllInjectionResult.DllNotFound;
				}
				else
				{
					uint num = 0u;
					Process[] processes = Process.GetProcesses();
					for (int i = 0; i < processes.Length; i++)
					{
						bool flag2 = processes[i].ProcessName == sProcName;
						if (flag2)
						{
							num = (uint)processes[i].Id;
							break;
						}
					}
					bool flag3 = num == 0u;
					if (flag3)
					{
						result = Injector.DllInjectionResult.GameProcessNotFound;
					}
					else
					{
						bool flag4 = !this.bInject(num, sDllPath);
						if (flag4)
						{
							result = Injector.DllInjectionResult.InjectionFailed;
						}
						else
						{
							result = Injector.DllInjectionResult.Success;
						}
					}
				}
				return result;
			}

			// Token: 0x06000027 RID: 39 RVA: 0x00003C80 File Offset: 0x00001E80
			private bool bInject(uint pToBeInjected, string sDllPath)
			{
				IntPtr intPtr = Injector.DllInjector.OpenProcess(1082u, 1, pToBeInjected);
				bool flag = intPtr == Injector.DllInjector.INTPTR_ZERO;
				bool result;
				if (flag)
				{
					result = false;
				}
				else
				{
					IntPtr procAddress = Injector.DllInjector.GetProcAddress(Injector.DllInjector.GetModuleHandle("kernel32.dll"), "LoadLibraryA");
					bool flag2 = procAddress == Injector.DllInjector.INTPTR_ZERO;
					if (flag2)
					{
						result = false;
					}
					else
					{
						IntPtr intPtr2 = Injector.DllInjector.VirtualAllocEx(intPtr, (IntPtr)null, (IntPtr)sDllPath.Length, 12288u, 64u);
						bool flag3 = intPtr2 == Injector.DllInjector.INTPTR_ZERO;
						if (flag3)
						{
							result = false;
						}
						else
						{
							byte[] bytes = Encoding.ASCII.GetBytes(sDllPath);
							bool flag4 = Injector.DllInjector.WriteProcessMemory(intPtr, intPtr2, bytes, (uint)bytes.Length, 0) == 0;
							if (flag4)
							{
								result = false;
							}
							else
							{
								bool flag5 = Injector.DllInjector.CreateRemoteThread(intPtr, (IntPtr)null, Injector.DllInjector.INTPTR_ZERO, procAddress, intPtr2, 0u, (IntPtr)null) == Injector.DllInjector.INTPTR_ZERO;
								if (flag5)
								{
									result = false;
								}
								else
								{
									Injector.DllInjector.CloseHandle(intPtr);
									result = true;
								}
							}
						}
					}
				}
				return result;
			}

			// Token: 0x04000021 RID: 33
			private static readonly IntPtr INTPTR_ZERO = (IntPtr)0;

			// Token: 0x04000022 RID: 34
			private static Injector.DllInjector _instance;
		}
	}
}
