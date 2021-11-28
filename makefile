cc = g++
prom = main
deps = reader.h printer.h evaluator.h environment.h type.h core.h helper.h memory.h pointer.h
obj = main.o reader.o printer.o evaluator.o environment.o common.o core.o helper.o memory.o pointer.o
 
$(prom): $(obj)
    $(cc) -o $(prom) $(obj)
 
%.o: %.c $(deps)
    $(cc) -c $< -o $@