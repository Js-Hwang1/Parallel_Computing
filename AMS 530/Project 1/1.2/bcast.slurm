#!/bin/bash
#SBATCH --job-name=my_mpi_job          # Job name
#SBATCH --output=output_%j.txt         # Standard output and error log
#SBATCH --ntasks=16                    # Number of MPI tasks
#SBATCH --time=01:00:00                # Time limit (hh:mm:ss)
#SBATCH --partition=v100        # Partition name (modify as needed)

# Load any necessary modules (if applicable)
module load mpich/gcc/3.2.1
module load gcc/10.2.0


# Run your MPI program(Double check job name!!!!!!!!!!!)
mpicxx -std=c++11 -o Bcast Bcast.cpp
mpirun  ./Bcast >> Bcast_result.txt #Results will be collected in this one txt file, making it easier for stastical analysis.
