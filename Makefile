
ray: ray.c
	cc -Wno-implicit-function-declaration \
		-Wno-implicit-int \
		-Wno-unused-value \
		-Wno-return-type \
		ray.c -o ray
