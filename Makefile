CXX=g++
CXXFLAGS=-std=c++17 -I $(HOME)/git/EiCOS/include/ -L $(HOME)/git/EiCOS/ -l eicos_MP -Wl,-rpath,$(HOME)/git/EiCOS/ -Ofast

SSOCsMP_TARGET=SSOCs.exe
SSOCs_PERK4_MP_TARGET=SSOCs_PERK4.exe

all: $(SSOCsMP_TARGET) $(SSOCs_PERK4_MP_TARGET)

$(SSOCsMP_TARGET): SSOCs.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(SSOCs_PERK4_MP_TARGET): SSOCs_PERK4.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(SSOCsMP_TARGET) $(SSOCs_PERK4_MP_TARGET)

cleanout:
	rm *.txt