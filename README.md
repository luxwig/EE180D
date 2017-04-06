# EE180D
EE 180D Project for Motion Detection

Regan Hsu, 
Gary Sullivan, 
Sichen (Ludwig) Zhao 

1. Run `$ make`
2. Run `$ ./main > results.txt`


In `results.txt`:
* `0 -> 1`, `0` indicates walking, and `1` indicates speed 1
* `f1` indicates step up
* `f2` indicates step down
* Generally, any discontinuity represents a misclassifcation 


`FANN` Interface provided:
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
  
