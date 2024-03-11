# Third order accurate stability polynomial of degree 8
echo Third order accurate stability polynomial of degree 8
echo 
echo ../SSOCs_MP.exe 8 3 0.2 Spectrum.txt
echo
../SSOCs_MP.exe 8 3 0.2 Spectrum.txt

echo ../SSOCs_MP.exe 8 3 0.2 Spectrum.txt
echo
../SSOCs_MP.exe 8 3 0.2 Spectrum.txt 0.1 1e-5

echo

# Fourth order accurate stability polynomials of degree 7
echo Fourth order accurate stability polynomials of degree 7
echo
echo ../SSOCs_PERK4_MP.exe 7 0.2 Spectrum.txt
echo
../SSOCs_PERK4_MP.exe 7 0.2 Spectrum.txt

echo ../SSOCs_PERK4_MP.exe 7 0.2 Spectrum.txt 0.05 1e-5
echo
../SSOCs_PERK4_MP.exe 7 0.2 Spectrum.txt 0.05 1e-5