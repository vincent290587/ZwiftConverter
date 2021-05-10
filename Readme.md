
# Zwift workout text converter

As the name states, this program converts raw text to Zwift workouts.

## Supported input formats

Currently, only text from https://whatsonzwift.com is supported.

In order to run the program, you must copy paste the text from a workout in a text file that you will give to the program as argument.

Exemple.txt:  

`10min from 25 to 75% FTP`  
`1x 30sec @ 95rpm, 95% FTP,`  
`30sec @ 85rpm, 50% FTP`  
`1x 30sec @ 105rpm, 105% FTP,`  
`30sec @ 85rpm, 50% FTP`  
`1x 30sec @ 115rpm, 115% FTP,`  
`30sec @ 85rpm, 50% FTP`  
`2min @ 85rpm, 50% FTP`  
`10min @ 90rpm, 100% FTP`  
`2min @ 85rpm, 50% FTP`  
`3min @ 90rpm, 65% FTP`  
`5x 1min @ 100rpm, 65% FTP,`  
`1min @ 70rpm, 65% FTP`  
`2min @ 90rpm, 65% FTP`  
`6x 3min @ 90rpm, 77% FTP,`  
`20sec @ 110rpm, 120% FTP`  
`2min @ 85rpm, 50% FTP`  
`3min @ 90rpm, 65% FTP`  
`4x 1min @ 100rpm, 65% FTP,`  
`1min @ 70rpm, 65% FTP`  
`2min @ 90rpm, 65% FTP`  
`5x 1min @ 100rpm, 120% FTP,`  
`2min @ 85rpm, 55% FTP`  
`15min @ 90rpm, 65% FTP`  
`10min @ 90rpm, 100% FTP`  
`5min from 60 to 25% FTP`  

## Usage

First create a folder in the current directory called `out`.  

- `../TDDL/Zwifter.exe Exemple.txt`
- `../TDDL/Zwifter.exe *.txt`

All converted files will be written in the out/ folder

