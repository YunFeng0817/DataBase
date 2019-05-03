source = init.c
library = extmem.c
target_file = database.out
database_file = *.blk

run: $(target_file)
	@echo "Run result:\n"
	@./$(target_file)
	@rm $(target_file) $(database_file)

$(target_file): 
	@gcc $(library) $(source) -o database.out;
	@echo "Compile finished!"