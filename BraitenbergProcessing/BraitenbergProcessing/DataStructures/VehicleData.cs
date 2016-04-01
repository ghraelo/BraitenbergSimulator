using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BraitenbergProcessing.DataStructures
{
    class VehicleData
    {
        public VehicleData()
        {
            Events = new List<string>();
            Time = new List<double>();
            Position = new List<XYPoint>();
        }

        public List<string> Events { get; set; }
        public List<double> Time { get; set; }
        public List<XYPoint> Position { get; set; }
    }
}
