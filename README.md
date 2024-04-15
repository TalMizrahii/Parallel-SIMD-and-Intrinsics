<h1 align="center">
  
![Assembly-and-C-exercise](https://your-repo-url/assembly-and-C-exercise/assets/103560553/501dc4f5-570a-4b61-952a-477884518fce)

  Assembly and C Exercise
  <br>
</h1>

<h4 align="center"> </h4>

<p align="center">
  <a href="#description">Description</a> •
  <a href="#part-i---strings">Part I - Strings</a> •
  <a href="#part-ii---formulas">Part II - Formulas</a> •
  <a href="#dependencies">Dependencies</a> •
  <a href="#installing-and-executing">Installing And Executing</a> •
  <a href="#author">Author</a> 
</p>

## Description

### About The Exercise

The Assembly and C Exercise aims to practice assembly language programming using SSE/AVX instructions and C SSE/AVX intrinsics. It consists of two unrelated parts: string processing and floating-point calculations. Each part comprises various tasks and utilizes different source files for its environment.

## Part I - Strings

In this part, two string operators are implemented using SSE/AVX instructions and intrinsics.

### Hamming Distance

The Hamming distance between two strings is calculated as the number of positions at which the corresponding symbols are different. The implementation is done in assembly language only, specifically in the file named `hamming.s`.

### Base 64 Distance

The Base64 distance between two strings involves:
1. Removing non-base 64 characters while maintaining the order of every other character.
2. Calculating the numbers represented by the strings and subtracting the first from the second.
The implementation is in C using intrinsics, in the file named `b64.c`.

## Part II - Formulas

This part involves mathematical formulas applied to arrays of floating-point numbers.

### The First Formula

Formula 1 calculates the expression:
\[1 + \sum_{k=1}^{n} \sqrt{x_k} \times \prod_{k=1}^{n} (x_k^2 + 1)\]
The implementation is in C using intrinsics, in the file named `formula1.c`.

### The Second Formula

Formula 2 computes the expression:
\[ \sum_{k=1}^{n} x_k \times y_k \times \prod_{i=1}^{n} (x_i^2 + y_i^2 - 2x_i y_i + 1) \]
The implementation is in assembly language, specifically in the file named `formula2.s`.

## Dependencies

This exercise has the following dependencies:

- **SSE/AVX Support**: Ensure that the target platform supports SSE/AVX instructions for optimal performance.
- **Compiler and Assembler**: A compatible C compiler and assembler are required to compile and assemble the provided source files.

## Installing And Executing

To install and run the program you use [Git](https://git-scm.com). From your command line:

```bash
# Clone this repository.
$ git clone https://github.com/TalMizrahii/MusicBrains-WebCralwer

# Navigate to the repository directory:
$ cd MusicBrains-WebCrawler

# Run the program
$ python musicrawl.py
```
## Author

* [@Tal Mizrahi](https://github.com/TalMizrahii)
* Taltalon1927@gmail.com
