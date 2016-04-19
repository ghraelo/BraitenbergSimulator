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

using System.Numerics;

using OxyPlot;
using OxyPlot.Series;
using OxyPlot.WindowsForms;
using OxyPlot.Axes;

using BraitenbergProcessing.DataStructures;

using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

using MathNet.Numerics.Statistics;

namespace BraitenbergProcessing
{
    public partial class Form1 : Form
    {
        YStats stats;
        public Form1()
        {
            InitializeComponent();
            ContextMenu cm = new ContextMenu();
            cm.MenuItems.Add("Save plot...", new EventHandler(savePlotsToolStripMenuItem_Click));
            plotView1.ContextMenu = cm;
            treeView1.NodeMouseClick += TreeView1_NodeMouseClick;

        }

        private void TreeView1_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            if(e.Button == MouseButtons.Right)
            {

                plotView1.Model.Title = e.Node.Text;
                plotView1.Invalidate();
                System.Diagnostics.Debug.WriteLine("yo yo yo");
            }
        }

        LineSeries MakeCircle(double xCentre, double yCentre, double radius)
        {
            var ret = new LineSeries();
            ret.CanTrackerInterpolatePoints = true;
            int numPoints = 36;
            double step = 2 * Math.PI / numPoints;
            for(int i =0; i< numPoints;i++)
            {
                double x = xCentre + radius * Math.Cos(i * step);
                double y = yCentre + radius * Math.Sin(i * step);
                ret.Points.Add(new DataPoint(x, y));
            }
            ret.Points.Add(ret.Points[0]);
           return ret;
        }

        static bool IsDiscontinuity(XYPoint first, XYPoint second, double threshold)
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

        static bool IsSignDifference(double a, double b)
        {
            return (Math.Sign(a) > 0 && Math.Sign(b) < 0) || (Math.Sign(b) > 0 && Math.Sign(a) < 0);
        }

        private void loadButton_Click(object sender, EventArgs e)
        {
            treeView1.Nodes.Clear();
            var ofd = new OpenFileDialog();
            ofd.InitialDirectory = @"H: \Users\Jack\Source\Repos\BraitenbergSimulator\BraitenbergSimulator\BraitenbergSimulator";
            ofd.Filter = "yaml files (*.yaml)|*.yaml|All files (.*)|*.*";
            ofd.FilterIndex = 0;
            ofd.Multiselect = true;
            ofd.RestoreDirectory = true;

            if(ofd.ShowDialog() == DialogResult.OK)
            {
          
                var myModel = new PlotModel { Title = "Full graph" };
                myModel.PlotType = PlotType.Cartesian;
                myModel.Axes.Add(new LinearAxis { Position = AxisPosition.Bottom, Minimum = -50, Maximum = 50 });
                myModel.Axes.Add(new LinearAxis { Position = AxisPosition.Left, Minimum = -50, Maximum = 50 });

                var myModel2 = new PlotModel { Title = "Up to first boundary collision" };
                myModel2.PlotType = PlotType.Cartesian;

                var myModel3 = new PlotModel { Title = "Angle" };
                myModel3.PlotType = PlotType.Cartesian;

                List<OxyColor> colors = new List<OxyColor>(myModel.DefaultColors);
                while(ofd.FileNames.Length > colors.Count)
                {
                    colors.AddRange(colors);
                }
                int i = 0;

                List<double> distances = new List<double>();
                List<double> closestApproaches = new List<double>();
                foreach (var filename in ofd.FileNames)
                {
                    TestResult t = new TestResult(filename, @"H: \Users\Jack\Source\Repos\BraitenbergSimulator\BraitenbergSimulator\BraitenbergSimulator\yaml");
                    foreach (var v in t.VehiclesData)
                    {
                        DrawGraph(ref myModel, v.Value, t.Scene, colors[i]);
                    }
                    DrawGraph2(ref myModel2, t.VehiclesData.First().Value, t.Scene, t.InnerTestResult.VehicleData[0].BoundaryCollisions != null ? t.InnerTestResult.VehicleData[0].BoundaryCollisions[0].Time : -1, colors[i]);
                    DrawGraph3(ref myModel3, t.VehiclesData.First().Value, t.Scene, t.InnerTestResult.VehicleData[0].BoundaryCollisions != null ? t.InnerTestResult.VehicleData[0].BoundaryCollisions[0].Time : -1, colors[i]);
                    //System.Diagnostics.Debug.WriteLine(colors[i].ToString());
                    TreeNode tn = new TreeNode(t.Scene.Name);
                    tn.ForeColor = colors[i].ToColor();
                    tn.Nodes.Add("Distance travelled: " + t.InnerTestResult.VehicleData[0].Distance);
                    distances.Add(t.InnerTestResult.VehicleData[0].Distance);
                    treeView1.Nodes.Add(tn);
                    i++;
                }

                stats = new YStats {TestCount = ofd.FileNames.Length, Test = treeView1.Nodes[0].Text};


                TreeNode distStats = new TreeNode("distance stats");
                DescriptiveStatistics ds = new DescriptiveStatistics(distances);
                distStats.Nodes.Add("Mean: " + ds.Mean);
                distStats.Nodes.Add("Standard deviation: " + ds.StandardDeviation);

                stats.Distance.Max = ds.Maximum;
                stats.Distance.Min = ds.Maximum;
                stats.Distance.StandardDeviation = ds.StandardDeviation;
                stats.Distance.Mean = ds.Mean;

                treeView1.Nodes.Add(distStats);

                TreeNode approachNode = new TreeNode("closest approach stats");
                DescriptiveStatistics cs = new DescriptiveStatistics(closestApproaches);
                approachNode.Nodes.Add("Mean: " + cs.Mean);
                approachNode.Nodes.Add("Standard deviation: " + cs.StandardDeviation);
                treeView1.Nodes.Add(approachNode);

                stats.ClosestApproach.Max = cs.Maximum;
                stats.ClosestApproach.Min = cs.Maximum;
                stats.ClosestApproach.StandardDeviation = cs.StandardDeviation;
                stats.ClosestApproach.Mean = cs.Mean;

                this.plotView1.Model = myModel;
                this.plotView2.Model = myModel2;
                this.plotView3.Model = myModel3;
            }
        }

        Task<List<List<double>>> SlidePeriodsAsync(List<double> data, int windowLength)
        {
            return Task.Run(() =>
            {
                var od = new OscillationDetector(data);
                var res = od.SlidePeriods(windowLength);
                return res;
            });
        }

        private void DrawGraph(ref PlotModel myModel, VehicleData v, Scene scene, OxyColor c)
        {
            //detect discontinuities in position and split lists accordingly
            var prevPos = v.Position[0];
            var sections = new List<List<XYPoint>>();

            //set up first list and point
            sections.Add(new List<XYPoint>());
            int currList = 0;
            sections[0].Add(prevPos);

            for (int i = 1; i < v.Position.Count; i++)
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

            foreach (var section in sections)
            {
                var positionData = new LineSeries();
                positionData.Color = c;
                positionData.Points.AddRange(section.Select(xy => (DataPoint)xy));

                myModel.Series.Add(positionData);
            }
            //var lights = new ScatterSeries();
            //lights.MarkerType = MarkerType.Circle;
            foreach (var light in scene.Lights)
            {
                LineSeries circle;
                if (light.Position != null)
                {
                    circle = MakeCircle(light.Position[0], light.Position[1], light.Radius);
                }
                else
                {
                    circle = new LineSeries();
                    foreach(List<double> d in light.Path)
                    {
                        circle.Points.Add(new DataPoint(d[0], d[1]));
                    }
                    circle.Points.Add(new DataPoint(light.Path[0][0], light.Path[0][1]));
                }
                circle.Color = Color.Black.ToOxyColor();
                myModel.Series.Add(circle);

                //lights.Points.Add(new ScatterPoint(light.Position[0], light.Position[1],light.Radius));
            }
            //myModel.Series.Add(lights);
           
        }

        private void DrawGraph2(ref PlotModel myModel, VehicleData v, Scene scene, double time, OxyColor c)
        {
            //detect discontinuities in position and split lists accordingly
            var prevPos = v.Position[0];
            var sections = new List<List<XYPoint>>();

            //set up first list and point
            sections.Add(new List<XYPoint>());
            int currList = 0;
            sections[0].Add(prevPos);

            for (int i = 1; i < v.Position.Count; i++)
            {
                var currPos = v.Position[i];
                if (IsDiscontinuity(prevPos, currPos, 2.0f))
                {
                    currList++;
                    sections.Add(new List<XYPoint>());
                }
                sections[currList].Add(currPos);
                prevPos = currPos;

                if(time > 0 && v.Time[i] > time)
                {
                    break;
                }
            }

            foreach (var section in sections)
            {
                var positionData = new LineSeries();
                positionData.Color = c;
                positionData.Points.AddRange(section.Select(xy => (DataPoint)xy));

                myModel.Series.Add(positionData);
            }
            //var lights = new ScatterSeries();
            //lights.MarkerType = MarkerType.Circle;
            foreach (var light in scene.Lights)
            {
                LineSeries circle;
                if (light.Position != null)
                {
                    circle = MakeCircle(light.Position[0], light.Position[1], light.Radius);
                }
                else
                {
                    circle = MakeCircle(light.Path[0][0], light.Path[0][1], light.Radius);
                }
                circle.Color = Color.Black.ToOxyColor();
                myModel.Series.Add(circle);

                //lights.Points.Add(new ScatterPoint(light.Position[0], light.Position[1],light.Radius));
            }
            //myModel.Series.Add(lights);

        }

        private void DrawGraph3(ref PlotModel myModel, VehicleData v, Scene scene, double time, OxyColor c)
        {
            //detect discontinuities in position and split lists accordingly
            var prevPos = v.Position[0];
            var sections = new List<List<XYPoint>>();

            //set up first list and point
            sections.Add(new List<XYPoint>());
            int currList = 0;
            sections[0].Add(prevPos);

            var positionData = new LineSeries();
            positionData.Color = c;

            for (int i = 1; i < v.Position.Count; i++)
            {
                positionData.Points.Add(new DataPoint(v.Time[i], v.Angle[i]));

                if (time > 0 && v.Time[i] > time)
                {
                    break;
                }
            }

            myModel.Series.Add(positionData);
        }

        private void generateButton_Click(object sender, EventArgs e)
        {
            Generator g = new Generator();
            g.Show();
        }

        private void savePlotsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var sfd = new SaveFileDialog();
            sfd.InitialDirectory = @"H: \Users\Jack\Source\Repos\BraitenbergSimulator\BraitenbergSimulator\BraitenbergSimulator";
            sfd.Filter = "PNG files (*.png)|*.png|All files (.*)|*.*";
            sfd.FilterIndex = 0;
            sfd.RestoreDirectory = true;

            if (sfd.ShowDialog() == DialogResult.OK)
            {
                var pngExporter = new PngExporter { Width = 500, Height = 500, Background = OxyColors.White };
                pngExporter.ExportToFile(plotView1.Model, sfd.FileName);
            }
        }

        private void saveButton_Click(object sender, EventArgs e)
        {
            var sfd = new SaveFileDialog();
            sfd.InitialDirectory = @"H: \Users\Jack\Source\Repos\BraitenbergSimulator\BraitenbergSimulator\BraitenbergSimulator";
            sfd.Filter = "PNG files (*.png)|*.png|All files (.*)|*.*";
            sfd.FilterIndex = 0;
            sfd.RestoreDirectory = true;

            if (sfd.ShowDialog() == DialogResult.OK)
            {
                var pngExporter = new PngExporter { Width = 500, Height = 500, Background = OxyColors.White };
                pngExporter.ExportToFile(plotView1.Model, sfd.FileName);

                string subName = Path.Combine(Path.GetDirectoryName(sfd.FileName), Path.GetFileNameWithoutExtension(sfd.FileName) + "pip" + Path.GetExtension(sfd.FileName));
                pngExporter.ExportToFile(plotView2.Model, subName);

                using (var writer = File.CreateText(Path.ChangeExtension(sfd.FileName, "yaml")))
                {
                    Serializer s = new Serializer(namingConvention: new CamelCaseNamingConvention());
                    s.Serialize(writer, stats);
                }
            }
        }
    }

    public static class ColorExtension
    {
        public static OxyColor ToOxyColor(this Color c)
        {
            return OxyColor.FromArgb(c.A, c.R, c.G, c.B);
        }

        public static Color ToColor(this OxyColor o)
        {
            return Color.FromArgb(o.A, o.R, o.G, o.B);
        }
    }
}
