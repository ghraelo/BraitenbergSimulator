using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace BraitenbergProcessing.DataStructures
{
    public class YVehicleData
    {
        public string Name { get; set; }
        public float Distance { get; set; }
        [YamlAlias("numeric-data")]
        public string NumericData { get; set; }
        [YamlAlias("boundary-collisions")]
        public List<BoundaryCollision> BoundaryCollisions { get; set; }
    }
}
