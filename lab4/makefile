CreateIndex = create_index.cpp
BPlusTree = BplusTree.cpp
select = select.cpp
init = init.cpp
print = print.cpp
library = extmem.cpp
target_file = "database.out"
database_file = *.blk

index: clean
	-@rm ./data/*.blk
	@cp ./database/*.blk ./data/
	@g++ -g $(library) $(BPlusTree) $(CreateIndex)  -o $(target_file);
	@echo "Compile finished!"
	@./$(target_file)
	@echo "\nestablish database index successfully!\n"

# print all data in database
print: clean
	@g++ -g $(library) $(print) -o $(target_file);
	@echo "Compile finished!"
	@./$(target_file)
	@echo "\nprint database successfully!\n"

# generate all data
init: clean
	@rm ./database/*.blk
	@g++ -g $(library) $(init) -o $(target_file);
	@echo "Compile finished!"
	@./$(target_file)
	@echo "\nInit database successfully!\n"

select: clean
	@g++ -g $(library) $(BPlusTree) $(select) -o $(target_file);
	@echo "Compile finished!"
	@./$(target_file)
	@echo "\nselect data from database successfully!\n"

clean:
	-@rm $(target_file)
	