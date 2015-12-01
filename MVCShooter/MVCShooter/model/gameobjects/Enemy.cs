using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MVCShooter.model.gameobjects
{
    class Enemy : GameObject
    {
        
        public void Accept(IVisitor visitor)
        {
            visitor.VisitEnemy(this);
        }
    }
}
