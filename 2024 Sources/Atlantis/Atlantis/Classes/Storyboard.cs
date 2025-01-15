using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media.Animation;

namespace Atlantis.Classes
{
    public static class Animate
    {
        private static IEasingFunction Ease = new QuarticEase { EasingMode = EasingMode.EaseInOut };

        public static void Fade(UIElement Target, double From, double To, double Time = 1)
        {
            try
            {
                var animation = new DoubleAnimation()
                {
                    Duration = TimeSpan.FromSeconds(Time),
                    From = From,
                    To = To,
                    EasingFunction = Ease
                };

                Storyboard.SetTarget(animation, Target);
                Storyboard.SetTargetProperty(animation, new PropertyPath("(UIElement.Opacity)"));

                var sb = new Storyboard();
                sb.Children.Add(animation);
                sb.Begin();
            }
            catch (Exception ExceptionMessage)
            {
                Debug.Write(ExceptionMessage.ToString());
            }
        }

        public static void Move (UIElement Target, Thickness From, Thickness To, double Time = 1)
        {
            try
            {
                var animation = new ThicknessAnimation()
                {
                    Duration = TimeSpan.FromSeconds(Time),
                    From = From,
                    To = To,
                    EasingFunction = Ease
                };

                Storyboard.SetTarget(animation, Target);
                Storyboard.SetTargetProperty(animation, new PropertyPath("(Panel.Margin)"));

                var sb = new Storyboard();
                sb.Children.Add(animation);
                sb.Begin();
            }
            catch (Exception ExceptionMessage)
            {
                Debug.Write(ExceptionMessage.ToString());
            }
        }
    }
}
