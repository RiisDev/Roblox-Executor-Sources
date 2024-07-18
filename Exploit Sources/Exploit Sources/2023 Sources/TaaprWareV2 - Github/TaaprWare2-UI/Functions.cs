using Microsoft.Win32;
using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Windows;
using TaaprWare2_UI;

namespace TaaprWare2_UI
{
    class Functions
    {
        [DllImport("kernel32.dll")]
        static extern IntPtr GetConsoleWindow();

        [DllImport("user32.dll")]
        static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);

        const int SW_HIDE = 0;
        const int SW_SHOW = 5;
        public static string exploitdllname = "./bin/TaaprWare2Internal.dll";
        public static void Inject()
        {
            Console.Clear();
            ShowWindow(GetConsoleWindow(), SW_SHOW);
            while (true)
            {
                Console.WriteLine("Checking key...");
                if (!InsecureKeysystem.IsKeyValid(InsecureKeysystem.GetKeyStorage()))
                {
                    Console.WriteLine("Invalid key!");
                    Console.WriteLine("Get a key at https://1.kelprepl.repl.co/getkey/taaprware and enter it below:");
                    Console.Write(">");
                    InsecureKeysystem.SetKeyStorage(Console.ReadLine());
                    continue;
                }
                break;
            }
            Console.WriteLine("Key valid! Injecting...");
            System.Threading.Thread.Sleep(500);
            Console.Clear();
            ShowWindow(GetConsoleWindow(), SW_HIDE);
            if (NamedPipes.NamedPipeExist(NamedPipes.luapipename))
            {
                MessageBox.Show("Already injected!", "Error", MessageBoxButton.OK, MessageBoxImage.Asterisk);
                return;
            }
            else
            {

                switch (Injector.DllInjector.GetInstance.Inject("Roblox", AppDomain.CurrentDomain.BaseDirectory + exploitdllname))
                {
                    case Injector.DllInjectionResult.DllNotFound:
                        MessageBox.Show($"Couldn't find {exploitdllname}!", "Dll not found", MessageBoxButton.OK, MessageBoxImage.Error);
                        return;
                    case Injector.DllInjectionResult.GameProcessNotFound:
                        MessageBox.Show("Couldn't find Roblox!", "Target process not found", MessageBoxButton.OK, MessageBoxImage.Error);
                        return;
                    case Injector.DllInjectionResult.InjectionFailed:
                        MessageBox.Show("Injection Failed!", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                        return;
                    case Injector.DllInjectionResult.Success:
                        Process[] _procs = Process.GetProcesses();
                        for (int i = 0; i < _procs.Length; i++)
                        {
                            if (_procs[i].MainWindowTitle == "Roblox")
                            {
                                while (!NamedPipes.NamedPipeExist(NamedPipes.luapipename + _procs[i].Id.ToString()))
                                {
                                    System.Threading.Thread.Sleep(100);
                                }
                            }
                        }
                        StringBuilder script = new StringBuilder();
                        DirectoryInfo directory = new DirectoryInfo(".\\autoexec");
                        foreach (FileInfo file in directory.GetFiles())
                        {
                            script.Append("spawn(function()\nlocal success = pcall(function()\n");
                            script.Append(File.ReadAllText(file.FullName));
                            script.Append("\nend)\nend)\n");
                        }
                        if (script.Length > 0)
                        {
                            NamedPipes.LuaPipe(script);
                        }
                        return;
                }

            }
        }

        public static OpenFileDialog openfiledialog = new OpenFileDialog
        {
            Filter = "Script File|*.txt;*.lua|All files (*.*)|*.*",//add txt,lua and all files filter
            FilterIndex = 1,//choose what filter will be the default
            RestoreDirectory = true,//restore the last used directory
            Title = "Axon Open Script"//OpenFileDialog Tittle
        };//Initialize OpenFileDialog
    }
}