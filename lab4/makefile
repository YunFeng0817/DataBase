init = init.c
print = print.c
library = extmem.c
target_file = "database.out"
database_file = *.blk

# print all data in database
print: $(print)
	@gcc $(library) $(print) -o $(target_file);
	@echo "Compile finished!"
	@./$(target_file)
	@echo "print database successfully!:\n"

# generate all data
init: clean $(init)
	@rm -rf /database/
	@gcc $(library) $(init) -o $(target_file);
	@echo "Compile finished!"
	@./$(target_file)
	@echo "Init database successfully!:\n"

clean:
	-@rm $(target_file)
	