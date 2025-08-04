CXX=g++
CXXFLAGS=-std=c++17 -I $(HOME)/git/EiCOS/include/ -L $(HOME)/git/EiCOS/ -Wl,-rpath,$(HOME)/git/EiCOS/ -Ofast

LIB_MP=-l eicos_MP
LIB_LDBL=-l eicos_LDBL 

# Standard SSOCs
SSOCs_MP_TARGET=SSOCs_MP
SSOCs_LDBL_TARGET=SSOCs_LDBL
SSOCs_LIST_LDBL_TARGET=SSOCs_List_LDBL

# SSOCs for PERK-4 stability polynomials
SSOCs_PERK4_MP_TARGET=SSOCs_PERK4_MP
SSOCs_PERK4_LDBL_TARGET=SSOCs_PERK4_LDBL

all: $(MP_TARGETS) $(LDBL_TARGETS)

MP_TARGETS: $(SSOCs_MP_TARGET) $(SSOCs_PERK4_MP_TARGET)
LDBL_TARGETS: $(SSOCs_LDBL_TARGET) $(SSOCs_LIST_LDBL_TARGET) $(SSOCs_PERK4_LDBL_TARGET)

$(SSOCs_MP_TARGET): SSOCs.cpp
	$(CXX) $< $(CXXFLAGS) -o $@.exe $(LIB_MP)

$(SSOCs_LDBL_TARGET): SSOCs.cpp
	$(CXX) $< $(CXXFLAGS) -o $@.exe $(LIB_LDBL)

$(SSOCs_LIST_LDBL_TARGET): SSOCs_List.cpp
	$(CXX) $< $(CXXFLAGS) -o $@.exe $(LIB_LDBL)


$(SSOCs_PERK4_MP_TARGET): SSOCs_PERK4.cpp
	$(CXX) $< $(CXXFLAGS) -o $@.exe $(LIB_MP)

$(SSOCs_PERK4_LDBL_TARGET): SSOCs_PERK4.cpp
	$(CXX) $< $(CXXFLAGS) -o $@.exe $(LIB_LDBL)

clean:
	rm -f *.exe

cleanout:
	rm -f *.txt *.exe