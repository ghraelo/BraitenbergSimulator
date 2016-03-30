module PeriodicityDetection

open MathNet.Numerics.IntegralTransforms

let RealToComplex x = 
    MathNet.Numerics.Complex.mkRect(x,0.0)

let ComplexToTuple x = 
    (MathNet.Numerics.Complex.realPart x, MathNet.Numerics.Complex.imagPart x)

let fft data =
    let mutable temp = data |> List.map RealToComplex |> List.toArray
    temp |> Fourier.Forward
    temp |> Array.toList |> List.map ComplexToTuple