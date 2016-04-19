using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace BraitenbergProcessing.DataStructures
{
    public class Light
    {
        public Light()
        {
            Path = new List<List<double>>();
        }
        public string Name { get; set; }
        public List<double> Position { get; set; }
        public double Radius { get; set; }
        public List<List<double>> Path { get; set; }
        public double Speed { get; set; }
    }
}
