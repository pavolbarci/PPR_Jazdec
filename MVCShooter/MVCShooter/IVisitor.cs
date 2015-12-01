using MVCShooter.model.gameobjects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MVCShooter
{
    interface IVisitor
    {
        void VisitCannon(Cannon cannon);
        void VisitEnemy(Enemy enemy);
        void VisitMissile(Missile misille);
        void VisitCollision(Collision collision);
    }
}
