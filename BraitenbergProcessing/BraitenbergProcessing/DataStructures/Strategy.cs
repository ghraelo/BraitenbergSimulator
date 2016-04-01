using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace BraitenbergProcessing.DataStructures
{
    public class VStrategy
    {
        public string Type { get; set; }
        public double Gi { get; set; }
        public double Gf { get; set; }
    }
}
