gcc ex1.c -o ex1

if [ $? -eq 0 ]; then
	./ex1
	rm ex1
else
	echo "Compilation failed."
fi		
