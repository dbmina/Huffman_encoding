# Huffman_encoding
This is implementation of Huffman encoding using C language.

Introduction
This project demonstrates how we can compress data using a simplified Huffman coding. The purpose of this project is to make you familiar with the binary representations of strings/integers and the logical operations supported in the C programming language. Another goal is to make your Linux or MacOS development environment ready and to get familiar with our project submission server.

A Simplified Huffman coding
Huffman coding
A Huffman code is a particular type of optimal prefix code that is commonly used for loseless data compression. The output from Huffman's algorithm can be viewed as a variable-length code table for encoding a source symbol. Basically, the Huffman's algorithm measures the frequency of each symbol and uses this information to assign fewer bits to more common symbols. For more information on Huffman coding, please refer to https://en.wikipedia.org/wiki/Huffman_coding.

Our simplified Huffman coding
In the original Huffman's algorithm, the actual encoded values are determined dynamically depending on the frequencies of source symbols. To make the problem simpler, we just use a static encoding table. Our simplified Huffman coding works as follows:

Each symbol consists of 4 bits. Therefore, the input data is divided into 4-bit symbols and there will be 16 different symbols from 0000 to 1111.
For each 4-bit symbol, we count the number of occurrences in the input data.
Now we sort the symbols based on the total number of occurrences and assign the code statically as shown in the following table.
Symbol Rank	Code	Description
0	000	the most frequent symbol
1	001	the next most frequent symbol
2	010	
3	011	
4	1000	...
5	1001	
6	1010	
7	1011	the 8th most frequent symbol
8	11000	the smallest symbol value among the rest
9	11001	the next smallest symbol value among the rest
10	11010	
11	11011	
12	11100	...
13	11101	
14	11110	
15	11111	the largest symbol value among the rest
We keep the symbol values of the most frequent 8 symbols in the header of the output for decompression (shown below). If the number of occurrences is same, we give a higher rank to a smaller symbol value. For the rest of them (i.e., the least frequent 8 symbols), however, we can see that the lengths of the encoded values are all the same. In order to reduce the output size, we don't keep the symbol values in the header. Instead, we arrange them according to the actual symbol value by interpreting them as unsigned integers (see the example below).

The output has the following format.
R (Rank table)	E (End info)	D (Encoded data)	Padding
(32 bits)	(4 bits)	(n bits)	(0~7 bits)
The Rank table records the most frequent 8 symbols in the order of their ranks. The End info tells the number of bits padded in the last byte. The Encoded data area has a sequence of encoded values that corresponds to the input data. The final output should be aligned to bytes. If the total number of bits for the header and the encoded data (i.e., 32 (R) + 4 (E) + n (D)) is not a multiple of 8, you should pad 0's in the last byte so that the total number of bits becomes a multiple of 8.
