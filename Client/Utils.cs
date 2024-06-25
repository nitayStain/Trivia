using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;
using System.Windows;

namespace Client
{
    public static class Utils
    {
        public static T FindElementByTag<T>(this DependencyObject parent, object tag) where T : FrameworkElement
        {
            if (parent == null) return null;

            // Check if the current parent matches the tag
            if (parent is T frameworkElement && frameworkElement.Tag?.Equals(tag) == true)
            {
                return frameworkElement;
            }

            // Traverse children
            int childrenCount = VisualTreeHelper.GetChildrenCount(parent);
            for (int i = 0; i < childrenCount; i++)
            {
                var child = VisualTreeHelper.GetChild(parent, i);
                var result = FindElementByTag<T>(child, tag);
                if (result != null) return result;
            }

            return null;
        }
    }
}
