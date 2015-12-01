using MVCShooter.model;
using MVCShooter.model.gameobjects;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MVCShooter.view
{
    class Canvas : UserControl, IObserver
    {
        GraphicsDrawer drawer;
        Model model;
        Graphics graphics;

        public Canvas(int x, int y, int width, int height, Model model, Graphics graphics)
        {
            this.graphics = graphics;
            this.drawer = new GraphicsDrawer(graphics);
            this.BackColor = Color.White;
            //this.setDoubleBuffered(true);
            this.Location = new Point(x, y);
            this.Size = new Size(width, height);
            this.Visible = true;
            this.model = model;
        }


        public void Update()
        {
            this.Refresh();
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            drawer.drawCannon(graphics, new Cannon());
        }
    }
}
