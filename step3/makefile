cc = g++
prom = step1_read_print
deps = reader.h printer.h evaluator.h environment.h common.h
obj = step1_read_print.o reader.o printer.o evaluator.o environment.o common.o
 
$(prom): $(obj)
    $(cc) -o $(prom) $(obj)
 
%.o: %.c $(deps)
    $(cc) -c $< -o $@