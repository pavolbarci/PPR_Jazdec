using MVCShooter.model.gameobjects;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MVCShooter.view
{
    class GraphicsDrawer : IVisitor
    {
        private static int INFO_X = 5;
        private static int INFO_Y = 15;

        private Graphics graphics;

        private Image cannonImage;
        private Image enemyImage1;
        private Image enemyImage2;
        private Image missileImage;
        private Image collisionImage;

        public void VisitCannon(Cannon cannon) { drawCannon(graphics, cannon); }
        public void VisitEnemy(Enemy enemy) { drawEnemy(graphics, enemy); }
        public void VisitMissile(Missile missile) { drawMissile(graphics, missile); }
        public void VisitCollision(Collision collision) { drawCollision(graphics, collision); }

        public GraphicsDrawer(Graphics graphics)
        {
            try
            {
                this.graphics = graphics;
                cannonImage = Image.FromFile("cannon.png");
                enemyImage1 = Image.FromFile("/images/enemy1.png");
                enemyImage2 = Image.FromFile("/images/enemy2.png");
                missileImage = Image.FromFile("/images/missile.png");
                collisionImage = Image.FromFile("/images/collision.png");
            }
            catch (IOException ex)
            {
                Console.WriteLine(ex.Message);
            }
        }


        public void drawCannon(Graphics g, Cannon cannon)
        {
            g.DrawImage(cannonImage, 
                new Point(cannon.GetX() - cannonImage.Width / 2, cannon.GetY() - cannonImage.Height / 2));
        }

        public void drawMissile(Graphics g, Missile missile)
        {

        }

        public void drawEnemy(Graphics g, Enemy enemy)
        {

        }

        public void drawCollision(Graphics g, Collision collision)
        {

        }

        //public void drawInfo(Graphics g, ModelInfo info)
        //{

        //}        

        //class ModelInfo { }
    }
}
