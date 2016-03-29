// Learn more about F# at http://fsharp.org
// See the 'F# Tutorial' project for more help.
module Main

open PeriodicityDetection
open FSharp.Data
open FSharp.Charting
open System
open System.Windows.Forms

type Log = CsvProvider<"data/blank-log.csv">

///<summary>Finds distance of a point from the origin</summary>
let distanceFromOrigin (x, y) =
    ///<summary>Squares a number</summary>
    let sq x = x * x

    sqrt(sq(x) + sq(y))

[<EntryPoint>]
let main (args : string[]) = 
    
    if args.Length <> 1 then
        failwith "Error: expected arguments <fileName>"

    let fileName = args.[0]

    let currentLog = Log.Load(fileName)

    let firstRow = currentLog.Rows |> Seq.head

    let xyData = [ yield! currentLog.Rows |> Seq.map (fun x -> x.X.AsFloat(), x.Y.AsFloat())]

    let distances = List.map distanceFromOrigin xyData

    List.iter (fun x -> printfn "%f" x) distances
    List.reduce (+) distances |> (fun x -> x / (float distances.Length)) |> printfn "mean: %f"
    let fftData = fft distances |> List.map (fun (x, y) -> sqrt(x*x + y*y))
    let norm = fftData |> List.map (fun x -> x / (float fftData.Length))

    let chart = Chart.Line(fftData)
    chart.ShowChart()
    chart.SaveChartAs("chart.png",ChartTypes.ChartImageFormat.Png)

    //exit code
    0