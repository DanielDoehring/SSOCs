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

To generate the fourth order accurate stability polynomial for the fourth order Paired-Explicit Runge-Kutta schemes type
```bash
./SSOCs_PERK4_MP.exe <Degree> <dtMax> <path/to/spectrum/file> <OPTIONAL:Stages>
```
where 
* `<Degree>` needs to be exchanged for the stability polynomial degree.
For the PERK4 schemes (`SSOCs_PERK4.cpp`), this needs to be an integer > 5, while for the general version (`SSOCs.cpp`) also a polynomial with degreee 5 may be optimized.
* `<dtMax>` is the maximum timestep that may be possible. In principle this can be chosen arbitrarily large, which slows down the optimization. Supplying a too small value gives wrong results if the true admissible timestep is larger.
* `<path/to/spectrum/file>` is the path to the file with the eigenvalues used for constraining the stability polynomial. The file should contain the eigenvalues should that one eigenvalue is there per row with syntax `Re(lambda)+Im(lambda)i` to allow correct processing.
* The last argument is optional and may be supplied to obtain the Butcher array coefficients of a method with `<Degree>` stage-evaluations which is embedded into a `<Stages>` stage overall paired-explicit Runge-Kutta method.

## Credit

If you use the implementations provided here, please also cite this repository as
```
@misc{doehring2024ssocs,
  title={{SSOCs}: Arbitrary Precision Stability Polynomials by Second Order Cones},
  author={Doehring, Daniel},
  year={2024},
  howpublished={\url{https://github.com/DanielDoehring/SSOCs}},
  doi={https://doi.org/10.5281/zenodo.11184359}
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
