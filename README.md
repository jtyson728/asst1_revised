# IMDB CSV Sorter 

## Programming Concepts Used:
  -Efficient file/directory access (recursive directory search)
  
  -Forking processes in C
  
  -Avoiding zombie + orphan processes
 
## Abstract
  In this project, you will read in a directory name and walk through the directory structure to find all IMDB
  .csv files. There may be multiple levels of directories that you will need to recurse through. For each
  directory you find you should fork() a child process that will sort each of the files in it and output the
  results to a different file. You should NOT use exec for this project. You should write one program that,
  when copied from the parent to the child process, will continue running. You can use the return value of
  fork() in a conditional to choose different blocks of code to run within your code. You will want to
  make sure to prevent zombie and orphan child processes. You will also want to make sure you to not
  create fork bombs that will bring down machines. In all cases of bad input, you should fail gracefully
  (e.g. no segfaults).
  You will output metadata about your processes to STDOUT. This metadata will show the total number
  of processes created and the pids of all created processes.
