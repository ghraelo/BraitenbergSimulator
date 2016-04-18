using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO;

using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace BraitenbergProcessing.DataStructures
{
    public class Scene
    {
        public string Name { get; set; }
        public List<Vehicle> Vehicles { get; set; }
        public List<Light> Lights { get; set; }

        public Scene Clone()
        {
            return (Scene)this.MemberwiseClone();
        }

        public static Scene FromFile(string path)
        { 
            //read file
            var sr = new StreamReader(File.OpenRead(path));
            var deserializer = new Deserializer(namingConvention: new CamelCaseNamingConvention());

            var result = deserializer.Deserialize<Scene>(sr);

            sr.Close();

            return result;
        }
    }
}
