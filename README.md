# MPI Sieve of Eratosthenes

<p align="center">
   <img src="https://res.cloudinary.com/practicaldev/image/fetch/s--99UhkUVh--/c_imagga_scale,f_auto,fl_progressive,h_900,q_auto,w_1600/https://dev-to-uploads.s3.amazonaws.com/i/lo9f1k07sxb8qleixpzg.png" width="540px">
</p>

<p>
	<a href="https://github.com/moon1ock/MPI_primes/blob/main/LICENSE"><img src="https://img.shields.io/badge/license-MIT-blue.svg" alt="MIT license"></a>
</p>

# MPI Installation

## Linux

Beforedoing any MPI programming, make sure to load the MPI module on Unix:

```bash
$ module load mpi/openmpi-x86_64
```

## Mac

Or, if you are using Mac, just:
```bash
$ brew install open-mpi
```


# Module

## Executable

You can make the module with `<dir> $make`

```bash
$ make
```
> on `make` you will have an executable compiled with _debug_ and _compilation warnings_ turned *on*.
> `-lm` stands for `<math.h>` library inclusion.

## Clean

``` bash
$ make clean
```
> this will remove the executable as well as all of the generated `.txt` files.


## Usage

```bash
$ mpiexec -n <P> genprimes <N>
```
Where `<P>` stands for the _number of MPI processes_ and `<N>` is the _upper bound_ on the prime range.


# Efficiency

| #P vs N 	| 1k     	| 10k    	| 100k   	| 1M     	| 10M     	| 1B       	|
|:-------:	|--------	|--------	|--------	|--------	|---------	|----------	|
|  **1**  	| 0.249s 	|  0.25s 	| 0.273s 	| 0.701s 	| 10.946s 	|     X    	|
|  **2**  	| 0.265s 	| 0.265s 	| 0.273s 	| 0.457s 	| 4.485s  	|     X    	|
|  **5**  	| 0.298s 	| 0.296s 	| 0.301s 	| 0.380s 	| 1.645s  	|     X    	|
|  **10** 	| 0.370s 	| 0.372s 	| 0.377s 	| 0.417s 	| 1.023s  	| 289.819s 	|
| **100** 	| 3.721s 	| 3.766s 	| 3.703s 	| 3.678s 	| 4.072s  	|  42.161s 	|

> the numbers are averaged over 10 consecutive runs of the program.
<br>
> Note: **100** processes perform worse due to the fact that for `N < 1B` the communication between the processes is more expensive than the computation. 

These measurements were taken on a `Four AMD Opteron 6272 (2.1 GHz) (64 cores)`, with `256GB RAM` and `Cent OS 7` in a virtual environment with `4GB RAM` available and `1 CPU` available.


--------------------
Copyright 2020 [Andrii Lunin](https://github.com/moon1ock).
<br>
Open source code available under [MIT Licence](https://github.com/moon1ock).
