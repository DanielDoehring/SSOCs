CXX=g++
CXXFLAGS=-std=c++17 -I $(HOME)/git/EiCOS/include/ -L $(HOME)/git/EiCOS/ -Wl,-rpath,$(HOME)/git/EiCOS/ -Ofast

LIB_MP=-l eicos_MP
LIB_LDBL=-l eicos_LDBL 

SSOCsMP_TARGET=SSOCs.exe
SSOCs_PERK4_MP_TARGET=SSOCs_PERK4.exe

all: $(SSOCsMP_TARGET) $(SSOCs_PERK4_MP_TARGET)

$(SSOCsMP_TARGET): SSOCs.cpp
	$(CXX) $< $(CXXFLAGS) -o $@ $(LIB_MP)

$(SSOCs_PERK4_MP_TARGET): SSOCs_PERK4.cpp
	$(CXX) $< $(CXXFLAGS) -o $@ $(LIB_MP)

clean:
	rm -f $(SSOCsMP_TARGET) $(SSOCs_PERK4_MP_TARGET)

cleanout:
	rm *.txt