using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using MathNet.Numerics.Statistics;

namespace BraitenbergProcessing
{
    
    public class OscillationDetector
    {
        List<double> mData = new List<double>();
        DiscreteCosineTransform mDct;

        public OscillationDetector(List<double> data)
        {
            mData = data;
            mDct = new DiscreteCosineTransform(@"H:\Users\Jack\Source\Repos\BraitenbergSimulator\MATLAB");
        }

        /// <summary>
        /// Divides the dataset into chunks of length windowLength and looks for oscillations in each.
        /// </summary>
        /// <param name="windowLength">Length of chunk</param>
        /// <returns></returns>
        public List<List<double>> GetPeriods(int windowLength)
        {
            var ret = new List<List<double>>();

            int max = (int)Math.Floor((double)mData.Count / windowLength);

            int prev = 0;
            for(int i = 1; i< max; i++)
            {
                var slice = mData.Skip(prev*windowLength).Take((i - prev)*windowLength).ToList();
                slice = DetectOscillation(slice);
                ret.Add(slice);
                prev = i;
            }
            return ret;
        }

        /// <summary>
        /// Slides a window along the dataset, checking for oscillation.
        /// </summary>
        /// <param name="windowLength"></param>
        /// <returns></returns>
        public List<List<double>> SlidePeriods(int windowLength)
        {
            var ret = new List<List<double>>();

            for(int i =0;i < mData.Count - windowLength;i++)
            {
                var slice = mData.Skip(i).Take(windowLength).ToList();
                slice = DetectOscillation(slice);
                ret.Add(slice);
                //System.Diagnostics.Debug.WriteLine(i);
            }
            return ret;
        }


        public List<double> DetectOscillation(List<double> data)
        {
            var dct = mDct.Forward(data);

            var stats = new DescriptiveStatistics(dct);

            double sigma = stats.StandardDeviation;
            double seaLevel = 3 * sigma;

            //set all values below sea level to zero
            dct = dct.Select(x => (Math.Abs(x) > seaLevel) ? x : 0.0).ToList();



            var ret = new List<double>();

            foreach (var component in ExtractFrequencyComponents(dct))
            {
                //get zero crossings
                var zeroCrossings = Enumerable.Range(0, component.Count()-1)
                                    .Where(i => Math.Sign(component[i]) > 0 && Math.Sign(component[i + 1]) < 0)
                                    .ToList();

                var increments = new List<double>();

                for (int i = 1; i < zeroCrossings.Count(); i++)
                {
                    increments.Add(2 * (zeroCrossings[i] - zeroCrossings[i - 1]));
                }

                var iStats = new DescriptiveStatistics(increments);

                if ((iStats.StandardDeviation / iStats.Mean) < (1.0 / 3))
                {
                    ret.Add(iStats.Mean);
                }
            }

            return ret;

        }

        public List<List<double>> ExtractFrequencyComponents(List<double> data)
        {
            var components = new List<List<double>>();

            bool open = false;
            int startIndex = 0;
            for (int i = 1; i < data.Count()-4; i++)
            {

                if (!open)
                {
                    //start point
                    if (data[i] != 0 && data[i - 1] == 0)
                    {
                        startIndex = i;
                        open = true;
                    }
                }
                else
                {
                    //end point
                    if (data[i] != 0 && data[i + 1] == 0 && data[i + 2] == 0 && data[i + 3] == 0 && data[i + 4] == 0)
                    {
                        //zero pad range to be same length as data (by setting all elements outside range to zero)
                        var cpy = new List<double>(data);

                        cpy = cpy.Select(x => (cpy.IndexOf(x) >= startIndex && cpy.IndexOf(x) <= i) ? x : 0).ToList();
                        components.Add(mDct.Reverse(cpy));
                        open = false;
                    }

                }
            }

            return components;

        }

    }

    public static class LinqExtensions
    {
    }


}
