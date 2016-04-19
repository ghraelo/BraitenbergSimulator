using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using OxyPlot;
using OxyPlot.Series;

namespace BraitenbergProcessing
{
    class ResultModel
    {
        ResultModel()
        {

        }
        
        public PlotModel PositionGraphModel { get; set; }
                                        
        //graph of vehicle position

        public static ResultModel FromFile(string filePath)
        {
            return new ResultModel();
        }
    }
}
