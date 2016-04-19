using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace BraitenbergProcessing.DataStructures
{
    public class YTestResult
    {
        [YamlAlias("scene-file")]
        public string SceneFile { get; set; }
        [YamlAlias("vehicle-data")]
        public List<YVehicleData> VehicleData { get; set; }
        [YamlAlias("light-data")]
        public List<YLightData> LightData { get; set; }
     }
}
