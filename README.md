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
To build `EiCOS` you need `Cmake` to be installed on your system, which is the case on most Linux distributions.

### EiCOS

To be able to use `SSOCs` you need to compile the shared library `libeicos_MP.so` of [my fork](https://github.com/DanielDoehring/EiCOS) from the [`EiCOS` repository](https://github.com/EmbersArc/EiCOS).
After installing `Eigen3` and the Boost multiprecision library you need to `cd` to the directory where you downloaded or forked `EiCOS`.
It is assumed that this is `~/git/EiCOS/` in the compile scripts.

Then, execute 
```bash
cmake -DCMAKE_BUILD_TYPE=Release .
```
to generate the make files.
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

It might be required that you give the shell script for compiling the code executable mode, i.e., 
```bash
chmod +x *.sh
``` 

If you installed `EiCOS` not in `~/git/EiCOS` you need to exchange that path wherever it occurrs in `Compile_SSOCs_MP.sh`.
Then, you can simply execute 
```bash
./Compile_SSOCs_MP.sh
./Compile_SSOCs_PERK4_MP.sh
```
which should give you the executable `SSOCs.exe`.
Again, it might be required that you make this executable, i.e., 
```bash
chmod +x *.exe
```

## Usage

To generate the fourth order accurate stability polynomial for the fourth order Paired-Explicit Runge-Kutta schemes type
```bash
./SSOCs_PERK4.exe <Degree> <dtMax> <path/to/spectrum/file> <OPTIONAL:Stages>
```
where 
* `<Degree>` needs to be exchanged for the stability polynomial degree, an integer > 6. 
* `<dtMax>` is the maximum timestep that may be possible. In principle this can be chosen arbitrarily large, which slows down the optimization. Supplying a too small value gives wrong results if the true admissible timestep is larger.
* `<path/to/spectrum/file>` is the path to the file with the eigenvalues used for constraining the stability polynomial. The file should contain the eigenvalues should that one eigenvalue is there per row with syntax `Re(lambda)+Im(lambda)i` to allow correct processing.
* The last argument is optional and may be supplied to obtain the Butcher array coefficients of a method with `<Degree>` stage-evaluations which is embedded into a `<Stages>` stage overall paired-explicit Runge-Kutta method.


## Acknowledgements
![DFG](https://github.com/DanielDoehring/OSPREI/assets/75639095/a0ebb057-a0c5-4dc9-96df-03ac74894d2a)

This project has benefited from funding by the Deutsche Forschungsgemeinschaft (DFG, German Research Foundation)
through the research unit FOR 5409 "Structure-Preserving Numerical Methods for Bulk- and
Interface Coupling of Heterogeneous Models (SNuBIC)" (project number 463312734).
