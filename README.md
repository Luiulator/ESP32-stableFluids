# [Stable Fluids](https://github.com/Luiulator/ESP32-stableFluids)

A C++ port of [my Julia module](https://github.com/Luiulator/colourlessFD) that implements the Stable Fluids algorithm, which is commonly used to simulate smoke and other slightly dense gaseous flows in videogames in real time. It is quite lightweight and inconditionally stable. As such, modern microcontrollers like the ESP32 should be able to run it at sufficient framerates for it to look "fluid" enough too. This is my attempt at that.



## A few Highlights





## Some Benchmarks


<div align="center">
  <table>
    <tr>
      <td align="center" width="50%">
        <img src="media/performance.png" alt="Gauss-Seidel Solver Performance" width="100%">
        <br>
        <sub><b>Gauss-Seidel Solver Performance (FPS vs Grid Size)</b></sub>
      </td>
    </tr>
  </table>
</div>



