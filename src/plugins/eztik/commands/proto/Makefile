PROTOC              := protoc
OUTPUT_LANGUAGE     ?= cpp
OUTPUT_DIR          ?= ../generated/

PROTO_FILES         := $(wildcard *.proto */*.proto)
SOURCE_FILES        := $(patsubst %.proto,$(OUTPUT_DIR)/%.pb.h,$(PROTO_FILES))
HEADER_FILES        := $(patsubst %.proto,$(OUTPUT_DIR)/%.pb.h,$(PROTO_FILES))

all: $(OUTPUT_DIR) $(SOURCE_FILES) $(HEADER_FILES)

$(OUTPUT_DIR)/%.pb.cc $(OUTPUT_DIR)/%.pb.h: %.proto
	@echo "[+] Compiling '$<'";
	@$(PROTOC) -I. --$(OUTPUT_LANGUAGE)_out=$(OUTPUT_DIR) $<;

$(OUTPUT_DIR):
	@echo "[+] Creating $@";
	@mkdir -p $@

.PHONY: clean
clean:
	@echo "[*] Cleaning ${OUTPUT_DIR}";
	@rm -rf $(OUTPUT_DIR)/*;
