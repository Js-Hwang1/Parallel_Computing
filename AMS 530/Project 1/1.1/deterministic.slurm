#!/bin/bash
#SBATCH --job-name=my_mpi_job          # Job name
#SBATCH --output=output_%j.txt         # Standard output and error log
#SBATCH --ntasks=15                    # Number of MPI tasks
#SBATCH --time=01:00:00                # Time limit (hh:mm:ss)
#SBATCH --partition=short-28core       # Partition name 

# Load modules 
module load mpich/gcc/3.2.1

# Run Program
mpic++ -o deterministic_order_mpi deterministic_order_mpi.cpp
mpirun ./deterministic_order_mpi
