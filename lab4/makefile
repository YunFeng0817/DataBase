join = join.cpp
projection = projection.cpp
CreateIndex = create_index.cpp
BPlusTree = BplusTree.cpp
select = select.cpp
init = init.cpp
print = print.cpp
library = extmem.cpp
target_file = "database.out"
database_file = *.blk

join: clean
	@g++ -g $(library) $(BPlusTree) $(join) -o $(target_file);
	@echo "Compile finished!\n"
	@./$(target_file)
	@echo "\njoin operation successfully!\n"

projection: clean
	@g++ -g $(library) $(BPlusTree) $(projection) -o $(target_file);
	@echo "Compile finished!\n"
	@./$(target_file)
	@echo "\nprojection from database successfully!\n"

index: clean
	-@mkdir ./data
	-@rm ./data/*.blk
	@cp ./database/*.blk ./data/
	@g++ -g $(library) $(BPlusTree) $(CreateIndex)  -o $(target_file);
	@echo "Compile finished!\n"
	@./$(target_file)
	@echo "\nestablish database index successfully!\n"

# print all data in database
print: clean
	@g++ -g $(library) $(print) -o $(target_file);
	@echo "Compile finished!\n"
	@./$(target_file)
	@echo "\nprint database successfully!\n"

# generate all data
init: clean
	@rm ./database/*.blk
	@g++ -g $(library) $(init) -o $(target_file);
	@echo "Compile finished!\n"
	@./$(target_file)
	@echo "\nInit database successfully!\n"

select: clean
	@g++ -g $(library) $(BPlusTree) $(select) -o $(target_file);
	@echo "Compile finished!\n"
	@./$(target_file)
	@echo "\nselect data from database successfully!\n"

clean:
	-@rm $(target_file)
	