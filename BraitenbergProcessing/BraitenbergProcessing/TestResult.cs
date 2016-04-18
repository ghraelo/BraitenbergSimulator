using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using BraitenbergProcessing.DataStructures;

using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace BraitenbergProcessing
{
    public class TestResult
    {
        string mPath;
        string mFolder;
        YTestResult mTestResult;

        public TestResult(string filePath, string sceneFolder)
        {
            var extension = Path.GetExtension(filePath);

            //check for correct extension
            if(extension == ".yaml")
            {
                mPath = filePath;
            }
            else
            {
                throw new FileFormatException();
            }

            mFolder = sceneFolder;

            //deserialize test file
            mTestResult = DeserializeFromFile<YTestResult>(mPath);

            //deserialise scene
            var scenePath = Path.Combine(mFolder, mTestResult.SceneFile);
            Scene = DeserializeFromFile<Scene>(scenePath);

            //get vehicle data from CSV
            VehiclesData = new Dictionary<string, VehicleData>();

            foreach (YVehicleData yvd in mTestResult.VehicleData)
            {
                string path = Path.GetDirectoryName(filePath);
                VehicleData vd = VehicleData.FromFile(Path.Combine(path,yvd.NumericData));
                VehiclesData.Add(yvd.Name, vd);
            }
        }

        /// <summary>
        /// Deserializes a YAML file.
        /// </summary>
        /// <typeparam name="T">The type to deserialize to.</typeparam>
        /// <param name="path">The path to the YAML file.</param>
        /// <returns></returns>|
        protected T DeserializeFromFile<T>(string path)
        {
            //read file
            var sr = new StreamReader(File.OpenRead(path));
            var deserializer = new Deserializer(namingConvention: new CamelCaseNamingConvention());

            var result = deserializer.Deserialize<T>(sr);

            sr.Close();

            return result;
        }

        public Scene Scene { get; }

        public Dictionary<string, VehicleData> VehiclesData { get; }

        public YTestResult InnerTestResult { get { return mTestResult; } }
    }
}
