using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace IceSourceUI
{
    class Functions
    {
        public static string exploitdll = "IceSource.dll";//this is the name of your dll
        public static void Inject()
        {
            if (NamedPipes.NamedPipeExist(NamedPipes.scriptpipe))//check if the pipe exist
            {
                MessageBox.Show("Already injected!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);//if the pipe exist that's mean that we don't need to inject
                return;
            }
            else if (!NamedPipes.NamedPipeExist(NamedPipes.scriptpipe))//check if the pipe don't exist
            {
                switch (Injector.DllInjector.GetInstance.Inject("RobloxPlayerBeta", AppDomain.CurrentDomain.BaseDirectory + exploitdll))//Process name and dll directory
                {
                    case Injector.DllInjectionResult.DllNotFound://if can't find the dll
                        MessageBox.Show("Couldn't find " + exploitdll, "Dll was not found!", MessageBoxButtons.OK, MessageBoxIcon.Error);//display messagebox to tell that dll was not found
                        return;
                    case Injector.DllInjectionResult.GameProcessNotFound://if can't find the process
                        MessageBox.Show("Couldn't find RobloxPlayerBeta.exe!", "Target process was not found!", MessageBoxButtons.OK, MessageBoxIcon.Error);//display messagebox to tell that proccess was not found
                        return;
                    case Injector.DllInjectionResult.InjectionFailed://if injection fails(this don't work or only on special cases)
                        MessageBox.Show("Injection Failed!", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);//display messagebox to tell that injection failed
                        return;
                }
                Thread.Sleep(2000);//pause the ui for 2 seconds
                if (NamedPipes.NamedPipeExist(NamedPipes.scriptpipe))//check if the pipe exist
                {
                    MessageBox.Show("Injected!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);//display that the pipe was found so the injection was successful
                }
                else
                {
                    MessageBox.Show("Injection Failed!\nMaybe you are Missing something\nor took more time to check if was ready", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);//display that the pipe was not found so the injection was unsuccessful
                }
            }
        }

        public static string[] TextToBox =
        {
            //Commands [0]
            "\n" +
            "Commands:\n" +
            "ff [p]\n" +
            "heaven [p]\n" +
            "ghost [p]\n" +
            "statchange [p] [stat] [#]\n" +
            "keemstar [p]\n" +
            "illuminati [p]\n" +
            "duck [p]\n" +
            "mlg [p]\n" +
            "pussy [p]\n" +
            "fog [#]\n" +
            "rfog\n" +
            "rhat [p]\n" +
            "ws [p] [#]\n" +
            "sit [p]\n" +
            "hipheight [p] [#]\n" +
            "jp [p] [#]\n" +
            "kill [p]\n" +
            "drivebloxmoney [p]\n" +
            "gravity [#]\n" +
            "btools [p]\n" +
            "god [p]\n" +
            "bigfire [p]\n" +
            "time [#]\n" +
            "select [p]\n" +
            "fencingr\n" +
            "forcechat [p] [blue/red/green]\n" +
            "charapp [p] [#]\n" +
            "noob [p]\n" +
            "fire [p]\n" +
            "smoke [p]\n" +
            "sethealth [p] [#]\n" +
            "addhealth [p] [#]\n" +
            "sparkles [p]\n" +
            "criminal\n" +
            "garage\n" +
            "bank\n" +
            "prison\n" +
            "nodoors\n" +
            "banklazers\n" +
            "jewelrycameras\n" +
            "jewelrylazers\n" +
            "jewelryflazers\n" +
            "jewelry\n" +
            "rickroll\n" +
            "ppap\n" +
            "billnye\n" +
            "illuminati\n" +
            "rage\n" +
            "cringe\n" +
            "clearws\n" +
            "unanchorall\n" +
            "fecheck\n" +
            "play [#]\n" +
            "stopmusic\n" +
            "Some Commands may not work",
            //Credits [1]
            "\n" +
            "Credits;\n" +
            "rakion99\n" +
            "Roblox\n" +
            "Eternal for RetCheck\n" +
            "AutisticBobby\n" +
            "Josh()"
        };

        public static OpenFileDialog openfiledialog = new OpenFileDialog
        {
            Filter = "Lua C Script Txt (*.txt)|*.txt|All files (*.*)|*.*",//add txt and all files filter
            FilterIndex = 1,//choose what filter will be the default
            RestoreDirectory = true//restore the last used directory
        };//Initialize OpenFileDialog
    }
}
