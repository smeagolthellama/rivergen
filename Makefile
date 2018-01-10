.PHONY: clean All

All:
	@echo "----------Building project:[ rivergen - Debug ]----------"
	@"$(MAKE)" -f  "rivergen.mk"
clean:
	@echo "----------Cleaning project:[ rivergen - Debug ]----------"
	@"$(MAKE)" -f  "rivergen.mk" clean
