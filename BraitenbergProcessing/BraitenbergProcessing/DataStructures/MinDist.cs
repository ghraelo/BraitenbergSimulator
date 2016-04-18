using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace BraitenbergProcessing.DataStructures
{
    public class MinDist
    {
        [YamlAlias("light-name")]
        public string LightName { get; set; }
        [YamlAlias("min-dist")]
        public float Distance { get; set; }
        [YamlAlias("vehicle-name")]
        public string VehicleName { get; set; }
    }
}
