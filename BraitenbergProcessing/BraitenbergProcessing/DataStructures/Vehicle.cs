using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace BraitenbergProcessing.DataStructures
{
    public class Vehicle
    {
        public string Name { get; set; }
        public List<double> Position { get; set; }
        public VStrategy Strategy { get; set; }
        [YamlAlias("left-sensor")]
        public Sensor LeftSensor { get; set; }
        [YamlAlias("right-sensor")]
        public Sensor rightSensor { get; set; }
    }
}
