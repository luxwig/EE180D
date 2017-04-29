/*-------------------------------------------------------------------------------
 This file is part of Ranger.

 Ranger is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Ranger is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Ranger. If not, see <http://www.gnu.org/licenses/>.

 Written by:

 Marvin N. Wright
 Institut für Medizinische Biometrie und Statistik
 Universität zu Lübeck
 Ratzeburger Allee 160
 23562 Lübeck

 http://www.imbs-luebeck.de
 wright@imbs.uni-luebeck.de
 #-------------------------------------------------------------------------------*/

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "ranger.h"
#include "globals.h"
#include "ArgumentHandler.h"
#include "ForestClassification.h"
#include "ForestRegression.h"
#include "ForestSurvival.h"
#include "ForestProbability.h"

int train_random_forest(char* data, int ntree, const char* filename) {
    
    Forest* forest = 0;
    try {
        ArgumentHandler arg_handler(0, 0);
        // Handle command line arguments
        
        // Create forest object
        forest = new ForestClassification;
        arg_handler.ntree = ntree;
        arg_handler.verbose = true;
        arg_handler.file = data;
        arg_handler.depvarname = "TYPE";
        arg_handler.treetype = TREE_CLASSIFICATION;
        arg_handler.nthreads = 5;
        
        arg_handler.outprefix = filename;
        // Verbose output to logfile if non-verbose mode
        std::ostream* verbose_out = &std::cerr;
        arg_handler.write = true;
        // Call Ranger
        *verbose_out << "Starting Ranger." << std::endl;
        forest->initCpp( arg_handler.depvarname, arg_handler.memmode, arg_handler.file, arg_handler.mtry,
                        arg_handler.outprefix, arg_handler.ntree, verbose_out, arg_handler.seed, arg_handler.nthreads,
                        arg_handler.predict, arg_handler.impmeasure, arg_handler.targetpartitionsize, arg_handler.splitweights,
                        arg_handler.alwayssplitvars, arg_handler.statusvarname, arg_handler.replace, arg_handler.catvars,
                        arg_handler.savemem, arg_handler.splitrule, arg_handler.caseweights, arg_handler.predall, arg_handler.fraction,
                        arg_handler.alpha, arg_handler.minprop, arg_handler.holdout, arg_handler.predictiontype,
                        arg_handler.randomsplits);
        
        forest->run(true);
        if (arg_handler.write) {
            forest->saveToFile();
        }
        forest->writeOutput();
        std::vector<double> prediction = forest->getResults();
        for (size_t i = 0; i < prediction.size(); i++)
            std::cout << prediction[i] << std::endl;
        
        *verbose_out << "Finished Ranger." << std::endl;
        
        delete forest;
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << " Ranger will EXIT now." << std::endl;
        delete forest;
        return 0;
    }
    return 1;
}

int exec_random_forest(char* data, const char* filename, double* result) {

  Forest* forest = 0;
  try {
    ArgumentHandler arg_handler(0, 0);
    // Handle command line arguments

    // Create forest object
    forest = new ForestClassification;
    std::string fn = filename;
    fn += ".forest";
    arg_handler.verbose = false;
    arg_handler.file = data;
    arg_handler.depvarname = "TYPE";
    arg_handler.treetype = TREE_CLASSIFICATION;
    arg_handler.predict = fn;
    arg_handler.nthreads = 5;
    // Verbose output to logfile if non-verbose mode
    std::ostream* verbose_out = &std::cerr;

    // Call Ranger
    *verbose_out << "Starting Ranger." << std::endl;
    forest->initCpp( arg_handler.depvarname, arg_handler.memmode, arg_handler.file, arg_handler.mtry,
        arg_handler.outprefix, arg_handler.ntree, verbose_out, arg_handler.seed, arg_handler.nthreads,
        arg_handler.predict, arg_handler.impmeasure, arg_handler.targetpartitionsize, arg_handler.splitweights,
        arg_handler.alwayssplitvars, arg_handler.statusvarname, arg_handler.replace, arg_handler.catvars,
        arg_handler.savemem, arg_handler.splitrule, arg_handler.caseweights, arg_handler.predall, arg_handler.fraction,
        arg_handler.alpha, arg_handler.minprop, arg_handler.holdout, arg_handler.predictiontype,
        arg_handler.randomsplits);

    forest->run(true);
    if (arg_handler.write) {
      forest->saveToFile();
    }
    forest->writeOutput();
    std::vector<double> prediction = forest->getResults();
    for (size_t i = 0; i < prediction.size(); i++){
        *verbose_out << prediction[i] << std::endl;
        result[i] = prediction[i];
    }
    
    *verbose_out << "Finished Ranger." << std::endl;

    delete forest;
  } catch (std::exception& e) {
    std::cerr << "Error: " << e.what() << " Ranger will EXIT now." << std::endl;
    delete forest;
    return 0;
  }
  return 1;
}
