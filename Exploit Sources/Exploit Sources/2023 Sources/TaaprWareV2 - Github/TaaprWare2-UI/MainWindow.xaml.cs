using ICSharpCode.AvalonEdit.Highlighting.Xshd;
using ICSharpCode.AvalonEdit.Highlighting;
using System.IO;
using System;
using System.Xml;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.TextBox;
using System.Runtime.InteropServices;

namespace TaaprWare2_UI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>

    public partial class MainWindow : Window
    {
        [DllImport("kernel32.dll")]
        static extern IntPtr GetConsoleWindow();

        [DllImport("user32.dll")]
        static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);
        const int SW_HIDE = 0;
        public MainWindow()
        {
            InitializeComponent();
            ShowWindow(GetConsoleWindow(), SW_HIDE);
            Color string_color = new Color
            {
                A = 0xFF,
                R = 0x15,
                G = 0x81,
                B = 0xFF
            };
            SolidColorBrush string_brush = new SolidColorBrush(string_color);
            CodeBox.TextArea.TextView.LinkTextForegroundBrush = string_brush;
            Stream stream = File.OpenRead("./bin/SyntaxHighlighting.xshd");
            if (stream.CanRead)
            {
                XmlTextReader reader = new XmlTextReader(stream);
                CodeBox.SyntaxHighlighting = HighlightingLoader.Load(reader, HighlightingManager.Instance);
            }
            else
            {
                MessageBox.Show("Couldn't read bin/SyntaxHighlighting.xshd", "Warning", MessageBoxButton.OK, MessageBoxImage.Warning);
            }
        }

        private void RichTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            
        }

        public static string ReadFile(string filePath)
        {
            // Declare a string variable to store the file contents
            string fileContents;

            // Use a try-catch block to handle any exceptions that may occur
            try
            {
                // Read the file contents into the string variable
                fileContents = File.ReadAllText(filePath);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Custom functions are disabled because an error reading the InitScript.lua file: " + ex.Message, "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return "";
            }

            // Return the file contents
            return fileContents;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (sender == Inject)
            {
                TaaprWare2_UI.Functions.Inject();
            }
            else if (sender == Execute)
            {
                int FoundPipes = 0;

                Process[] _procs = Process.GetProcesses();
                for (int i = 0; i < _procs.Length; i++)
                {
                    if (_procs[i].ProcessName == "RobloxPlayerBeta")
                    {
                        if (NamedPipes.NamedPipeExist(NamedPipes.luapipename + _procs[i].Id.ToString()))
                        {
                            FoundPipes++;
                        }
                    }
                }
                if (FoundPipes > 0)
                {
                    StringBuilder script = new StringBuilder("spawn(function()\n");
                    script.Append(ReadFile("bin/InitScript.lua"));
                    script.Append("\n");
                    script.Append(CodeBox.Text);
                    script.Append("\nend)");
                    NamedPipes.LuaPipe(script);
                }
                else
                {
                    MessageBox.Show("Not injected", "Error", MessageBoxButton.OK, MessageBoxImage.Exclamation);//if the pipe can't be found a messagebox will appear
                    return;
                }
            }
            else if (sender == CloseRoblox)
            {
                foreach (var process in Process.GetProcessesByName("RobloxPlayerBeta"))
                {
                    process.Kill();
                }
            }
            else if (sender == Discord)
            {
                Process.Start("https://discord.gg/nAEHrW9EF9");
            }
        }

        private void CodeBoxHost_ChildChanged(object sender, System.Windows.Forms.Integration.ChildChangedEventArgs e)
        {
            
        }

        private void Top_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
                this.DragMove();
        }

        private void TopButton_Click(object sender, RoutedEventArgs e)
        {
            
        }

        private void TopButton_MouseDown(object sender, MouseButtonEventArgs e)
        {
            
        }

        private void Windo_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
                this.DragMove();
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            if (sender == _Close)
            {
                GetWindow(Windo).Close();
            }
            else if (sender == Minimise)
            {
                WindowState = WindowState.Minimized;
            }
        }
    }
}
