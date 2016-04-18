using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

using System.Numerics;
using MathNet.Numerics.IntegralTransforms;

namespace BraitenbergProcessing
{
    public class DiscreteCosineTransform
    {
        MLApp.MLApp mInstance;

        public DiscreteCosineTransform(string matlabDirectory)
        {
            mInstance = new MLApp.MLApp(); ;
            mInstance.Execute("cd " + matlabDirectory);
        }

        public List<double> Forward(List<double> input)
        {
            System.Array inArr = input.ToArray();
            object result = null;
            mInstance.Feval("dct", 1, out result, inArr);

            object[] res = result as object[];

            double[,] d = res[0] as double[,];

            var x = d.Cast<double>().ToList();

            return x;
        }

        public List<double> Reverse(List<double> input)
        {
            System.Array inArr = input.ToArray();
            object result = null;
            mInstance.Feval("idct", 1, out result, inArr);

            object[] res = result as object[];

            double[,] d = res[0] as double[,];

            var x = d.Cast<double>().ToList();

            return x;
        }

        static void PrintList<T>(string name, List<T> ls)
        {
            System.Diagnostics.Debug.Write(name + ": [");
            foreach (T d in ls)
            {
                System.Diagnostics.Debug.Write(d + " ");
            }
            System.Diagnostics.Debug.Write("]\n");
        }
    }
}
