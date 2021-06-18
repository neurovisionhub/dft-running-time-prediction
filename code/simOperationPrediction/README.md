# Preparation performance evaluation

## Step 1 - Install g++ and STL Library
```sh
sudo apt update && sudo apt upgrade
sudo apt install gcc gcc-9 g++
sudo apt install build-essential
```
## Step 2 - Compile 

simulation prediction program for evaluation performance
```sh
 g++  measureCostPredictions.cc -o medicion.o
```

## Step 3 - Tests

evaluation performance

```sh
sh timeNeuralNetworks.sh
```

## Example outputs

output logs-times

```sh
Test: 6 features
inputs:6
neurons hidden layers: 25
outputs: 25
size querie term: 5
build vector querie
1600 ns. (nanoseconds)
runtime neural networks
11700 ns. (nanoseconds)
Neural Networks Multiplications: 175
evaluation Sigmoide function: 26
.
.
Test: 42 features
inputs:42
neurons hidden layers: 25
outputs: 25
size querie term: 5
build vector querie
29100 ns. (nanoseconds)
runtime neural networks
14800 ns. (nanoseconds)
Neural Networks Multiplications: 1075
evaluation Sigmoide function: 26
```