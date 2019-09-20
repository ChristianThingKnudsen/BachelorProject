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
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace BachelorProject
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        
        public MainWindow()
        {
            InitializeComponent();
            // Setting collors
            BrushConverter bc = new BrushConverter();         
            mainWindow.Background = (Brush)bc.ConvertFrom("#0F4571");
            ButtonLoadCT.Background = (Brush)bc.ConvertFrom("#1367AB");
            ButtonLoadCBCT.Background = (Brush)bc.ConvertFrom("#1367AB");
            

        }
        private void loadCT(object sender, RoutedEventArgs e)
        {
            int test = Dllwrapper.BuildRTKCommandFilter();
            MessageBox.Show("Number: "+ test);
        }
        private void loadCBCT(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("This Button was Clicked");
        }
        private void checkCheckBoxCT(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("This Button was Clicked");
        }
        private void goToInfo(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("This Button was Clicked");
        }
    }
}
