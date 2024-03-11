#include <eicos_MP.hpp> // For multiprecision version
//#include <eicos_LDBL.hpp> // For long double version

#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm> // std::reverse

#include <complex>
#include <boost/multiprecision/cpp_complex.hpp>
//#include <boost/multiprecision/cpp_bin_float.hpp> // For quad, oct precision

using float_type = boost::multiprecision::number<boost::multiprecision::cpp_dec_float<40>>;
//using float_type = long double; 

using complex_type = std::complex<float_type>;


// Chose float_type sinze even long long int runs out of "space" at ~22 digits
inline float_type factorial(const size_t n) {
  float_type res = 1;
  for(int i = 1; i <= n; i++)
    res *= i;
  return res;
}

int main(int argc, char *argv[]) {
  assert(argc >= 4); // Order is NOT variable for this case!
  const int NumStageEvals = std::stoi(argv[1]);

  const int ConsOrder     = 4; // This optimization problem is fixed for my special case of 4th order PERK methods

  float_type dtMax        = static_cast<float_type>(std::stod(argv[2]));
  std::cout << "Supplied dtMax: " << dtMax << std::endl;

  const std::string EvalFileName = std::string(argv[3]);

  // Optional argument for minimal timestep to speed up convergence of the bisection
  float_type dtMin = argc >= 5 ? static_cast<float_type>(std::stod(argv[4])) : static_cast<float_type>(0.);
  std::cout << "Supplied dtMin: " << dtMin << std::endl;

  // his is somewhat problem dependent, the larger the dt the larger the bisection gap
  const float_type dtEps = argc == 6 ? static_cast<float_type>(std::stod(argv[5])): static_cast<float_type>(1e-6);
  std::cout << "Supplied dtEps: " << dtEps << std::endl;

  // If NumStages is supplied, use it, otherwise use NumStageEvals
  const int NumStages = argc >= 7 ? std::stoi(argv[6]) : NumStageEvals;
  assert(NumStages >= NumStageEvals);

  const int NumCoeffs = NumStageEvals - 5;
  // Number of optimization variables;
  // increased by one due to dummy variable required for EiCOS
  const int NumOpt = NumCoeffs + 1;

  std::cout << std::endl << "Datatype in use has " << std::numeric_limits<float_type>::digits10
            << " significant digits." << std::endl;

  std::cout.precision(5);
  std::cout << "Smallest number roughly of scale " << static_cast<float_type>(1)/factorial(NumStageEvals) << std::endl;
  std::cout.precision(15); // Some "not too busy" value

  int i = 0;
  std::ifstream EigValsFile(EvalFileName);
  assert(EigValsFile);
  std::string line;
  while (std::getline(EigValsFile, line))
    i++;
  EigValsFile.close();

  const int NumEigVals = i; // This is the number cones (and also number of inequalities)
  std::cout << std::endl << "Number of Eigenvalues = Number of constraints is: " << NumEigVals << std::endl << std::endl;
  assert(NumEigVals >= 1);
  const int ConstraintDims = 3 * NumEigVals; // 3 Since one column is for the dummy variable, the others for the real and imag part

  // Restriced precision sufficient here (Eigenvalues in Julia per default computed up to double precision)
  auto EigVals = new Eigen::Vector<std::complex<long double>, Eigen::Dynamic>; // Dynamically allocate 'EigVals' to be able to delete later
  EigVals->resize(NumEigVals);

  long double real, imag; // Restriced precision sufficient here (Eigenvalues in Julia per default computed up to double precision)
  char dummy;
  i = 0;
  EigValsFile.open(EvalFileName);
  while (std::getline(EigValsFile, line)) {
    std::stringstream stream(line);
    while (stream >> real >> dummy >> imag) {
      assert(i <= NumEigVals-1);
      (*EigVals)[i] = std::complex<long double>(real, imag);
    }
    i++;
  }
  EigValsFile.close();

  // Dynamically allocate 'EigValsPowers' to be able to delete later (save memory)
  auto EigValsPowers = new Eigen::Matrix<complex_type, Eigen::Dynamic, Eigen::Dynamic>;
  // Do not safe zero'th power = 1
  EigValsPowers->resize(NumEigVals, NumStageEvals);
  for(size_t i = 0; i < NumEigVals; i++) {
    for(size_t j = 0; j < NumStageEvals; j++) {
      (*EigValsPowers)(i, j) = static_cast<complex_type>(pow((*EigVals)[i], j + 1));
    }
  }

  delete EigVals;

  Eigen::Matrix<float_type, Eigen::Dynamic, Eigen::Dynamic> EigValsPowersReal;
  EigValsPowersReal.resize(NumEigVals, NumStageEvals);

  Eigen::Matrix<float_type, Eigen::Dynamic, Eigen::Dynamic> EigValsPowersImag;
  EigValsPowersImag.resize(NumEigVals, NumStageEvals);

  float_type Factorial;
  for(size_t j = 0; j < NumStageEvals; j++) {
    Factorial = factorial(j + 1); // Compute only once per j
    for(size_t i = 0; i < NumEigVals; i++) {
      EigValsPowersReal(i, j) = static_cast<float_type>((*EigValsPowers)(i, j).real()) / Factorial;
      //EigValsPowersReal(i, j) = static_cast<float_type>((*EigValsPowers)(i, j).real());

      EigValsPowersImag(i, j) = static_cast<float_type>((*EigValsPowers)(i, j).imag()) / Factorial;
      //EigValsPowersImag(i, j) = static_cast<float_type>((*EigValsPowers)(i, j).imag());
    }
  }

  delete EigValsPowers;

  /// Timestep-independent variables ///
  // Array with dimensions of cones
  Eigen::Vector<int, Eigen::Dynamic> q;
  q.resize(NumEigVals);
  for(size_t i = 0; i < NumEigVals; i++)
    q(i) = 3;

  // Cost-vector (defines objective)
  Eigen::Vector<float_type, Eigen::Dynamic> c; // Use float_type everywhere to avoid truncation
  c.resize(NumOpt);
  c[0] = static_cast<float_type>(1.); // Acts on dummy variable
  for(size_t i = 1; i < NumOpt; i++)
    c[i] = static_cast<float_type>(0.);

  // No equality constraints present in problem, but variables have to be supplied
  Eigen::SparseMatrix<float_type> A; // Use float_type everywhere to avoid truncation
  Eigen::Vector<float_type, Eigen::Dynamic> b; // Use float_type everywhere to avoid truncation

  /// Timestep related variables ///
  float_type dt;

  /// Variables depending on time ///
  Eigen::Matrix<float_type, Eigen::Dynamic, Eigen::Dynamic> G_Dense;
  G_Dense.resize(ConstraintDims, NumOpt);
  for(size_t i = 0; i < ConstraintDims; i++)
    for(size_t j = 0; j < NumOpt; j++)
      G_Dense(i, j) = static_cast<float_type>(0.);

  Eigen::Vector<float_type, Eigen::Dynamic> h;
  h.resize(ConstraintDims);
  for(size_t i = 0; i < ConstraintDims; i++)
    h(i) = static_cast<float_type>(0.);

  Eigen::Vector<float_type, Eigen::Dynamic> t_gamma; // This is for the solution
  t_gamma.resize(NumOpt);

  // This is needed to specify the structure of G & h which are supplied to the solver.
  // Then, the "updateData" method can be used instead of having to re-initialize the solver in every iteration
  for(size_t i = 0; i < NumEigVals; i++) {
      h(3 * i + 1) = static_cast<float_type>(1.); // + 1 since this is the real part
      h(3 * i + 2) = static_cast<float_type>(0.);

      G_Dense(3 * i, 0) = -1.; // This acts on the dummy variable
      // These act on the actual optimization variables
      for(size_t j = 1; j < NumOpt; j++) {
        // Use placeholder variables here
        G_Dense(3 * i + 1, j) = 42.;
        G_Dense(3 * i + 2, j) = 42.;
      }
    }
  EiCOS::Solver solver(G_Dense.sparseView(), A, c, h, b, q);

  // c_{S-3}
  const float_type cS3 = 1.0;
  // Constants arising from the particular form of Butcher tableau chosen for the 4th order PERK methods
  const float_type k1 = static_cast<float_type>(0.001055026310046423/cS3);
  const float_type k2 = static_cast<float_type>(0.03726406530405851/cS3);

  const int NumC = NumStages - 5;
  // Choice of timesteps:  Uniformaly ascending timesteps
  std::vector<float_type> c_Free_Flipped(NumC, 0);
  for (int i = 0; i < NumC; ++i) {
    // Linear increasing
    //c_Free_Flipped[i] = static_cast<float_type>((NumC - i) / (NumC + 1.0));
    // Quadratically increasing
    //c_Free_Flipped[i] = static_cast<float_type>((NumC - i) / (NumC + 1.0) * (NumC - i) / (NumC + 1.0));
    // Constant
    c_Free_Flipped[i] = static_cast<float_type>(1.0);
  }

  float_type t;
  while(dtMax - dtMin > dtEps) {
    dt = static_cast<float_type>(0.5) * (dtMax + dtMin);

    for(size_t i = 0; i < NumEigVals; i++) {
      h(3 * i + 1) = static_cast<float_type>(1.); // + 1 since this is the real part
      h(3 * i + 2) = static_cast<float_type>(0.);

      for(size_t j = 0; j < ConsOrder; j++) {
        h(3 * i + 1) += pow(dt, j+1) * EigValsPowersReal(i, j);
        h(3 * i + 2) += pow(dt, j+1) * EigValsPowersImag(i, j);
      }
      // Addition for 4th order PERK methods
      h(3 * i + 1) += k1 * pow(dt, 5) * EigValsPowersReal(i, 4) * factorial(5);
      h(3 * i + 2) += k1 * pow(dt, 5) * EigValsPowersImag(i, 4) * factorial(5);

      //G_Dense(3 * i, 0) = -1.; // This acts on the dummy variable
      for(size_t j = 1; j < NumOpt; j++) {
        G_Dense(3 * i + 1, j) = -c_Free_Flipped[j-1] * (k2 * pow(dt, j+ConsOrder) * EigValsPowersReal(i, j+ConsOrder-1) +
                                                        k1 * pow(dt, j+ConsOrder+1) * EigValsPowersReal(i, j+ConsOrder) *
                                                        // normalize with same factorial
                                                        (j+ConsOrder+1));
        G_Dense(3 * i + 2, j) = -c_Free_Flipped[j-1] * (k2 * pow(dt, j+ConsOrder) * EigValsPowersImag(i, j+ConsOrder-1) +
                                                        k1 * pow(dt, j+ConsOrder+1) * EigValsPowersImag(i, j+ConsOrder) *
                                                        // normalize with same factorial
                                                        (j+ConsOrder+1));
      }
    }

    std::cout << "Current dt is: " << dt << std::endl;

    //EiCOS::Solver solver(G_Dense.sparseView(), A, c, h, b, q);
    solver.updateData(G_Dense.sparseView(), A, c, h, b); // In theory faster

    solver.solve();
    t = solver.solution()[0];

    std::cout << "Current stability bound is: " << t << std::endl << std::endl;

    // Bisection procedure
    if(t < static_cast<float_type>(1.) )
      dtMin = dt;
    else
      dtMax = dt;
  }
  std::cout << std::endl;

  t_gamma = solver.solution();

  for(size_t i = 1; i < NumOpt; i++) { // Do not rescale stability value (i=0)
    t_gamma(i) /= factorial(i + ConsOrder);
  }

  std::vector<float_type> A_free(NumCoeffs, 42);
  A_free[0] = t_gamma[1];
  int l = 2;
  for (int i = 5; i < NumStageEvals - 1; ++i) {
    A_free[l-1] = t_gamma[l] / t_gamma[l - 1];
    l++;
  }

  // Flip A_free to have ascending order
  std::reverse(A_free.begin(), A_free.end());

  std::cout << "Values of coefficients" << std::endl;

  const std::string filename = "a_" + std::to_string(NumStageEvals) + "_" + std::to_string(NumStages) + ".txt";
  //const std::string filename = "a_" + std::to_string(NumStageEvals) + ".txt";
  std::ofstream gamma_file(filename);
  for(size_t i = 0; i < NumOpt-1; i++) { // Omit stability value (first position)
    std::stringstream StringStr; // On purpose within loop (automatic reset)
    StringStr << std::setprecision(std::numeric_limits<float_type>::max_digits10);
    StringStr << A_free[i];
    gamma_file << StringStr.str();
    std::cout << StringStr.str() << std::endl;
    if(i != NumOpt - 1)
      gamma_file << "\n";
  }
  gamma_file.close();
  std::cout << std::endl;

  return 0;
}
