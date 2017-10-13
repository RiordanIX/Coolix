.PHONY: clean All

All:
	@echo "----------Building project:[ CoolixOS - Debug ]----------"
	@"$(MAKE)" -f  "CoolixOS.mk"
clean:
	@echo "----------Cleaning project:[ CoolixOS - Debug ]----------"
	@"$(MAKE)" -f  "CoolixOS.mk" clean
