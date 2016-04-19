using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BraitenbergProcessing.DataStructures
{
    public class LightData
    {
        public LightData()
        {
            Time = new List<double>();
            Position = new List<XYPoint>();
        }

        public List<double> Time { get; set; }
        public List<XYPoint> Position { get; set; }
        public static LightData FromFile(string path)
        {
            var reader = new StreamReader(File.OpenRead(path));
            LightData v = new LightData();
            //skip first line (header
            reader.ReadLine();
            while (reader.EndOfStream == false)
            {
                var line = reader.ReadLine();
                var pieces = line.Split(',');
                v.Time.Add(Convert.ToDouble(pieces[0]));
                v.Position.Add(new XYPoint(Convert.ToDouble(pieces[1]), Convert.ToDouble(pieces[2])));
            }
            reader.Close();

            return v;
        }
    }
}

