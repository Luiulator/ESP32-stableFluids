# [Stable Fluids](https://github.com/Luiulator/ESP32-stableFluids)

A C++ port of [my Julia module](https://github.com/Luiulator/colourlessFD) that implements the Stable Fluids algorithm, which is commonly used to simulate smoke and other slightly dense gaseous flows in videogames in real time. It is quite lightweight and unconditionally stable. As such, modern microcontrollers like the ESP32 should be able to run it at sufficient framerates for it to look "fluid" enough too. This is my attempt at that.



## A few Highlights

- This software solves the incompressible Navier-Stokes equations. That's the Continuity Equation

$$ \nabla \cdot \mathbf{u} = 0 $$

and the Momentum Equation

$$ \frac{\partial \mathbf{u}}{\partial t} + (\mathbf{u} \cdot \nabla) \mathbf{u} = \nu \nabla^2 \mathbf{u} + \frac{1}{\rho} \nabla p + \frac{1}{\rho}\mathbf{f} $$

It does so by breaking down each term of the sum and solving them separately, then summing up. Lastly, it applies a correction pressure so that we enforce that the fluid remains incompressible.   

- Stable Fluids is, as per its name, unconditionally stable. That means you won't get infinite velocities, vorticities or whatever magnitudes, under any circumstance. It is then theoretically possible to use a dt as large as you wish, although at some point the physics will completely break. Feel free to tweak it and find out!


*Be advised!!!* As a tradeoff for the unconditional stability, this method sacrifices precise recreation of fluid dynamics. You should expect fluid-like motion, but do not use it for any sensitive purpose as it will not behave physically.

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



