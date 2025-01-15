using Atlantis.Classes;
using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;
using System.Windows.Threading;
using System.Diagnostics;
using System.Windows.Media.Media3D;
using System.Diagnostics.SymbolStore;
using System.Net;
using System.Net.WebSockets;
using System.Threading;

namespace Atlantis
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        List<Border> adsList;
        Style tabStyle;
        ScriptBlox scriptBloxAPI;
        List<Grid> gridlist;
        List<Button> buttonlist;
        List<Label> labellist;

        private const int WS_NOP = 77;
        private const int WS_EXEC = 78;
        private const int WS_MSG = 80;

        private static WebSocket global_websocket = null;
        private static Thread webSocketThread = null;

        public MainWindow()
        {
            InitializeComponent();
        }

        private static async Task WebSocket_Connection()
        {
            HttpListener httpListener = new HttpListener();
            httpListener.Prefixes.Add("http://localhost:8080/");
            httpListener.Start();

            while (true)
            {
                HttpListenerContext context = await httpListener.GetContextAsync();

                if (context.Request.IsWebSocketRequest)
                {
                    HttpListenerWebSocketContext wsContext = await context.AcceptWebSocketAsync(null);
                    global_websocket = wsContext.WebSocket;
                    _ = OnWebSocketConnection(wsContext.WebSocket);
                }
                else
                {
                    context.Response.StatusCode = 400;
                    context.Response.Close();
                }
            }
        }

        private static async Task OnWebSocketConnection(WebSocket webSocket)
        {
            byte[] buffer = new byte[1024];
            WebSocketReceiveResult result;

            try
            {
                while (webSocket.State == WebSocketState.Open)
                {
                    Application.Current.Dispatcher.Invoke(() =>
                    {
                        var mainWindow = (MainWindow)Application.Current.MainWindow;
                        mainWindow.AttachStatus.Content = "ATTACHED";
                    });
                    result = await webSocket.ReceiveAsync(new ArraySegment<byte>(buffer), CancellationToken.None);

                    if (result.MessageType == WebSocketMessageType.Close)
                    {
                        global_websocket = null;
                        Application.Current.Dispatcher.Invoke(() =>
                        {
                            var mainWindow = (MainWindow)Application.Current.MainWindow;
                            mainWindow.AttachStatus.Content = "NOT ATTACHED";
                        });
                        await webSocket.CloseAsync(WebSocketCloseStatus.NormalClosure, "Closing", CancellationToken.None);
                    }
                    else
                    {
                        string message = Encoding.UTF8.GetString(buffer, 0, result.Count);
                    }
                }
            }
            catch (Exception ex)
            {
                // L nigga i dont care haha
            }
            finally
            {
                webSocket.Dispose();
                global_websocket = null;
            }
        }


        private void OnLoad(object sender, RoutedEventArgs e)
        {
            adsList = new List<Border> { ad1, ad2, ad3 };
            tabStyle = TryFindResource("TabStyle") as Style;
            scriptBloxAPI = new ScriptBlox();
            gridlist = new List<Grid> { g1, g2, g3, g4 };
            buttonlist = new List<Button> { b1, b2, b3, b4 };
            labellist  = new List<Label> { l1, l2, l3, l4 };
            HomePageButton.IsChecked = true;

            var current = 1;
            var timer = new DispatcherTimer { IsEnabled = true, Interval = TimeSpan.FromSeconds(3) };
            timer.Tick += (x, y) =>
            {
                Animate.Fade(adsList[current == 0 ? 2 : current - 1], 1, 0, 1);
                Animate.Fade(adsList[current], 0, 1, 1);

                current++;
                if (current > 2)
                    current = 0;
            };
            InitializeScripts();
            TabControl.Items.Add(TabsAPI.CreateNewTab(tabStyle, "Untitled " + (TabControl.Items.Count + 1).ToString()));

            webSocketThread = new Thread(() =>
            {
                WebSocket_Connection().GetAwaiter().GetResult();
            });

            webSocketThread.Start();
        }

        private void MenuCheckedHandle(object sender, RoutedEventArgs e)
        {
            HomeGrid.Visibility = Visibility.Hidden;
            ExecutorGrid.Visibility = Visibility.Hidden;
            ScriptHubGrid.Visibility = Visibility.Hidden;

            switch (((RadioButton)sender).Name)
            {
                case "HomePageButton":
                    HomeGrid.Visibility = Visibility.Visible;
                    Animate.Move(TabIndicator, TabIndicator.Margin, new Thickness(-13, 24, 68, 175), 0.4);
                    break;
                case "ExecutorPageButton":
                    ExecutorGrid.Visibility = Visibility.Visible;
                    Animate.Move(TabIndicator, TabIndicator.Margin, new Thickness(-13, 75, 68, 124), 0.4);
                    break;
                case "ScriptHubPageButton":
                    ExecutorGrid.Visibility = Visibility.Visible;
                    Animate.Move(TabIndicator, TabIndicator.Margin, new Thickness(-13, 75, 68, 124), 0.4);
                    break;
            }
        }

        private async void ButtonHandler(object sender, RoutedEventArgs e)
        {
            var monaco = TabControl.SelectedContent as WebView2API;

            switch (((Button)sender).Name)
            {
                case "MinimizeButton":
                    this.WindowState = WindowState.Minimized;

                    break;
                case "AddTabButton":
                    TabControl.Items.Add(TabsAPI.CreateNewTab(tabStyle, "Untitled " + (TabControl.Items.Count + 1).ToString()));
                    break;
                case "CloseTabButton":
                    if (TabControl.Items.Count > 1)
                        TabControl.Items.Remove(TabControl.SelectedItem);
                    break;
                case "ExecuteButton":
                    if (global_websocket == null || global_websocket.State != WebSocketState.Open)
                    {
                        MessageBox.Show("Atlantis is not attached!");
                    }
                    else
                    {
                        string content = await monaco.GetText();
                        string msg = ((char)WS_EXEC) + content;// replaece with the monaco thing or whatever
                        byte[] msgBytes = Encoding.UTF8.GetBytes(msg);
                        await global_websocket.SendAsync(new ArraySegment<byte>(msgBytes), WebSocketMessageType.Text, true, CancellationToken.None);
                    }
                    break;
                case "ExecuteIY":
                    // TO DO FOR LXZP
                    break;
                case "AttachButton":
                    if (global_websocket == null)
                    {
                        string injector_path = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "bin", "oRniz4vtL94l.exe");

                        if (File.Exists(injector_path))
                        {

                            Application.Current.Dispatcher.Invoke(() =>
                            {
                                var mainWindow = (MainWindow)Application.Current.MainWindow;
                                mainWindow.AttachStatus.Content = "ATTACHING";
                            });
                            ProcessStartInfo startInfo = new ProcessStartInfo
                                {
                                    FileName = injector_path,
                                    Arguments = $"keyhere",
                                    UseShellExecute = false,
                                    CreateNoWindow = true
                                };

                                try
                                {
                                    Process process = Process.Start(startInfo);
                                }
                                catch (Exception ex)
                                {
                                    MessageBox.Show("An Unknown Exception occured while trying to attach to roblox.");
                                }
    
                        }
                        else
                        {
                            MessageBox.Show("Injector wasn't found\nMake sure to disable the Windows Defender");
                        }
                    }
                    else
                    {
                        MessageBox.Show("Atlantis is already attached!");
                    }
                    break;
                case "ClearButton":
                    monaco.SetText("");
                    break;
                case "OpenTabButton":
                    var dialog = new OpenFileDialog();

                    if (dialog.ShowDialog() == true)
                        monaco.SetText(File.ReadAllText(dialog.FileName));
                    break;
                case "SaveTabButton":
                    var sdialog = new SaveFileDialog();

                    if (sdialog.ShowDialog() == true)
                        File.WriteAllText(sdialog.FileName, await monaco.GetText());
                    break;
                case "DiscordButton":
                    Process.Start("https://discord.gg/getatlantis");
                    break;
            }
        }

        // TEMP IGNORE ASS CODE
        async void InitializeScripts()
        {
            //scriptBloxAPI.FetchPopularScripts();
            //await Task.Delay(1000);
            //List<Script> scripts = r.rr;

            //for (int i = 0; i < 3; i++)
            //{
            //    var script = scripts[i];
            //    string title = script.title;
            //    string imageUrl = script.game.imageUrl;

            //    var b = new ImageBrush();
            //    b.ImageSource = new BitmapImage(new Uri(new WebClient().DownloadString(imageUrl)));

            //    labellist[i].Content = title;
            //    gridlist[i].Background = b;
            //}
        }

 

        private void OnUnload(object sender, RoutedEventArgs e)
        {
            webSocketThread.Interrupt();

            if (global_websocket != null)
            {
                global_websocket.Dispose();
            }
        }

 
    }
}
