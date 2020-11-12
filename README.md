# MPI Sieve of Eratosthenes for prime numbers below N

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

| #P _vs_ N 	| 1k     	| 10k    	| 100k   	| 1M     	| 10M     	| 1B       	|
|:-------:	|--------	|--------	|--------	|--------	|---------	|----------	|
|  **1**  	| 0.249_s_ 	|  0.25_s_ 	| 0.273_s_ 	| 0.701_s_ 	| 10.946_s_ 	|     X    	|
|  **2**  	| 0.265_s_ 	| 0.265_s_ 	| 0.273_s_ 	| 0.457_s_ 	| 4.485_s_  	|     X    	|
|  **5**  	| 0.298_s_ 	| 0.296_s_ 	| 0.301_s_ 	| 0.380_s_ 	| 1.645_s_  	|     X    	|
|  **10** 	| 0.370_s_ 	| 0.372_s_ 	| 0.377_s_ 	| 0.417_s_ 	| 1.023_s_  	| 289.819_s_ 	|
| **100** 	| 3.721_s_ 	| 3.766_s_ 	| 3.703_s_ 	| 3.678_s_ 	| 4.072_s_  	|  42.161_s_ 	|

> the numbers are averaged over 10 consecutive runs of the program.

> Note: **100** processes perform worse due to the fact that for `N < 1B` the communication between the processes is more expensive than the computation. 

These measurements were taken on a `Four AMD Opteron 6272 (2.1 GHz) (64 cores)`, with `256GB RAM` and `Cent OS 7` in a virtual environment with `4GB RAM` available and `1 CPU` available.


--------------------
Copyright 2020 [Andrii Lunin](https://github.com/moon1ock).
<br>
Open source code available under [MIT Licence](https://github.com/moon1ock).
