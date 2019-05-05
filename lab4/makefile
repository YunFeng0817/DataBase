select = select.c
init = init.c
print = print.c
library = extmem.c
target_file = "database.out"
database_file = *.blk

# print all data in database
print: clean
	@gcc $(library) $(print) -o $(target_file);
	@echo "Compile finished!"
	@./$(target_file)
	@echo "\nprint database successfully!\n"

# generate all data
init: clean
	@rm -rf /database/
	@gcc $(library) $(init) -o $(target_file);
	@echo "Compile finished!"
	@./$(target_file)
	@echo "\nInit database successfully!\n"

select: clean
	@gcc $(library) $(select) -o $(target_file);
	@echo "Compile finished!"
	@./$(target_file)
	@echo "\nselect data from database successfully!\n"

clean:
	-@rm $(target_file)
	