using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MVCShooter
{
    interface IVisitable
    {
        void Accept(IVisitor visitor);
    }
}
