using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BraitenbergProcessing.DataStructures
{
    public class VehicleData
    {
        public VehicleData()
        {
            Events = new List<string>();
            Time = new List<double>();
            Position = new List<XYPoint>();
            Angle = new List<double>();
        }

        public List<string> Events { get; set; }
        public List<double> Time { get; set; }
        public List<XYPoint> Position { get; set; }
        public List<double> Angle { get; set; }
        public static VehicleData FromFile(string path)
        {
            var reader = new StreamReader(File.OpenRead(path));
            VehicleData v = new VehicleData();
            //skip first line (header
            reader.ReadLine();
            while (reader.EndOfStream == false)
            {
                var line = reader.ReadLine();
                var pieces = line.Split(',');
                v.Events.Add(pieces[0]);
                v.Time.Add(Convert.ToDouble(pieces[1]));
                v.Position.Add(new XYPoint(Convert.ToDouble(pieces[2]), Convert.ToDouble(pieces[3])));
                v.Angle.Add(Convert.ToDouble(pieces[4]));
            }
            reader.Close();

            return v;
        }
    }
}
