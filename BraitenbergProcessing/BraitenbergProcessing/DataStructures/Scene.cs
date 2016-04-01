using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace BraitenbergProcessing.DataStructures
{
    public class Scene
    {
        public string Name { get; set; }
        public List<Vehicle> Vehicles { get; set; }
        public List<Light> Lights { get; set; }
    }
}
