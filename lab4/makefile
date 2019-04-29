source = test.c
library = extmem.c
target_file = database.out

run: $(target_file)
	@echo "Run result:\n"
	@./$(target_file)
	@rm $(target_file)

$(target_file): 
	@gcc $(library) $(source) -o database.out;
	@echo "Compile finish!"