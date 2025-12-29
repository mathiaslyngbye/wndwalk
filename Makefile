APP=wndwalk
EXE=$(APP).exe
OUTDIR=build

SRCS=src\main.cpp src\key_manager.cpp

CXX=cl
CXXFLAGS=/nologo /std:c++17 /O2 /DNDEBUG /DUNICODE /D_UNICODE
LDFLAGS=/nologo /SUBSYSTEM:WINDOWS user32.lib shell32.lib ole32.lib

all: $(OUTDIR)\$(EXE)

$(OUTDIR):
	@if not exist $(OUTDIR) mkdir $(OUTDIR)

$(OUTDIR)\$(EXE): $(SRCS) $(OUTDIR)
	$(CXX) $(CXXFLAGS) /Fe$@ $(SRCS) /link $(LDFLAGS)

clean:
	@if exist $(OUTDIR) rmdir /s /q $(OUTDIR)
