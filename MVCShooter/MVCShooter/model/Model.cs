using MVCShooter.model.gameobjects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MVCShooter.model
{
    class Model : IObservable
    {
        private List<Missile> missiles;
        private List<Enemy> enemies;
        private List<Collision> collisions;
        private List<IObserver> observers = new List<IObserver>();
        private Cannon cannon;
        //private Timer timer;
        private int gravity;

        public void Attach(IObserver observer) { observers.Add(observer); }
        public void Detach(IObserver observer) { observers.Remove(observer); }

        public Model()
        {
            missiles = new List<Missile>();
            enemies = new List<Enemy>();
            collisions = new List<Collision>();
            cannon = new Cannon();
            //timer = new Timer();
        }

        public void MoveCannonUp()
        {
            cannon.MoveUp();
            this.Notify();
        }

        public void ChangeCannonAngle()
        {

        }

        public void Shoot()
        {
            missiles.Add(cannon.Shoot());
        }

        public void ChangeGravity()
        {

        }

        public void Notify()
        {
            foreach (IObserver observer in observers)
            {
                observer.Update();
            }
        }
    }
}
