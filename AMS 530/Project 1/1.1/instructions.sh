#Login using login1 and login2 node since it already has necessary compliers for the job!! (Xeonmax will not work!!!!!)

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


# OR You can import files directly from your local machine via
# scp /path/to/my/file netid@login.seawulf.stonybrook.edu:/path/to/destination/


# After writing the cpp files submit jobs via slurm
sbatch non_deterministic.slurm
sbatch deterministic.slurm


# Check output files: They are in txt format
ls


# To check printing results open txt file via vim or nano
vim <output_file_name>.txt
