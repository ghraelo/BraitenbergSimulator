using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using OxyPlot;
using OxyPlot.Series;

using BraitenbergProcessing.DataStructures;

using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace BraitenbergProcessing
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            string path = @"H:\Users\Jack\Source\Repos\BraitenbergSimulator\BraitenbergSimulator\BraitenbergSimulator\logs\testVehicle201641-1414.csv";

            string name = @"H:\Users\Jack\Source\Repos\BraitenbergSimulator\BraitenbergSimulator\BraitenbergSimulator\yaml\testScene.yaml";
            //var order = YamlTest(name);
            var scene = MakeScene(name);

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
            }

             //detect discontinuities in position and split lists accordingly
            var prevPos = v.Position[0];
            var sections = new List<List<XYPoint>>();

            //set up first list and point
            sections.Add(new List<XYPoint>());
            int currList = 0;
            sections[0].Add(prevPos);

            for(int i =1;i<v.Position.Count;i++)
            {
                var currPos = v.Position[i];
                if (IsDiscontinuity(prevPos, currPos, 2.0f))
                {
                    currList++;
                    sections.Add(new List<XYPoint>());
                }
                sections[currList].Add(currPos);
                prevPos = currPos;
            }

            var myModel = new PlotModel { Title = "Example 1" };

            foreach (var section in sections)
            {
                var positionData = new LineSeries();
                positionData.Color = Color.Red.ToOxyColor();
                positionData.Points.AddRange(section.Select(xy => (DataPoint)xy));
                myModel.Series.Add(positionData);
            }
            //var lights = new ScatterSeries();
            //lights.MarkerType = MarkerType.Circle;
            foreach (var light in scene.Lights)
            {
                var circle = MakeCircle(light.Position[0], light.Position[1], light.Radius);
                circle.Color = Color.Black.ToOxyColor();
                myModel.Series.Add(circle);

                //lights.Points.Add(new ScatterPoint(light.Position[0], light.Position[1],light.Radius));
            }
            //myModel.Series.Add(lights);
            this.plot1.Model = myModel;
        }

        public Order YamlTest(string fileName)
        {
            var sr = new StreamReader(File.OpenRead(fileName));
            var deserializer = new Deserializer(namingConvention: new CamelCaseNamingConvention());

            return deserializer.Deserialize<Order>(sr);
        }

        public LineSeries MakeCircle(double xCentre, double yCentre, double radius)
        {
            var ret = new LineSeries();
            ret.CanTrackerInterpolatePoints = true;

            var points = new List<DataPoint>();
            for(int x= -(int)radius;x <= radius;x++)
            {
                int y = (int)(Math.Sqrt(radius * radius - x * x) + 0.5);
                points.Add(new DataPoint(xCentre + x,yCentre + y));
                points.Add(new DataPoint(xCentre+ x, yCentre - y));
            }
            points.Add(points[0]);
           ret.Points.AddRange(points.OrderBy(x => Math.Atan2(x.X - xCentre, x.Y - yCentre)).ToList());
           return ret;
        }

        public Scene MakeScene(string fileName)
        {
            var sr = new StreamReader(File.OpenRead(fileName));
            var deserializer = new Deserializer(namingConvention: new CamelCaseNamingConvention());

            return deserializer.Deserialize<Scene>(sr);
        }

        public static bool IsDiscontinuity(XYPoint first, XYPoint second, double threshold)
        {
            if((Math.Abs(first.X) > threshold) && (Math.Abs(second.X) > threshold))
            {
                if (IsSignDifference(first.X, second.X))
                    return true;
            }

            if ((Math.Abs(first.Y) > threshold) && (Math.Abs(second.Y) > threshold))
            {
                if (IsSignDifference(first.Y, second.Y))
                    return true;
            }

            return false;
        }

        public static bool IsSignDifference(double a, double b)
        {
            return (Math.Sign(a) > 0 && Math.Sign(b) < 0) || (Math.Sign(b) > 0 && Math.Sign(a) < 0);
        }
    }

    public static class ColorExtension
    {
        public static OxyColor ToOxyColor(this Color c)
        {
            return OxyColor.FromArgb(c.A, c.R, c.G, c.B);
        }
    }
}
