using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using OxyPlot;

namespace BraitenbergProcessing.DataStructures
{
    public struct XYPoint
    {
        public XYPoint(double x, double y)
        {
            X = x;
            Y = y;
        }

        public double X { get; set; }
        public double Y { get; set; }

        public static explicit operator DataPoint(XYPoint pt)
        {
            return new DataPoint(pt.X, pt.Y);
        }
    }
}
