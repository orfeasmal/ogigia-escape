PROJECT_NAME = jam

NC     = \033[0m
BLUE   = \033[1;34m
CYAN   = \033[1;36m
GREEN  = \033[1;32m
YELLOW = \033[1;33m

#MINIAUDIO_DIR = miniaudio-0.11.25
RAYLIB_DIR = raylib-5.5

CC = clang
LD = clang

CFLAGS =  -std=c11 -Wall -Wextra -Wpedantic -I.
CFLAGS += -Ideps/$(RAYLIB_DIR)/src -Ideps/$(RAYLIB_DIR)/src/external # -Ideps/$(MINIAUDIO_DIR)

CFLAGS_DEB = -O0 -g -gdwarf-4 -fsanitize=address
CFLAGS_REL = -O3

LDFLAGS = -lm deps/build/$(RAYLIB_DIR)/raylib/libraylib.a #deps/build/$(MINIAUDIO_DIR)/*.o
LDFLAGS_DEB = -fsanitize=address

rwildcard = $(foreach d, $(wildcard $1*), $(call rwildcard, $d/, $2) $(filter $(subst *, %, $2), $d))

BUILD_DEB   = build/debug
BUILD_REL   = build/release
OBJ_DEB_DIR = $(BUILD_DEB)/obj
OBJ_REL_DIR = $(BUILD_REL)/obj

SRC_DIR     = src
SRC         = $(call rwildcard, $(SRC_DIR), *.c)
OBJ_DEB     = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DEB_DIR)/%.o, $(SRC))
OBJ_REL     = $(patsubst $(SRC_DIR)/%.c, $(OBJ_REL_DIR)/%.o, $(SRC))

EXE_DEB = $(BUILD_DEB)/$(PROJECT_NAME)
EXE_REL = $(BUILD_REL)/$(PROJECT_NAME)

.PHONY: run clean deps deps_clean

debug: $(EXE_DEB)
release: $(EXE_REL)

$(EXE_DEB): $(OBJ_DEB)
	@ echo -e "$(GREEN)LINKING EXECUTABLE$(NC) $@"
	@ $(LD) $(OBJ_DEB) -o $@ $(LDFLAGS) $(LDFLAGS_DEB)

$(EXE_REL): $(OBJ_REL)
	@ echo -e "$(GREEN)LINKING EXECUTABLE$(NC) $@"
	@ $(LD) $(OBJ_REL) -o $@ $(LDFLAGS)

$(OBJ_REL_DIR)/%.o: $(SRC_DIR)/%.c
	@ mkdir -p $(@D)
	@ echo -e "$(GREEN)COMPILING OBJECT$(NC) $@"
	@ $(CC) $(CFLAGS) $(CFLAGS_REL) -c $< -o $@

$(OBJ_DEB_DIR)/%.o: $(SRC_DIR)/%.c
	@ mkdir -p $(@D)
	@ echo -e "$(GREEN)COMPILING OBJECT$(NC) $@"
	@ $(CC) $(CFLAGS) $(CFLAGS_DEB) -c $< -o $@

run: debug
	@ echo -e "$(CYAN)EXECUTING$(NC) $(EXE_DEB)"
	@ ./$(EXE_DEB)

clean:
	@ echo -e "$(YELLOW)CLEANING PROJECT$(NC)"
	@ rm -rf build

deps: raylib# miniaudio

raylib:
	@ echo -e "$(BLUE)BUILDING DEPENDENCY $(RAYLIB_DIR)$(NC)"
	@ mkdir -p deps/build/$(RAYLIB_DIR) && cd deps/build/$(RAYLIB_DIR) && cmake ../../$(RAYLIB_DIR)/ && make -j4

#miniaudio:
#	@ echo -e "$(BLUE)BUILDING DEPENDENCY $(MINIAUDIO_DIR)$(NC)"
#	@ mkdir -p deps/build/$(MINIAUDIO_DIR) && cd deps/build/$(MINIAUDIO_DIR) && cc -O3 -I../../$(MINIAUDIO_DIR) -c ../../$(MINIAUDIO_DIR)/*.c

deps_clean:
	@ echo -e "$(YELLOW)CLEANING DEPENDENCIES$(NC)"
	@ rm -rf deps/build/
