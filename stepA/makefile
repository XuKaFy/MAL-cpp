cc = g++
prom = main
deps = reader.h printer.h evaluator.h environment.h common.h core.h helper.h memory.h
obj = main.o reader.o printer.o evaluator.o environment.o common.o core.o helper.o memory.o
 
$(prom): $(obj)
    $(cc) -o $(prom) $(obj)
 
%.o: %.c $(deps)
    $(cc) -c $< -o $@