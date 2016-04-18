using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;

using BraitenbergProcessing.DataStructures;

namespace BraitenbergProcessing
{
    public class SceneGenerator
    {
        Scene mTemplate;

        public SceneGenerator(Scene template)
        {
            mTemplate = template;
        }

        public void Get(string property, double min, double max, double step)
        {
            int count = (int)((max - min) / step);

            string[] strings = property.Split('.').Skip(1).ToArray();

            string root = property.Split('.').First();
            PropertyInfo info = mTemplate.GetType().GetProperty(root);

            foreach (string s in strings)
            {
               info = mTemplate.GetType().GetProperty(s);
            }
            System.Diagnostics.Debug.WriteLine(info.Name);

            //var generatedScenes = new List<Scene>();

            //for (int i = 0; i < count; i++)
            //{
            //    Scene s = mTemplate.Clone();
            //    info.SetValue(s, min + i * step);
            //    generatedScenes.Add(s);
            //}
        }
    }
}
