
ray: ray.c
	cc -Wno-implicit-function-declaration \
		-Wno-implicit-int \
		-Wno-return-type \
		ray.c -o ray
