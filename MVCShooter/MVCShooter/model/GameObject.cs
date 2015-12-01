using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MVCShooter.model
{
    class GameObject : IVisitable
    {
        public Position position;

        void IVisitable.Accept(IVisitor visitor)
        {
        }
    }
}
