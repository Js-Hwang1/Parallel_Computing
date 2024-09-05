# Change to the scratch directory (or a specific location) after logging into the HPC.
cd /gpfs/scratch/$USER  

# Create C++ files for both non-deterministic and deterministic MPI programs.
touch non_deterministic_order_mpi.cpp  # File for non-deterministic order printing
touch deterministic_order_mpi.cpp      # File for deterministic order printing

# Verify that the files were created successfully.
ls

# Open 'non_deterministic_order_mpi.cpp' using vim to add the code.
vim non_deterministic_order_mpi.cpp  
# Press 'i' to enter insert mode, paste the code, then press 'Esc' to exit insert mode.
# Press ':wq' to save and exit the file.

# Open 'deterministic_order_mpi.cpp' using vim to add the deterministic order code.
vim deterministic_order_mpi.cpp  
# Follow the same process: Press 'i' to enter insert mode, paste the code, then press 'Esc'.
# Press ':wq' to save and exit the file.

# You can also use other editors like nano or nvim according to your preference.

# Load the appropriate MPI module (other versions can be loaded based on user preference).
module load mpich/gcc/3.2.1  

# Compile the non-deterministic MPI code.
mpic++ -o non_deterministic_order_mpi non_deterministic_order_mpi.cpp  

# Run the non-deterministic MPI code using 50 processors (adjust the number of processors as needed).
mpirun -np 50 ./non_deterministic_order_mpi  

# Compile the deterministic MPI code.
mpic++ -o deterministic_order_mpi deterministic_order_mpi.cpp  

# Run the deterministic MPI code using 50 processors.
mpirun -np 50 ./deterministic_order_mpi  
