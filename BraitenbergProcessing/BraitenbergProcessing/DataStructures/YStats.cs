using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;


namespace BraitenbergProcessing.DataStructures
{
    class YStats
    {
        public YStats()
        {
            ClosestApproach = new YStatsGroup();
            Distance = new YStatsGroup();
        }
        public string Test { get; set; }
        public YStatsGroup Distance { get; set; }
        [YamlAlias("closest-approach")]
        public YStatsGroup ClosestApproach { get; set; }
        [YamlAlias("test-count")]
        public int TestCount { get; set; }
    }

    class YStatsGroup
    {
        public double Mean { get; set; }
        [YamlAlias("standard-deviation")]
        public double StandardDeviation { get; set; }
        public double Min { get; set; }
        public double Max { get; set; }
    }
}
