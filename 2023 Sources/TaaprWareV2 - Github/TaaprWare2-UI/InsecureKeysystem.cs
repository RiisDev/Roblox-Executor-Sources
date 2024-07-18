using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Http;
using System.IO;
using System.Security.Policy;
using System.Net;

namespace TaaprWare2_UI
{
    internal class InsecureKeysystem
    {
        private static readonly string AppDataPath = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), "TaaprWare2");
        private static readonly string FilePath = Path.Combine(AppDataPath, "key");
        public WebClient Client
                => new WebClient { Headers = new WebHeaderCollection { { HttpRequestHeader.ContentType, "application/json" } } };

        private static void CheckKeyStorage()
        {
            if (!Directory.Exists(AppDataPath))
            {
                Console.WriteLine("AppData folder not found!");
                Directory.CreateDirectory(AppDataPath);
                Console.WriteLine("Created AppData folder");
            }
            if (!File.Exists(FilePath))
            {
                Console.WriteLine("Key storage not found!");
                using (FileStream stream = File.Create(FilePath))
                {
                    stream.WriteByte(0x50);
                }
                Console.WriteLine("Created key storage");
            }
        }

        public static void SetKeyStorage(string str)
        {
            CheckKeyStorage();
            File.WriteAllText(FilePath, str);
        }

        public static string GetKeyStorage()
        {
            CheckKeyStorage();
            try
            {
                return File.ReadAllText(FilePath);
            }
            catch
            {
                return "";
            }
        }
        public static bool IsKeyValid(string key)
        {
            CheckKeyStorage();
            try
            {
                InsecureKeysystem ik = new InsecureKeysystem();
                string response = ik.Client.DownloadString("https://1.kelprepl.repl.co/verify/taaprware?verify_key=" + key);
                ik.Client.Dispose();
                return !response.Contains(" ");
            }
            catch
            {
                return false;
            }
        }
    }
}