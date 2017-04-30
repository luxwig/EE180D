# EE180D
EE 180D Project for Motion Detection

Regan Hsu, 
Gary Sullivan, 
Sichen (Ludwig) Zhao 

## How to Compile

If you are compiling on OS X, the full version would not be complied since OS X does not support 9DOF libaray. Instead, a mock interface has been created for the testing purpose. When running the `main` function, you need to specify what data file to read in the argument
1. Run `$ make`
 It should detect OS version as Darwin and ignore all 9DOF code section
2. Run `$ ./main <data_file.csv> > results.txt`

If you are compiling on Linux/9DOF directly, the full version **WILL BE** complied and no argument is needed to run:
1. Run `$ make`
 It should detect OS version as Linux and compile the full version
2. Run `$ ./main > results.txt`

If you are compiling on Linux/9DOF directly and want to **disable 9DOF functions**, aka. read data from file:
1. Run `$ make $MODE=DEBUG`
 It should detect DEBUG flag and ignore all 9DOF code section
2. Run `$ ./main <data_file.csv> > results.txt`

## `results.txt`:
* `0 -> 1`, `0` indicates walking, and `1` indicates speed 1
* `f1` indicates step up
* `f2` indicates step down
* Generally, any discontinuity represents a misclassifcation 


## `FANN` Interface 
1. `train_from_data_float`: Train the model based on current data (*float* type)
  * `input` - input for the nerual network
  * `output` - output for the nerual network
  * `num_data` - number of dataset
  * `num_input` - number of input for each dataset
  * `num_output` - number of output for each dataset
  * `ann` - FANN struct 
  
2. `test_from_data_float`: Predict the output by giving unknown data (*float* type)
  * `data` - input for the unknown data
  * `ann` - FANN struct
  * `n` - number of dataset
  * `predict` - the output/predictions of dataset

Both two functions have its *double* version `train_from_data_double` and `test_from_data_double` which takes in arrays of *double* as input
  
