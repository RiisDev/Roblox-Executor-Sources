using System;
using System.Diagnostics;
using System.IO;
using System.IO.Pipes;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Windows;
using TaaprWare2_UI;

namespace TaaprWare2_UI
{
    class NamedPipes
    {
        public static string luapipename = "TaaprWare2_Pipe";//Axon name of lua pipe

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool WaitNamedPipe(string name, int timeout);
        //function to check if the pipe exist
        public static bool NamedPipeExist(string pipeName)
        {
            try
            {
                if (!WaitNamedPipe($"\\\\.\\pipe\\{pipeName}", 0))
                {
                    int lastWin32Error = Marshal.GetLastWin32Error();
                    if (lastWin32Error == 0)
                    {
                        return false;
                    }
                    if (lastWin32Error == 2)
                    {
                        return false;
                    }
                }
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        //lua pipe function
        public static void LuaPipe(StringBuilder script)
        {
            int UninjectedProcesses = 0;
            Process[] _procs = Process.GetProcesses();
            for (int i = 0; i < _procs.Length; i++)
            {
                if (_procs[i].ProcessName == "RobloxPlayerBeta" && _procs[i].PagedMemorySize64 > 0x10000000)
                {
                    if (NamedPipeExist(luapipename + _procs[i].Id.ToString()))
                    {
                        try
                        {
                            using (NamedPipeClientStream namedPipeClientStream = new NamedPipeClientStream(".", luapipename + _procs[i].Id.ToString(), PipeDirection.Out))
                            {
                                namedPipeClientStream.Connect(0);
                                using (StreamWriter streamWriter = new StreamWriter(namedPipeClientStream, System.Text.Encoding.Default, 999999))//changed buffer to max 1mb since default buffer is 1kb
                                {
                                    streamWriter.Flush();
                                    streamWriter.Write(script);
                                }
                            }
                        }
                        catch (IOException)
                        {
                            MessageBox.Show("Error occured connecting to the named pipe.", "Connection Failed!", MessageBoxButton.OK, MessageBoxImage.Hand);
                        }
                        catch (TimeoutException)
                        {
                            MessageBox.Show("Named pipe doesn't exist.", "Connection Failed!", MessageBoxButton.OK, MessageBoxImage.Hand);
                        }
                        catch (Exception ex)
                        {
                            MessageBox.Show(ex.Message.ToString());
                        }
                    }
                }
            }
            if (UninjectedProcesses > 0)
            {
                MessageBox.Show("Not injected into " + UninjectedProcesses + " process" + (UninjectedProcesses == 1 ? "" : "es") + ". Please inject again so your execution will work on all processes.", "Warning", MessageBoxButton.OK, MessageBoxImage.Warning);
            }
        }
    }
}