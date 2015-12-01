using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MVCShooter.model.gameobjects
{
    class Cannon : GameObject
    {
        public Cannon()
        {

        }

        public Missile Shoot()
        {
            Console.WriteLine("move up");
            return new Missile();
        }

        public void MoveUp()
        {
            position.Move(1, 1);
        }

        public int GetX() { return position.x; }
        public int GetY() { return position.y; }
        
        public void Accept(IVisitor visitor)
        {
            visitor.VisitCannon(this);
        }
    }
}
