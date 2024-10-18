#!/bin/bash
#SBATCH --job-name=my_mpi_job          # Job name
#SBATCH --output=output_%j.txt         # Standard output and error log
#SBATCH --ntasks=32                    # Number of MPI tasks
#SBATCH --time=01:00:00                # Time limit (hh:mm:ss)
#SBATCH --partition=short-28core      # Partition name (modify as needed)

# Load any necessary modules (if applicable)
module load mpich/gcc/3.2.1
module load gcc/10.2.0


# Run your MPI program(Double check job name!!!!!!!!!!!)
mpicxx -std=c++11 -o bid32 bid32.cpp
mpirun  ./bid32 >> bid32.txt
