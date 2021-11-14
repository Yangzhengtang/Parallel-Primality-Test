#!/bin/bash
#SBATCH -N 1
#SBATCH -n 48
#SBATCH -o prime-test.log
#SBATCH -J prime-test
#SBATCH -p skx-normal 
#SBATCH -A cse38018 
#SBATCH -t 00:40:00

# setup launcher environment
module purge
module load TACC 
module load launcher

export LAUNCHER_PLUGIN_DIR=$LAUNCHER_DIR/plugins 
export LAUNCHER_RMI=SLURM
export LAUNCHERSCHED=interleaved

# define file with commands to be run in parallel
export LAUNCHER_JOB_FILE=./test.sh
$LAUNCHER_DIR/paramrun