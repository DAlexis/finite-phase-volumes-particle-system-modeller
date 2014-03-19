CXXFLAGS += -Wall -std=c++0x $(include_dirs) -march=native
CFLAGS += -Wall $(include_dirs)
CC = gcc
CXX = g++

GLOBAL_BUILD_DIR = $(HOME_DIR)/build

ifndef cfg
    cfg=debug
endif

ifeq ($(cfg), debug)
    CXXFLAGS += -O0 -D_DEBUG -g $(defines)
    CFLAGS += -O0 -D_DEBUG -g $(defines)
    BUILD_DIR +=$(GLOBAL_BUILD_DIR)/debug
else
    ifeq ($(cfg), release)
        CXXFLAGS += -O3 -DNDEBUG
        CFLAGS += -O3 -DNDEBUG
        BUILD_DIR +=$(GLOBAL_BUILD_DIR)/release
    else
        $(error Error! Can not build with configuration $(cfg))
    endif
endif

c_sources_full = $(c_sources:%.c=$(sources_dir)/%.c)
cpp_sources_full = $(cpp_sources:%.cpp=$(sources_dir)/%.cpp)

static_libs_full = $(static_libs:%.a=$(BUILD_DIR)/%.a)

dirs_to_create_in_build = $(dir fvm/axis.cpp)
dirs_to_create = $(dirs_to_create_in_build:%=$(BUILD_DIR)/%)

c_objs = $(c_sources:%.c=$(BUILD_DIR)/%.o)
cpp_objs = $(cpp_sources:%.cpp=$(BUILD_DIR)/%.o)

c_depends = $(c_objs:%.o=%.d)
cpp_depends = $(cpp_objs:%.o=%.d)

all: deps $(aname) $(soname) $(exe_name) $(post_build)

-include $(c_depends)
-include $(cpp_depends)

# Making .d files for .c sources
$(c_depends):
	mkdir -p $(dirs_to_create)
	$(CC) -MM -MT '$(patsubst %.d,%.o,$@) $@' -MF $@ $(patsubst $(BUILD_DIR)/%.d,$(sources_dir)/%.c,$@) $(CFLAGS) 

# Making .d files for .cpp sources
$(cpp_depends):
	mkdir -p $(dirs_to_create)
	$(CXX) -MM -MT '$(patsubst %.d,%.o,$@) $@' -MF $@ $(patsubst $(BUILD_DIR)/%.d,$(sources_dir)/%.cpp,$@) $(CXXFLAGS) 

# Patently talk make to build .d files for each .o file
%.o: %.d

# Compiling .c sources
$(c_objs): $(c_sources_full)
	mkdir -p $(dirs_to_create)
	$(CC) -c -o $@ $(patsubst $(BUILD_DIR)/%.o,$(sources_dir)/%.c,$@) $(CFLAGS)

# Compiling .cpp sources
$(cpp_objs):
	mkdir -p $(dirs_to_create)
	$(CXX) -c -o $@ $(patsubst $(BUILD_DIR)/%.o,$(sources_dir)/%.cpp,$@) $(CXXFLAGS)

# Linking executable
$(exe_name): $(c_objs) $(cpp_objs) 
	mkdir -p $(dirs_to_create)
	$(CXX) -o $(BUILD_DIR)/$@ $^ $(static_libs_full) $(CXXFLAGS) $(LDFLAGS)

# Making static libraries
$(aname): $(c_objs) $(cpp_objs) 
	mkdir -p $(dirs_to_create)
	ar r $(BUILD_DIR)/$(aname) $^

# Making dynamic libraries
$(soname): $(c_objs) $(cpp_objs)
	$(CXX) -shared -Wl,-soname,$(soname) -o $(BUILD_DIR)/$(soname) $(c_objs) $(cpp_objs)

deps:
	for i in $(dependencies) ; do make -C $$i cfg=$(cfg);  done

# Generating documentation
doc: doc_en doc_ru

doc_en: $(c_sources_full) $(cpp_sources_full) $(c_depends) $(cpp_depends) Doxyfile.en
	doxygen Doxyfile.en

doc_ru: $(c_sources_full) $(cpp_sources_full) $(c_depends) $(cpp_depends) Doxyfile.ru
	doxygen Doxyfile.ru
