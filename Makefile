PROJECT_NAME = ogigia

NC     = \033[0m
BLUE   = \033[1;34m
CYAN   = \033[1;36m
GREEN  = \033[1;32m
YELLOW = \033[1;33m

RAYLIB_DIR = raylib-5.5
RAYLIB_DIR_WIN = raylib-5.5_win64_mingw-w64

CC = clang
LD = clang

CC_WIN = x86_64-w64-mingw32-gcc
LD_WIN = x86_64-w64-mingw32-gcc

CFLAGS =  -std=c11 -Wall -Wpedantic -I.
CFLAGS += -Ideps/ -Ideps/$(RAYLIB_DIR)/src

CFLAGS_DEB = -O0 -g -gdwarf-4 -fsanitize=address
CFLAGS_REL = -O3

LDFLAGS = -lm deps/build/$(RAYLIB_DIR)/raylib/libraylib.a
LDFLAGS_DEB = -fsanitize=address
LDFLAGS_WIN = deps/$(RAYLIB_DIR_WIN)/lib/libraylib.a -lm -lwinmm -lgdi32

rwildcard = $(foreach d, $(wildcard $1*), $(call rwildcard, $d/, $2) $(filter $(subst *, %, $2), $d))

BUILD_DEB   = build/debug
BUILD_REL   = build/release
BUILD_WIN   = build/windows
OBJ_DEB_DIR = $(BUILD_DEB)/obj
OBJ_REL_DIR = $(BUILD_REL)/obj
OBJ_WIN_DIR = $(BUILD_WIN)/obj

SRC_DIR     = src
SRC         = $(call rwildcard, $(SRC_DIR), *.c)
OBJ_DEB     = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DEB_DIR)/%.o, $(SRC))
OBJ_REL     = $(patsubst $(SRC_DIR)/%.c, $(OBJ_REL_DIR)/%.o, $(SRC))
OBJ_WIN     = $(patsubst $(SRC_DIR)/%.c, $(OBJ_WIN_DIR)/%.o, $(SRC))

EXE_DEB = $(BUILD_DEB)/$(PROJECT_NAME)
EXE_REL = $(BUILD_REL)/$(PROJECT_NAME)
EXE_WIN = $(BUILD_WIN)/$(PROJECT_NAME)

.PHONY: run clean deps deps_clean

debug: $(EXE_DEB)
release: $(EXE_REL)
windows: $(EXE_WIN)

$(EXE_DEB): $(OBJ_DEB)
	@ echo -e "$(GREEN)LINKING EXECUTABLE$(NC) $@"
	@ $(LD) $(OBJ_DEB) -o $@ $(LDFLAGS) $(LDFLAGS_DEB)

$(EXE_REL): $(OBJ_REL)
	@ echo -e "$(GREEN)LINKING EXECUTABLE$(NC) $@"
	@ $(LD) $(OBJ_REL) -o $@ $(LDFLAGS)

$(EXE_WIN): $(OBJ_WIN)
	@ echo -e "$(GREEN)LINKING EXECUTABLE$(NC) $@"
	@ $(LD_WIN) $(OBJ_WIN) -o $@ $(LDFLAGS_WIN)

$(OBJ_REL_DIR)/%.o: $(SRC_DIR)/%.c
	@ mkdir -p $(@D)
	@ echo -e "$(GREEN)COMPILING OBJECT$(NC) $@"
	@ $(CC) $(CFLAGS) $(CFLAGS_REL) -c $< -o $@

$(OBJ_DEB_DIR)/%.o: $(SRC_DIR)/%.c
	@ mkdir -p $(@D)
	@ echo -e "$(GREEN)COMPILING OBJECT$(NC) $@"
	@ $(CC) $(CFLAGS) $(CFLAGS_DEB) -c $< -o $@

$(OBJ_WIN_DIR)/%.o: $(SRC_DIR)/%.c
	@ mkdir -p $(@D)
	@ echo -e "$(GREEN)COMPILING OBJECT$(NC) $@"
	@ $(CC_WIN) $(CFLAGS) $(CFLAGS_REL) -c $< -o $@

run: debug
	@ echo -e "$(CYAN)EXECUTING$(NC) $(EXE_DEB)"
	@ ./$(EXE_DEB)

clean:
	@ echo -e "$(YELLOW)CLEANING PROJECT$(NC)"
	@ rm -rf build

deps: raylib

raylib:
	@ echo -e "$(BLUE)BUILDING DEPENDENCY $(RAYLIB_DIR)$(NC)"
	@ mkdir -p deps/build/$(RAYLIB_DIR) && cd deps/build/$(RAYLIB_DIR) && cmake ../../$(RAYLIB_DIR)/ && make -j4

deps_clean:
	@ echo -e "$(YELLOW)CLEANING DEPENDENCIES$(NC)"
	@ rm -rf deps/build/
