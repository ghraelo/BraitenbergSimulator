using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.IO;

using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

using BraitenbergProcessing.DataStructures;

namespace BraitenbergProcessing
{
    public partial class Generator : Form
    {
        Scene theScene;
        public Generator()
        {
            InitializeComponent();
            strategyComboBox.Items.Add(1.0);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void loadButton_Click(object sender, EventArgs e)
        {
            var ofd = new OpenFileDialog();
            ofd.InitialDirectory = @"H: \Users\Jack\Source\Repos\BraitenbergSimulator\BraitenbergSimulator\BraitenbergSimulator\yaml";
            ofd.Filter = "yaml files (*.yaml)|*.yaml|All files (.*)|*.*";
            ofd.FilterIndex = 0;
            ofd.RestoreDirectory = true;

            if (ofd.ShowDialog() == DialogResult.OK)
            {
                theScene = Scene.FromFile(ofd.FileName);
            }
        }

        private void generateButton_Click(object sender, EventArgs e)
        {
            if(theScene != null)
            {
                //generate strategies
                List<VStrategy> stratList = new List<VStrategy>();


                //gf
                List<double> gfList = new List<double>();

                double gfLower = (double)gfMinUD.Value;
                double gfUpper = (double)gfMaxUD.Value;
                double gfIncrement = (double)gfIncrementUD.Value;

                double accumulator = gfLower;
                while(accumulator < gfUpper)
                {
                    gfList.Add(accumulator);
                    accumulator += gfIncrement;
                }

                //gi
                List<double> giList = new List<double>();

                double giLower = (double)giMinUD.Value;
                double giUpper = (double)giMaxUD.Value;
                double giIncrement = (double)giIncrementUD.Value;

                accumulator = giLower;
                while (accumulator < giUpper)
                {
                    giList.Add(accumulator);
                    accumulator += giIncrement;
                }


                //list of strategies
                foreach(double gi in giList)
                {
                    foreach(double gf in gfList)
                    {
                        VStrategy strat = new VStrategy();
                        strat.Type = strategyComboBox.Text;
                        strat.Gi = gi;
                        strat.Gf = gf;
                        stratList.Add(strat);
                    }
                }
                int i = 0;
                foreach(var strat in stratList)
                {
                    Scene newScene = theScene.Clone();

                    foreach (Vehicle v in newScene.Vehicles)
                    {
                        v.Strategy = strat;
                    }
                    Directory.CreateDirectory("test");
                    using (var writer = File.CreateText(@"test/" + DateTime.Now.ToString("yymmddhhmm-") + i.ToString() + ".yaml"))
                    {
                        Serializer s = new Serializer(namingConvention: new CamelCaseNamingConvention());
                        s.Serialize(writer, newScene);
                        
                    }

                    i++;
                }
            }
        }
    }
}
