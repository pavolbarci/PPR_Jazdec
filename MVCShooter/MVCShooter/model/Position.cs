using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MVCShooter.model
{
    class Position
    {
        public int x;
        public int y;

        public void Move(int i, int j)
        {
            this.x += i;
            this.y += j;
        }
    }
}
