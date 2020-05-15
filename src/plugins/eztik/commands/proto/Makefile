PROTOC              := protoc
OUTPUT_LANGUAGE     ?= cpp
OUTPUT_DIR          ?= ../src/impl/proto

PROTO_FILES         := $(wildcard *.proto)
SOURCE_FILES        := $(patsubst %.proto,$(OUTPUT_DIR)/%.pb.h,$(PROTO_FILES))
HEADER_FILES        := $(patsubst %.proto,$(OUTPUT_DIR)/%.pb.h,$(PROTO_FILES))

all: $(SOURCE_FILES) $(HEADER_FILES)

$(OUTPUT_DIR)/%.pb.cc $(OUTPUT_DIR)/%.pb.h: $(PROTO_FILES)
	@echo "[+] Compiling '$<'";
	@$(PROTOC) -I. --$(OUTPUT_LANGUAGE)_out=$(OUTPUT_DIR) $<;

.PHONY: clean
clean:
	@echo "[*] Cleaning ${OUTPUT_DIR}";
	@rm $(OUTPUT_DIR)/*;
