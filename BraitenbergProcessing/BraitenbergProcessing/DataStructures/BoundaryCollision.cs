using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace BraitenbergProcessing.DataStructures
{
    public class BoundaryCollision
    {
        public string Type { get; set; }
        public List<double> Position { get; set; }
        public double Time { get; set; }
    }
}
