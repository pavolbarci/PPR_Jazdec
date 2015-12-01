using MVCShooter.controller;
using MVCShooter.model;
using MVCShooter.view;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MVCShooter
{
    public partial class MainWindow : Form
    {
        private static Model model = new Model();
        private static Controller controller = new Controller(model);
        
        public MainWindow()
        {
            try
            {
                Canvas view = new Canvas(0, 0, 500, 500, controller.GetModel(), this.CreateGraphics());
                this.Text = "MyShooter";
               // Dimension window = Toolkit.getDefaultToolkit().getScreenSize();
                this.Location = new Point((int)(Screen.GetBounds(this).Width / 2 - 250),
                                          (int)(Screen.GetBounds(this).Height / 2 - 250));
                this.KeyPreview = true;
                this.KeyUp += new KeyEventHandler(KeyEvent);
                this.Controls.Add(view);
                //this.KeyPress = { }
                //this.keyPressed(new KeyAdapter() {
                //@Override
                //public void keyPressed(KeyEvent evt)
                //{
                //     // delegate to controller
                //        System.out.println("key pressed: " + evt.getKeyChar());
                //        controller.KeyPressed(evt);
                //}
                //});

                //this.add(view);
                //this.pack();
           } 
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }

        private void KeyEvent(object sender, KeyEventArgs e)
        {
            Console.WriteLine("Key pressed: " + e.KeyValue);
            controller.KeyPressed(e);
        }

        public void showHelp()
        {
            MessageBox.Show("Controls: \n" + "here goes some description...");
        }
    }
}
