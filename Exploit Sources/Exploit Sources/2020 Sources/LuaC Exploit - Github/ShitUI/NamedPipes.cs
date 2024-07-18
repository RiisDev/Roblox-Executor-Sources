using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Pipes;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ShitUI
{
    
    class NamedPipes
    {
        public static string dllname = "BuaBBog.dll";
        public static string cmdpipename = "Commandpipe";//name of command pipe
        public static string luacpipename = "LuaCpipe";//

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool WaitNamedPipe(string name, int timeout);
        //function to check if the pipe exist
        public static bool NamedPipeExist(string pipeName)
        {
            bool result;
            try
            {
                int timeout = 0;
                if (!WaitNamedPipe(Path.GetFullPath(string.Format("\\\\\\\\.\\\\pipe\\\\{0}", pipeName)), timeout))
                {
                    int lastWin32Error = Marshal.GetLastWin32Error();
                    if (lastWin32Error == 0)
                    {
                        result = false;
                        return result;
                    }
                    if (lastWin32Error == 2)
                    {
                        result = false;
                        return result;
                    }
                }
                result = true;
            }
            catch (Exception)
            {
                result = false;
            }
            return result;
        }
        //command pipe function
        public static void CommandPipe(string command)
        {
            if (NamedPipeExist(cmdpipename))
            {
                new Thread(() =>//lets run this in another thread so if roblox crash the ui/gui don't freeze or something
                {
                    try
                    {
                        using (NamedPipeClientStream namedPipeClientStream = new NamedPipeClientStream(".", cmdpipename, PipeDirection.Out))
                        {
                            namedPipeClientStream.Connect();
                            using (StreamWriter streamWriter = new StreamWriter(namedPipeClientStream))
                            {
                                streamWriter.Write(command);
                                streamWriter.Dispose();
                            }
                            namedPipeClientStream.Dispose();
                        }
                    }
                    catch (IOException)
                    {
                        MessageBox.Show("Error occured connecting to the pipe.", "Connection Failed!", MessageBoxButtons.OK, MessageBoxIcon.Hand);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message.ToString());
                    }
                }).Start();
            }
            else
            {
                MessageBox.Show("Inject " + dllname + " before Using this!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }
        }
        //lua c pipe function
        public static void LuaCPipe(string script)
        {
            if (NamedPipeExist(luacpipename))
            {
                new Thread(() =>//lets run this in another thread so if roblox crash the ui/gui don't freeze or something
                {
                    try
                    {
                        using (NamedPipeClientStream namedPipeClientStream = new NamedPipeClientStream(".", luacpipename, PipeDirection.Out))
                        {
                            namedPipeClientStream.Connect();
                            using (StreamWriter streamWriter = new StreamWriter(namedPipeClientStream, System.Text.Encoding.Default, 999999))//changed buffer to max 1mb since default buffer is 1kb
                            {
                                streamWriter.Write(script);
                                streamWriter.Dispose();
                            }
                            namedPipeClientStream.Dispose();
                        }
                    }
                    catch (IOException)
                    {
                        MessageBox.Show("Error occured connecting to the pipe.", "Connection Failed!", MessageBoxButtons.OK, MessageBoxIcon.Hand);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message.ToString());
                    }
                }).Start();
            }
            else
            {
                MessageBox.Show("Inject " + dllname + " before Using this!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }
        }
    }
}
