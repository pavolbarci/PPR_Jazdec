using MVCShooter.model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MVCShooter.controller
{
    class Controller
    {
        private Model model;

        public Controller(Model model)
        {
            this.model = model;
        }

        public void KeyPressed(KeyEventArgs e)
        {
            switch (e.KeyCode)
            {
            case Keys.Up:
                //model.MoveCannonUp();
            break;
            default:
                break;
        }
        }

        public Model GetModel() { return this.model; }
    }
}
