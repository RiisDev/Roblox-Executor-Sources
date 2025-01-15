using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace Atlantis.Classes
{
    public static class TabsAPI
    {
        public static TabItem CreateNewTab(Style TabStyle, string Title = "", string Content = "")
            => new TabItem { Header = Title, Content = CreateNewEditor(Content), Style = TabStyle, FontSize = 13, Foreground = Brushes.White, IsSelected = true};

        public static WebView2API CreateNewEditor(string Content)
            => new WebView2API(Content);
    }
}