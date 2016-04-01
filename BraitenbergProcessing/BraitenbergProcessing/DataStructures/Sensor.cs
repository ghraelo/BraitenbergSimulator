using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace BraitenbergProcessing.DataStructures
{
    public class Sensor
    {
        public List<double> Offset { get; set; }
        [YamlAlias("aperture-angle")]
        public double ApertureAngle { get; set; }
        public List<double> Direction { get; set; }
    }
}
