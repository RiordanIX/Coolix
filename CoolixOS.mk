##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=CoolixOS
ConfigurationName      :=Debug
WorkspacePath          :="C:/Users/Will/Documents/School/CS 3502/CoolixOS"
ProjectPath            :="C:/Users/Will/Documents/School/CS 3502/CoolixOS"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Will
Date                   :=13/10/2017
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :="C:/Program Files/TDM-GCC-64/bin/g++.exe"
SharedObjectLinkerName :="C:/Program Files/TDM-GCC-64/bin/g++.exe" -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="CoolixOS.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :="C:/Program Files/TDM-GCC-64/bin/windres.exe"
LinkOptions            :=  -O0
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)Debug 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := "C:/Program Files/TDM-GCC-64/bin/ar.exe" rcu
CXX      := "C:/Program Files/TDM-GCC-64/bin/g++.exe"
CC       := "C:/Program Files/TDM-GCC-64/bin/gcc.exe"
CXXFLAGS :=  -std=c++11 -g -Wall $(Preprocessors)
CFLAGS   :=   $(Preprocessors)
ASFLAGS  := 
AS       := "C:/Program Files/TDM-GCC-64/bin/as.exe"


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/src_cpu.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_disk.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_init.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_loader.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_ram.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_virt_comp.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_dispatcher.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_OSDriver.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_loadtest.cpp$(ObjectSuffix) $(IntermediateDirectory)/FIFO.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/pcb.cpp$(ObjectSuffix) $(IntermediateDirectory)/Scheduler.cpp$(ObjectSuffix) $(IntermediateDirectory)/PriorityQueue.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_cpu.cpp$(ObjectSuffix): src/cpu.cpp $(IntermediateDirectory)/src_cpu.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Will/Documents/School/CS 3502/CoolixOS/src/cpu.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_cpu.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_cpu.cpp$(DependSuffix): src/cpu.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_cpu.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_cpu.cpp$(DependSuffix) -MM src/cpu.cpp

$(IntermediateDirectory)/src_cpu.cpp$(PreprocessSuffix): src/cpu.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_cpu.cpp$(PreprocessSuffix) src/cpu.cpp

$(IntermediateDirectory)/src_disk.cpp$(ObjectSuffix): src/disk.cpp $(IntermediateDirectory)/src_disk.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Will/Documents/School/CS 3502/CoolixOS/src/disk.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_disk.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_disk.cpp$(DependSuffix): src/disk.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_disk.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_disk.cpp$(DependSuffix) -MM src/disk.cpp

$(IntermediateDirectory)/src_disk.cpp$(PreprocessSuffix): src/disk.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_disk.cpp$(PreprocessSuffix) src/disk.cpp

$(IntermediateDirectory)/src_init.cpp$(ObjectSuffix): src/init.cpp $(IntermediateDirectory)/src_init.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Will/Documents/School/CS 3502/CoolixOS/src/init.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_init.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_init.cpp$(DependSuffix): src/init.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_init.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_init.cpp$(DependSuffix) -MM src/init.cpp

$(IntermediateDirectory)/src_init.cpp$(PreprocessSuffix): src/init.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_init.cpp$(PreprocessSuffix) src/init.cpp

$(IntermediateDirectory)/src_loader.cpp$(ObjectSuffix): src/loader.cpp $(IntermediateDirectory)/src_loader.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Will/Documents/School/CS 3502/CoolixOS/src/loader.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_loader.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_loader.cpp$(DependSuffix): src/loader.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_loader.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_loader.cpp$(DependSuffix) -MM src/loader.cpp

$(IntermediateDirectory)/src_loader.cpp$(PreprocessSuffix): src/loader.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_loader.cpp$(PreprocessSuffix) src/loader.cpp

$(IntermediateDirectory)/src_ram.cpp$(ObjectSuffix): src/ram.cpp $(IntermediateDirectory)/src_ram.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Will/Documents/School/CS 3502/CoolixOS/src/ram.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_ram.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_ram.cpp$(DependSuffix): src/ram.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_ram.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_ram.cpp$(DependSuffix) -MM src/ram.cpp

$(IntermediateDirectory)/src_ram.cpp$(PreprocessSuffix): src/ram.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_ram.cpp$(PreprocessSuffix) src/ram.cpp

$(IntermediateDirectory)/src_virt_comp.cpp$(ObjectSuffix): src/virt_comp.cpp $(IntermediateDirectory)/src_virt_comp.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Will/Documents/School/CS 3502/CoolixOS/src/virt_comp.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_virt_comp.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_virt_comp.cpp$(DependSuffix): src/virt_comp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_virt_comp.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_virt_comp.cpp$(DependSuffix) -MM src/virt_comp.cpp

$(IntermediateDirectory)/src_virt_comp.cpp$(PreprocessSuffix): src/virt_comp.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_virt_comp.cpp$(PreprocessSuffix) src/virt_comp.cpp

$(IntermediateDirectory)/src_dispatcher.cpp$(ObjectSuffix): src/dispatcher.cpp $(IntermediateDirectory)/src_dispatcher.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Will/Documents/School/CS 3502/CoolixOS/src/dispatcher.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_dispatcher.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_dispatcher.cpp$(DependSuffix): src/dispatcher.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_dispatcher.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_dispatcher.cpp$(DependSuffix) -MM src/dispatcher.cpp

$(IntermediateDirectory)/src_dispatcher.cpp$(PreprocessSuffix): src/dispatcher.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_dispatcher.cpp$(PreprocessSuffix) src/dispatcher.cpp

$(IntermediateDirectory)/src_OSDriver.cpp$(ObjectSuffix): src/OSDriver.cpp $(IntermediateDirectory)/src_OSDriver.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Will/Documents/School/CS 3502/CoolixOS/src/OSDriver.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_OSDriver.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_OSDriver.cpp$(DependSuffix): src/OSDriver.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_OSDriver.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_OSDriver.cpp$(DependSuffix) -MM src/OSDriver.cpp

$(IntermediateDirectory)/src_OSDriver.cpp$(PreprocessSuffix): src/OSDriver.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_OSDriver.cpp$(PreprocessSuffix) src/OSDriver.cpp

$(IntermediateDirectory)/test_loadtest.cpp$(ObjectSuffix): test/loadtest.cpp $(IntermediateDirectory)/test_loadtest.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Will/Documents/School/CS 3502/CoolixOS/test/loadtest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_loadtest.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_loadtest.cpp$(DependSuffix): test/loadtest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_loadtest.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_loadtest.cpp$(DependSuffix) -MM test/loadtest.cpp

$(IntermediateDirectory)/test_loadtest.cpp$(PreprocessSuffix): test/loadtest.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_loadtest.cpp$(PreprocessSuffix) test/loadtest.cpp

$(IntermediateDirectory)/FIFO.cpp$(ObjectSuffix): FIFO.cpp $(IntermediateDirectory)/FIFO.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Will/Documents/School/CS 3502/CoolixOS/FIFO.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FIFO.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FIFO.cpp$(DependSuffix): FIFO.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FIFO.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FIFO.cpp$(DependSuffix) -MM FIFO.cpp

$(IntermediateDirectory)/FIFO.cpp$(PreprocessSuffix): FIFO.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FIFO.cpp$(PreprocessSuffix) FIFO.cpp

$(IntermediateDirectory)/pcb.cpp$(ObjectSuffix): pcb.cpp $(IntermediateDirectory)/pcb.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Will/Documents/School/CS 3502/CoolixOS/pcb.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/pcb.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/pcb.cpp$(DependSuffix): pcb.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/pcb.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/pcb.cpp$(DependSuffix) -MM pcb.cpp

$(IntermediateDirectory)/pcb.cpp$(PreprocessSuffix): pcb.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/pcb.cpp$(PreprocessSuffix) pcb.cpp

$(IntermediateDirectory)/Scheduler.cpp$(ObjectSuffix): Scheduler.cpp $(IntermediateDirectory)/Scheduler.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Will/Documents/School/CS 3502/CoolixOS/Scheduler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Scheduler.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Scheduler.cpp$(DependSuffix): Scheduler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Scheduler.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Scheduler.cpp$(DependSuffix) -MM Scheduler.cpp

$(IntermediateDirectory)/Scheduler.cpp$(PreprocessSuffix): Scheduler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Scheduler.cpp$(PreprocessSuffix) Scheduler.cpp

$(IntermediateDirectory)/PriorityQueue.cpp$(ObjectSuffix): PriorityQueue.cpp $(IntermediateDirectory)/PriorityQueue.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Will/Documents/School/CS 3502/CoolixOS/PriorityQueue.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PriorityQueue.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PriorityQueue.cpp$(DependSuffix): PriorityQueue.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PriorityQueue.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PriorityQueue.cpp$(DependSuffix) -MM PriorityQueue.cpp

$(IntermediateDirectory)/PriorityQueue.cpp$(PreprocessSuffix): PriorityQueue.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PriorityQueue.cpp$(PreprocessSuffix) PriorityQueue.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


