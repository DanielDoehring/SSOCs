[![License: MIT](https://img.shields.io/badge/License-MIT-success.svg)](https://opensource.org/licenses/MIT)
[![DOI](https://zenodo.org/badge/758503799.svg)](https://zenodo.org/doi/10.5281/zenodo.11184359)

# SSOCs

```
                                                        ..........
                                                ....:::::^^^^^^^^^^^.
                                            .::^^^^^^^^^^^^^^^^^^^:::.
                                          .~~~~~~~^^^^^^^^^:::::::::.
                                           :^^^^^^^::::::::::::::::::
                                           .:^^^^::::::::::::::::::::
                                            :^^^^^::::::::::::::::::.
                            ::::^^^^^^^^^^^^^^^^^^::::::::::::::::::.
                            ::::^^^^^^^^^^^^:^^^^^:::::::::::::::::::
                            ::::^^^^^^^^^^^^^:~#####################:
                            .:::^^^^^^^^^^^^^^^######################.
                            .::^^^^^^^^^^^^^^^:^#####################.
                             ::::^^^^^^^^^^^::::####################:.
                             :####################~~^^:::::::::::::::.
                             :####################:^^::::::::::::::::.
                             :###################..^^^^::::::::::::::.
                             :###################. :^^^::::::::::::::.
                              !^^^^^^^^^^^^^^^      ^^^:::::SSOCs::::.
                              :::^^^^^^^^^^^^^^^.   .^^^::::::::::::::
                              .::^^^^^^^~^^^^^^^.    :^^::::::::::::::
                              .::^^^^^^~~^^^^^^^     .^^::::::::::::::
                              .::^^^^^SSOCs~^^^:      :^^:::::::::::::
                               :::^^^^~~~~~~~^^.      :^^:::::::::::::.
                               :::^^^~~~~~~~~~^       .^:::::::::::::::.
                               :::^^^~~~~~~~~~:       .:::::::::::::::::.
                               :::^^^~~~~~~~~~.        ::::::::::::::::::.
                               :::^^^~~~~~~~~^         ::::::::::::::::::::
                              .::^^^^~~~~~~~~^        .:::::::::::::::::::::
                              :^^^^^~~~~~~~!~^        :::::::::::::::::::::::
                            .:^^^^^~~~~~~~~!~^       .:::::..::::::::::::::::.
                          .:^^^^^~~~~~~~~~!!~~.      ::::::..::::::::::::::::
                       .::^^^^^^^~~~~~~~~!!!~~~     .:::::...::::::::::::::.
                     .::^:^^^^^^~~~~!!!!!!!!!~~^   .::::::...:::::::::::::.
                  .::::::::^^^^~~~~!!!!!!!!~~~~~.  ::::::....:::::::::::.
              ..::^^:::::::^^^~~~~~!!!!!!!~~~~~~^.::::::....::::::::::.
     ...::::::::::^!~:::::^^^~~~~~~!!!!!!~~~~~~^^:::::::::..:::::::::.
   .::::::::::::::::::::^^^^^~~~~~!!!!!!~~~~^^^:::::::::::::::::::::.
  .::::::.::::::::::::^^^^^~~~!!!!!!!~^^:::::::::::::::::::::::::::.
   .::::::::::::::::::^^^^^^^^^::::::::::..................:::::::.
        .....  .                    ::::::::................:::::.
```

Code for generating optimal **S**tability Polynomials by **S**econd **O**rder **C**one**s** in arbitrary precision.

## Dependencies
### Eigen3
In order to use [`EiCOS`](https://github.com/EmbersArc/EiCOS) you need to have [`Eigen3`](https://eigen.tuxfamily.org/index.php?title=Main_Page) installed. If you are on Ubuntu or an other Debian based Linux distribution it is recommended to install `Eigen3` via 
```bash
sudo apt install libeigen3-dev
```

### Boost Multiprecision Types
Within this project the multiprecision floating point types are based on the [Boost Multiprecision](https://github.com/boostorg/multiprecision) types. See the [docs](https://www.boost.org/doc/libs/1_77_0/libs/multiprecision/doc/html/index.html) for a detailed introduction.

On Debian-based systems it is again easiest to install Boost over the package manager via
```bash
sudo apt-get install libboost-all-dev
```
although this brings in the whole Boost library.

### CMake
To build `EiCOS` you need `CMake` to be installed on your system, which is the case on most Linux distributions.

### EiCOS

To be able to use `SSOCs` you need to compile the shared library `libeicos_MP.so` of [my fork](https://github.com/DanielDoehring/EiCOS) from the [`EiCOS` repository](https://github.com/EmbersArc/EiCOS).
After installing `Eigen3` and the Boost multiprecision library you need to `cd` to the directory where you downloaded or forked `EiCOS`.
It is assumed that this is `~/git/EiCOS/` in the compile scripts, where `~` is synonymous with `$HOME`.

Then, execute 
```bash
cmake -DCMAKE_BUILD_TYPE=Release .
```
to generate the make files with release flags.
Then type 
```bash
make
```
to compile the shared library `libeicos_MP.so`.
A successful output should look like this:
```
[ 50%] Building CXX object CMakeFiles/eicos_MP.dir/src/eicos_MP.cpp.o
[100%] Linking CXX shared library libeicos_MP.so
[100%] Built target eicos_MP
```

## Compiling `SSOCs`

If you installed `EiCOS` not in `~/git/EiCOS` you need to exchange that path wherever it occurrs in the `Makefile`.
Then, you can simply execute 
```bash
make MP_TARGETS -j 2
```
which gives you both `SSOCs_MP.exe` and `SSOCs_PERK4.exe`.
It might be required that you make these binaries executable, i.e.,
```bash
chmod +x *.exe
```

## Usage

### Orders 2 and 3
```bash
./SSOCs_MP.exe <Degree> <Order> <dtMax> <path/to/spectrum/file> <OPTIONAL:dtMin dtEps>
```
where 
* `<Degree>` is the number of stage evaluations (or stability polynomial degree). This determines the number of optimization variables.
* `<Order>` is the order of consistency of the method (`2` or `3`). The number of polynomial coefficients is determined by `Degree - Order`.
* `<dtMax>` is the maximum timestep that may be possible. In principle this can be chosen arbitrarily large, which slows down the optimization. Supplying a too small value gives wrong results if the true admissible timestep is larger.
* `<path/to/spectrum/file>` is the path to the file with the eigenvalues used for constraining the stability polynomial. The file should contain one eigenvalue per row with syntax `Re(lambda)+Im(lambda)i` (e.g., `-5.2-0.3i` or `0+2i`). The number of eigenvalues determines the number of constraints.
* `<dtMin>` (Optional): Minimum timestep for bisection routine. Can enhance efficiency if guessed appropriately; too large values may cause optimization to fail. Defaults to `0.0`.
* `<dtEps>` (Optional): Tolerance when bisection of the timestep stops. Defaults to `1e-6`.

### P-ERK 4
To generate the fourth order accurate stability polynomial for the fourth order Paired-Explicit Runge-Kutta schemes type
```bash
./SSOCs_PERK4_MP.exe <Degree> <dtMax> <path/to/spectrum/file> <OPTIONAL:dtMin dtEps dtStages>
```
where 
* `<Degree>` is the number of stage evaluations (stability polynomial degree). Must be an integer > 5 for the PERK4 version. The order of consistency is fixed at 4 for this specialized scheme.
* `<dtMax>` is the maximum timestep estimate for the optimization. See general orders section for details.
* `<path/to/spectrum/file>` is the path to the file with the eigenvalues. See general orders section for file format details.
* `<dtMin>` (Optional): Minimum timestep for bisection routine. Defaults to `0.0`.
* `<dtEps>` (Optional): Tolerance when bisection of the timestep stops. Defaults to `1e-10` (tighter tolerance than general orders version).
* `<Stages>` (Optional): Total number of stages in the final paired-explicit Runge-Kutta method. If supplied, the optimized polynomial with `<Degree>` stage-evaluations is embedded into a `<Stages>` stage overall PERK4 scheme. Must be >= `<Degree>`. If omitted, defaults to `<Degree>`.

### List version
```bash
./SSOCs_List_MP.exe <NumStageEvalFile> <Order> <dtMax> <path/to/spectrum/file> <OPTIONAL:dtMin dtEps>
```
where
* `<NumStageEvalFile>` is the path to a file containing a list of polynomial degrees to optimize. Each integer should be on a separate line. The program will optimize stability polynomials for all degrees specified in this file.
* `<Order>` is the order of consistency of the methods (`2` or `3`).
* `<dtMax>` is the maximum timestep for the optimization. See general orders section for details.
* `<path/to/spectrum/file>` is the path to the file with the eigenvalues. See general orders section for file format details.
* `<dtMin>` (Optional): Minimum timestep for bisection routine. Defaults to `0.0`.
* `<dtEps>` (Optional): Tolerance when bisection of the timestep stops. Defaults to `1e-6`.

## Input File Formats

### Eigenvalue Spectrum File
The eigenvalue file contains one complex eigenvalue per line in the format `Re(λ)+Im(λ)i`. Examples:
```
-5.2+0i
-3.1-2.5i
-3.1+2.5i
0+1.5i
0-1.5i
-1.0+0i
```
The eigenvalues are used to construct stability constraints for the optimization problem. The number of eigenvalues determines the number of inequality constraints in the second-order cone program.

### NumStageEvalFile (List Version)
A simple text file with one positive integer per line, specifying the polynomial degrees to optimize:
```
5
6
7
8
```

## Credit

If you use the implementations provided here, please also cite this repository as
```bibtex
@misc{doehring2024ssocs,
  title={{SSOCs}: Arbitrary Precision Stability Polynomials by Second-Order Cones},
  author={Doehring, Daniel},
  year={2024},
  howpublished={\url{https://github.com/DanielDoehring/SSOCs}},
  doi={https://doi.org/10.5281/zenodo.11184359}
}
```

The original paper where this approach for optimizing stability polynomials has been developed is

```bibtex
@article{ketcheson2013optimal,
  title={Optimal stability polynomials for numerical integration of initial value problems},
  author={Ketcheson, David and Ahmadia, Aron},
  journal={Communications in Applied Mathematics and Computational Science},
  volume={7},
  number={2},
  pages={247--271},
  year={2013},
  publisher={Mathematical Sciences Publishers},
  doi={10.2140/camcos.2012.7.247}
}
```

## Affiliation
![rwth_acom_en_rgb_cropped](https://github.com/DanielDoehring/SSOCs/assets/75639095/50d979dc-71ac-45a6-b580-ae23c9f87835)

This code was developed at the Institute of [Applied and Computational Mathematics (ACoM) at RWTH Aachen University](https://www.acom.rwth-aachen.de/).

## Acknowledgements
This code is a result of research performed in the research unit "Structure-Preserving Numerical Methods for Bulk- and
Interface Coupling of Heterogeneous Models (SNuBIC)"

![snubic-logo6](https://github.com/DanielDoehring/OSPREI/assets/75639095/a88000ca-d43c-4ac2-94d8-a0636fc7f32c)

This project has benefited from funding by the Deutsche Forschungsgemeinschaft (DFG, German Research Foundation)
through the research unit FOR 5409 "Structure-Preserving Numerical Methods for Bulk- and
Interface Coupling of Heterogeneous Models (SNuBIC)" (project number 463312734).

![DFG](https://github.com/DanielDoehring/OSPREI/assets/75639095/a0ebb057-a0c5-4dc9-96df-03ac74894d2a)
